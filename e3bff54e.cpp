#pragma hdrstop("library.pch")
#pragma comment(lib, "library.lib")
#define NAME "e3bff54e"

struct ROOT
{
    VECTOR3 *image;
    u32 image_width;
    u32 image_height;
};

static void
Generate_Image(ROOT &root)
{
    VECTOR2 *positions = nullptr;
    VECTOR3 *colors = nullptr;

    for (u32 i = 0; i < 10000000; ++i)
    {
        arrput(positions, (VECTOR2{ Rand(-0.5f, 0.5f), Rand(-0.5f, 0.5f) }));
        arrput(colors, (VECTOR3{ Rand01(), Rand01(), Rand01() }));
    }

    for (u32 i = 0; i < (u32)arrlenu(positions); ++i)
    {
        u32 x = (u32)((0.5f + 0.5f * positions[i].x) * root.image_width);
        u32 y = (u32)((0.5f + 0.5f * positions[i].y) * root.image_height);
        assert(x < root.image_width && y < root.image_height);

        const u32 idx = x + y * root.image_width;
        root.image[idx] -= colors[i] * 0.005f;
    }
}

static void
Save_Image(VECTOR3 *image, u32 width, u32 height)
{
    u8 *data = (u8 *)malloc(width * height * 3);
    if (data == nullptr)
    {
        printf("not enough memory\n");
        exit(1);
    }
    for (u32 y = 0; y < height; ++y)
    {
        for (u32 x = 0; x < width; ++x)
        {
            const u32 idx = x + y * width;
            data[idx * 3 + 0] = (u8)(255.0f * Saturate(image[idx].x));
            data[idx * 3 + 1] = (u8)(255.0f * Saturate(image[idx].y));
            data[idx * 3 + 2] = (u8)(255.0f * Saturate(image[idx].z));
        }
    }
    stbi_write_png(NAME".png", width, height, 3, data, width * 3);
    free(data);
}

i32
main()
{
    ROOT root = {};
    root.image_width = 1920;
    root.image_height = 1080;
    root.image = (VECTOR3 *)malloc(root.image_width * root.image_height * sizeof(*root.image));
    if (root.image == nullptr)
    {
        printf("not enough memory\n");
        exit(1);
    }
    for (u32 i = 0; i < root.image_width * root.image_height; ++i)
    {
        root.image[i] = VECTOR3{ 1.0f, 1.0f, 1.0f };
    }
    Generate_Image(root);
    Save_Image(root.image, root.image_width, root.image_height);
    free(root.image);
    return 0;
}
