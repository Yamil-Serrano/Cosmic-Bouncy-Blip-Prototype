#include "platform.hpp"

Platform::Platform(Vector2 position, Vector2 size, Color color){
    this->position = position;
    this->size = size;
    this->color = color;
}

Platform::~Platform() {}

void Platform::Draw() {
    DrawRectangleV(position, size, color);
}

Vector2 Platform::GetPosition() const {
    return position;
}

bool Platform::CheckCollision(Vector2 playerPosition, Vector2 playerSize) {
    // Simple AABB collision detection
    return (playerPosition.x < position.x + size.x &&
            playerPosition.x + playerSize.x > position.x &&
            playerPosition.y < position.y + size.y &&
            playerPosition.y + playerSize.y > position.y);
}

float Platform::GetWidth(){
    return size.x;
}

float Platform::GetHeight(){
    return size.y;
}
