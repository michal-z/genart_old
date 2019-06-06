#include "external.h"
#include "library.h"
#define NAME "e3bff54e"

struct ROOT {
    VECTOR3 *image;
    u32 w, h;
};

static void Generate_Image(ROOT &root) {
    for (u32 y = 0; y < root.h; ++y) {
        for (u32 x = 0; x < root.w; ++x) {
            const u32 idx = x + y * root.w;
            root.image[idx] = VECTOR3{ 0.1f, 5.0f, 0.0f };
        }
    }
}

static void Save_Image(ROOT &root) {
    u8 *data = (u8 *)malloc(root.w * root.h * 3);
    if (data == nullptr) {
        printf("not enough memory\n");
        exit(1);
    }
    for (u32 y = 0; y < root.h; ++y) {
        for (u32 x = 0; x < root.w; ++x) {
            const u32 idx = x + y * root.w;
            data[idx * 3 + 0] = (u8)(255.0f * Saturate(root.image[idx].x));
            data[idx * 3 + 1] = (u8)(255.0f * Saturate(root.image[idx].y));
            data[idx * 3 + 2] = (u8)(255.0f * Saturate(root.image[idx].z));
        }
    }
    stbi_write_png(NAME".png", root.w, root.h, 3, data, root.w * 3);
    free(data);
}

int main() {
    ROOT root = {};
    root.w = 1920;
    root.h = 1080;
    root.image = (VECTOR3 *)malloc(root.w * root.h * sizeof(*root.image));
    if (root.image == nullptr) {
        printf("not enough memory\n");
        exit(1);
    }
    memset(root.image, 0, root.w * root.h * sizeof(*root.image));
    Generate_Image(root);
    Save_Image(root);
    free(root.image);
    return 0;
}
