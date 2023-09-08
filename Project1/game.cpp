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

internal void update_player(float *pos, float *dp, float ddp, float delta_time)
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

internal bool aabb_vs_aabb(float p1x, float p1y, float hs1x, float hs1y,
							float p2x, float p2y, float hs2x, float hs2y)
{
	return(p1x + hs1x > p2x - hs2x &&
		p1x - hs1x < p2x + hs2x &&
		p1y + hs1y > p2y - hs2y &&
		p1y + hs1y < p2y + hs2y);
}

internal void update_game(Input *input, float delta_time)
{
	clear_screen(0x808080);
	draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0x000000);

	float player_1_ddp = 0.0f;
	if (is_down(BUTTON_UP)) { player_1_ddp += 2000; }
	if (is_down(BUTTON_DOWN)) { player_1_ddp -= 2000; }

	float player_2_ddp = 0.0f;
	if (is_down(BUTTON_W)) { player_2_ddp += 2000; }
	if (is_down(BUTTON_S)) { player_2_ddp -= 2000; }

	update_player(&player_1_pos, &player_1_dp, player_1_ddp, delta_time);
	update_player(&player_2_pos, &player_2_dp, player_2_ddp, delta_time);

	ball_pos_x += ball_dp_x * delta_time;
	ball_pos_y += ball_dp_y * delta_time;

	if (aabb_vs_aabb(ball_pos_x, ball_pos_y, ball_half_size, ball_half_size, 80, 
		player_2_pos, player_half_size_x, player_half_size_y))
	{
		ball_pos_x = 80.0f - player_half_size_x - ball_half_size;
		ball_dp_x *= -1;
		ball_dp_y = (ball_pos_y - player_2_pos) * 2 + player_2_dp * 0.75f;
	}
    else if (aabb_vs_aabb(ball_pos_x, ball_pos_y, ball_half_size, ball_half_size, -80,
				player_1_pos, player_half_size_x, player_half_size_y))
		     {	 
				ball_pos_x = -80.0f + player_half_size_x + ball_half_size;
				ball_dp_x *= -1;
				ball_dp_y = (ball_pos_y - player_1_pos) * 2 + player_1_dp * 0.75f;
		     }

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

	draw_rect(ball_pos_x, ball_pos_y, 1, 1, 0xffffff);

	draw_rect(80, player_2_pos, player_half_size_x, player_half_size_y, 0xffffff);
	draw_rect(-80, player_1_pos, player_half_size_x, player_half_size_y, 0xffffff);
}