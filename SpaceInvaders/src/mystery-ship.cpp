#include "mystery-ship.hpp"

MysteryShip::MysteryShip()
{
    image = LoadTexture("graphics/mystery1.png");

    // Definir el tamaño del frame
    frameWidth = image.width / 6; // Ancho de un cuadro en el sprite sheet
    frameHeight = image.height;   // Altura del cuadro
    currentFrame = 0;
    frameTime = 0.05f; // Tiempo por cuadro en segundos
    frameCounter = 0.0f;

    alive = false;
}

MysteryShip::~MysteryShip()
{
    UnloadTexture(image);
}

void MysteryShip::Spawn()
{
    position.y = 70;
    int side = GetRandomValue(0, 1);

    if (side == 0)
    {
        position.x = 25;
        speed = 2;
    }
    else
    {
        position.x = GetScreenWidth() - frameWidth - 25;
        speed = -2;
    }
    alive = true;
}

Rectangle MysteryShip::getRect()
{
    if(alive){
        return {position.x, position.y, static_cast<float>(frameWidth), static_cast<float>(frameHeight)};
    } else {
        return {position.x, position.y, 0, 0};
    }
}
void MysteryShip::Update()
{
    if (alive)
    {
        position.x += speed;
        if (position.x > GetScreenWidth() -25 || position.x < 25)
        {
            alive = false;
        }

        // Actualizar el contador de cuadros
        frameCounter += GetFrameTime();

        // Actualizar el cuadro actual si ha pasado suficiente tiempo
        if (frameCounter >= frameTime)
        {
            frameCounter = 0.0f;
            currentFrame++;

            // Alternar entre los seis cuadros (0, 1, 2, 3, 4, 5)
            if (currentFrame > 5)
            {
                currentFrame = 0;
            }
        }
    }
}

void MysteryShip::Draw()
{
    if (alive)
    {
        Rectangle source = { static_cast<float>(currentFrame * frameWidth), 0.0f, static_cast<float>(frameWidth), static_cast<float>(frameHeight) };
        DrawTextureRec(image, source, position, WHITE);
    }
}