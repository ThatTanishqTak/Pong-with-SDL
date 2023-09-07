internal void clear_screen(u32 color)
{
	u32* pixel = (u32*)render_state.memory;
	for (int i = 0; i < render_state.height; i++)
	{
		for (int j = 0; j < render_state.width; j++)
			*pixel++ = color;
	}
}

internal void draw_rect_in_pixels(int x0, int y0, int x1, int y1, u32 color)
{
	x0 = clamp(0, x0, render_state.width);
	x1 = clamp(0, x1, render_state.width);
	y0 = clamp(0, y0, render_state.height);
	y1 = clamp(0, y1, render_state.height);

	for (int i = y0; i < y1; i++)
	{
		u32* pixel = (u32*)render_state.memory + x0 + i * render_state.width;
		for (int j = x0; j < x1; j++)
			*pixel++ = color;
	}
}

global_variable float render_scale = 0.01f;

internal void draw_rect(float x, float y, float half_size_x, float half_size_y, u32 color)
{
	x *= render_state.height * render_scale;
	y *= render_state.height * render_scale;
	half_size_x *= render_state.height * render_scale;
	half_size_y *= render_state.height * render_scale;

	x += render_state.width / 2.0f;
	y += render_state.height / 2.0f;

	int x0 = x - half_size_x;
	int y0 = y - half_size_y;
	int x1 = x + half_size_x;
	int y1 = y + half_size_y;

	draw_rect_in_pixels(x0, y0, x1, y1, color);
}