// Clear the screen with a specified color
internal void clear_screen(u32 color)
{
    // Cast the memory pointer to a pointer to 32-bit unsigned integers (pixels)
    u32* pixel = (u32*)render_state.memory;

    // Iterate through each row (height) of the screen
    for (int i = 0; i < render_state.height; i++)
    {
        // Iterate through each column (width) of the screen
        for (int j = 0; j < render_state.width; j++)
            *pixel++ = color; // Set the pixel color to the specified color
    }
}

// Draw a rectangle in pixels with a specified color and coordinates
internal void draw_rect_in_pixels(int x0, int y0, int x1, int y1, u32 color)
{
    // Ensure that the coordinates (x0, y0, x1, y1) are within the screen boundaries
    x0 = clamp(0, x0, render_state.width);
    x1 = clamp(0, x1, render_state.width);
    y0 = clamp(0, y0, render_state.height);
    y1 = clamp(0, y1, render_state.height);

    // Iterate through each row (height) of the rectangle
    for (int i = y0; i < y1; i++)
    {
        // Calculate the starting pixel pointer for the current row
        u32* pixel = (u32*)render_state.memory + x0 + i * render_state.width;

        // Iterate through each column (width) of the rectangle
        for (int j = x0; j < x1; j++)
            *pixel++ = color; // Set the pixel color to the specified color
    }
}

// Global variable representing the scaling factor for rendering
global_variable float render_scale = 0.01f;

// Draw a rectangle with a specified position, size, and color
internal void draw_rect(float x, float y, float half_size_x, float half_size_y, u32 color)
{
    // Scale the coordinates and size based on the rendering scale
    x *= render_state.height * render_scale;
    y *= render_state.height * render_scale;
    half_size_x *= render_state.height * render_scale;
    half_size_y *= render_state.height * render_scale;

    // Adjust the coordinates to be centered on the screen
    x += render_state.width / 2.0f;
    y += render_state.height / 2.0f;

    // Calculate the pixel coordinates for the rectangle's boundaries
    int x0 = x - half_size_x;
    int y0 = y - half_size_y;
    int x1 = x + half_size_x;
    int y1 = y + half_size_y;

    // Draw the rectangle in pixels using the previously defined function
    draw_rect_in_pixels(x0, y0, x1, y1, color);
}