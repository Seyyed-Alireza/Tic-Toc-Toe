#include <iostream>
#include <raylib.h>
#include <cstring>

const int BOARD_SIZE = 5; // Changed from 6 to 5
const int CELL_SIZE = 100;
const int BOARD_OFFSET_X = 450;
const int BOARD_OFFSET_Y = 50;
bool Player_One_Input_Finished = false;
bool Player_Two_Input_Finished = false;
bool start = false;
bool Error = false;
bool Possible_To_Start = false;
char Player_One_Name[16] = "";
char Player_Two_Name[16] = "";
Rectangle Start_Botton = {50, 700, 200, 50};
Rectangle Reset_Botton = {1150, 700, 200, 50};
int Active_Text_Box = 1;
int key1, key2;
int Player1_W = 0, Player2_W = 0, Player1_D = 0, Player2_D = 0, Player1_L = 0, Player2_L = 0;
bool gameOver = false;
int winner = 0; // 0 for no winner, 1 for Player 1, 2 for Player 2
float spacing = 1.5f;

char board[BOARD_SIZE][BOARD_SIZE] = {0}; // 0 for empty, 1 for Player 1 (*), 2 for Player 2 (#)
bool playerOneTurn = true;

void Display_Board()
{
    for (int i = 0; i <= BOARD_SIZE; i++)
    {
        DrawLine(BOARD_OFFSET_X + i * CELL_SIZE, BOARD_OFFSET_Y, BOARD_OFFSET_X + i * CELL_SIZE, BOARD_OFFSET_Y + BOARD_SIZE * CELL_SIZE, BLACK);
        DrawLine(BOARD_OFFSET_X, BOARD_OFFSET_Y + i * CELL_SIZE, BOARD_OFFSET_X + BOARD_SIZE * CELL_SIZE, BOARD_OFFSET_Y + i * CELL_SIZE, BLACK);
    }

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] == 1)
            {
                DrawText("*", BOARD_OFFSET_X + j * CELL_SIZE + 40, BOARD_OFFSET_Y + i * CELL_SIZE + 30, 40, BLACK);
            }
            else if (board[i][j] == 2)
            {
                DrawText("#", BOARD_OFFSET_X + j * CELL_SIZE + 40, BOARD_OFFSET_Y + i * CELL_SIZE + 30, 40, BLACK);
            }
        }
    }
}

void Score_Board(int Player1_W, int Player2_W, int Player1_L, int Player2_L, int Player1_D, int Player2_D , Font Main_Font)
{
    // Draw the score table
    int tableWidth = 400; // Increased width to fit all columns
    int tableHeight = 150;
    int tableX = 500;
    int tableY = 600;
    DrawRectangle(tableX, tableY, tableWidth, tableHeight, LIGHTGRAY);
    DrawRectangleLines(tableX, tableY, tableWidth, tableHeight, BLACK);

    // Draw headers
    int headerX = tableX + 20; // Starting X position for headers
    int headerY = tableY + 10; // Starting Y position for headers
    DrawTextEx(Main_Font, "Wins", (Vector2){(float)headerX, (float)headerY}, Main_Font.baseSize , spacing, BLACK);
    DrawTextEx(Main_Font, "Loses", (Vector2){(float)headerX + 120, (float)headerY}, Main_Font.baseSize , spacing, BLACK);
    DrawTextEx(Main_Font, "Draws", (Vector2){(float)headerX + 240, (float)headerY}, Main_Font.baseSize , spacing, BLACK);

    // Draw Player 1 scores
    int player1X = tableX + 20;
    int player1Y = tableY + 40;
    DrawTextEx(Main_Font, TextFormat("%d", Player1_W), (Vector2){(float)player1X, (float)player1Y}, Main_Font.baseSize , spacing, BLACK);
    DrawTextEx(Main_Font, TextFormat("%d", Player1_L), (Vector2){(float)player1X + 120, (float)player1Y}, Main_Font.baseSize , spacing, BLACK);
    DrawTextEx(Main_Font, TextFormat("%d", Player1_D), (Vector2){(float)player1X + 240, (float)player1Y}, Main_Font.baseSize , spacing, BLACK);

    // Draw Player 2 scores
    int player2X = tableX + 20;
    int player2Y = tableY + 70;
    DrawTextEx(Main_Font, TextFormat("%d", Player2_W), (Vector2){(float)player2X, (float)player2Y}, Main_Font.baseSize , spacing, BLACK);
    DrawTextEx(Main_Font, TextFormat("%d", Player2_L), (Vector2){(float)player2X + 120, (float)player2Y}, Main_Font.baseSize , spacing, BLACK);
    DrawTextEx(Main_Font, TextFormat("%d", Player2_D), (Vector2){(float)player2X + 240, (float)player2Y}, Main_Font.baseSize , spacing, BLACK);
}

bool CheckWin(int player)
{
    // Check rows and columns
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j <= BOARD_SIZE - 5; j++)
        {
            bool rowWin = true;
            bool colWin = true;
            for (int k = 0; k < 5; k++)
            {
                if (board[i][j + k] != player) rowWin = false;
                if (board[j + k][i] != player) colWin = false;
            }
            if (rowWin || colWin) return true;
        }
    }

    // Check diagonals
    for (int i = 0; i <= BOARD_SIZE - 5; i++)
    {
        for (int j = 0; j <= BOARD_SIZE - 5; j++)
        {
            bool diag1Win = true;
            bool diag2Win = true;
            for (int k = 0; k < 5; k++)
            {
                if (board[i + k][j + k] != player) diag1Win = false;
                if (board[i + k][j + 4 - k] != player) diag2Win = false;
            }
            if (diag1Win || diag2Win) return true;
        }
    }

    return false;
}

bool CheckDraw()
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] == 0) return false;
        }
    }
    return true;
}

void ResetGame(bool& start, bool& gameOver, int& winner, bool& playerOneTurn)
{
    // Reset the board
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            board[i][j] = 0;
        }
    }

    // Reset game state
    start = false;
    gameOver = false;
    winner = 0;
    playerOneTurn = true;
    for ( int i = 0 ; i < 16 ; i++ )  Player_One_Name[i] = '\0';
    for ( int i = 0 ; i < 16 ; i++ )  Player_Two_Name[i] = '\0';
    Active_Text_Box = 1;
    Player1_W = 0; Player2_W = 0; Player1_D = 0; Player2_D = 0; Player1_L = 0; Player2_L = 0;
}

int main()
{
    const char* text1 = "Enter players names (Maximum 15 characters)";
    int screenWidth = 1400;
    int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Tic Tac Toe");
    InitAudioDevice(); // Initialize audio device

    // Load sounds
    Sound clickSound = LoadSound("sounds/clicksound.wav");
    Sound winSound = LoadSound("sounds/winsound.wav");
    Sound drawSound = LoadSound("sounds/drawsound.wav");
    Music backgroundMusic = LoadMusicStream("sounds/backsound.mp3"); // Load background music

    // Load backgrounds
    Texture2D menuBackground = LoadTexture("images/b1.jpg"); // Background for the menu
    Texture2D gameBackground = LoadTexture("images/b2.jpg"); // Background for the game

    Font Main_Font = LoadFont("fonts/calibri.ttf");
    SetTextureFilter(Main_Font.texture, TEXTURE_FILTER_POINT);
    SetTargetFPS(60);

    PlayMusicStream(backgroundMusic);
    SetMusicVolume(backgroundMusic, 0.9f); // Set volume (0.0 to 1.0)

    while (!WindowShouldClose())
    {
        // Update music stream
        UpdateMusicStream(backgroundMusic);

        if (!start)
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                Vector2 mousePos = GetMousePosition();
                if (CheckCollisionPointRec(mousePos, {500, 450, 400, 50}))
                {
                    Active_Text_Box = 1;
                    Player_One_Input_Finished = false;
                    Player_Two_Input_Finished = true;
                }
                else if (CheckCollisionPointRec(mousePos, {500, 520, 400, 50}))
                {
                    Active_Text_Box = 2;
                    Player_One_Input_Finished = true;
                    Player_Two_Input_Finished = false;
                }
            }

            if (Active_Text_Box == 1)
            {
                key1 = GetCharPressed();
                while (key1 > 0)
                {
                    if (strlen(Player_One_Name) < 15)
                    {
                        Player_One_Name[strlen(Player_One_Name)] = static_cast<char>(key1);
                        Player_One_Name[strlen(Player_One_Name) + 1] = '\0';
                    }
                    key1 = GetCharPressed();
                }
                if (IsKeyPressed(KEY_BACKSPACE) && strlen(Player_One_Name) > 0)
                {
                    Player_One_Name[strlen(Player_One_Name) - 1] = '\0';
                }
                if (IsKeyPressed(KEY_ENTER))
                {
                    Player_One_Input_Finished = true;
                    Active_Text_Box = 2;
                }
            }
            else if (!Player_Two_Input_Finished && Active_Text_Box == 2 && Player_One_Input_Finished)
            {
                key2 = GetCharPressed();
                while (key2 > 0)
                {
                    if (strlen(Player_Two_Name) < 15)
                    {
                        Player_Two_Name[strlen(Player_Two_Name)] = static_cast<char>(key2);
                        Player_Two_Name[strlen(Player_Two_Name) + 1] = '\0';
                    }
                    key2 = GetCharPressed();
                }
                if (IsKeyPressed(KEY_BACKSPACE) && strlen(Player_Two_Name) > 0)
                {
                    Player_Two_Name[strlen(Player_Two_Name) - 1] = '\0';
                }
                if (IsKeyPressed(KEY_ENTER))
                {
                    Player_Two_Input_Finished = true;
                }
            }
            if (strcmp(Player_One_Name, Player_Two_Name) == 0 && strlen(Player_One_Name) > 0) Error = true;
            else Error = false;
            if (strlen(Player_One_Name) > 0 && strlen(Player_Two_Name) > 0 && !Error) Possible_To_Start = true;
            else Possible_To_Start = false;
            if (CheckCollisionPointRec(GetMousePosition(), Start_Botton) && Possible_To_Start)
            {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    start = true;
                }
            }
            BeginDrawing();
            ClearBackground(RAYWHITE);

            // Draw menu background
            Rectangle source = {0, 0, (float)menuBackground.width, (float)menuBackground.height};
            Rectangle dest = {0, 0, (float)screenWidth, (float)screenHeight};
            Vector2 origin = {0, 0};
            DrawTexturePro(menuBackground, source, dest, origin, 0.0f, WHITE);

            // Draw the text in green
            DrawTextEx(Main_Font, text1, (Vector2){(screenWidth - MeasureTextEx(Main_Font, text1, Main_Font.baseSize, spacing).x) / 2, (screenHeight - MeasureTextEx(Main_Font, text1, Main_Font.baseSize, spacing).y) / 2}, Main_Font.baseSize, spacing, GREEN);

            DrawRectangle(500, 450, 400, 50, Active_Text_Box == 1 ? LIGHTGRAY : WHITE);
            DrawRectangleLines(500, 450, 400, 50, Active_Text_Box == 1 ? BLUE : DARKGRAY);
            DrawTextEx(Main_Font, Player_One_Name, (Vector2){510, 465}, Main_Font.baseSize, spacing, BLACK);
            DrawRectangle(500, 520, 400, 50, Active_Text_Box == 2 ? LIGHTGRAY : WHITE);
            DrawRectangleLines(500, 520, 400, 50, Active_Text_Box == 2 ? BLUE : DARKGRAY);
            DrawTextEx(Main_Font, Player_Two_Name, (Vector2){510, 535}, Main_Font.baseSize, spacing, BLACK);
            DrawRectangleRec(Start_Botton, Possible_To_Start ? GREEN : LIGHTGRAY);
            DrawTextEx(Main_Font, "Start", (Vector2){120, 710}, Main_Font.baseSize, spacing, BLACK);
            DrawRectangleRec(Reset_Botton, GREEN);
            DrawTextEx(Main_Font, "Reset", (Vector2){1212, 710}, Main_Font.baseSize, spacing, BLACK);
            if (Error)
            {
                DrawTextEx(Main_Font, "Names can't be same", (Vector2){550, 585}, Main_Font.baseSize, spacing, WHITE);
            }
            EndDrawing();
        }
        else
        {
            if (!gameOver)
            {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    Vector2 mousePos = GetMousePosition();
                    int row = (mousePos.y - BOARD_OFFSET_Y) / CELL_SIZE;
                    int col = (mousePos.x - BOARD_OFFSET_X) / CELL_SIZE;

                    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE && board[row][col] == 0)
                    {
                        board[row][col] = playerOneTurn ? 1 : 2;
                        PlaySound(clickSound); // Play click sound

                        if (CheckWin(playerOneTurn ? 1 : 2))
                        {
                            gameOver = true;
                            winner = playerOneTurn ? 1 : 2;
                            if (winner == 1) 
                            {
                                Player1_W++;
                                Player2_L++;
                            }
                            else 
                            {
                                Player2_W++;
                                Player1_L++;
                            }
                            PlaySound(winSound); // Play win sound
                        }
                        else if (CheckDraw())
                        {
                            gameOver = true;
                            winner = 0;
                            Player1_D++;
                            Player2_D++;
                            PlaySound(drawSound); // Play draw sound
                        }
                        else
                        {
                            playerOneTurn = !playerOneTurn;
                        }
                    }
                }
            }

            // Reset button functionality
            if (CheckCollisionPointRec(GetMousePosition(), Reset_Botton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                ResetGame(start, gameOver, winner, playerOneTurn);
            }

            BeginDrawing();
            ClearBackground(RAYWHITE);

            // Draw game background
            Rectangle source = {0, 0, (float)gameBackground.width, (float)gameBackground.height};
            Rectangle dest = {0, 0, (float)screenWidth, (float)screenHeight};
            Vector2 origin = {0, 0};
            DrawTexturePro(gameBackground, source, dest, origin, 0.0f, WHITE);

            DrawTextEx(Main_Font, Player_One_Name, (Vector2){20, 20}, Main_Font.baseSize, spacing, BLACK);
            DrawTextEx(Main_Font, Player_Two_Name, (Vector2){1200, 20}, Main_Font.baseSize, spacing, BLACK);
            DrawTextEx(Main_Font, "Tic Tac Toe", (Vector2){620, 12}, Main_Font.baseSize, spacing, BLACK);
            Display_Board();

            // Draw the score table
            Score_Board(Player1_W, Player2_W, Player1_L, Player2_L, Player1_D, Player2_D , Main_Font);

            // Draw Win/Draw text on the left side of the game field
            if (gameOver)
            {
                if (winner == 0)
                {
                    char Draw_Text[6] = "Draw!";
                    DrawTextEx(Main_Font, "Draw!", (Vector2){(screenWidth - MeasureTextEx(Main_Font, Draw_Text, Main_Font.baseSize, spacing).x) / 2, 560}, 50, spacing, BLACK);
                }
                else
                {
                    char Win_Message[20]; 
                    strcpy(Win_Message, winner == 1 ? Player_One_Name : Player_Two_Name); 
                    strcat(Win_Message, " Won!"); 
                    DrawTextEx(Main_Font, Win_Message, (Vector2){(screenWidth - MeasureTextEx(Main_Font, Win_Message, Main_Font.baseSize, spacing).x) / 2, 560}, Main_Font.baseSize, spacing, BLACK);
                }
            }

            DrawRectangleRec(Start_Botton, Possible_To_Start ? GREEN : LIGHTGRAY);
            DrawTextEx(Main_Font, "Start", (Vector2){120, 710}, Main_Font.baseSize, spacing, BLACK);
            DrawRectangleRec(Reset_Botton, GREEN);
            DrawTextEx(Main_Font, "Reset", (Vector2){1212, 710}, Main_Font.baseSize, spacing, BLACK);
            EndDrawing();
        }
    }

    // Unload sounds
    UnloadSound(clickSound);
    UnloadSound(winSound);
    UnloadSound(drawSound);
    UnloadMusicStream(backgroundMusic); 

    // Unload backgrounds
    UnloadTexture(menuBackground);
    UnloadTexture(gameBackground);

    UnloadFont(Main_Font);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}