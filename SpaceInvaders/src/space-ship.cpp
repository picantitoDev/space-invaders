#include "space-ship.hpp"

Spaceship::Spaceship()
{
    image = LoadTexture("graphics/spritesheet.png");

    // Definir el frame size
    frameWidth = image.width / 5;  
    frameHeight = image.height;
    currentFrame = 1;
    frameTime = 0.5f; 
    frameCounter = 0.0f;

    // Dimensiones correctas de la nave
    destWidth = 44;  // Ancho deseado
    destHeight = 74; // Altura deseada

    // Calcular al posicion de nuestra nave
    position.x = (GetScreenWidth() - destWidth) / 2;
    position.y = GetScreenHeight() - destHeight - 100;

    laserFireTime = 0.0;
    laserSound = LoadSound("sounds/lasers.ogg");

}

Spaceship::~Spaceship()
{
    UnloadTexture(image);
    UnloadSound(laserSound);
}

void Spaceship::Update(float deltaTime)
{
    // Actualizar contador de frames
    frameCounter += deltaTime;
    
    if (frameCounter >= frameTime)
    {
        frameCounter = 0.0f;
        currentFrame++;
        
        // Alternate entre mis dos sprites
        if (currentFrame > 2)
        {
            currentFrame = 1;
        }
    }
}

void Spaceship::Draw()
{
    Rectangle source = { static_cast<float>(currentFrame * frameWidth), 0.0f, static_cast<float>(frameWidth), static_cast<float>(frameHeight) };
    Rectangle dest = { position.x, position.y, static_cast<float>(destWidth), static_cast<float>(destHeight) };
    DrawTexturePro(image, source, dest, Vector2{0, 0}, 0.0f, WHITE);
}

void Spaceship::MoveLeft(){
    position.x -= 7;
    if(position.x < 25){
        position.x = 25;
    }
}

void Spaceship::MoveRight(){
    position.x += 7;
    if(position.x > (GetScreenWidth() - destWidth) - 25){
        position.x = GetScreenWidth() - destWidth - 25;
    }

}

void Spaceship::FireLaser(){
    // Calcular la posición inicial del láser para que se dispare desde el centro de la nave
    float laserX = position.x + (destWidth / 2) - 2; // -2 para centrar el láser
    float laserY = position.y;
    if(GetTime() - laserFireTime >= 0.35){
        lasers.push_back(Laser({laserX, laserY}, -6));
        laserFireTime = GetTime();
        PlaySound(laserSound);
    }
}

void Spaceship::Reset()
{

    position.x = (GetScreenWidth() - destWidth) / 2.0f;
    position.y = GetScreenHeight() - destHeight - 100;
    lasers.clear();

}

Rectangle Spaceship::getRect()
{
    return {position.x, position.y, static_cast<float>(destWidth), static_cast<float>(destHeight)};
}