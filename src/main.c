/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"
#include "resource_dir.h"    // utility header for SearchAndSetResourceDir

// App Constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 450;

// Game Constants
const float GRAVITY = 0.3f;
const float JUMP_FORCE = 7.0f;
const float PIPE_SPEED = 2.0f;
const float PIPE_GAP = 150.0f;
const float PIPE_WIDTH = 60.0f;
const float PIPE_SPACING = 300.0f;

// Simple Pipe struct
typedef struct Pipe {
    float xPos;
    float yPos;  // Y position of the gap center
} Pipe;

// Reset game state
void ResetGame(Vector2* pos, Vector2* vel, Pipe pipes[], int pipeCount) {
    pos->x = 100;
    pos->y = SCREEN_HEIGHT / 2;
    vel->x = 0;
    vel->y = 0;

    // Reset pipes
    for (int i = 0; i < pipeCount; i++) {
        pipes[i].xPos = SCREEN_WIDTH + i * PIPE_SPACING;
        pipes[i].yPos = GetRandomValue(PIPE_GAP, SCREEN_HEIGHT - PIPE_GAP);
    }
}

int main()
{
    // Tell the window to use vsync and work on high DPI displays
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

    // Create the window and OpenGL context
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Flappy Rabbit");

    // Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
    SearchAndSetResourceDir("resources");

    // Load a texture from the resources directory
    Texture wabbit = LoadTexture("wabbit_alpha.png");

    // Game variables
    Vector2 rabbitPos = { 100, SCREEN_HEIGHT / 2 };
    Vector2 rabbitVel = { 0, 0 };
    Pipe pipes[3];

    int score = 0;


    // Game loop
    while (!WindowShouldClose())
    {
        // UPDATE
        // Jump on space
        if (IsKeyPressed(KEY_SPACE)) {
            rabbitVel.y = -JUMP_FORCE;
        }

        // Apply gravity
        rabbitVel.y += GRAVITY;
        rabbitPos.y += rabbitVel.y;

        // Update pipes
        for (int i = 0; i < 3; i++) {
            pipes[i].xPos -= PIPE_SPEED;

            // Wrap pipe to the right when it goes off screen
            if (pipes[i].xPos + PIPE_WIDTH < 0) {
                pipes[i].xPos += PIPE_SPACING * 3;
                pipes[i].yPos = GetRandomValue(PIPE_GAP, SCREEN_HEIGHT - PIPE_GAP);
                score++;
            }
        }

        // Check collision with ground/ceiling
        if (rabbitPos.y < 0 || rabbitPos.y + wabbit.height > SCREEN_HEIGHT) {
            ResetGame(&rabbitPos, &rabbitVel, pipes, 3);
            score = 0;
        }

        // Check collision with pipes
        for (int i = 0; i < 3; i++) {
            bool inPipeX = rabbitPos.x + wabbit.width > pipes[i].xPos &&
                rabbitPos.x < pipes[i].xPos + PIPE_WIDTH;
            bool hitTopPipe = rabbitPos.y < pipes[i].yPos - PIPE_GAP / 2;
            bool hitBottomPipe = rabbitPos.y + wabbit.height > pipes[i].yPos + PIPE_GAP / 2;

            if (inPipeX && (hitTopPipe || hitBottomPipe)) {
                ResetGame(&rabbitPos, &rabbitVel, pipes, 3);
                score = 0;
            }
        }

        // DRAW
        BeginDrawing();
        ClearBackground(SKYBLUE);

        // Draw pipes
        for (int i = 0; i < 3; i++) {
            // Top pipe
            DrawRectangle(pipes[i].xPos, 0, PIPE_WIDTH, pipes[i].yPos - PIPE_GAP / 2, GREEN);
            // Bottom pipe
            DrawRectangle(pipes[i].xPos, pipes[i].yPos + PIPE_GAP / 2, PIPE_WIDTH, SCREEN_HEIGHT, GREEN);
        }

        // Draw rabbit
        DrawTexture(wabbit, rabbitPos.x, rabbitPos.y, WHITE);

        // Draw score
        DrawText(TextFormat("Score: %d", score), 10, 10, 30, WHITE);
        DrawText("Press SPACE to jump", 10, SCREEN_HEIGHT - 30, 20, LIGHTGRAY);

        EndDrawing();
    }

    // Cleanup
    UnloadTexture(wabbit);
    CloseWindow();
    return 0;
}