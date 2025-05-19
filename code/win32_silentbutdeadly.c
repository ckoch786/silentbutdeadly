#include <stdio.h>
#include <raylib.h>
#include <raymath.h>

#include "silentbutdeadly.h"
#include "win32_silentbutdeadly.h"


const int G  = 400;
const float PLAYER_JUMP_SPEED = 350.0f;
const float PLAYER_SPEED = 200.0f; // pixels per second

bool started = false;
bool gameOver = false;


void restart() {

}

void updatePlayer(Player *player, EnvItem *envItems, int evnItemsLength, float delta);
void updateCamera(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height);

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
    Music menuMusic = LoadMusicStream("assets/ProjectFelineTheme.mp3");
    PlayMusicStream(menuMusic);

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

    int envItemsLength = sizeof(envItems)/sizeof(envItems[0]);

    Camera2D camera = { 0 };
    camera.target = player.position;
    camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    // camera.zoom = (float)GetScreenHeight()/SCREEN_SIZE;

    //
    // Game loop
    //
    while (!WindowShouldClose()) {
        //
        // Input and Update
        //
        UpdateMusicStream(menuMusic);

        // TODO properly handle game time
        float deltaTime = GetFrameTime();
        updatePlayer(&player, envItems, envItemsLength, deltaTime);

        camera.zoom  += ((float)GetMouseWheelMove()*0.05f);

        if (camera.zoom > 3.0f) {
            camera.zoom = 3.0f;
        } else if (camera.zoom < 0.25f) {
            camera.zoom = 0.25f;
        }

        // Reset camera and player
        if (IsKeyPressed(KEY_R)) {
            puts("Reset");
            camera.zoom = 1.0f;
            player.position = (Vector2){ 400, 280 };
        }

        updateCamera(&camera, &player, envItems, envItemsLength, deltaTime, screenWidth, screenHeight);

        //
        // Render
        //
        BeginDrawing();
        ClearBackground(LIGHTGRAY);
        
        BeginMode2D(camera);

        for (int i = 0; i < envItemsLength; i++) {
            DrawRectangleRec(envItems[i].rect, envItems[i].color);
        }

        Rectangle playerRect = { 
            player.position.x - 20,
            player.position.y - 40,
            40.0f,
            40.0f
        };
        DrawRectangleRec(playerRect, RED);

        //PlaySound(game_over_sound);

        if (!started) {}
        if (!gameOver) {}


        EndMode2D();
        EndDrawing();
    }

    //
    // Shutdown
    //
    UnloadMusicStream(menuMusic);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}

void updatePlayer(Player *player, EnvItem *envItems, int envItemsLength, float delta) {

    if (IsKeyDown(KEY_LEFT)) { 
        player->position.x -= PLAYER_SPEED * delta; 
    }

    if (IsKeyDown(KEY_RIGHT)) { 
        player->position.x += PLAYER_SPEED * delta; 
    }

    if (IsKeyDown(KEY_SPACE) && player->canJump) { 
        player->speed = -PLAYER_JUMP_SPEED;
        player->canJump = false;
    }
    
    bool hitObstacle = false;

    for (int i = 0; i < envItemsLength; i++) {
        EnvItem *ei = envItems + i;
        Vector2 *p = &(player->position);

        if (ei->blocking &&
            ei->rect.x <= p->x &&
            ei->rect.x + ei->rect.width >= p->x &&
            ei->rect.y >= p->y &&
            ei->rect.y <= p->y + player->speed*delta) {

            hitObstacle = true;
            player->speed = 0.0f;
            p->y = ei->rect.y;
            break;
        }
    }

    if (!hitObstacle) {
        player->position.y += player->speed*delta;
        player->speed += G*delta;
        player->canJump = false;
    } else {
        player->canJump = true;
    }
}

void updateCamera(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height) {
    static float minSpeed = 30;
    static float minEffectLength = 10;
    static float fractionSpeed = 0.8f;

    camera->offset = (Vector2){ width/2.0f, height/2.0f };
    Vector2 diff = Vector2Subtract(player->position, camera->target);
    float length = Vector2Length(diff);

    if (length > minEffectLength) {
        float speed = fmaxf(fractionSpeed*length, minSpeed);
        camera->target = Vector2Add(camera->target, Vector2Scale(diff, speed*delta/length));
    }
}
