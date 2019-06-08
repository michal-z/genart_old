#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include "stb_ds.h"
#include "stb_image_write.h"
#include "stb_perlin.h"

#ifdef USE_DX12
#define WIN32_LEAN_AND_MEAN
#include <d3d12.h>
#include <dxgi1_4.h>
#include "d3dx12.h"
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#define VHR(hr) if (FAILED(hr)) { assert(0); }
#define SAFE_RELEASE(obj) if ((obj)) { (obj)->Release(); (obj) = nullptr; }
#endif // USE_DX12

typedef char i8;
typedef short i16;
typedef int i32;
typedef long long i64;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;

struct VECTOR2
{
    f32 x, y;
};

struct VECTOR3
{
    f32 x, y, z;
};

struct VECTOR4
{
    f32 x, y, z, w;
};

inline VECTOR2 operator*(f32 a, VECTOR2 b) { return VECTOR2{ a * b.x, a * b.y }; }
inline VECTOR2 operator*(VECTOR2 a, f32 b) { return VECTOR2{ a.x * b, a.y * b }; }
inline VECTOR3 operator*(f32 a, VECTOR3 b) { return VECTOR3{ a * b.x, a * b.y, a * b.z }; }
inline VECTOR3 operator*(VECTOR3 a, f32 b) { return VECTOR3{ a.x * b, a.y * b, a.z * b }; }
inline VECTOR4 operator*(f32 a, VECTOR4 b) { return VECTOR4{ a * b.x, a * b.y, a * b.z, a * b.w }; }
inline VECTOR4 operator*(VECTOR4 a, f32 b) { return VECTOR4{ a.x * b, a.y * b, a.z * b, a.w * b }; }
inline VECTOR2 operator+(VECTOR2 a, VECTOR2 b) { return VECTOR2{ a.x + b.x, a.y + b.y }; }
inline VECTOR3 operator+(VECTOR3 a, VECTOR3 b) { return VECTOR3{ a.x + b.x, a.y + b.y, a.z + b.z }; }
inline VECTOR4 operator+(VECTOR4 a, VECTOR4 b) { return VECTOR4{ a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w }; }
inline VECTOR2 operator-(VECTOR2 a, VECTOR2 b) { return VECTOR2{ a.x - b.x, a.y - b.y }; }
inline VECTOR3 operator-(VECTOR3 a, VECTOR3 b) { return VECTOR3{ a.x - b.x, a.y - b.y, a.z - b.z }; }
inline VECTOR4 operator-(VECTOR4 a, VECTOR4 b) { return VECTOR4{ a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w }; }

inline f32 Dot(VECTOR2 a, VECTOR2 b) { return a.x * b.x + a.y * b.y; }
inline f32 Dot(VECTOR3 a, VECTOR3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
inline f32 Dot(VECTOR4 a, VECTOR4 b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

inline f32 Length(VECTOR2 a) { return sqrtf(Dot(a, a)); }
inline f32 Length(VECTOR3 a) { return sqrtf(Dot(a, a)); }
inline f32 Length(VECTOR4 a) { return sqrtf(Dot(a, a)); }

inline VECTOR2
Normalize(VECTOR2 a)
{
    f32 len = Length(a);
    assert(len != 0.0f);
    return 1.0f / len * a;
}

inline VECTOR3
Normalize(VECTOR3 a)
{
    f32 len = Length(a);
    assert(len != 0.0f);
    return 1.0f / len * a;
}

inline VECTOR4
Normalize(VECTOR4 a)
{
    f32 len = Length(a);
    assert(len != 0.0f);
    return 1.0f / len * a;
}

inline VECTOR3
Cross(VECTOR3 a, VECTOR3 b)
{
    return VECTOR3{ a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
}

inline f32 Distance(VECTOR2 a, VECTOR2 b) { return Length(a - b); }
inline f32 Distance(VECTOR3 a, VECTOR3 b) { return Length(a - b); }
inline f32 Distance(VECTOR4 a, VECTOR4 b) { return Length(a - b); }

inline f32 Min(f32 a, f32 b) { return a < b ? a : b; }
inline f32 Max(f32 a, f32 b) { return a > b ? a : b; }
inline f32 Clamp(f32 a, f32 min, f32 max) { return Min(Max(min, a), max); }
inline f32 Saturate(f32 a) { return Min(Max(0.0f, a), 1.0f); }

#ifdef USE_DX12
struct DESCRIPTOR_HEAP
{
    ID3D12DescriptorHeap *heap;
    D3D12_CPU_DESCRIPTOR_HANDLE cpu_start;
    D3D12_GPU_DESCRIPTOR_HANDLE gpu_start;
    u32 size;
    u32 capacity;
};

struct GRAPHICS_CONTEXT
{
    ID3D12Device3 *device;
    ID3D12GraphicsCommandList2 *cmdlist;
    ID3D12CommandQueue *cmdqueue;
    ID3D12CommandAllocator *cmdalloc[2];
    u32 resolution[2];
    u32 descriptor_size;
    u32 descriptor_size_rtv;
    u32 frame_index;
    u32 back_buffer_index;
    ID3D12Resource *swapbuffers[4];
    IDXGISwapChain3 *swapchain;
    ID3D12Resource *ds_buffer;
    DESCRIPTOR_HEAP rt_heap;
    DESCRIPTOR_HEAP ds_heap;
    DESCRIPTOR_HEAP cpu_descriptor_heap;
    DESCRIPTOR_HEAP gpu_descriptor_heaps[2];
    ID3D12Fence *frame_fence;
    HANDLE frame_fence_event;
    u64 frame_count;
};

void
Init_Graphics_Context(HWND window,
                      GRAPHICS_CONTEXT &gfx);

void
Shutdown_Graphics_Context(GRAPHICS_CONTEXT &gfx);

DESCRIPTOR_HEAP &
Get_Descriptor_Heap(GRAPHICS_CONTEXT &gfx,
                    D3D12_DESCRIPTOR_HEAP_TYPE type,
                    D3D12_DESCRIPTOR_HEAP_FLAGS flags,
                    u32 &descriptor_size);

void
Present_Frame(GRAPHICS_CONTEXT &gfx,
              u32 swap_interval);

void
Wait_For_GPU(GRAPHICS_CONTEXT &gfx);

void
Load_File(const char *filename,
          u32 &size,
          u8 *&data);

void
Update_Frame_Stats(HWND window,
                   const char *name,
                   f64 &time,
                   f32 &delta_time);

f64
Get_Time();

HWND
Create_Window(const char *name,
              u32 width,
              u32 height);

inline ID3D12GraphicsCommandList2 *
Get_And_Reset_Command_List(GRAPHICS_CONTEXT &gfx)
{
    gfx.cmdalloc[gfx.frame_index]->Reset();
    gfx.cmdlist->Reset(gfx.cmdalloc[gfx.frame_index], nullptr);
    return gfx.cmdlist;
}

inline void
Allocate_Descriptors(GRAPHICS_CONTEXT &gfx,
                     D3D12_DESCRIPTOR_HEAP_TYPE type,
                     u32 count,
                     D3D12_CPU_DESCRIPTOR_HANDLE& handle)
{
    u32 descriptor_size;
    DESCRIPTOR_HEAP &heap = Get_Descriptor_Heap(gfx, type, D3D12_DESCRIPTOR_HEAP_FLAG_NONE, descriptor_size);
    assert((heap.size + count) < heap.capacity);
    handle.ptr = heap.cpu_start.ptr + heap.size * descriptor_size;
    heap.size += count;
}

inline void
Allocate_GPU_Descriptors(GRAPHICS_CONTEXT &gfx,
                         u32 count,
                         D3D12_CPU_DESCRIPTOR_HANDLE &cpu_handle,
                         D3D12_GPU_DESCRIPTOR_HANDLE &gpu_handle)
{
    u32 descriptor_size;
    DESCRIPTOR_HEAP &heap = Get_Descriptor_Heap(gfx, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
                                                D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, descriptor_size);
    assert((heap.size + count) < heap.capacity);

    cpu_handle.ptr = heap.cpu_start.ptr + heap.size * descriptor_size;
    gpu_handle.ptr = heap.gpu_start.ptr + heap.size * descriptor_size;

    heap.size += count;
}

inline D3D12_GPU_DESCRIPTOR_HANDLE
Copy_Descriptors_To_GPU(GRAPHICS_CONTEXT &gfx,
                        u32 count,
                        D3D12_CPU_DESCRIPTOR_HANDLE src_base)
{
    D3D12_CPU_DESCRIPTOR_HANDLE cpu_dst_base;
    D3D12_GPU_DESCRIPTOR_HANDLE gpu_dst_base;
    Allocate_GPU_Descriptors(gfx, count, cpu_dst_base, gpu_dst_base);
    gfx.device->CopyDescriptorsSimple(count, cpu_dst_base, src_base, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    return gpu_dst_base;
}

inline void
Get_Back_Buffer(GRAPHICS_CONTEXT &gfx,
                ID3D12Resource *&buffer,
                D3D12_CPU_DESCRIPTOR_HANDLE &handle)
{
    buffer = gfx.swapbuffers[gfx.back_buffer_index];
    handle = gfx.rt_heap.cpu_start;
    handle.ptr += gfx.back_buffer_index * gfx.descriptor_size_rtv;
}

inline void
Get_Depth_Stencil_Buffer(GRAPHICS_CONTEXT &gfx,
                         ID3D12Resource *&buffer,
                         D3D12_CPU_DESCRIPTOR_HANDLE &handle)
{
    buffer = gfx.ds_buffer;
    handle = gfx.ds_heap.cpu_start;
}

inline void
Bind_GPU_Descriptor_Heap(const GRAPHICS_CONTEXT &gfx)
{
    gfx.cmdlist->SetDescriptorHeaps(1, &gfx.gpu_descriptor_heaps[gfx.frame_index].heap);
}
#endif // USE_DX12

inline f32
Rand01()
{
    u32 exponent = 127;
    u32 significand = (u32)(rand() & 0x7fff); // get 15 random bits
    u32 result = (exponent << 23) | (significand << 8);
    return *(f32 *)&result - 1.0f;
}

inline f32
Rand(f32 begin, f32 end)
{
    assert(begin < end);
    return begin + (end - begin) * Rand01();
}
