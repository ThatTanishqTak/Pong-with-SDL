#include <Windows.h>

bool running = true;

struct Render_State
{
	int width, height;
	void* memory;

	BITMAPINFO bitmap_info;
};

Render_State render_state;

LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	switch (uMsg)
	{
		case WM_CLOSE:
		case WM_DESTROY: { running = false; } break;

		case WM_SIZE:
		{
			RECT rect;
			GetClientRect(hwnd, &rect);

			render_state.width = rect.right - rect.left;
			render_state.height = rect.bottom - rect.top;

			int size = render_state.width * render_state.height * sizeof(unsigned int);

			if (render_state.memory) { VirtualFree(render_state.memory, 0, MEM_RELEASE); }
			render_state.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

			render_state.bitmap_info.bmiHeader.biSize = sizeof(render_state.bitmap_info.bmiHeader);
			render_state.bitmap_info.bmiHeader.biWidth = render_state.width;
			render_state.bitmap_info.bmiHeader.biHeight = render_state.height;
			render_state.bitmap_info.bmiHeader.biPlanes = 1;
			render_state.bitmap_info.bmiHeader.biBitCount = 32;
			render_state.bitmap_info.bmiHeader.biCompression = BI_RGB;
		} break;

		default: { result = DefWindowProc(hwnd, uMsg, wParam, lParam); }
	}

	return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// Create window class
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpszClassName = "PONG";
	window_class.lpfnWndProc = window_callback;

	// Create register class
	RegisterClass(&window_class);

	// Create window
	HWND window = CreateWindow(window_class.lpszClassName, "PONG", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, 
							   CW_USEDEFAULT, 1080, 720, 0, 0, hInstance, 0);
	HDC hdc = GetDC(window);

	while (running)
	{
		// Input
		MSG message;
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		// Update
		unsigned int* pixel = (unsigned int*)render_state.memory;
		for (int i = 0; i < render_state.height; i++)
		{
			for (int j = 0; j < render_state.width; j++)
				*pixel++ = 0x000000;
		}

		// Render
		StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory,
			&render_state.bitmap_info, DIB_RGB_COLORS, SRCCOPY);
	}
}