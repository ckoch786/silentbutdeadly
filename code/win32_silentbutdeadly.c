#include <stdio.h>
#include <raylib.h>

#include "silentbutdeadly.h"
#include "win32_silentbutdeadly.h"


const float PLAYER_JUMP_SPEED 350.0f;
const float PLAYER_SPEED = 200.0f; // pixels per second
// TODO delete these
const float PLAYER_HEIGHT = 10.f;
const float PLAYER_WIDTH = 10.0f;
float player_pos_x = 400.0f;
float player_pos_y = 280.0f;

bool started = false;
bool game_over = false;


void restart() {

}

void updatePlayer(Player *player, EnvItem *envItems, int evnItemsLength, float delta);

int main(void) {
    // 
    // Init
    //
    puts("Silent but Deadly");
    test();

    SetConfigFlags(FLAG_VSYNC_HINT);
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Silent but Deadly");
    InitAudioDevice();
    SetTargetFPS(500);

    //Sound game_over_sound = LoadSound("assets/game_over.wav");
    Music menu_music = LoadMusicStream("assets/ProjectFelineTheme.mp3");
    PlayMusicStream(menu_music);

    Player player = { 0 };
    player.position = (Vector2){ 400, 280 };
    player.speed = 0;
    player.canJump = false;
    EnvItem envItems[] = {
        // TODO x, y, width, height
        {{ 0, 0, 1000, 400 }, 0, LIGHTGRAY },
        {{ 0, 400, 1000, 200 }, 1, GRAY },
        {{ 300, 200, 400, 10 }, 1, GRAY },
        {{ 250, 300, 100, 10 }, 1, GRAY },
        {{ 650, 300, 100, 10 }, 1, GRAY }
    };

    int envItemsLength = sizeof(evnItems)/sizeof(envItems[0]);

    Camera2D camera = { 0 };
    camera.target = player.position;
    camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    //camera.target = (Vector2){ player_pos_x + 20.0f, player_pos_y + 20.0f };
    //camera.offset = (Vector2){ /2.0f, };
    /* camera.rotation = 0.0f; */
    // TODO figure out why the initilization syntax is now working with vc++
    // camera.zoom = (float)GetScreenHeight()/SCREEN_SIZE;

    //
    // Game loop
    //
    while (!WindowShouldClose()) {
        //
        // Input
        //

        float player_move_velocity = 0.0f;
        // TODO also handle asdf controls
        if (IsKeyDown(KEY_RIGHT)) {
            player_move_velocity += PLAYER_SPEED;
        }
        
        if (IsKeyDown(KEY_LEFT)) {
            player_move_velocity -= PLAYER_SPEED;
        }

        camera.target = (Vector2){ player_pos_x + 20, player_pos_y + 20 };


        // 
        // Update
        //
        UpdateMusicStream(menu_music);

        Rectangle player_rect = {
            player_pos_x + player_move_velocity, player_pos_y,
            PLAYER_WIDTH, PLAYER_HEIGHT
        };
        // TODO collision dection



        //
        // Render
        //
        BeginDrawing();
        ClearBackground(GetColor(0x96bedcff));
        
        BeginMode2D(camera);

        DrawRectangleRec(player_rect, GetColor(0x32965aff));

        //PlaySound(game_over_sound);

        if (!started) {}
        if (!game_over) {}


        EndMode2D();
        EndDrawing();
    }

    //
    // Shutdown
    //
    UnloadMusicStream(menu_music);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}

void updatePlayer(Player *player, EnvItem *envItems, int envItemLength, float delta) {

}

