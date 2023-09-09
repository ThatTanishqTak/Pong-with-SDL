// Define macros to simplify button state checks
#define is_down(key) input->buttons[key].is_down
#define pressed(key) (input->buttons[key].is_down && input->buttons[key].changed)
#define released(key) !(input->buttons[key].is_down && input->buttons[key].changed)

// Declare global variables for player positions, velocities, arena size, and ball properties
float player_1_pos, player_1_dp, player_2_pos, player_2_dp;
float arena_half_size_x = 85.0f;
float arena_half_size_y = 45.0f;
float player_half_size_x = 2.5f;
float player_half_size_y = 12.0f;
float ball_pos_x, ball_pos_y, ball_dp_x = 100.0f, ball_dp_y;
float ball_half_size = 1.0f;

// Function to update player position and velocity
internal void update_player(float* pos, float* dp, float ddp, float delta_time)
{
    ddp -= *dp * 10.0f;

    *pos = *pos + *dp * delta_time + ddp * delta_time * delta_time * 0.5f;
    *dp = *dp + ddp * delta_time;

    if (*pos + player_half_size_y > arena_half_size_y)
    {
        *pos = arena_half_size_y - player_half_size_y;
        *dp = 0;
    }
    else if (*pos - player_half_size_y < -arena_half_size_y)
    {
        *pos = -arena_half_size_y + player_half_size_y;
        *dp = 0;
    }
}

// Function to check if two axis-aligned bounding boxes (AABBs) overlap
internal bool aabb_vs_aabb(float p1x, float p1y, float hs1x, float hs1y,
    float p2x, float p2y, float hs2x, float hs2y)
{
    return (p1x + hs1x > p2x - hs2x &&
        p1x - hs1x < p2x + hs2x &&
        p1y + hs1y > p2y - hs2y &&
        p1y + hs1y < p2y + hs2y);
}

// Function to update the game state
internal void update_game(Input* input, float delta_time)
{
    // Clear the screen with a gray color
    clear_screen(0x808080);

    // Draw the game arena
    draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0x000000);

    // Calculate player 1's desired velocity based on input
    float player_1_ddp = 0.0f;
    if (is_down(BUTTON_W)) { player_1_ddp += 2000; }
    if (is_down(BUTTON_S)) { player_1_ddp -= 2000; }

    // Calculate player 2's desired velocity based on input
    float player_2_ddp = 0.0f;
    if (is_down(BUTTON_UP)) { player_2_ddp += 2000; }
    if (is_down(BUTTON_DOWN)) { player_2_ddp -= 2000; }

    // Update player 1 and player 2 positions and velocities
    update_player(&player_1_pos, &player_1_dp, player_1_ddp, delta_time);
    update_player(&player_2_pos, &player_2_dp, player_2_ddp, delta_time);

    // Update ball position and handle collisions
    {
        ball_pos_x += ball_dp_x * delta_time;
        ball_pos_y += ball_dp_y * delta_time;

        // Check for collision between the ball and player 2's paddle
        if (aabb_vs_aabb(ball_pos_x, ball_pos_y, ball_half_size, ball_half_size, 80,
            player_2_pos, player_half_size_x, player_half_size_y))
        {
            ball_pos_x = 80.0f - player_half_size_x - ball_half_size;
            ball_dp_x *= -1;
            ball_dp_y = (ball_pos_y - player_2_pos) * 2 + player_2_dp * 0.75f;
        }
        // Check for collision between the ball and player 1's paddle
        else if (aabb_vs_aabb(ball_pos_x, ball_pos_y, ball_half_size, ball_half_size, -80,
            player_1_pos, player_half_size_x, player_half_size_y))
        {
            ball_pos_x = -80.0f + player_half_size_x + ball_half_size;
            ball_dp_x *= -1;
            ball_dp_y = (ball_pos_y - player_1_pos) * 2 + player_1_dp * 0.75f;
        }

        // Handle ball collisions with the top and bottom walls
        if (ball_pos_y + ball_half_size > arena_half_size_y)
        {
            ball_pos_y = arena_half_size_y - ball_half_size;
            ball_dp_y *= -1.0f;
        }
        else if (ball_pos_y - ball_half_size < -arena_half_size_y)
        {
            ball_pos_y = -arena_half_size_y + ball_half_size;
            ball_dp_y *= -1.0f;
        }

        // Handle ball reaching the left or right walls (score)
        if (ball_pos_x + ball_half_size > arena_half_size_x)
        {
            ball_dp_x *= -1.0f;
            ball_dp_y = 0;
            ball_pos_x = 0;
            ball_pos_y = 0;
        }
        else if (ball_pos_x - ball_half_size < -arena_half_size_x)
        {
            ball_dp_x *= -1.0f;
            ball_dp_y = 0;
            ball_pos_x = 0;
            ball_pos_y = 0;
        }
    }

    // Rendering: Draw the ball and both player paddles
    draw_rect(ball_pos_x, ball_pos_y, 1, 1, 0xffffff);
    draw_rect(80, player_2_pos, player_half_size_x, player_half_size_y, 0xffffff);
    draw_rect(-80, player_1_pos, player_half_size_x, player_half_size_y, 0xffffff);
}