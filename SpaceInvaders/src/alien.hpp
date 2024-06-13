#pragma once
#include "raylib.h"

class Alien {
public:
    static Texture2D alienImages[3];
    int type;
    Vector2 position;
    
    int frameWidth;
    int frameHeight;
    int currentFrame;
    float frameTime;
    float frameCounter;
    
    Rectangle getRect();
    Alien(int type, Vector2 position);
    void Draw();
    int GetType();
    void Update(int direction, float deltaTime);
    static void UnloadImages();
};