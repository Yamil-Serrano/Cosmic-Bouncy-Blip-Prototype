#include "gamestate.hpp"
#include "raylib.h"

GameStateManager::GameStateManager() {
    currentState = GameState::TITLE;
    
    // Initialize title screen resources
    titleBackground = LoadTexture("Space_Background/space.png");
    buttonSound = LoadSound("Audio/select_005.ogg");
    SetSoundVolume(buttonSound, 0.4f);
    
    // Initialize camera with default values
    camera = { 0 };
    camera.zoom = MIN_ZOOM;
    camera.rotation = 0.0f;
    
    // Load game background
    gameBackground = LoadTexture("Space_Background/space.png");
}

GameStateManager::~GameStateManager() {
    UnloadTexture(titleBackground);
    UnloadTexture(gameBackground);
    UnloadSound(buttonSound);
}

void GameStateManager::Initialize(Player* mainPlayer, std::vector<Platform>* platformList, Music* music) {
    player = mainPlayer;
    platforms = platformList;
    backgroundMusic = music;
}

void GameStateManager::Update() {
    int windowWidth = GetScreenWidth();
    int windowHeight = GetScreenHeight();
    
    // Update camera offset based on current window size
    camera.offset = { (float)windowWidth / 2.0f, (float)windowHeight / 2.0f };
    
    // Update play button position based on current window size
    playButton = Rectangle{ 
        (float)windowWidth/2 - 100, 
        (float)windowHeight/2 - 30,
        200, 
        60 
    };
    
    switch(currentState) {
        case GameState::TITLE: {
            Vector2 mousePos = GetMousePosition();
            isPlayButtonHovered = CheckCollisionPointRec(mousePos, playButton);
            
            if ((IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && isPlayButtonHovered) || IsKeyDown(KEY_ENTER)) {
                PlaySound(buttonSound);
                currentState = GameState::GAMEPLAY;
                // Reset player position to center of current window
                player->SetplayerPosition({ (float)windowWidth / 2.0f, (float)windowHeight / 2.0f });

                // Restart the music from the beginning
                StopMusicStream(*backgroundMusic);
                PlayMusicStream(*backgroundMusic);

            }
            break;
        }
        
        case GameState::GAMEPLAY: {
            UpdateMusicStream(*backgroundMusic);
            player->Update(*platforms);
            
            // Update camera target to follow player
            Vector2 target = { 
                player->GetplayerPosition().x + 12.0f,
                player->GetplayerPosition().y + 12.0f 
            };
            camera.target.x += (target.x - camera.target.x) * CAMERA_SMOOTH_FACTOR;
            camera.target.y += (target.y - camera.target.y) * CAMERA_SMOOTH_FACTOR;
            
            // Handle zoom
            camera.zoom += GetMouseWheelMove() * ZOOM_SPEED;
            if (camera.zoom > MAX_ZOOM) camera.zoom = MAX_ZOOM;
            if (camera.zoom < MIN_ZOOM) camera.zoom = MIN_ZOOM;
            
            // Handle restart
            if (IsKeyDown(KEY_R)) {
                player->Restart({ (float)windowWidth / 2.0f, (float)windowHeight / 2.0f });
            }

            // Pause 
            if (IsKeyPressed(KEY_P) || (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT))) {
                currentState = GameState::PAUSE;
        
            }
            
            // Return to title
            if (IsKeyPressed(KEY_ESCAPE)) {
                currentState = GameState::TITLE;
            }
            break;
        }

        case GameState::PAUSE: {
            // Update button rectangles
            resumeButton = { (float)(windowWidth / 2 - 100), (float)(windowHeight / 2 - 30), 200.0f, 60.0f };
            exitButton = { resumeButton.x, resumeButton.y + resumeButton.height + 20, resumeButton.width, resumeButton.height };

            Vector2 mousePos = GetMousePosition();
            isResumeButtonHovered = CheckCollisionPointRec(mousePos, resumeButton);
            isExitButtonHovered = CheckCollisionPointRec(mousePos, exitButton);

            if (isResumeButtonHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                PlaySound(buttonSound);
                currentState = GameState::GAMEPLAY;
            } else if (isExitButtonHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                PlaySound(buttonSound);
                currentState = GameState::TITLE;
            }

            // Allow unpausing with P or Pause
            if (IsKeyPressed(KEY_P) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT)) {
                PlaySound(buttonSound);
                currentState = GameState::GAMEPLAY;
            }
            break;
        }
    }
}

void GameStateManager::Draw() {

    BeginDrawing();
    ClearBackground(Color{ 20, 29, 39, 255 });
    
    int windowWidth = GetScreenWidth();
    int windowHeight = GetScreenHeight();
    
    switch(currentState) {
        case GameState::TITLE: {
            DrawTextureEx(titleBackground, { 0,0 }, 0.0f, 1.0f, WHITE);
            
            // Draw play button
            DrawRectangleRec(playButton, isPlayButtonHovered ? Color{100, 100, 100, 255} : Color{72, 61, 139, 255});
            DrawText("PLAY", (int)(playButton.x + playButton.width/2 - MeasureText("PLAY", 30)/2), (int)(playButton.y + playButton.height/2 - 15), 30, WHITE);
            
            // Draw title text centered
            const char* title = "Cosmic Bouncy Blip";
            DrawText(title, windowWidth/2 - MeasureText(title, 40)/2, 100, 40, WHITE);
            break;
        }
        
        case GameState::GAMEPLAY: {
            DrawTextureEx(gameBackground, { 0,0 }, 0.0f, 1.0f, WHITE);
            
            BeginMode2D(camera);
                // Draw platforms
                for (Platform& platform : *platforms) {
                    platform.Draw();
                }
                // Draw player
                player->Draw();
            EndMode2D();
            
            // Draw debug info if enabled
            if (player->Getdebug()) {
                const int TEXT_SIZE = 20;
                const Color DEBUG_COLOR = Color{ 61, 185, 162, 255 };
                const int Y_SPACING = 30;
                
                DrawText("Debug Mode: Enabled", 10, 10, TEXT_SIZE, DEBUG_COLOR);
                DrawText(TextFormat("Player X: %.2f", player->GetplayerPosition().x), 10, 10 + Y_SPACING * 2, TEXT_SIZE, DEBUG_COLOR);
                DrawText(TextFormat("Player Y: %.2f", player->GetplayerPosition().y), 10, 10 + Y_SPACING, TEXT_SIZE, DEBUG_COLOR);
                DrawText(TextFormat("Camera Target X: %.2f", camera.target.x), 10, 10 + Y_SPACING * 3, TEXT_SIZE, DEBUG_COLOR);
                DrawText(TextFormat("Camera Target Y: %.2f", camera.target.y), 10, 10 + Y_SPACING * 4, TEXT_SIZE, DEBUG_COLOR);
                DrawText(TextFormat("Camera Zoom: %.2f", camera.zoom), 10, 10 + Y_SPACING * 5, TEXT_SIZE, DEBUG_COLOR);
                DrawText(TextFormat("FPS: %i", GetFPS()), 10, 10 + Y_SPACING * 6, TEXT_SIZE, DEBUG_COLOR);
            } else {
                DrawText("Debug Mode: Disabled, Press F3", 10, 10, 20, Color{ 61, 185, 162, 255 });
            }
            break;
        }
        case GameState::PAUSE: {
            // Draw the frozen game state in the background
            DrawTextureEx(gameBackground, {0, 0}, 0.0f, 1.0f, WHITE);
            
            // Render the game scene with platforms and player
            BeginMode2D(camera);
            for (Platform& platform : *platforms) {
                platform.Draw();
            }
            player->Draw();
            EndMode2D();

            // Overlay a semi-transparent dimming effect
            DrawRectangle(0, 0, windowWidth, windowHeight, {0, 0, 0, 100});
            
            // Draw "PAUSED" text centered
            const char* pausedText = "PAUSED";
            int pausedTextWidth = MeasureText(pausedText, 40);
            DrawText(pausedText, (windowWidth - pausedTextWidth) / 2, windowHeight / 2 - 120, 40, WHITE);
            
            // Define resume button rectangle and hover effect
            resumeButton = { 
                (float)(windowWidth / 2 - 100), 
                (float)(windowHeight / 2 - 30), 
                200.0f, 
                60.0f 
            };
            Color resumeButtonColor = isResumeButtonHovered ? Color{100, 100, 100, 255} : Color{72, 61, 139, 255};
            DrawRectangleRec(resumeButton, resumeButtonColor);

            // Draw resume button text centered in the button
            const char* resumeText = "RESUME";
            int resumeTextWidth = MeasureText(resumeText, 30);
            DrawText(resumeText, 
                    (int)(resumeButton.x + (resumeButton.width - resumeTextWidth) / 2), 
                    (int)(resumeButton.y + (resumeButton.height - 30) / 2), 
                    30, 
                    WHITE);

            // Define exit button rectangle and hover effect
            Rectangle exitButton = { 
                resumeButton.x, 
                resumeButton.y + resumeButton.height + 20, // Positioned below resume button with a gap
                resumeButton.width, 
                resumeButton.height 
            };
            Color exitButtonColor = isExitButtonHovered ? Color{100, 100, 100, 255} : Color{72, 61, 139, 255};
            DrawRectangleRec(exitButton, exitButtonColor);

            // Draw exit button text centered in the button
            const char* exitText = "EXIT";
            int exitTextWidth = MeasureText(exitText, 30);
            DrawText(exitText, 
                    (int)(exitButton.x + (exitButton.width - exitTextWidth) / 2), 
                    (int)(exitButton.y + (exitButton.height - 30) / 2), 
                    30, 
                    WHITE);

            // Draw pause instructions at the bottom of the screen
            const char* pauseInstructions = "Press P or Pause to resume";
            int pauseInstructionsWidth = MeasureText(pauseInstructions, 20);
            DrawText(pauseInstructions, 
                    (windowWidth - pauseInstructionsWidth) / 2, 
                    windowHeight - 50, 
                    20, 
                    {200, 200, 200, 255});

            break;
        }

    }
    
    EndDrawing();
}