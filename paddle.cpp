#include "paddle.h"
#include "level.h"

#include "raylib.h"

#include <cmath>

extern float paddle_boost_timer;

void spawn_paddle()
{
    for (int column = 0; column < current_level.columns; column++) {
        for (int row = 0; row < current_level.rows; row++) {
            if (get_level_cell(row, column) == PADDLE) {
                set_level_cell(row, column, VOID);
                paddle_pos = { static_cast<float>(column), static_cast<float>(row) };
                goto outer_loop_end;
            }
        }
    }
outer_loop_end:;
}

void move_paddle(const float x_offset)
{
    float next_paddle_pos_x = paddle_pos.x + x_offset;
    if (is_colliding_with_level_cell({ next_paddle_pos_x, paddle_pos.y }, paddle_size, WALL)) {
        next_paddle_pos_x = std::round(next_paddle_pos_x);
    }
    paddle_pos.x = next_paddle_pos_x;
}

bool is_colliding_with_paddle(const Vector2 pos, const Vector2 size)
{
    const Rectangle paddle_hitbox = { paddle_pos.x, paddle_pos.y, paddle_size.x, paddle_size.y };
    const Rectangle hitbox = { pos.x, pos.y, size.x, size.y };
    return CheckCollisionRecs(paddle_hitbox, hitbox);
}
void update_paddle_boost(float dt)
{
        paddle_size = base_paddle_size;
    }
