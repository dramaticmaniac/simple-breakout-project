#ifndef PADDLE_H
#define PADDLE_H

#include "raylib.h"

inline Vector2 paddle_size = { 3.0f, 1.0f };
inline Vector2 base_paddle_size = { 3.0f, 1.0f };
inline constexpr float paddle_speed = 0.1f;

inline Vector2 paddle_pos;

void spawn_paddle();
void move_paddle(float x_offset);
bool is_colliding_with_paddle(Vector2 pos, Vector2 size);
void update_paddle_boost(float dt);
#endif // PADDLE_H
