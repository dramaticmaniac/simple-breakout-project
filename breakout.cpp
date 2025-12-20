#include "assets.h"
#include "ball.h"
#include "colors.h"
#include "game.h"
#include "graphics.h"
#include "level.h"
#include "paddle.h"

#include "raylib.h"

bool victory_sound_played = false;

void update()
{
    switch (game_state) {
    case menu_state:
        if (IsKeyPressed(KEY_ENTER)) {
            PlaySound(hit_sound);
            load_level();
            game_state = in_game_state;
        }
        break;

    case in_game_state:
        if (IsKeyPressed(KEY_ESCAPE)) {
            game_state = paused_state;
            break;
        }

        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
            move_paddle(-paddle_speed);
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
            move_paddle(paddle_speed);

        move_ball();

        if (!is_ball_inside_level()) {
            PlaySound(lose_sound);
            game_state = game_over_state;
            break;
        }

        if (current_level_blocks == 0) {
            load_level(1);
        }
        break;

    case paused_state:
        if (IsKeyPressed(KEY_ESCAPE)) {
            game_state = in_game_state;
        }
        break;

    case game_over_state:
        if (IsKeyPressed(KEY_ENTER)) {
            current_level_index = 0;
            load_level();
            game_state = in_game_state;
        }
        if (IsKeyPressed(KEY_BACKSPACE)) {
            game_state = menu_state;
        }
        break;

    case victory_state:
        if (!victory_sound_played) {
            PlaySound(win_sound);
            victory_sound_played = true;
        }
        if (IsKeyPressed(KEY_ENTER)) {
            game_state = menu_state;
            victory_sound_played = false;
        }
        DrawText("YOU WIN!", 520, 240, 60, TITLE_COLOR);
        DrawText("Press ENTER to Return to Menu", 410, 330, 24, TEXT_COLOR);
        break;
    }
}

void draw()
{
    ClearBackground(GAME_BG);

    switch (game_state) {
    case menu_state:
        draw_menu();
        break;

    case in_game_state:
        draw_level();
        draw_paddle();
        draw_ball();
        draw_ui();
        break;

    case paused_state:
        draw_level();
        draw_paddle();
        draw_ball();
        draw_ui();
        DrawRectangle(0, 0, 1280, 720, Fade(BLACK, 0.6f));
        DrawText("PAUSED", 540, 260, 50, TITLE_COLOR);
        DrawText("Press ESC to Resume", 500, 330, 24, TEXT_COLOR);
        break;

    case game_over_state:
        DrawText("GAME OVER", 500, 240, 60, TITLE_COLOR);
        DrawText("Press ENTER to Try Again", 460, 330, 24, TEXT_COLOR);
        DrawText("Press BACKSPACE for Menu", 440, 370, 24, TEXT_COLOR);
        break;

    case victory_state:
        DrawText("YOU WIN!", 520, 240, 60, TITLE_COLOR);
        DrawText("Press ENTER to Return to Menu", 410, 330, 24, TEXT_COLOR);
        break;
    }
}

int main()
{
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(1280, 720, "Breakout");
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

    load_fonts();
    load_textures();
    load_level();
    load_sounds();
    SetMusicVolume(bg_music, 0.3f);
    PlayMusicStream(bg_music);
    while (!WindowShouldClose()) {
        UpdateMusicStream(bg_music);

        BeginDrawing();

        draw();
        update();

        EndDrawing();
    }
    CloseWindow();

    unload_sounds();
    unload_level();
    unload_textures();
    unload_fonts();

    return 0;
}
