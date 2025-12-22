#ifndef PADDLE_H
#define PADDLE_H

#include "raylib.h"

constexpr float paddle_base_w = 3.0f;
constexpr float paddle_boost_w = 5.0f;

constexpr float paddle_height = 1.0f;

inline Vector2 paddle_size = { paddle_base_w, paddle_height };

inline Vector2 paddle_pos;

constexpr float paddle_speed = 0.1f;

void spawn_paddle();
void move_paddle(float x_offset);
bool is_colliding_with_paddle(Vector2 pos, Vector2 size);

#endif // PADDLE_H