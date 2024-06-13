#include "game.hpp"
#include <iostream>
#include <fstream>

Game::Game()
{
    music = LoadMusicStream("sounds/music.ogg");
    explosionSound = LoadSound("sounds/explosion.ogg");
    gameOverSound = LoadSound("sounds/gameover.wav");
    InitGame();
}

Game::~Game()
{
    Alien::UnloadImages();
    UnloadMusicStream(music);
    UnloadSound(explosionSound);
    UnloadSound(gameOverSound);
}

void Game::Update()
{

    if (run)
    {
        double currentTime = GetTime();
        if (currentTime - timeLastSpawn > mysteryShipSpawnInterval)
        {
            mysteryShip.Spawn();
            timeLastSpawn = GetTime();                         // Actualiza el tiempo del último spawn
            mysteryShipSpawnInterval = GetRandomValue(20, 40); // Establece un nuevo intervalo aleatorio
        }
        for (auto &laser : spaceship.lasers)
        {
            laser.Update();
        }
        AlienShootLaser();
        MoveAliens();

        for (auto &laser : alienLasers)
        {
            laser.Update();
        }
        DeleteInactiveLasers();

        mysteryShip.Update();

        CheckForCollisions();
    }else{
        if(IsKeyDown(KEY_ENTER)){
            Reset();
            InitGame();
        }
    }
}

void Game::Draw()
{
    float deltaTime = GetFrameTime();
    spaceship.Update(deltaTime);

    spaceship.Draw();

    for (auto &laser : spaceship.lasers)
    {
        laser.Draw();
    }

    for (auto &obstacle : obstacles)
    {
        obstacle.Draw();
    }

    for (auto &alien : aliens)
    {
        alien.Draw();
    }

    for (auto &laser : alienLasers)
    {
        laser.Draw();
    }

    mysteryShip.Draw();
}

void Game::HandleInput()
{

    if (run)
    {
        if (IsKeyDown(KEY_LEFT))
        {
            spaceship.MoveLeft();
        }
        else if (IsKeyDown(KEY_RIGHT))
        {
            spaceship.MoveRight();
        }
        else if (IsKeyDown(KEY_SPACE))
        {
            spaceship.FireLaser();
        }
    }
}

void Game::DeleteInactiveLasers()
{
    for (auto it = spaceship.lasers.begin(); it != spaceship.lasers.end();)
    {
        if (!it->active)
        {
            it = spaceship.lasers.erase(it);
        }
        else
        {
            ++it;
        }
    }

    for (auto it = alienLasers.begin(); it != alienLasers.end();)
    {
        if (!it->active)
        {
            it = alienLasers.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

std::vector<Obstacle> Game::CreateObstacles()
{
    int obstacleWidth = Obstacle::grid[0].size() * 3;
    float gap = (GetScreenWidth() - (4 * obstacleWidth)) / 5;

    for (int i = 0; i < 4; i++)
    {
        float offSetX = (i + 1) * gap + i * obstacleWidth;
        obstacles.push_back(Obstacle({offSetX, float(GetScreenHeight() - 240)}));
    }

    return obstacles;
}

std::vector<Alien> Game::CreateAliens()
{
    std::vector<Alien> aliens;
    for (int row = 0; row < 5; row++)
    {
        for (int column = 0; column < 11; column++)
        {
            int alienType;

            if (row == 0)
            {
                alienType = 3;
            }
            else if (row == 1 || row == 2)
            {
                alienType = 2;
            }
            else
            {
                alienType = 1;
            }

            int cellSize = 55, offSetX = 75, offSetY = 110;
            float x = offSetX + column * cellSize;
            float y = offSetY + row * cellSize;
            aliens.push_back(Alien(alienType, {x, y}));
        }
    }
    return aliens;
}

void Game::MoveAliens()
{
    for (auto &alien : aliens)
    {
        if (alien.position.x + alien.alienImages[alien.type - 1].width > GetScreenWidth() - 25)
        {
            aliensDirections = -1;
            MoveAliensDown(4);
        }

        else if (alien.position.x < 25)
        {
            aliensDirections = 1;
            MoveAliensDown(4);
        }
        float deltaTime = GetFrameTime();
        alien.Update(aliensDirections, deltaTime);
    }
}

void Game::MoveAliensDown(int distance)
{
    for (auto &alien : aliens)
    {
        alien.position.y += distance;
    }
}

void Game::AlienShootLaser()
{
    double currentTime = GetTime();
    if (currentTime - timeLastAlienFired >= alienLaserShootInterval && !aliens.empty())
    {
        int randomIndex = GetRandomValue(0, aliens.size() - 1);
        Alien &alien = aliens[randomIndex];
        alienLasers.push_back(Laser({alien.position.x + alien.alienImages[alien.type - 1].width / 2,
                                     alien.position.y + alien.alienImages[alien.type - 1].height},
                                    6));
        timeLastAlienFired = GetTime();
    }
}

void Game::CheckForCollisions()
{
    // Spaceship Lasers
    for (auto &laser : spaceship.lasers)
    {
        auto it = aliens.begin();
        while (it != aliens.end())
        {
            if (CheckCollisionRecs(it->getRect(), laser.getRect()))
            {
                PlaySound(explosionSound);
                if(it -> type == 1){
                    score += 100;
                } else if(it -> type == 2){
                    score += 200;
                } else if(it -> type == 3){
                    score += 300;
                }
                CheckForHighScore();
                it = aliens.erase(it);
                laser.active = false;
            }
            else
            {
                ++it;
            }
        }

        for (auto &obstacle : obstacles)
        {
            auto it = obstacle.blocks.begin();
            while (it != obstacle.blocks.end())
            {
                if (CheckCollisionRecs(it->getRect(), laser.getRect()))
                {
                    it = obstacle.blocks.erase(it);
                    laser.active = false;
                }
                else
                {
                    ++it;
                }
            }
        }

        if (CheckCollisionRecs(mysteryShip.getRect(), laser.getRect()))
        {
            mysteryShip.alive = false;
            laser.active = false;
            score+=500;
            CheckForHighScore();
            PlaySound(explosionSound);
        }
    }

    // Alien lasers
    for (auto &laser : alienLasers)
    {
        if (CheckCollisionRecs(laser.getRect(), spaceship.getRect()))
        {
            laser.active = false;
            lives--;
            if (lives == 0)
            {
                GameOver();
            }
        }

        for (auto &obstacle : obstacles)
        {
            auto it = obstacle.blocks.begin();
            while (it != obstacle.blocks.end())
            {
                if (CheckCollisionRecs(it->getRect(), laser.getRect()))
                {
                    it = obstacle.blocks.erase(it);
                    laser.active = false;
                }
                else
                {
                    ++it;
                }
            }
        }
    }

    // Colision de Alien con Obstaculo

    for (auto &alien : aliens)
    {
        for (auto &obstacle : obstacles)
        {
            auto it = obstacle.blocks.begin();
            while (it != obstacle.blocks.end())
            {
                if (CheckCollisionRecs(it->getRect(), alien.getRect()))
                {
                    it = obstacle.blocks.erase(it);
                }
                else
                {
                    it++;
                }
            }
        }

        if (CheckCollisionRecs(alien.getRect(), spaceship.getRect()))
        {
            std::cout << "Nave atacada por alien!" << std::endl;
            GameOver();
        }
    }
}

void Game::GameOver()
{
    run = false;
    PlaySound(gameOverSound);
    StopMusicStream(music);
    std::cout << "Game Over" << std::endl;
}

void Game::Reset()
{
    spaceship.Reset();
    aliens.clear();
    alienLasers.clear();
    obstacles.clear();
}

void Game::InitGame()
{
    obstacles = CreateObstacles();
    aliens = CreateAliens();
    aliensDirections = 1;
    timeLastAlienFired = 0;
    timeLastSpawn = GetTime();
    mysteryShipSpawnInterval = GetRandomValue(10, 20);
    mysteryShip.Spawn();
    lives = 3;
    score = 0;
    highScore = LoadHighScore();
    run = true;
    PlayMusicStream(music);
}

void Game::CheckForHighScore()
{
    if(score > highScore){
        highScore = score;
        SaveHighScore(highScore);
    }
}

void Game::SaveHighScore(int highScore)
{
    std::ofstream highScoreFile("highscore.txt");
    if(highScoreFile.is_open()){
        highScoreFile << highScore;
        highScoreFile.close();
    }else{
        std::cerr << "No se pudo guardar el puntaje" << std::endl;
    }
}

int Game::LoadHighScore()
{
    int loadedHighScore = 0;
    std::ifstream highScoreFile("highscore.txt");
    if(highScoreFile.is_open()){
        highScoreFile >> loadedHighScore;
        highScoreFile.close();
    }else{
        std::cerr << "No se pudo leer el puntaje" << std::endl;
    }
    return loadedHighScore;
}
