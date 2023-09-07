#include <Windows.h>

BITMAPINFO buffer_bitmap_info;

void* buffer_memory;
int buffer_width;
int buffer_height;
bool running = true;

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

			buffer_width = rect.right - rect.left;
			buffer_height = rect.bottom - rect.top;

			int buffer_size = buffer_width * buffer_height * sizeof(unsigned int);

			if (buffer_memory) { VirtualFree(buffer_memory, 0, MEM_RELEASE); }
			buffer_memory = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

			buffer_bitmap_info.bmiHeader.biSize = sizeof(buffer_bitmap_info.bmiHeader);
			buffer_bitmap_info.bmiHeader.biWidth = buffer_width;
			buffer_bitmap_info.bmiHeader.biHeight = buffer_height;
			buffer_bitmap_info.bmiHeader.biPlanes = 1;
			buffer_bitmap_info.bmiHeader.biBitCount = 32;
			buffer_bitmap_info.bmiHeader.biCompression = BI_RGB;
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
		unsigned int* pixel = (unsigned int*)buffer_memory;
		for (int i = 0; i < buffer_height; i++)
		{
			for (int j = 0; j < buffer_width; j++)
				*pixel++ = 0x000000;
		}

		// Render
		StretchDIBits(hdc, 0, 0, buffer_width, buffer_height, 0, 0, buffer_width, buffer_height, buffer_memory,
			&buffer_bitmap_info, DIB_RGB_COLORS, SRCCOPY);
	}
}