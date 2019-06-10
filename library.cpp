#include "library.h"
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

#ifdef USE_DX12
void Init_Descriptor_Heaps(DX12_CONTEXT &dx);

void
Init_DX12_Context(HWND window, DX12_CONTEXT &dx)
{
    IDXGIFactory4 *factory;
#ifdef _DEBUG
    VHR(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&factory)));
#else
    VHR(CreateDXGIFactory2(0, IID_PPV_ARGS(&factory)));
#endif
#ifdef _DEBUG
    {
        ID3D12Debug *dbg;
        D3D12GetDebugInterface(IID_PPV_ARGS(&dbg));
        if (dbg)
        {
            dbg->EnableDebugLayer();
            ID3D12Debug1 *dbg1;
            dbg->QueryInterface(IID_PPV_ARGS(&dbg1));
            if (dbg1)
            {
                dbg1->SetEnableGPUBasedValidation(TRUE);
            }
            SAFE_RELEASE(dbg);
            SAFE_RELEASE(dbg1);
        }
    }
#endif
    if (FAILED(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_1, IID_PPV_ARGS(&dx.device))))
    {
        MessageBox(window, "This application requires Windows 10 1709 (RS3) or newer.", "D3D12CreateDevice failed",
                   MB_OK | MB_ICONERROR);
        return;
    }

    D3D12_COMMAND_QUEUE_DESC cmdqueue_desc = {};
    cmdqueue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    cmdqueue_desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    cmdqueue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    VHR(dx.device->CreateCommandQueue(&cmdqueue_desc, IID_PPV_ARGS(&dx.cmdqueue)));

    DXGI_SWAP_CHAIN_DESC swapchain_desc = {};
    swapchain_desc.BufferCount = 4;
    swapchain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchain_desc.OutputWindow = window;
    swapchain_desc.SampleDesc.Count = 1;
    swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    swapchain_desc.Windowed = TRUE;

    IDXGISwapChain *temp_swapchain;
    VHR(factory->CreateSwapChain(dx.cmdqueue, &swapchain_desc, &temp_swapchain));
    VHR(temp_swapchain->QueryInterface(IID_PPV_ARGS(&dx.swapchain)));
    SAFE_RELEASE(temp_swapchain);
    SAFE_RELEASE(factory);

    RECT rect;
    GetClientRect(window, &rect);
    dx.resolution[0] = (u32)rect.right;
    dx.resolution[1] = (u32)rect.bottom;

    for (u32 i = 0; i < 2; ++i)
    {
        VHR(dx.device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&dx.cmdalloc[i])));
    }

    dx.descriptor_size = dx.device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    dx.descriptor_size_rtv = dx.device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    Init_Descriptor_Heaps(dx);

    // swap buffer render targets
    {
        D3D12_CPU_DESCRIPTOR_HANDLE handle;
        Allocate_Descriptors(dx, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 4, handle);

        for (u32 i = 0; i < 4; ++i)
        {
            VHR(dx.swapchain->GetBuffer(i, IID_PPV_ARGS(&dx.swapbuffers[i])));
            dx.device->CreateRenderTargetView(dx.swapbuffers[i], nullptr, handle);
            handle.ptr += dx.descriptor_size_rtv;
        }
    }
    // depth-stencil target
    {
        auto image_desc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, dx.resolution[0], dx.resolution[1],
                                                       1, 1);
        image_desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL | D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;
        VHR(dx.device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE,
                                               &image_desc, D3D12_RESOURCE_STATE_DEPTH_WRITE,
                                               &CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
                                               IID_PPV_ARGS(&dx.ds_buffer)));

        D3D12_CPU_DESCRIPTOR_HANDLE handle;
        Allocate_Descriptors(dx, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, handle);

        D3D12_DEPTH_STENCIL_VIEW_DESC view_desc = {};
        view_desc.Format = DXGI_FORMAT_D32_FLOAT;
        view_desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
        view_desc.Flags = D3D12_DSV_FLAG_NONE;
        dx.device->CreateDepthStencilView(dx.ds_buffer, &view_desc, handle);
    }

    VHR(dx.device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, dx.cmdalloc[0], nullptr,
                                     IID_PPV_ARGS(&dx.cmdlist)));
    dx.cmdlist->Close();

    VHR(dx.device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&dx.frame_fence)));
    dx.frame_fence_event = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
}

void
Shutdown_DX12_Context(DX12_CONTEXT &dx)
{
    // @Incomplete: Release all resources.
    SAFE_RELEASE(dx.cmdlist);
    SAFE_RELEASE(dx.cmdalloc[0]);
    SAFE_RELEASE(dx.cmdalloc[1]);
    SAFE_RELEASE(dx.rt_heap.heap);
    SAFE_RELEASE(dx.ds_heap.heap);
    for (u32 i = 0; i < 4; ++i)
    {
        SAFE_RELEASE(dx.swapbuffers[i]);
    }
    CloseHandle(dx.frame_fence_event);
    SAFE_RELEASE(dx.frame_fence);
    SAFE_RELEASE(dx.swapchain);
    SAFE_RELEASE(dx.cmdqueue);
    SAFE_RELEASE(dx.device);
}

void
Present_Frame(DX12_CONTEXT &dx, u32 swap_interval)
{
    dx.swapchain->Present(swap_interval, 0);
    dx.cmdqueue->Signal(dx.frame_fence, ++dx.frame_count);

    const u64 gpu_frame_count = dx.frame_fence->GetCompletedValue();

    if ((dx.frame_count - gpu_frame_count) >= 2)
    {
        dx.frame_fence->SetEventOnCompletion(gpu_frame_count + 1, dx.frame_fence_event);
        WaitForSingleObject(dx.frame_fence_event, INFINITE);
    }

    dx.frame_index = !dx.frame_index;
    dx.back_buffer_index = dx.swapchain->GetCurrentBackBufferIndex();
    dx.gpu_descriptor_heaps[dx.frame_index].size = 0;
}

void
Wait_For_GPU(DX12_CONTEXT &dx)
{
    dx.cmdqueue->Signal(dx.frame_fence, ++dx.frame_count);
    dx.frame_fence->SetEventOnCompletion(dx.frame_count, dx.frame_fence_event);
    WaitForSingleObject(dx.frame_fence_event, INFINITE);
}

DESCRIPTOR_HEAP &
Get_Descriptor_Heap(DX12_CONTEXT &dx,
                    D3D12_DESCRIPTOR_HEAP_TYPE type,
                    D3D12_DESCRIPTOR_HEAP_FLAGS flags,
                    u32 &descriptor_size)
{
    if (type == D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
    {
        descriptor_size = dx.descriptor_size_rtv;
        return dx.rt_heap;
    }
    else if (type == D3D12_DESCRIPTOR_HEAP_TYPE_DSV)
    {
        descriptor_size = dx.descriptor_size_rtv;
        return dx.ds_heap;
    }
    else if (type == D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
    {
        descriptor_size = dx.descriptor_size;
        if (flags == D3D12_DESCRIPTOR_HEAP_FLAG_NONE)
        {
            return dx.cpu_descriptor_heap;
        }
        else if (flags == D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE)
        {
            return dx.gpu_descriptor_heaps[dx.frame_index];
        }
    }
    assert(0);
    descriptor_size = 0;
    return dx.cpu_descriptor_heap;
}

static void
Init_Descriptor_Heaps(DX12_CONTEXT &dx)
{
    // render target descriptor heap (RTV)
    {
        dx.rt_heap.capacity = 16;

        D3D12_DESCRIPTOR_HEAP_DESC heap_desc = {};
        heap_desc.NumDescriptors = dx.rt_heap.capacity;
        heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        VHR(dx.device->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(&dx.rt_heap.heap)));
        dx.rt_heap.cpu_start = dx.rt_heap.heap->GetCPUDescriptorHandleForHeapStart();
    }
    // depth-stencil descriptor heap (DSV)
    {
        dx.ds_heap.capacity = 8;

        D3D12_DESCRIPTOR_HEAP_DESC heap_desc = {};
        heap_desc.NumDescriptors = dx.ds_heap.capacity;
        heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        VHR(dx.device->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(&dx.ds_heap.heap)));
        dx.ds_heap.cpu_start = dx.ds_heap.heap->GetCPUDescriptorHandleForHeapStart();
    }
    // non-shader visible descriptor heap (CBV, SRV, UAV)
    {
        dx.cpu_descriptor_heap.capacity = 10000;

        D3D12_DESCRIPTOR_HEAP_DESC heap_desc = {};
        heap_desc.NumDescriptors = dx.cpu_descriptor_heap.capacity;
        heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        VHR(dx.device->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(&dx.cpu_descriptor_heap.heap)));
        dx.cpu_descriptor_heap.cpu_start = dx.cpu_descriptor_heap.heap->GetCPUDescriptorHandleForHeapStart();
    }
    // shader visible descriptor heaps (CBV, SRV, UAV)
    {
        for (u32 i = 0; i < 2; ++i)
        {
            dx.gpu_descriptor_heaps[i].capacity = 10000;

            D3D12_DESCRIPTOR_HEAP_DESC heap_desc = {};
            heap_desc.NumDescriptors = dx.gpu_descriptor_heaps[i].capacity;
            heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
            heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
            VHR(dx.device->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(&dx.gpu_descriptor_heaps[i].heap)));

            dx.gpu_descriptor_heaps[i].cpu_start = dx.gpu_descriptor_heaps[i].heap->GetCPUDescriptorHandleForHeapStart();
            dx.gpu_descriptor_heaps[i].gpu_start = dx.gpu_descriptor_heaps[i].heap->GetGPUDescriptorHandleForHeapStart();
        }
    }
}

void
Load_File(const char *filename, u32 &size, u8 *&data)
{
    FILE *file = fopen(filename, "rb");
    assert(file);
    fseek(file, 0, SEEK_END);
    long ret_size = ftell(file);
    if (ret_size == -1)
    {
        assert(0);
        return;
    }
    size = (u32)ret_size;
    data = (u8 *)malloc(size);
    fseek(file, 0, SEEK_SET);
    fread(data, 1, size, file);
    fclose(file);
}

f64
Get_Time()
{
    static LARGE_INTEGER start_counter;
    static LARGE_INTEGER frequency;
    if (start_counter.QuadPart == 0)
    {
        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&start_counter);
    }
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return (counter.QuadPart - start_counter.QuadPart) / (f64)frequency.QuadPart;
}

void
Update_Frame_Stats(HWND window,
                   const char *name,
                   f64 &time,
                   f32 &delta_time)
{
    static f64 previous_time = -1.0;
    static f64 header_refresh_time = 0.0;
    static u32 frame_count = 0;

    if (previous_time < 0.0)
    {
        previous_time = Get_Time();
        header_refresh_time = previous_time;
    }

    time = Get_Time();
    delta_time = (f32)(time - previous_time);
    previous_time = time;

    if ((time - header_refresh_time) >= 1.0)
    {
        const f64 fps = frame_count / (time - header_refresh_time);
        const f64 ms = (1.0 / fps) * 1000.0;
        char header[256];
        snprintf(header, sizeof(header), "[%.1f fps  %.3f ms] %s", fps, ms, name);
        SetWindowText(window, header);
        header_refresh_time = time;
        frame_count = 0;
    }
    frame_count++;
}

static LRESULT CALLBACK
Process_Window_Message(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_KEYDOWN:
        if (wparam == VK_ESCAPE)
        {
            PostQuitMessage(0);
            return 0;
        }
        break;
    }
    return DefWindowProc(window, message, wparam, lparam);
}

HWND
Create_Window(const char *name, u32 width, u32 height)
{
    WNDCLASS winclass = {};
    winclass.lpfnWndProc = Process_Window_Message;
    winclass.hInstance = GetModuleHandle(nullptr);
    winclass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    winclass.lpszClassName = name;
    if (!RegisterClass(&winclass))
    {
        assert(0);
    }

    RECT rect = { 0, 0, (LONG)width, (LONG)height };
    if (!AdjustWindowRect(&rect, WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX, 0))
    {
        assert(0);
    }

    HWND window = CreateWindowEx(0, name, name, WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_VISIBLE,
                                 CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,
                                 nullptr, nullptr, nullptr, 0);
    assert(window);
    return window;
}
#endif // USE_DX12
