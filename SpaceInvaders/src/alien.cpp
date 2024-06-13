#include "alien.hpp"

Texture2D Alien::alienImages[3] = {};

Alien::Alien(int type, Vector2 position)
{
    this->type = type;
    this->position = position;

    if (alienImages[type - 1].id == 0) {
        switch(type){
            case 1:
                alienImages[0] = LoadTexture("graphics/alien1_sprite.png");
                break;
            case 2:
                alienImages[1] = LoadTexture("graphics/alien2_sprite.png");
                break;
            case 3:
                alienImages[2] = LoadTexture("graphics/alien3_sprite.png");
                break;
            default:
                alienImages[0] = LoadTexture("graphics/alien1_sprite.png");
                break;
        }
    }

    frameWidth = alienImages[type - 1].width / 2; 
    frameHeight = alienImages[type - 1].height;
    currentFrame = 0;
    frameTime = 0.5f; // Time per frame in seconds
    frameCounter = 0.0f;
}

void Alien::Draw() {
    Rectangle source = { static_cast<float>(currentFrame * frameWidth), 0.0f, static_cast<float>(frameWidth), static_cast<float>(frameHeight) };
    DrawTextureRec(alienImages[type - 1], source, position, WHITE);
}

int Alien::GetType() {
    return type;
}

void Alien::UnloadImages() {
    for (int i = 0; i < 3; i++) {
        if (alienImages[i].id != 0) {
            UnloadTexture(alienImages[i]);
        }
    }
}

void Alien::Update(int direction, float deltaTime) {
    // Mover alien
    position.x += direction;

    // Actualizar el contador de frames
    frameCounter += deltaTime;

    // Actualizar contador
    if (frameCounter >= frameTime) {
        frameCounter = 0.0f;
        currentFrame++;
        
        // Alternar entre index 0 y 1 para mis 2 sprites
        if (currentFrame > 1) {
            currentFrame = 0;
        }
    }
}


Rectangle Alien::getRect()
{
    return {position.x, position.y,
            static_cast<float>(frameWidth),
            static_cast<float>(frameHeight)};
}