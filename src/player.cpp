#include "player.hpp"
#include "raylib.h"
#include <cmath>  
#include <vector>

int gamepad = 0;


Player::Player() {
    velocity = {0.0f, 0.0f};
    frameTime = 0.0f;
    currentFrame = 0;
    isMoving = false;
    isJumping = false;
    debugModeEnabled = false;
}

Player::~Player() {
    UnloadSound(jumpSound);
    UnloadTexture(spriteSheet);
}

void Player::init(const char * texturePath){
    spriteSheet = LoadTexture(texturePath);
    jumpSound = LoadSound("Audio/phaseJump2.ogg");
    SetSoundVolume(jumpSound, 0.3f);
    frameRec = {0.0f, 0.0f, 24.0f, 24.0f};
}

void Player::MoveRight(){

    position.x += playerSpeed;
    frameRec.width = -24;  // Mirror the sprite for right direction
    isMoving = true;       // Player is moving
    
}

void Player::MoveLeft(){

    position.x -= playerSpeed;
    frameRec.width = 24;   // Mirror the sprite for left direction
    isMoving = true;       // Player is moving
    
}

void Player::Jump(){
    if (!isJumping) {
        PlaySound(jumpSound);
        frameRec.x = 25.0f; // jump sprite (open legs)
        velocity.y = jumpForce;  // push the player up
        isJumping = true;  // Now the player is in the air
    }
}

void Player::Run(){
    playerSpeed = 3.0f;
}

void Player::Restart(Vector2 position){
    this->position = position;
}

void Player:: Setdebug(){
    this->debugModeEnabled = !debugModeEnabled;
}

void Player::Animation(){

    if (isMoving) {
        frameTime += GetFrameTime();
        if (frameTime >= frameSpeed) {
            frameTime = 0.0f;
            currentFrame = !currentFrame;

            // Update the texture rectangle for the new frame
            frameRec.x = (float)(currentFrame * 25);
        }
    } else {
        // reset the frame to the first frame when not moving (idle)
        currentFrame = 0;
        frameRec.x = 0.0f;
    }
}

void Player::Update(std::vector<Platform>& platforms) {
    // Reset movement state
    isMoving = false;

    // Store previous position for collision resolution
    Vector2 previousPosition = position;

    // Handle horizontal movement
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D) || (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_RIGHT))) MoveRight();
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A) || (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_LEFT))) MoveLeft();
    if ((!isJumping) && (IsKeyDown(KEY_SPACE) || (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)))) Jump();
    if (IsKeyDown(KEY_LEFT_SHIFT) || (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_LEFT))) Run();
    else playerSpeed = 1.8f;
    if (IsKeyPressed(KEY_F3) || ((IsGamepadButtonPressed(gamepad, GAMEPAD_BUTTON_MIDDLE_LEFT)))) Setdebug();

    // Apply gravity
    velocity.y += gravity;

    // Update position
    position.x += velocity.x;
    position.y += velocity.y;

    // Platform collision check
    isJumping = true;  // Assume in air unless proven otherwise
    
    // Create a slightly wider collision box for the player
    float collisionMargin = 5.0f;  // Adjust this value as needed
    float effectiveWidth = abs(frameRec.width) - (2 * collisionMargin);
    Vector2 collisionPosition = {
        position.x + collisionMargin,
        position.y
    };

    for (auto& platform : platforms) {
        // Use the existing CheckCollision method with Vector2 and dimensions
        if (platform.CheckCollision(collisionPosition, {effectiveWidth, frameRec.height})) {
            Rectangle platformBox = {
                platform.GetPosition().x,
                platform.GetPosition().y,
                platform.GetWidth(),
                platform.GetHeight()
            };

            // Vertical collision
            if (velocity.y > 0.0f && previousPosition.y + frameRec.height <= platformBox.y) {
                // Landing on top of platform
                position.y = platformBox.y - frameRec.height;
                velocity.y = 0.0f;
                isJumping = false;
            }
            else if (velocity.y < 0.0f && previousPosition.y >= platformBox.y + platformBox.height) {
                // Hitting platform from below
                position.y = platformBox.y + platformBox.height;
                velocity.y = 0.0f;
            }
            
            // Horizontal collision
            if (previousPosition.x + frameRec.width <= platformBox.x && velocity.x > 0.0f) {
                // Collision from left
                position.x = platformBox.x - frameRec.width;
                velocity.x = 0.0f;
            }
            else if (previousPosition.x >= platformBox.x + platformBox.width && velocity.x < 0.0f) {
                // Collision from right
                position.x = platformBox.x + platformBox.width;
                velocity.x = 0.0f;
            }
        }
    }

    // Update animation
    if (!isJumping) {
        Animation();
    }
}

void Player::Draw() {
    DrawTextureRec(spriteSheet, frameRec, position, WHITE);
}

//Draw a platform (Testing)
void Player::DrawPlatform(){
    DrawRectangle(125, 465, 500, 10, GRAY); 
}

Vector2 Player::GetplayerPosition(){
    return position;
}

void Player:: SetplayerPosition(Vector2 position){
    this->position = position;
}

bool Player::Getdebug(){
    return debugModeEnabled;
}
