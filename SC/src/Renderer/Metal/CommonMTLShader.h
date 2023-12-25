// This is going to be used in shaders as well as code

#include <simd/simd.h>
#include <simd/matrix_types.h>
#include <simd/vector_types.h>

#ifndef METAL_SHADER_SRC

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-attributes"

#endif
#define RW_STR @R"(
#define RW_STR_END )"
#ifdef METAL_SHADER_SRC
RW_STR
#endif

typedef struct {
	matrix_float4x4 vp;
} MVP_Uniform;

typedef struct {
	vector_float4 position;
	vector_short3 color;
	vector_float2 tex_coords;
	simd::uint1 tex;
} Vertex_data;

typedef struct {
	vector_float4 position [[position]];
	vector_float3 color;
} Fragment_data;

#ifdef METAL_SHADER_SRC
RW_STR_END
#else
#pragma clang diagnostic pop
#endif