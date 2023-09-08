#define is_down(key) input->buttons[key].is_down
#define pressed(key) (input->button[key].is_down && input->button[key].changed)
#define released(key) !(input->button[key].is_down && input->button[key].changed)

float player_1_pos, player_1_dp, player_2_pos, player_2_dp;
float arena_half_size_x = 85.0f;
float arena_half_size_y = 45.0f;
float player_half_size_x = 2.5f;
float player_half_size_y = 12.0f;
float ball_pos_x, ball_pos_y, ball_dp_x = 100.0f, ball_dp_y;
float ball_half_size = 1.0f;

internal void update_game(Input* input, float delta_time)
{
	clear_screen(0x808080);
	draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0x000000);

	float player_1_ddp = 0.0f;
	if (is_down(BUTTON_UP)) { player_1_ddp += 2000; }
	if (is_down(BUTTON_DOWN)) { player_1_ddp -= 2000; }

	float player_2_ddp = 0.0f;
	if (is_down(BUTTON_W)) { player_2_ddp += 2000; }
	if (is_down(BUTTON_S)) { player_2_ddp -= 2000; }

	player_1_ddp -= player_1_dp * 10.0f;

	player_1_pos = player_1_pos + player_1_dp * delta_time + player_1_ddp * delta_time * delta_time * 0.5f;
	player_1_dp = player_1_dp + player_1_ddp * delta_time;

	if (player_1_pos + player_half_size_y > arena_half_size_y)
	{
		player_1_pos = arena_half_size_y - player_half_size_y;
		player_1_dp = 0;
	}
	else if (player_1_pos - player_half_size_y < -arena_half_size_y)
	{
		player_1_pos = -arena_half_size_y + player_half_size_y;
		player_1_dp = 0;
	}

	player_2_ddp -= player_2_dp * 10.0f;

	player_2_pos = player_2_pos + player_2_dp * delta_time + player_2_ddp * delta_time * delta_time * 0.5f;
	player_2_dp = player_2_dp + player_2_ddp * delta_time;

	if (player_2_pos + player_half_size_y > arena_half_size_y)
	{
		player_2_pos = arena_half_size_y - player_half_size_y;
		player_2_dp = 0;
	}
	else if (player_2_pos - player_half_size_y < -arena_half_size_y)
	{
		player_2_pos = -arena_half_size_y + player_half_size_y;
		player_2_dp = 0;
	}

	ball_pos_x += ball_dp_x * delta_time;
	ball_pos_y += ball_dp_y * delta_time;
	
	draw_rect(ball_pos_x, ball_pos_y, 1, 1, 0xffffff);

	if (ball_pos_x + ball_half_size > 80 - player_half_size_x &&
		ball_pos_x - ball_half_size < 80 + player_half_size_x &&
		ball_pos_y + ball_half_size > player_2_pos - player_half_size_y &&
		ball_pos_y + ball_half_size < player_2_pos + player_half_size_y)
	{
		ball_pos_x = 80.0f - player_half_size_x - ball_half_size;
		ball_dp_x *= -1;
	}
	else if (ball_pos_x + ball_half_size > -80 - player_half_size_x &&
		ball_pos_x - ball_half_size < -80 + player_half_size_x &&
		ball_pos_y + ball_half_size > player_1_pos - player_half_size_y &&
		ball_pos_y + ball_half_size < player_1_pos + player_half_size_y)
	{
		ball_pos_x = -80.0f + player_half_size_x + ball_half_size;
		ball_dp_x *= -1;
	}

	draw_rect(80, player_2_pos, player_half_size_x, player_half_size_y, 0xffffff);
	draw_rect(-80, player_1_pos, player_half_size_x, player_half_size_y, 0xffffff);
}