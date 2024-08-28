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
	DWORD			window_style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
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

	AdjustWindowRect(&wnddim, window_style, FALSE);
	window_width = wnddim.right - wnddim.left;
	window_height = wnddim.bottom - wnddim.top;

	hwnd = CreateWindowEx(0, wndclass.lpszClassName, "qjulia", window_style,
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
    // Debug Info Queue

	ID3D12InfoQueue				*info_queue;
	D3D12_INFO_QUEUE_FILTER		new_filter = {};
	D3D12_MESSAGE_SEVERITY		severities[] =
	{
            D3D12_MESSAGE_SEVERITY_INFO
	};
	D3D12_MESSAGE_ID 			deny_ids[] =
	{
		D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,
		D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,                      
		D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE,                    
	};


	hr = device->QueryInterface(IID_PPV_ARGS(&info_queue));

	info_queue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
   	info_queue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
   	info_queue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);

	// Suppress individual messages by their ID
	new_filter.DenyList.NumSeverities = _countof(severities);
	new_filter.DenyList.pSeverityList = severities;
	new_filter.DenyList.NumIDs = _countof(deny_ids);
	new_filter.DenyList.pIDList = deny_ids;

    DX_CHECK(info_queue->PushStorageFilter(&new_filter));

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

	backbuffers[0]->SetName(L"Backbuffer 0");
	backbuffers[1]->SetName(L"Backbuffer 1");
	backbuffers[2]->SetName(L"Backbuffer 2");

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

	depth_buffers[0]->SetName(L"Depth buffer 0");
	depth_buffers[1]->SetName(L"Depth buffer 1");
	depth_buffers[2]->SetName(L"Depth buffer 2");

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
								ps_code,
								cs_code;
	ID3DBlob					*vs_blob,
								*ps_blob,
								*cs_blob;


	hr = D3DReadFileToBlob(L"julia_vs.cso", &vs_blob);
	hr = D3DReadFileToBlob(L"julia_ps.cso", &ps_blob);
	hr = D3DReadFileToBlob(L"julia_cs.cso", &cs_blob);

	vs_code.BytecodeLength = vs_blob->GetBufferSize();
	vs_code.pShaderBytecode = vs_blob->GetBufferPointer();
	ps_code.BytecodeLength = ps_blob->GetBufferSize();
	ps_code.pShaderBytecode = ps_blob->GetBufferPointer();
	cs_code.BytecodeLength = cs_blob->GetBufferSize();
	cs_code.pShaderBytecode = cs_blob->GetBufferPointer();

	//////////////////////////////////////////////////////////////////////////
	// Static sampler

	D3D12_STATIC_SAMPLER_DESC		sampler = {};


	sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	sampler.MinLOD = 0;
	sampler.MaxLOD = D3D12_FLOAT32_MAX;
	sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	//////////////////////////////////////////////////////////////////////////
	// Root signature

	ID3DBlob						*signature;
	D3D12_ROOT_SIGNATURE_DESC		gfx_signature_desc = {},
									cs_signature_desc = {};


	// Graphics signature
	{
		D3D12_ROOT_PARAMETER			root_parameters[1] = {};
		D3D12_DESCRIPTOR_RANGE			descriptor_ranges[1] = {};
		D3D12_ROOT_DESCRIPTOR_TABLE		descriptor_table = {};

		descriptor_ranges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		descriptor_ranges[0].NumDescriptors = 1;
		descriptor_ranges[0].BaseShaderRegister = 0;
		descriptor_ranges[0].RegisterSpace = 0;
		descriptor_ranges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		descriptor_table.NumDescriptorRanges = _countof(descriptor_ranges);
		descriptor_table.pDescriptorRanges = descriptor_ranges;

		root_parameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		root_parameters[0].DescriptorTable = descriptor_table;
		root_parameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		gfx_signature_desc.NumParameters = _countof(root_parameters);
		gfx_signature_desc.pParameters = root_parameters;
		gfx_signature_desc.NumStaticSamplers = 1;
		gfx_signature_desc.pStaticSamplers = &sampler;
		gfx_signature_desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		DX_CHECK(D3D12SerializeRootSignature(&gfx_signature_desc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, nullptr));
		DX_CHECK(device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), PPV_ARGS(&gfx_signature)));
	}

	// Compute signature
	{
		D3D12_ROOT_PARAMETER			root_parameters[2] = {};
		D3D12_ROOT_DESCRIPTOR			root_descriptor = {};
		D3D12_DESCRIPTOR_RANGE			descriptor_ranges[1] = {};
		D3D12_ROOT_DESCRIPTOR_TABLE		descriptor_table = {};

		root_descriptor.ShaderRegister = 0;
		root_descriptor.RegisterSpace = 0;

		descriptor_ranges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
		descriptor_ranges[0].NumDescriptors = 1;
		descriptor_ranges[0].BaseShaderRegister = 0;
		descriptor_ranges[0].RegisterSpace = 0;
		descriptor_ranges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		descriptor_table.NumDescriptorRanges = _countof(descriptor_ranges);
		descriptor_table.pDescriptorRanges = descriptor_ranges;

		root_parameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		root_parameters[0].Descriptor = root_descriptor;
		root_parameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		root_parameters[1].DescriptorTable = descriptor_table;
		root_parameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		cs_signature_desc.NumParameters = _countof(root_parameters);
		cs_signature_desc.pParameters = root_parameters;
		cs_signature_desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;

		DX_CHECK(D3D12SerializeRootSignature(&cs_signature_desc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, nullptr));
		DX_CHECK(device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), PPV_ARGS(&cs_signature)));
	}

	gfx_signature->SetName(L"Gfx signature");
	cs_signature->SetName(L"CS signature");

	//////////////////////////////////////////////////////////////////////////
	// Vertex buffers

	void		*ptr;
	f32			vertices[] =
	{
		 1.0f,  1.0f, 0.0f, 1.0f,	1.0f, 0.0f,
         1.0f, -1.0f, 0.0f, 1.0f,	1.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 1.0f,	0.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f,	0.0f, 0.0f
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
	vb_view.StrideInBytes = 6 * sizeof(f32);

	ib_view.BufferLocation = ib->GetGPUVirtualAddress();
	ib_view.SizeInBytes = sizeof(indices);
	ib_view.Format = DXGI_FORMAT_R32_UINT;

	vb->SetName(L"Vertex buffer");
	ib->SetName(L"Index buffer");

	//////////////////////////////////////////////////////////////////////////
	// Input layout

	D3D12_INPUT_ELEMENT_DESC		element_desc[2] = {};
	D3D12_INPUT_LAYOUT_DESC			input_layout = {};


	element_desc[0].SemanticName = "POSITION";
	element_desc[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	element_desc[0].InputSlot = 0;
	element_desc[0].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
	element_desc[1].SemanticName = "TEXCOORD";
	element_desc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	element_desc[1].InputSlot = 0;
	element_desc[1].AlignedByteOffset = 4 * sizeof(f32);
	element_desc[1].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;

	input_layout.NumElements = _countof(element_desc);
	input_layout.pInputElementDescs = element_desc;

	//////////////////////////////////////////////////////////////////////////
	// Texture + Descriptor heap

	D3D12_RESOURCE_DESC						texture_desc = {};
	D3D12_SHADER_RESOURCE_VIEW_DESC			srv_desc = {};
	D3D12_UNORDERED_ACCESS_VIEW_DESC		uav_desc = {};


	texture_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texture_desc.Alignment = 0;
	texture_desc.Width = SCR_WIDTH;
	texture_desc.Height = SCR_HEIGHT;
	texture_desc.DepthOrArraySize = 1;
	texture_desc.MipLevels = 1;
	texture_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texture_desc.SampleDesc = { 1, 0 };
	texture_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	texture_desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

	for (u32 i = 0; i < FRAMEBUFFER_COUNT; i++)
	{
		device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&texture_desc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			nullptr,
			PPV_ARGS(&texture[i]));
	}

	texture[0]->SetName(L"Texture 0");
	texture[1]->SetName(L"Texture 1");
	texture[2]->SetName(L"Texture 2");

	{
		D3D12_DESCRIPTOR_HEAP_DESC heap_desc = {};

		heap_desc.NumDescriptors = 6; // 6 descriptors, 3 for the SRVs and 3 for the UAVs
		heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

		DX_CHECK(device->CreateDescriptorHeap(&heap_desc, PPV_ARGS(&descriptor_heap)));
	}

	srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srv_desc.Format = texture_desc.Format;
	srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srv_desc.Texture2D.MipLevels = 1;

	uav_desc.Format = texture_desc.Format;
	uav_desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;

	CD3DX12_CPU_DESCRIPTOR_HANDLE heap_handle(descriptor_heap->GetCPUDescriptorHandleForHeapStart());

	for (u32 i = 0; i < FRAMEBUFFER_COUNT; i++)
	{
		device->CreateShaderResourceView(texture[i], &srv_desc, heap_handle);
		heap_handle.Offset(srv_descriptor_size);
		device->CreateUnorderedAccessView(texture[i], nullptr, &uav_desc, heap_handle);
		heap_handle.Offset(uav_descriptor_size);
	}

	//////////////////////////////////////////////////////////////////////////
	// Pipeline setup

	D3D12_GRAPHICS_PIPELINE_STATE_DESC		gfx_pipeline_desc = {};
	D3D12_COMPUTE_PIPELINE_STATE_DESC		cs_pipeline_desc = {};


	gfx_pipeline_desc.InputLayout = input_layout;
	gfx_pipeline_desc.pRootSignature = gfx_signature;
	gfx_pipeline_desc.VS = vs_code;
	gfx_pipeline_desc.PS = ps_code;
	gfx_pipeline_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gfx_pipeline_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	gfx_pipeline_desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	gfx_pipeline_desc.SampleDesc = { 1, 0 };
	gfx_pipeline_desc.SampleMask = 0xFFFFFFFF;
	gfx_pipeline_desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gfx_pipeline_desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	gfx_pipeline_desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gfx_pipeline_desc.NumRenderTargets = 1;

	cs_pipeline_desc.pRootSignature = cs_signature;
	cs_pipeline_desc.CS = cs_code;

	DX_CHECK(device->CreateGraphicsPipelineState(&gfx_pipeline_desc, PPV_ARGS(&gfx_pipeline)));
	DX_CHECK(device->CreateComputePipelineState(&cs_pipeline_desc, PPV_ARGS(&cs_pipeline)));

	gfx_pipeline->SetName(L"Gfx pipeline");
	cs_pipeline->SetName(L"CS pipeline");

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

	params_buffer[0]->SetName(L"Params 0");
	params_buffer[1]->SetName(L"Params 1");
	params_buffer[2]->SetName(L"Params 2");

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

	D3D12_GPU_DESCRIPTOR_HANDLE		srv_handle,
									uav_handle;


	srv_handle = descriptor_heap->GetGPUDescriptorHandleForHeapStart();
	uav_handle.ptr = srv_handle.ptr + srv_descriptor_size;

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
			ID3D12DescriptorHeap *descriptor_heaps[] = { descriptor_heap };

			// ImGui
			ImGui_ImplDX12_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Controls");
				ImGui::SliderFloat4("Mu", &params.mu.x, -1, 1, "%0.5f");
				ImGui::Checkbox("Self Shadow", (bool *)&params.self_shadow);
				ImGui::SliderFloat("Epsilon", &params.epsilon, 0.0001f, 0.01f, "%0.5f");
				ImGui::SliderInt("Iterations", &params.iterations, 5, 25);
            ImGui::End();
			ImGui::Render();

			params.rotation = trackball.get_rotation_matrix();
			CopyMemory(params_ptrs[backbuffer_index], &params, sizeof(params));

			// -------- Update pipeline -------- //
			hr = command_allocators[backbuffer_index]->Reset();
			hr = command_list->Reset(command_allocators[backbuffer_index], nullptr);

			rtv_handle.ptr = rtv_descriptor_heap->GetCPUDescriptorHandleForHeapStart().ptr + backbuffer_index * rtv_descriptor_size;
			dsv_handle.ptr = dsv_descriptor_heap->GetCPUDescriptorHandleForHeapStart().ptr + backbuffer_index * dsv_descriptor_size;

			srv_handle.ptr = descriptor_heap->GetGPUDescriptorHandleForHeapStart().ptr + backbuffer_index * srv_descriptor_size;
			uav_handle.ptr = descriptor_heap->GetGPUDescriptorHandleForHeapStart().ptr + (backbuffer_index + 1) * uav_descriptor_size;

			// -------- Compute pass -------- //
			command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texture[backbuffer_index], D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS));
			command_list->SetPipelineState(cs_pipeline);
			command_list->SetComputeRootSignature(cs_signature);
			command_list->SetDescriptorHeaps(1, descriptor_heaps);
			command_list->SetComputeRootConstantBufferView(0, params_buffer[backbuffer_index]->GetGPUVirtualAddress());
			command_list->SetComputeRootDescriptorTable(1, uav_handle);
			command_list->Dispatch(SCR_WIDTH / 16, SCR_HEIGHT / 16, 1);
			command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texture[backbuffer_index], D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
			
			// -------- Draw -------- //
			command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backbuffers[backbuffer_index], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
			command_list->OMSetRenderTargets(1, &rtv_handle, FALSE, &dsv_handle);
			command_list->ClearRenderTargetView(rtv_handle, clear_color, 0, nullptr);
			command_list->ClearDepthStencilView(dsv_handle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

			command_list->SetPipelineState(gfx_pipeline);
			command_list->SetGraphicsRootSignature(gfx_signature);
			command_list->SetGraphicsRootDescriptorTable(0, srv_handle);
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
					trackball.drag_start(x, y, SCR_WIDTH, SCR_HEIGHT);
					lbdown = TRUE;
				}
				if (msg == WM_MOUSEMOVE && lbdown)
				{
					trackball.drag_move(x, y, SCR_WIDTH, SCR_HEIGHT);
				}
				if (msg == WM_LBUTTONUP)
				{
					trackball.drag_end();
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

