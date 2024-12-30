#pragma once
#include "raylib.h"
#include "platform.hpp"
#include <vector>

class Player {

    public:
        Player();
        ~Player();

        Vector2 GetplayerPosition();
        void SetplayerPosition(Vector2 position);

        void init(const char * texturePath);
        void Update(std::vector<Platform>& platforms);
        void Draw();
        void DrawPlatform();
        void Setdebug();


        void MoveLeft();
        void MoveRight();
        void Jump();
        void Run();
        void Restart(Vector2 position);

        void Animation();
        Sound jumpSound;

        bool Getdebug();

    private:
        Texture2D spriteSheet;
        Rectangle frameRec;
        Vector2 position;
        Vector2 velocity;

        //Animation Variables
        float frameTime;
        int currentFrame;
        const float frameSpeed = 0.2f;

        bool isJumping;
        bool isMoving;
        bool debugModeEnabled;

        const float gravity = 0.025f; //Lunar gravity
        const float jumpForce = -2.5f;
        float playerSpeed = 1.8f;

};

