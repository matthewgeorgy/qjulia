#include <windows.h>
#include <stdio.h>

#define SCR_WIDTH		1024
#define SCR_HEIGHT 		768

LRESULT CALLBACK wndproc(HWND, UINT, WPARAM, LPARAM);

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

