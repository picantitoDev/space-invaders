#include <raylib.h>
#include "game.hpp"
#include <string>

std::string FormatWithLeadingZeros(int number, int width){
    std::string scoreText = std::to_string(number);
        int leadingZeros = width - scoreText.length();
        return scoreText = std::string(leadingZeros, '0') + scoreText;

}

int main()
{
    Color backgroundColor = {29, 29, 27, 255};
    Color guiColor = {243, 216, 63, 255};
    int offSet = 50;
    int windowWidth = 750;
    int windowHeight = 700;
    int fps = 60;
    
    InitWindow(windowWidth + offSet, windowHeight + 2 * offSet, "Guerra Espacial");
    InitAudioDevice();

    Font font = LoadFontEx("fonts/monogram.ttf", 64, 0, 0);
    Image livesImage = LoadImage("graphics/lives.png");
    ImageResize(&livesImage, 44, 28);
    Texture2D lives = LoadTextureFromImage(livesImage);
    SetTargetFPS(fps);

    Game game;

    while(!WindowShouldClose()){
        UpdateMusicStream(game.music);
        game.HandleInput();
        game.Update();
        BeginDrawing();
        ClearBackground(backgroundColor);
        DrawRectangleRoundedLines({10, 10, 780, 780}, 0.18f, 20, 2, guiColor);
        DrawLineEx({25, 730}, {775, 730}, 3, guiColor);
        
        
        if(!game.run){
            DrawTextEx(font, "GAME OVER", {570, 740}, 34, 2, guiColor);
        }else{
            DrawTextEx(font, "UPAO", {570, 740}, 34, 2, guiColor);
        }

        float x = 50.0f;
        for(int i = 0; i < game.lives; i++){
            DrawTextureV(lives, {x, 745}, WHITE);
            x+=50;
        }

        DrawTextEx(font, "SCORE", {50, 15}, 34, 2, guiColor);
        std::string scoreText = FormatWithLeadingZeros(game.score, 5);
        DrawTextEx(font, scoreText.c_str(), {50, 40}, 34, 2, guiColor);
        DrawTextEx(font, "HIGH-SCORE", {570, 15}, 34, 2, guiColor);
        std::string highScoreText = FormatWithLeadingZeros(game.highScore, 5);
        DrawTextEx(font, highScoreText.c_str(), {655, 40}, 34, 2, guiColor);
        game.Draw();

        EndDrawing();
    }

    CloseWindow();
    CloseAudioDevice();
}