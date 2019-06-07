#pragma hdrstop("external.pch")
#pragma comment(lib, "external.lib")
#include "library.h"
#define NAME "e3bff54e"

struct ROOT {
    VECTOR3 *image;
    u32 image_width;
    u32 image_height;
    VECTOR2 *points;
};

static void Generate_Image(ROOT &root) {
    VECTOR2 p{ 1.0f, 0.5f };
    arrput(root.points, p);

    for (u32 y = 0; y < root.image_height; ++y) {
        for (u32 x = 0; x < root.image_width; ++x) {
            const u32 idx = x + y * root.image_width;
            root.image[idx] = VECTOR3{ 1.1f, 0.5f + 0.5f * sinf(0.1f * x), 0.0f };
        }
    }
}

static void Save_Image(VECTOR3 *image, u32 width, u32 height) {
    u8 *data = (u8 *)malloc(width * height * 3);
    if (data == nullptr) {
        printf("not enough memory\n");
        exit(1);
    }
    for (u32 y = 0; y < height; ++y) {
        for (u32 x = 0; x < width; ++x) {
            const u32 idx = x + y * width;
            data[idx * 3 + 0] = (u8)(255.0f * Saturate(image[idx].x));
            data[idx * 3 + 1] = (u8)(255.0f * Saturate(image[idx].y));
            data[idx * 3 + 2] = (u8)(255.0f * Saturate(image[idx].z));
        }
    }
    stbi_write_png(NAME".png", width, height, 3, data, width * 3);
    free(data);
}

i32 main() {
    ROOT root = {};
    root.image_width = 1920;
    root.image_height = 1080;
    root.image = (VECTOR3 *)malloc(root.image_width * root.image_height * sizeof(*root.image));
    if (root.image == nullptr) {
        printf("not enough memory\n");
        exit(1);
    }
    memset(root.image, 0, root.image_width * root.image_height * sizeof(*root.image));
    Generate_Image(root);
    Save_Image(root.image, root.image_width, root.image_height);
    free(root.image);
    return 0;
}
