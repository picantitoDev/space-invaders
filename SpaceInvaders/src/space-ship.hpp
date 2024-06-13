#pragma once
#include <raylib.h>
#include "laser.hpp"
#include <vector>

class Spaceship {
public:
    Spaceship();
    ~Spaceship();
    void Update(float deltaTime);
    void Draw();
    void MoveLeft();
    void MoveRight();
    void FireLaser();
    void Reset();
    Rectangle getRect();
    std::vector<Laser> lasers;
private:
    Texture2D image;
    Vector2 position;
    double laserFireTime;
    int frameWidth;
    int frameHeight;
    int destWidth;
    int destHeight;
    int currentFrame;
    float frameTime;
    float frameCounter;
    Sound laserSound;
};