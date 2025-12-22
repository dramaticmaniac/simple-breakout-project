#include "graphics.h"

#include "assets.h"
#include "ball.h"
#include "colors.h"
#include "level.h"
#include "paddle.h"

#include "raylib.h"

#include <cmath>
#include <iostream>
#include <string>

void init_victory_confetti();

struct Text {
    std::string str;
    Vector2 position = { 0.50f, 0.50f };
    float size = 32.0f;
    Color color = WHITE;
    float spacing = 4.0f;
    Font* font = nullptr;
};

struct Confetti {
    Vector2 pos;
    float speed;
    float radius;
    Color color;
};

constexpr int CONFETTI_COUNT = 120;
static Confetti victory_confetti[CONFETTI_COUNT];
static bool confetti_initialized = false;

constexpr float cell_scale = 0.6f;
constexpr float screen_scale_divisor = 700.0f;

constexpr size_t victory_ball_launch_degree_offset = 3;
constexpr size_t victory_balls_count = 360 / victory_ball_launch_degree_offset;
constexpr float victory_balls_speed = 7.0f;
constexpr float victory_balls_size = 3.0f;

Vector2 screen_size;
float screen_scale;
float cell_size;
Vector2 shift_to_center;

Vector2 victory_balls_pos[victory_balls_count];
Vector2 victory_balls_vel[victory_balls_count];

size_t game_frame = 0;

void draw_image(const Texture2D& image, const float x, const float y, const float width, const float height)
{
    const Rectangle source = { 0.0f, 0.0f, static_cast<float>(image.width), static_cast<float>(image.height) };
    const Rectangle destination = { x, y, width, height };
    DrawTexturePro(image, source, destination, { 0.0f, 0.0f }, 0.0f, WHITE);
}

void draw_image(const Texture2D& image, const float x, const float y, const float size)
{
    draw_image(image, x, y, size, size);
}

void draw_sprite(sprite& sprite, const float x, const float y, const float width, const float height)
{
    draw_image(sprite.frames[sprite.frame_index], x, y, width, height);

    if (sprite.prev_game_frame == game_frame) {
        return;
    }
    if (sprite.frames_skipped < sprite.frames_to_skip) {
        ++sprite.frames_skipped;
    } else {
        sprite.frames_skipped = 0;

        ++sprite.frame_index;
        if (sprite.frame_index >= sprite.frame_count) {
            sprite.frame_index = sprite.loop ? 0 : sprite.frame_count - 1;
        }
    }
    sprite.prev_game_frame = game_frame;
}

void draw_sprite(sprite& sprite, const float x, const float y, const float size)
{
    draw_sprite(sprite, x, y, size, size);
}

void draw_text(const Text& text)
{
    const auto [x, y] = MeasureTextEx(*text.font, text.str.c_str(), text.size * screen_scale, text.spacing);
    const Vector2 pos = {
        screen_size.x * text.position.x - 0.5f * x,
        screen_size.y * text.position.y - 0.5f * y
    };
    DrawTextEx(*text.font, text.str.c_str(), pos, y, text.spacing, text.color);
}

void derive_graphics_metrics()
{
    screen_size.x = static_cast<float>(GetScreenWidth());
    screen_size.y = static_cast<float>(GetScreenHeight());

    cell_size = std::min(screen_size.x / static_cast<float>(current_level.columns), screen_size.y / static_cast<float>(current_level.rows));
    screen_scale = std::min(screen_size.x, screen_size.y) / screen_scale_divisor;

    const float level_width = static_cast<float>(current_level.columns) * cell_size;
    const float level_height = static_cast<float>(current_level.rows) * cell_size;
    shift_to_center = {
        (screen_size.x - level_width) * 0.5f,
        (screen_size.y - level_height) * 0.5f
    };
}

void draw_menu()
{
    ClearBackground(GAME_BG);

    const Text game_title = {
        "Breakout",
        { 0.50f, 0.50f },
        100.0f,
        TITLE_COLOR,
        4.0f,
        &menu_font
    };
    draw_text(game_title);

    const Text game_subtitle = {
        "Press Enter to Start",
        { 0.50f, 0.65f },
        32.0f,
        TEXT_COLOR,
        4.0f,
        &menu_font
    };
    draw_text(game_subtitle);
}

void draw_ui()
{
    const Text level_counter = {
        "LEVEL " + std::to_string(current_level_index + 1) + " OUT OF " + std::to_string(level_count),
        { 0.5f, 0.0375f },
        48.0f,
        TEXT_COLOR,
        4.0f,
        &menu_font
    };
    draw_text(level_counter);

    const Text boxes_remaining = {
        "BLOCKS " + std::to_string(current_level_blocks),
        { 0.5f, 0.9625f },
        48.0f,
        TEXT_COLOR,
        4.0f,
        &menu_font
    };
    draw_text(boxes_remaining);
    const Text fish_text = {
        "FISH: "
            + std::to_string(fish_collected)
            + " / "
            + std::to_string(fish_total),
        { 0.05f, 0.90f },
        28.0f,
        SKYBLUE,
        2.0f,
        &menu_font
    };
    draw_text(fish_text);

    const Text laser_text = {
        "LASERS: "
            + std::to_string(lasers_collected)
            + " / "
            + std::to_string(lasers_total),
        { 0.05f, 0.94f },
        28.0f,
        RED,
        2.0f,
        &menu_font
    };
    draw_text(laser_text);
}

void draw_level()
{
    ClearBackground(GAME_BG);

    for (size_t row = 0; row < current_level.rows; ++row) {
        for (size_t column = 0; column < current_level.columns; ++column) {
            const char data = current_level.data[row * current_level.columns + column];
            const float texture_x_pos = shift_to_center.x + static_cast<float>(column) * cell_size;
            const float texture_y_pos = shift_to_center.y + static_cast<float>(row) * cell_size;

            switch (data) {
            case WALL:
                draw_image(wall_texture, texture_x_pos, texture_y_pos, cell_size);
                break;
            case BLOCKS:
                draw_image(block_texture, texture_x_pos, texture_y_pos, cell_size);
                break;
            case LASER:
                DrawRectangle(
                    shift_to_center.x + column * cell_size,
                    shift_to_center.y + row * cell_size,
                    cell_size,
                    cell_size,
                    RED);
                break;
            case FISH:
                draw_image(fish_texture, texture_x_pos, texture_y_pos, cell_size);
                break;
            case ICY:
                draw_image(block_texture, texture_x_pos, texture_y_pos, cell_size);
                DrawRectangle(texture_x_pos, texture_y_pos, cell_size, cell_size, Fade(SKYBLUE, 0.45f));
                break;
            default:;
            }
        }
    }
}

void draw_paddle()
{
    const float texture_x_pos = shift_to_center.x + paddle_pos.x * cell_size;
    const float texture_y_pos = shift_to_center.y + paddle_pos.y * cell_size;
    draw_image(paddle_texture, texture_x_pos, texture_y_pos, paddle_size.x * cell_size, paddle_size.y * cell_size);
}

void draw_ball()
{
    const float texture_x_pos = shift_to_center.x + ball_pos.x * cell_size;
    const float texture_y_pos = shift_to_center.y + ball_pos.y * cell_size;
    draw_sprite(ball_sprite, texture_x_pos, texture_y_pos, cell_size);
}

void draw_pause_menu()
{
    ClearBackground(GAME_BG);

    const Text paused_title = {
        "Press Escape to Resume",
        { 0.50f, 0.50f },
        32.0f,
        TEXT_COLOR,
        4.0f,
        &menu_font
    };
    draw_text(paused_title);
}

void init_victory_menu()
{
    for (size_t i = 0; i < victory_balls_count; ++i) {
        victory_balls_pos[i] = { screen_size.x / 2, screen_size.y / 2 };
        victory_balls_vel[i] = {
            std::cos(static_cast<float>(i * victory_ball_launch_degree_offset)) * victory_balls_speed,
            std::sin(static_cast<float>(i * victory_ball_launch_degree_offset)) * victory_balls_speed
        };
    }
}

void draw_victory_menu()
{
    ClearBackground(GAME_BG);

    if (!confetti_initialized) {
        init_victory_confetti();
    }

    float dt = GetFrameTime();

    for (auto& c : victory_confetti) {
        c.pos.y += c.speed * dt;

        if (c.pos.y > screen_size.y) {
            c.pos.y = 0;
            c.pos.x = (float)GetRandomValue(0, (int)screen_size.x);
        }

        DrawCircleV(c.pos, c.radius, c.color);
    }

    const Text victory_title = {
        "Victory!",
        { 0.50f, 0.45f },
        100.0f,
        TITLE_COLOR,
        4.0f,
        &menu_font
    };
    draw_text(victory_title);

    const Text victory_subtitle = {
        "Press Enter to Restart",
        { 0.50f, 0.62f },
        32.0f,
        TEXT_COLOR,
        4.0f,
        &menu_font
    };
    draw_text(victory_subtitle);
}
void init_victory_confetti()
{
    for (auto& c : victory_confetti) {
        c.pos = {
            (float)GetRandomValue(0, (int)screen_size.x),
            (float)GetRandomValue(0, (int)screen_size.y)
        };

        c.speed = GetRandomValue(30, 80);
        c.radius = GetRandomValue(2, 4);

        c.color = Color {
            (unsigned char)GetRandomValue(150, 255),
            (unsigned char)GetRandomValue(150, 255),
            (unsigned char)GetRandomValue(150, 255),
            255
        };
    }

    confetti_initialized = true;
}
