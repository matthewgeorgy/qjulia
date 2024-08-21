#include <stdafx.h>


LRESULT CALLBACK wndproc(HWND, UINT, WPARAM, LPARAM);
void move_to_next_frame();

int
main(void)
{
	//////////////////////////////////////////////////////////////////////////
	// Win32 setup

	WNDCLASSEX		wndclass = {};
	ATOM			atom;
	RECT			wnddim = { 0 , 0, SCR_WIDTH, SCR_HEIGHT };
	INT				window_width, window_height;
	HWND			hwnd;
	MSG				msg;


	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	wndclass.lpszClassName = "qjulia_wndclass";
	wndclass.lpfnWndProc = &wndproc;
	wndclass.hInstance = GetModuleHandle(NULL);

	atom = RegisterClassEx(&wndclass);
	if (!atom)
	{
		printf("Failed to register class...!\r\n");
		return (-1);
	}

	AdjustWindowRect(&wnddim, WS_OVERLAPPEDWINDOW, FALSE);
	window_width = wnddim.right - wnddim.left;
	window_height = wnddim.bottom - wnddim.top;

	hwnd = CreateWindowEx(0, wndclass.lpszClassName, "qjulia", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, window_width, window_height,
		NULL, NULL, wndclass.hInstance, NULL);
	if (!hwnd)
	{
		printf("Failed to create window...!\r\n");
		return (-1);
	}

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	//////////////////////////////////////////////////////////////////////////
	// Initialize debug layer

	ID3D12Debug		*debug_layer;


	DX_CHECK(D3D12GetDebugInterface(PPV_ARGS(&debug_layer)));
	debug_layer->EnableDebugLayer();

	//////////////////////////////////////////////////////////////////////////
	// Adapter + Device

	UINT				create_factory_flags = DXGI_CREATE_FACTORY_DEBUG;
	UINT				adapter_index = 0;
	ID3D12Device		*temp_device;


	DX_CHECK(CreateDXGIFactory2(create_factory_flags, PPV_ARGS(&dxgi_factory)));

	while (dxgi_factory->EnumAdapters1(adapter_index, &dxgi_adapter) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC1 desc;

		dxgi_adapter->GetDesc1(&desc);

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			adapter_index++;
			continue;
		}

		hr = D3D12CreateDevice(dxgi_adapter, D3D_FEATURE_LEVEL_11_0, PPV_ARGS(&temp_device));
		if (SUCCEEDED(hr))
		{
			DX_CHECK(temp_device->QueryInterface(PPV_ARGS(&device)));
			break;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Command queue

	D3D12_COMMAND_QUEUE_DESC		command_queue_desc = {};


	command_queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	command_queue_desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;

	DX_CHECK(device->CreateCommandQueue(&command_queue_desc, PPV_ARGS(&command_queue)));

	//////////////////////////////////////////////////////////////////////////
	// Swapchain

	DXGI_SWAP_CHAIN_DESC1		swapchain_desc = {};
	IDXGISwapChain1				*temp_swapchain;


	swapchain_desc.Width = SCR_WIDTH;
	swapchain_desc.Height = SCR_HEIGHT;
	swapchain_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchain_desc.SampleDesc = { 1, 0 };
	swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchain_desc.BufferCount = FRAMEBUFFER_COUNT;
	swapchain_desc.Scaling = DXGI_SCALING_NONE;
	swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapchain_desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;

	DX_CHECK(dxgi_factory->CreateSwapChainForHwnd(command_queue, hwnd, &swapchain_desc, nullptr, nullptr, &temp_swapchain));
	DX_CHECK(temp_swapchain->QueryInterface(&swapchain));

	//////////////////////////////////////////////////////////////////////////
    // Descriptor sizes

	cbv_descriptor_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	srv_descriptor_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	uav_descriptor_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	rtv_descriptor_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	dsv_descriptor_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

    //////////////////////////////////////////////////////////////////////////
    // RTV descriptor heap

	D3D12_DESCRIPTOR_HEAP_DESC		rtv_descriptor_heap_desc = {};


	rtv_descriptor_heap_desc.NumDescriptors = FRAMEBUFFER_COUNT;
	rtv_descriptor_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

	DX_CHECK(device->CreateDescriptorHeap(&rtv_descriptor_heap_desc, IID_PPV_ARGS(&rtv_descriptor_heap)));

	//////////////////////////////////////////////////////////////////////////
    // Render target views

	CD3DX12_CPU_DESCRIPTOR_HANDLE		rtv_handle(rtv_descriptor_heap->GetCPUDescriptorHandleForHeapStart());


	for (int i = 0; i < FRAMEBUFFER_COUNT; i++)
	{
		ID3D12Resource *backbuffer;

		DX_CHECK(swapchain->GetBuffer(i, IID_PPV_ARGS(&backbuffer)));
		device->CreateRenderTargetView(backbuffer, nullptr, rtv_handle);

		backbuffers[i] = backbuffer;
		rtv_handle.Offset(rtv_descriptor_size);
	}

    //////////////////////////////////////////////////////////////////////////
    // Depth/stencil buffer

	D3D12_DESCRIPTOR_HEAP_DESC			dsv_descriptor_heap_desc = {};
	D3D12_DEPTH_STENCIL_VIEW_DESC		dsv_desc = {};
	D3D12_CLEAR_VALUE					depth_clear_value = {};
	CD3DX12_CPU_DESCRIPTOR_HANDLE		dsv_handle;


	dsv_descriptor_heap_desc.NumDescriptors = 3;
	dsv_descriptor_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

	DX_CHECK(device->CreateDescriptorHeap(&dsv_descriptor_heap_desc, IID_PPV_ARGS(&dsv_descriptor_heap)));

	dsv_handle = dsv_descriptor_heap->GetCPUDescriptorHandleForHeapStart();

	dsv_desc.Format = DXGI_FORMAT_D32_FLOAT;
	dsv_desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsv_desc.Flags = D3D12_DSV_FLAG_NONE;

	depth_clear_value.Format = DXGI_FORMAT_D32_FLOAT;
	depth_clear_value.DepthStencil.Depth = 1.0f;
	depth_clear_value.DepthStencil.Stencil = 0;

	for (int i = 0; i < FRAMEBUFFER_COUNT; i++)
	{
		device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, SCR_WIDTH, SCR_HEIGHT, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL),
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&depth_clear_value,
			IID_PPV_ARGS(&depth_buffers[i]));

		device->CreateDepthStencilView(depth_buffers[i], &dsv_desc, dsv_handle);
		dsv_handle.Offset(dsv_descriptor_size);
	}

    //////////////////////////////////////////////////////////////////////////
    // Command allocator + list

	for (int i = 0; i < FRAMEBUFFER_COUNT; i++)
	{
		DX_CHECK(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&command_allocators[i])));
	}

	DX_CHECK(device->CreateCommandList1(0, D3D12_COMMAND_LIST_TYPE_DIRECT, D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(&command_list)));

    //////////////////////////////////////////////////////////////////////////
    // Fences

	DX_CHECK(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));

	fence_event = CreateEvent(0, 0, 0, 0);

    //////////////////////////////////////////////////////////////////////////
    // Viewport + scissor

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = SCR_WIDTH;
	viewport.Height = SCR_HEIGHT;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	scissor_rect.left = 0;
	scissor_rect.top = 0;
	scissor_rect.right = SCR_WIDTH;
	scissor_rect.bottom = SCR_HEIGHT;

	//////////////////////////////////////////////////////////////////////////
	// Main loop

	for (;;)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			const FLOAT clear_color[] = { 0.f, 0.2f, 0.4f, 1.f };
			ID3D12CommandList *cmdlists[] = { command_list };

			hr = command_allocators[backbuffer_index]->Reset();
			hr = command_list->Reset(command_allocators[backbuffer_index], nullptr);

			rtv_handle.ptr = rtv_descriptor_heap->GetCPUDescriptorHandleForHeapStart().ptr + backbuffer_index * rtv_descriptor_size;
			dsv_handle.ptr = dsv_descriptor_heap->GetCPUDescriptorHandleForHeapStart().ptr + backbuffer_index * dsv_descriptor_size;

			command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backbuffers[backbuffer_index], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

			command_list->OMSetRenderTargets(1, &rtv_handle, FALSE, &dsv_handle);
			command_list->ClearRenderTargetView(rtv_handle, clear_color, 0, nullptr);
			command_list->ClearDepthStencilView(dsv_handle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

			command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backbuffers[backbuffer_index], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

			command_list->Close();

			command_queue->ExecuteCommandLists(1, cmdlists);
			hr = swapchain->Present(0, 0);

			move_to_next_frame();
		}
	}

	return (0);
}

LRESULT CALLBACK
wndproc(HWND hwnd,
		UINT msg,
		WPARAM wparam,
		LPARAM lparam)
{
	LRESULT		result = 0;


	switch (msg)
	{
		case WM_KEYDOWN:
		{
			if (wparam == VK_ESCAPE)
			{
				PostQuitMessage(0);
			}
		} break;

		case WM_CLOSE:
		case WM_DESTROY:
		{
			PostQuitMessage(0);
		} break;

		default:
		{
			result = DefWindowProc(hwnd, msg, wparam, lparam);
		} break;
	}

	return (result);
}

void 
move_to_next_frame()
{
	UINT64		current_fence_value = fence_values[backbuffer_index];


	DX_CHECK(command_queue->Signal(fence, current_fence_value));

	backbuffer_index = swapchain->GetCurrentBackBufferIndex();

	if (fence->GetCompletedValue() < fence_values[backbuffer_index])
	{
		DX_CHECK(fence->SetEventOnCompletion(fence_values[backbuffer_index], fence_event));
		WaitForSingleObject(fence_event, INFINITE);
	}

	fence_values[backbuffer_index] = current_fence_value + 1;
}

