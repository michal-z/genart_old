#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include <assert.h>
#include <stdio.h>

#ifdef USE_DX12
#define WIN32_LEAN_AND_MEAN
#include <d3d12.h>
#include <dxgi1_4.h>
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#endif

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

#include "stb_ds.h"
#include "stb_image_write.h"
#include "stb_perlin.h"
