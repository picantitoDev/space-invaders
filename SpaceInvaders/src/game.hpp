#pragma once
#include "space-ship.hpp"
#include "obstacle.hpp"
#include "alien.hpp"
#include "mystery-ship.hpp"

class Game{
    public:
        Game();
        ~Game();
        void Draw();
        void Update();
        void HandleInput();
        bool run;
        int lives;
        int score;
        int highScore;
        Music music;
    private:
        void DeleteInactiveLasers();
        std::vector<Obstacle> CreateObstacles();
        std::vector<Alien> CreateAliens();
        void MoveAliens();
        void MoveAliensDown(int distance);
        void AlienShootLaser();
        void CheckForCollisions();
        void GameOver();
        void Reset();
        void InitGame();
        void CheckForHighScore();
        void SaveHighScore(int highScore);
        int LoadHighScore();
        Spaceship spaceship;
        std::vector<Obstacle> obstacles;
        std::vector<Alien> aliens;
        std::vector<Laser> alienLasers;
        int aliensDirections;
        constexpr static float alienLaserShootInterval = 0.35;
        float timeLastAlienFired;
        MysteryShip mysteryShip;
        float mysteryShipSpawnInterval;
        float timeLastSpawn;
        Sound explosionSound;
        Sound gameOverSound;

};