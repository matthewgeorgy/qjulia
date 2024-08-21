#include <windows.h>
#include <d3dx12.h>
#include <d3dcompiler.h>
#include <dxgi1_5.h>
#define MG_IMPL
#define MG_USE_WINDOWS
#include <mg/mg.h>
#include <stdio.h>
#include <assert.h>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>

#define SCR_WIDTH				1280
#define SCR_HEIGHT 				960
#define FRAMEBUFFER_COUNT		3

#define PPV_ARGS		IID_PPV_ARGS
#define DX_CHECK(__call) \
	do \
	{ \
		HRESULT __dxhr = (__call); \
		assert(__dxhr >= 0); \
	} while (0)

typedef ID3D12Resource			ID3D12Buffer;
typedef ID3D12Resource			ID3D12Texture1D;
typedef ID3D12Resource			ID3D12Texture2D;
typedef ID3D12Resource			ID3D12Texture3D;

typedef struct _TAG_qjulia_params
{
	vec4_t		mu;
	s32			width,
				height;
	b32			self_shadow;
	f32			epsilon;
	f32			zoom;
	s32			iterations;
	f32			_pad0[2];
} qjulia_params_t;

UINT							cbv_descriptor_size;
UINT							srv_descriptor_size;
UINT							uav_descriptor_size;
UINT							rtv_descriptor_size;
UINT							dsv_descriptor_size;

HRESULT							hr;
IDXGIFactory4					*dxgi_factory;
IDXGIAdapter1					*dxgi_adapter;
IDXGISwapChain3					*swapchain;
ID3D12Device4					*device;
ID3D12CommandQueue				*command_queue;
ID3D12DescriptorHeap			*rtv_descriptor_heap;
ID3D12DescriptorHeap			*dsv_descriptor_heap;
ID3D12Texture2D					*backbuffers[FRAMEBUFFER_COUNT];
ID3D12Texture2D					*depth_buffers[FRAMEBUFFER_COUNT];
ID3D12CommandAllocator			*command_allocators[FRAMEBUFFER_COUNT];
ID3D12GraphicsCommandList		*command_list;
ID3D12Fence						*fence;
HANDLE							fence_event;
UINT64							fence_values[FRAMEBUFFER_COUNT] = {};
UINT							backbuffer_index;
D3D12_VIEWPORT					viewport = {};
D3D12_RECT						scissor_rect = {};

ID3D12RootSignature				*root_signature;
ID3D12PipelineState				*pipeline;

ID3D12Buffer					*vb;
D3D12_VERTEX_BUFFER_VIEW		vb_view;
ID3D12Buffer					*ib;
D3D12_INDEX_BUFFER_VIEW			ib_view;

qjulia_params_t					params;
ID3D12Buffer					*params_buffer[FRAMEBUFFER_COUNT];
BYTE							*params_ptrs[FRAMEBUFFER_COUNT];

