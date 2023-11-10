#include "Engine/Core/Application.h"
#include "Engine/Math/Math.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Resources/Resources.h"
#include <Foundation/Foundation.h>

#include "glm/glm.hpp"

#define NO_USING_NAMESPACE

#include "Engine/Renderer/Renderer.h"

#include <Metal/Metal.h>
#include <AppKit/AppKit.h>
#include <QuartzCore/QuartzCore.h>
#import "Foundation/Foundation.h"
#import "Quartz/Quartz.h"
#import <simd/simd.h>

#import "CommonMTLShader.h"

#define METAL_SHADER_SRC

namespace SC::Internal {

	static id<MTLDevice> gpu;
	static CAMetalLayer* layer;
	static id<MTLCommandQueue> commandQueue;
	
	static id<MTLBuffer> mvp_buffer;
	static id<MTLBuffer> vertex_buffer;
	static id<MTLBuffer> index_buffer;

	static id<MTLRenderPipelineState> RenderPiplineState_ErrorShader;

	static matrix_float4x4 mvp_mat;

	void Renderer::Init() {
		gpu = MTLCreateSystemDefaultDevice();
		
		layer = [CAMetalLayer layer];		
		layer.device = gpu;
		layer.pixelFormat = MTLPixelFormatRGBA8Unorm;
		
		NSWindow* window = (__bridge NSWindow*)Application::GetWindow().GetNativeWindow();

		window.contentView.layer = layer;
		window.contentView.wantsLayer = YES;

		// Buffers

		mvp_buffer = [gpu newBufferWithLength:sizeof(Matrix4) options:MTLResourceStorageModeShared];
		// 4 vertices each object
		vertex_buffer = [gpu newBufferWithLength:sizeof(Vertex_data) * MAX_BATCH_COUNT * 4 options:MTLResourceStorageModeShared];
		index_buffer = [gpu newBufferWithLength:sizeof(uint32_t) * MAX_BATCH_COUNT * 4 options:MTLResourceStorageModeShared];

		// Shaders
		MTLCompileOptions* compileOptions = [MTLCompileOptions new];
    	compileOptions.languageVersion = MTLLanguageVersion3_0;
        
		auto error_shader_src = @R"(
#include <metal_stdlib>
using namespace metal;
)"
#import "CommonMTLShader.h"
@R"(

vertex float4 v_error(
    constant Vertex_data* in  [[buffer(0)]],
	constant MVP_Uniform* vp  [[buffer(1)]],
    uint             vid [[vertex_id]])
{
    return float4(in[vid].position, 0, 1) * vp.vp);
}
fragment float4 f_error(
    Fragment_data in [[stage_in]])
{
    return float4()" ERROR_SHADER_COLOR_RGB @R"(, 1);
}
)";

		id<MTLLibrary> error_shader = [gpu newLibraryWithSource: error_shader_src
                                         options:compileOptions
                                           error:nullptr];
		
		MTLRenderPipelineDescriptor* rpd = [MTLRenderPipelineDescriptor new];
		rpd.vertexFunction = [error_shader newFunctionWithName:@"v_error"];
		rpd.fragmentFunction = [error_shader newFunctionWithName:@"f_error"];
		rpd.colorAttachments[0].pixelFormat = layer.pixelFormat;

		RenderPiplineState_ErrorShader = [gpu newRenderPipelineStateWithDescriptor:rpd error:nullptr];

		// Hard Coded texture shader
		Shader* shader = Resources::AddResource<Shader>("Default Sprite");
		shader->Compile(R"(
#include <metal_stdlib>
using namespace metal;
)"
#import "CommonMTLShader.h"
R"(

vertex Fragment_data v_sprite(
    uint             vid [[vertex_id]])
	constant MVP_Uniform* vp  [[buffer(0)]],
    constant Vertex_data* in  [[buffer(1)]],
{
    return Fragment_data(float4(in[vid].position, 0, 1) * vp.vp), in.color);
}
fragment float4 f_sprite(
    Fragment_data in [[stage_in]])
{
    return float4(in.color, 1);
}
		)");
		shader->uuid = 1;

		// CommandQueue
		commandQueue = [gpu newCommandQueue];
	}


	void Renderer::Render() {

		for (auto&& batch: batchData) {
			id<MTLCommandBuffer> cb = [commandQueue commandBuffer];

			auto drawable = [layer nextDrawable];

			Debug::Assert(drawable);

			MTLRenderPassDescriptor* rpd = [MTLRenderPassDescriptor new];
			MTLRenderPassColorAttachmentDescriptor* cd = rpd.colorAttachments[0];
        	cd.texture = drawable.texture;
        	cd.loadAction = MTLLoadActionClear;
        	cd.clearColor = MTLClearColorMake(1.0, 1.0, 1.0, 1.0);
        	cd.storeAction = MTLStoreActionStore;

			id<MTLRenderCommandEncoder> rce = [cb renderCommandEncoderWithDescriptor:rpd];

			[rce setVertexBuffer:mvp_buffer offset:0 atIndex:0];
			[rce setVertexBytes: &mvp_mat length:sizeof(mvp_mat) atIndex:0];

			[rce setVertexBuffer:vertex_buffer offset:vertex_buffer.allocatedSize atIndex:1];
			[rce setVertexBytes:batch.data() length:sizeof(batch) atIndex:1];

			//TODO: set textures using setFragmentTextures

			// [rce drawIndexedPrimitives:MTLPrimitiveTypeTriangle indexCount:4 indexType:MTLIndexTypeUInt32 indexBuffer: index_buffer indexBufferOffset:sizeof(Vertex_data)];
		}
	}

	void Renderer::SetMVP(Matrix4 _proj) {
		mvp_mat = *(matrix_float4x4*)&_proj[0].x;
	}
}