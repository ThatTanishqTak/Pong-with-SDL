// Include necessary headers
#include <Windows.h>
#include "utilities.cpp"

// Declare a global variable to control the main game loop
global_variable bool running = true;

// Define a structure to hold render state information
struct Render_State
{
	int width, height;
	void* memory;

	BITMAPINFO bitmap_info;
};

// Declare a global variable of type Render_State
global_variable Render_State render_state;

// Include additional source files
#include "platform_common.cpp"
#include "renderer.cpp"
#include "game.cpp"

// Window message callback function
LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	// Handle different window messages
	switch (uMsg)
	{
		// Handle window close and destroy messages to exit the program
		case WM_CLOSE:
		case WM_DESTROY: { running = false; } break;

		// Handle window size change
		case WM_SIZE:
		{
			RECT rect;
			GetClientRect(hwnd, &rect);

			// Update render state dimensions based on window size
			render_state.width = rect.right - rect.left;
			render_state.height = rect.bottom - rect.top;

			int size = render_state.width * render_state.height * sizeof(u32);

			// Allocate memory for rendering
			if (render_state.memory) { VirtualFree(render_state.memory, 0, MEM_RELEASE); }

			render_state.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

			// Configure bitmap info for rendering
			render_state.bitmap_info.bmiHeader.biSize = sizeof(render_state.bitmap_info.bmiHeader);
			render_state.bitmap_info.bmiHeader.biWidth = render_state.width;
			render_state.bitmap_info.bmiHeader.biHeight = render_state.height;
			render_state.bitmap_info.bmiHeader.biPlanes = 1;
			render_state.bitmap_info.bmiHeader.biBitCount = 32;
			render_state.bitmap_info.bmiHeader.biCompression = BI_RGB;
		} break;

		// Use default window procedure for unhandled messages
		default: { result = DefWindowProc(hwnd, uMsg, wParam, lParam); }
	}

	return result;
}

// Main entry point of the application
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// Create window class
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpszClassName = "PONG";
	window_class.lpfnWndProc = window_callback;

	// Register the window class
	RegisterClass(&window_class);

	// Create the main window
	HWND window = CreateWindow(window_class.lpszClassName, "PONG", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT,
		CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
	HDC hdc = GetDC(window);

	// Initialize input state
	Input input = {};

	// Initialize time-related variables
	float delta_time = 0.01666666f;
	LARGE_INTEGER frame_begin_time;
	QueryPerformanceCounter(&frame_begin_time);

	float performace_frequency;
	{
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		performace_frequency = (float)perf.QuadPart;
	}

	// Main game loop
	while (running)
	{
		// Input handling
		MSG message;

		// Reset button state change flags
		for (int i = 0; i < BUTTON_COUNT; i++) { input.buttons[i].changed = false; }

		// Process Windows messages
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE))
		{
			switch (message.message)
			{
				case WM_KEYUP:
				case WM_KEYDOWN:
				{
					u32 vk_code = (u32)message.wParam;
					bool is_down = ((message.lParam & (1 << 31)) == 0);

		#define process_button(key, vk)\
		case vk: {\
		input.buttons[key].changed = is_down != input.buttons[key].is_down;\
		input.buttons[key].is_down = is_down;\
		} break;

					switch (vk_code)
					{
						process_button(BUTTON_UP, VK_UP);
						process_button(BUTTON_DOWN, VK_DOWN);
						process_button(BUTTON_W, 'W');
						process_button(BUTTON_S, 'S');
					}

				} break;

				default:
				{
					TranslateMessage(&message);
					DispatchMessage(&message);
				}
			}
		}

		// Update game logic
		update_game(&input, delta_time);

		// Render the frame
		StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory,
			&render_state.bitmap_info, DIB_RGB_COLORS, SRCCOPY);

		// Calculate and update delta time
		LARGE_INTEGER frame_end_time;
		QueryPerformanceCounter(&frame_end_time);

		delta_time = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart) / performace_frequency;
		frame_begin_time = frame_end_time;
	}
}