#include "raylib.h"
#include "player.hpp"
#include "platform.hpp"
#include "gamestate.hpp"
#include <vector>

const int INITIAL_WINDOW_WIDTH = 1024;
const int INITIAL_WINDOW_HEIGHT = 750;
const int MAX_RESOLUTION_WIDTH = 1920;
const int MAX_RESOLUTION_HEIGHT = 1080;

int main() {
    // Initialize window and audio
    InitWindow(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, "Cosmic Bouncy Blip");
    //HideCursor();
    SetTargetFPS(60);
    InitAudioDevice();

    bool isFullScreen = false;

    // Initialize player
    Player mainPlayer;
    mainPlayer.init("Tilemap/tilemap-characters.png");
    mainPlayer.SetplayerPosition({INITIAL_WINDOW_WIDTH / 2.0f, INITIAL_WINDOW_HEIGHT / 2.0f});

    // Create platforms
    std::vector<Platform> platforms = {
        Platform({150, 900}, {150, 10}, Color{72, 61, 139, 100}),
        Platform({400, 800}, {250, 10}, Color{72, 61, 139, 100}),
        Platform({700, 700}, {300, 10}, Color{72, 61, 139, 100}),
        Platform({1000, 600}, {220, 10}, Color{72, 61, 139, 100}),
        Platform({1400, 500}, {180, 10}, Color{72, 61, 139, 100}),
        Platform({100, 400}, {250, 10}, Color{72, 61, 139, 100}),
        Platform({550, 300}, {150, 10}, Color{72, 61, 139, 100}),
        Platform({1000, 250}, {180, 10}, Color{72, 61, 139, 100}),
        Platform({1500, 150}, {250, 10}, Color{72, 61, 139, 100}),
        Platform({800, 100}, {300, 10}, Color{72, 61, 139, 100}),
        Platform({1700, 400}, {150, 10}, Color{72, 61, 139, 100}),
        Platform({300, 650}, {220, 10}, Color{72, 61, 139, 100}),
        Platform({600, 500}, {150, 10}, Color{72, 61, 139, 100}),
        Platform({950, 800}, {180, 10}, Color{72, 61, 139, 100}),
        Platform({1500, 600}, {150, 10}, Color{72, 61, 139, 100})
    };

    // Load background music
    Music backgroundMusic = LoadMusicStream("Audio/Melancholic Walk.ogg");
    PlayMusicStream(backgroundMusic);

    // Load full screen toggle sounds
    Sound fullScreenSound = LoadSound("Audio/maximize_001.ogg");
    SetSoundVolume(fullScreenSound, 0.5f);
    Sound minScreenSound = LoadSound("Audio/minimize_001.ogg");
    SetSoundVolume(minScreenSound, 0.5f);

    // Initialize game state manager
    GameStateManager gameState;
    gameState.Initialize(&mainPlayer, &platforms, &backgroundMusic);

    // Game loop
    while (!WindowShouldClose()) {
        // Handle F11 toggle for full-screen mode
        if (IsKeyPressed(KEY_F11)) {
            isFullScreen = !isFullScreen;
            if (isFullScreen) {
                SetWindowSize(MAX_RESOLUTION_WIDTH, MAX_RESOLUTION_HEIGHT);
                PlaySound(fullScreenSound);
                SetWindowState(FLAG_FULLSCREEN_MODE);
            } else {
                ClearWindowState(FLAG_FULLSCREEN_MODE);
                PlaySound(minScreenSound);
                SetWindowSize(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT);
            }
        }

        gameState.Update();
        gameState.Draw();
    }

    // Cleanup
    UnloadSound(minScreenSound);
    UnloadSound(fullScreenSound);
    UnloadMusicStream(backgroundMusic);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}