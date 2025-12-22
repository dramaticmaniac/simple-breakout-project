#include "assets.h"

#include "raylib.h"

void load_fonts()
{
    menu_font = LoadFontEx("data/fonts/ARCADECLASSIC.TTF", 256, nullptr, 0);
}

void unload_fonts()
{
    UnloadFont(menu_font);
}

void load_textures()
{
    wall_texture = LoadTexture("data/images/wall.png");
    void_texture = LoadTexture("data/images/void.png");
    block_texture = LoadTexture("data/images/block.png");
    paddle_texture = LoadTexture("data/images/paddle.png");
    ball_sprite = load_sprite("data/images/ball/ball", ".png", 8, true, 10);
    fish_texture = LoadTexture("data/images/fish.png");
}

void unload_textures()
{
    UnloadTexture(wall_texture);
    UnloadTexture(void_texture);
    UnloadTexture(block_texture);
    UnloadTexture(paddle_texture);
    unload_sprite(ball_sprite);
    UnloadTexture(fish_texture);
}

void load_sounds()
{
    InitAudioDevice();
    bg_music = LoadMusicStream("data/sounds/bg.mp3");
    SetMusicVolume(bg_music, 0.5f);
    win_sound = LoadSound("data/sounds/win.wav");
    lose_sound = LoadSound("data/sounds/lose.wav");
    hit_sound = LoadSound("data/sounds/hit.wav");
}

void unload_sounds()
{
    UnloadSound(win_sound);
    UnloadSound(lose_sound);
    UnloadSound(hit_sound);
}
