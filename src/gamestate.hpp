#pragma once
#include "raylib.h"
#include "player.hpp"
#include "platform.hpp"
#include <vector>

enum class GameState {
    TITLE,
    GAMEPLAY,
    PAUSE
};

class GameStateManager {
    private:
        GameState currentState;
        
        // Title screen resources
        Texture2D titleBackground;
        Rectangle playButton;
        Rectangle resumeButton;
        Rectangle exitButton;
        bool isPlayButtonHovered;
        bool isResumeButtonHovered;
        bool isExitButtonHovered;
        
        // Gameplay resources
        Player* player;
        std::vector<Platform>* platforms;
        Camera2D camera;
        Texture2D gameBackground;
        Music* backgroundMusic;
        Sound buttonSound;
        
        // Camera configuration
        const float MIN_ZOOM = 1.9f;
        const float MAX_ZOOM = 3.5f;
        const float CAMERA_SMOOTH_FACTOR = 0.1f;
        const float ZOOM_SPEED = 0.1f;
        
    public:
        GameStateManager();
        ~GameStateManager();
        
        void Initialize(Player* mainPlayer, std::vector<Platform>* platformList, Music* music);
        void Update();
        void Draw();
        GameState GetCurrentState() const { return currentState; }
};