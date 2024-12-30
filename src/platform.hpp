#pragma once
#include "raylib.h"

class Platform {
public:
    Platform(Vector2 position, Vector2 size, Color color);
    ~Platform();

    void Draw();
    bool CheckCollision(Vector2 playerPosition, Vector2 playerSize);
    Vector2 GetPosition() const;
    float GetWidth();
    float GetHeight();


private:
    Vector2 position;
    Vector2 size;
    Color color;
};
