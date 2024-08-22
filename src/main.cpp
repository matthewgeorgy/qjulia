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


	dsv_descriptor_heap_desc.NumDescriptors = FRAMEBUFFER_COUNT;
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
	// Shader setup

	D3D12_SHADER_BYTECODE		vs_code,
								ps_code;
	ID3DBlob					*vs_blob;
	ID3DBlob					*ps_blob;


	hr = D3DReadFileToBlob(L"bin/julia_vs.cso", &vs_blob);
	hr = D3DReadFileToBlob(L"bin/julia_ps.cso", &ps_blob);

	vs_code.BytecodeLength = vs_blob->GetBufferSize();
	vs_code.pShaderBytecode = vs_blob->GetBufferPointer();
	ps_code.BytecodeLength = ps_blob->GetBufferSize();
	ps_code.pShaderBytecode = ps_blob->GetBufferPointer();

	//////////////////////////////////////////////////////////////////////////
	// Root signature

	ID3DBlob						*signature;
	D3D12_ROOT_PARAMETER			root_parameters[1] = {};
	D3D12_ROOT_DESCRIPTOR			params_descriptor = {};
	D3D12_ROOT_SIGNATURE_DESC		root_signature_desc = {};


	params_descriptor.ShaderRegister = 0;

	root_parameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	root_parameters[0].Descriptor = params_descriptor;

	root_signature_desc.NumParameters = _countof(root_parameters);
	root_signature_desc.pParameters = root_parameters;
	root_signature_desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	DX_CHECK(D3D12SerializeRootSignature(&root_signature_desc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, nullptr));
	DX_CHECK(device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), PPV_ARGS(&root_signature)));

	//////////////////////////////////////////////////////////////////////////
	// Vertex buffers

	void		*ptr;
	f32			vertices[] =
	{
		 1.0f,  1.0f, 0.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f,
	};	
	u32			indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};
	
	hr = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		PPV_ARGS(&vb));

	hr = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(indices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		PPV_ARGS(&ib));

	vb->Map(0, nullptr, &ptr);
		CopyMemory(ptr, vertices, sizeof(vertices));
	vb->Unmap(0, nullptr);

	ib->Map(0, nullptr, &ptr);
		CopyMemory(ptr, indices, sizeof(indices));
	ib->Unmap(0, nullptr);

	vb_view.BufferLocation = vb->GetGPUVirtualAddress();
	vb_view.SizeInBytes = sizeof(vertices);
	vb_view.StrideInBytes = 4 * sizeof(f32);

	ib_view.BufferLocation = ib->GetGPUVirtualAddress();
	ib_view.SizeInBytes = sizeof(indices);
	ib_view.Format = DXGI_FORMAT_R32_UINT;

	//////////////////////////////////////////////////////////////////////////
	// Input layout

	D3D12_INPUT_ELEMENT_DESC		element_desc[1] = {};
	D3D12_INPUT_LAYOUT_DESC			input_layout = {};


	element_desc[0].SemanticName = "POSITION";
	element_desc[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	element_desc[0].InputSlot = 0;
	element_desc[0].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;

	input_layout.NumElements = _countof(element_desc);
	input_layout.pInputElementDescs = element_desc;

	//////////////////////////////////////////////////////////////////////////
	// Pipeline setup

	D3D12_GRAPHICS_PIPELINE_STATE_DESC		pipeline_desc = {};


	pipeline_desc.InputLayout = input_layout;
	pipeline_desc.pRootSignature = root_signature;
	pipeline_desc.VS = vs_code;
	pipeline_desc.PS = ps_code;
	pipeline_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	pipeline_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	pipeline_desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	pipeline_desc.SampleDesc = { 1, 0 };
	pipeline_desc.SampleMask = 0xFFFFFFFF;
	pipeline_desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	pipeline_desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	pipeline_desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	pipeline_desc.NumRenderTargets = 1;

	DX_CHECK(device->CreateGraphicsPipelineState(&pipeline_desc, PPV_ARGS(&pipeline)));

	//////////////////////////////////////////////////////////////////////////
	// Params buffer

	for (u32 i = 0; i < FRAMEBUFFER_COUNT; i++)
	{
		hr = device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeof(qjulia_params_t)),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			PPV_ARGS(&params_buffer[i]));

		hr = params_buffer[i]->Map(0, nullptr, (void **)&params_ptrs[i]);
	}

	params.mu = vec4(-0.278f, -0.479f, -0.231f, 0.235f);
	params.width = SCR_WIDTH;
	params.height = SCR_HEIGHT;
	params.self_shadow = TRUE;
	params.epsilon = 0.003f;
	params.zoom = 1.0f;
	params.iterations = 10;
	LONG cols = GetSystemMetrics(SM_CXSCREEN);
	LONG rows = GetSystemMetrics(SM_CYSCREEN);
	SetCursorPos(cols / 2, rows / 2);

	//////////////////////////////////////////////////////////////////////////
	// ImGui init

	D3D12_DESCRIPTOR_HEAP_DESC		imgui_heap_desc = {};
	ID3D12DescriptorHeap			*imgui_heap;


	imgui_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	imgui_heap_desc.NumDescriptors = 1;
	imgui_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	DX_CHECK(device->CreateDescriptorHeap(&imgui_heap_desc, PPV_ARGS(&imgui_heap)));

	ImGui::CreateContext();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX12_Init(device, FRAMEBUFFER_COUNT, DXGI_FORMAT_R8G8B8A8_UNORM, imgui_heap,
			imgui_heap->GetCPUDescriptorHandleForHeapStart(),
			imgui_heap->GetGPUDescriptorHandleForHeapStart());

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

			// ImGui
			ImGui_ImplDX12_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Controls");
				ImGui::SliderFloat4("Mu", &params.mu.x, -1, 1, "%0.5f");
				ImGui::Checkbox("Self Shadow", (bool *)&params.self_shadow);
				ImGui::SliderFloat("Epsilon", &params.epsilon, 0.0001f, 0.01f, "%0.5f");
				/* ImGui::SliderFloat("Zoom", &params.zoom, 0, 2, "%0.4f"); */
				ImGui::SliderInt("Iterations", &params.iterations, 5, 25);
            ImGui::End();
			ImGui::Render();

			params.rotation = trackball.GetRotationMatrix();
			CopyMemory(params_ptrs[backbuffer_index], &params, sizeof(params));

			// -------- Update pipeline -------- //
			hr = command_allocators[backbuffer_index]->Reset();
			hr = command_list->Reset(command_allocators[backbuffer_index], pipeline);

			rtv_handle.ptr = rtv_descriptor_heap->GetCPUDescriptorHandleForHeapStart().ptr + backbuffer_index * rtv_descriptor_size;
			dsv_handle.ptr = dsv_descriptor_heap->GetCPUDescriptorHandleForHeapStart().ptr + backbuffer_index * dsv_descriptor_size;

			command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backbuffers[backbuffer_index], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
			command_list->OMSetRenderTargets(1, &rtv_handle, FALSE, &dsv_handle);
			command_list->ClearRenderTargetView(rtv_handle, clear_color, 0, nullptr);
			command_list->ClearDepthStencilView(dsv_handle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

			// -------- Draw -------- //
			command_list->SetGraphicsRootSignature(root_signature);
			command_list->SetGraphicsRootConstantBufferView(0, params_buffer[backbuffer_index]->GetGPUVirtualAddress());
			command_list->RSSetViewports(1, &viewport);
			command_list->RSSetScissorRects(1, &scissor_rect);
			command_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			command_list->IASetVertexBuffers(0, 1, &vb_view);
			command_list->IASetIndexBuffer(&ib_view);
			command_list->DrawIndexedInstanced(6, 1, 0, 0, 0);

			command_list->SetDescriptorHeaps(1, &imgui_heap);
			ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), command_list);
			command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backbuffers[backbuffer_index], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
			command_list->Close();

			// -------- Present -------- //
			command_queue->ExecuteCommandLists(1, cmdlists);
			hr = swapchain->Present(0, 0);

			move_to_next_frame();
		}
	}

	return (0);
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK
wndproc(HWND hwnd,
        UINT msg,
        WPARAM wparam,
        LPARAM lparam)
{
    LRESULT         result = 0;
	static b32		lbdown = FALSE; 


	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
	{
		return (result);
	}

    switch (msg)
    {
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MOUSEMOVE:
		{
			ImGuiIO &io = ImGui::GetIO();

			if (!io.WantCaptureMouse)
			{
				LONG x = GET_X_LPARAM(lparam); 
				LONG y = GET_Y_LPARAM(lparam); 

				if (msg == WM_LBUTTONDOWN && !lbdown)
				{
					trackball.DragStart(x, y, SCR_WIDTH, SCR_HEIGHT);
					lbdown = TRUE;
				}
				if (msg == WM_MOUSEMOVE && lbdown)
				{
					trackball.DragMove(x, y, SCR_WIDTH, SCR_HEIGHT);
				}
				if (msg == WM_LBUTTONUP)
				{
					trackball.DragEnd();
					lbdown = FALSE;
				}
			}
		} break;

		case WM_MOUSEWHEEL:
		{
			s16 wheel_delta = HIWORD(wparam);

			if (wheel_delta > 0)
			{
				params.zoom *= 1.1f;
			}
			else
			{
				params.zoom /= 1.1f;
			}
		} break;

        case WM_KEYDOWN:
        {
            if (wparam == VK_ESCAPE)
            {
                PostQuitMessage(0);
            }
        } break;

        case WM_CLOSE:
        {
            DestroyWindow(hwnd);
        } break;

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

