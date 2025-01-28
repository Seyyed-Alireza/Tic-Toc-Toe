#include <iostream>
#include <raylib.h>
#include <cstring>

const int screenWidth = 1400;
const int screenHeight = 800;
const int BOARD_SIZE = 5; // Changed from 6 to 5
const int CELL_SIZE = 100;
const int BOARD_OFFSET_X = 450;
const int BOARD_OFFSET_Y = 50;
bool Player_One_Input_Finished = false;
bool Player_Two_Input_Finished = false;
bool start = false;
bool Error = false;
bool Possible_To_Start = false;
bool Possible_To_Reset = false;
bool End_Game = false;
bool Mouse_On_Start_Botton = false;
bool Mouse_On_Reset_Botton = false;
bool First_Time = true;
bool Mouse_On_Yes = false;
bool Mouse_On_No = false;
bool Is_Asking_To_Continue = false;
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
int Menu_X = 100 , Menu_Y = 300; 

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
            // if (board[i][j] == 1)
            // {
            //     DrawTextEx(Main_Font, "X", (Vector2){(float) BOARD_OFFSET_X + j * CELL_SIZE + 40, (float) BOARD_OFFSET_Y + i * CELL_SIZE + 30}, Main_Font.baseSize, spacing, BLACK);
            // }
            // else if (board[i][j] == 2)
            // {
            //     DrawTextEx(Main_Font, "O", (Vector2){(float) BOARD_OFFSET_X + j * CELL_SIZE + 40, (float) BOARD_OFFSET_Y + i * CELL_SIZE + 30}, Main_Font.baseSize, spacing, BLACK);
            // }
        }
    }
}

void Score_Board( Font Main_Font )
{
    // Draw the score table
    int tableWidth = 400; // Increased width to fit all columns
    int tableHeight = 150;
    int tableX = 500;
    int tableY = End_Game ? 300 : 600 ;
    DrawRectangle(tableX , tableY, tableWidth, tableHeight, LIGHTGRAY);
    DrawRectangleLines(tableX , tableY, tableWidth, tableHeight, BLACK);

    // Draw headers
    int player2X = tableX + 20;
    int player2Y = tableY + 100;
    int player1X = tableX + 20;
    int player1Y = tableY + 60;
    int headerX = tableX + 20; // Starting X position for headers
    int headerY = tableY + 10; // Starting Y position for headers
    DrawTextEx(Main_Font, Player_One_Name, (Vector2){(float)(tableX - (MeasureTextEx(Main_Font, Player_One_Name, Main_Font.baseSize, spacing).x) - 10 ), (float)player1Y}, Main_Font.baseSize , spacing, BLACK);
    DrawTextEx(Main_Font, Player_Two_Name, (Vector2){(float)(tableX - (MeasureTextEx(Main_Font, Player_Two_Name, Main_Font.baseSize, spacing).x) - 10 ), (float)player2Y}, Main_Font.baseSize , spacing, BLACK);
    DrawTextEx(Main_Font, "Wins", (Vector2){(float)headerX, (float)headerY}, Main_Font.baseSize , spacing, BLACK);
    DrawTextEx(Main_Font, "Loses", (Vector2){(float)headerX + 120, (float)headerY}, Main_Font.baseSize , spacing, BLACK);
    DrawTextEx(Main_Font, "Draws", (Vector2){(float)headerX + 240, (float)headerY}, Main_Font.baseSize , spacing, BLACK);

    // Draw player 1 scores
    DrawTextEx(Main_Font, TextFormat("%d", Player1_W), (Vector2){(float)player1X, (float)player1Y}, Main_Font.baseSize , spacing, BLACK);
    DrawTextEx(Main_Font, TextFormat("%d", Player1_L), (Vector2){(float)player1X + 120, (float)player1Y}, Main_Font.baseSize , spacing, BLACK);
    DrawTextEx(Main_Font, TextFormat("%d", Player1_D), (Vector2){(float)player1X + 240, (float)player1Y}, Main_Font.baseSize , spacing, BLACK);
    
    // Draw playaer 2 scores
    DrawTextEx(Main_Font, TextFormat("%d", Player2_W), (Vector2){(float)player2X, (float)player2Y}, Main_Font.baseSize , spacing, BLACK);
    DrawTextEx(Main_Font, TextFormat("%d", Player2_L), (Vector2){(float)player2X + 120, (float)player2Y}, Main_Font.baseSize , spacing, BLACK);
    DrawTextEx(Main_Font, TextFormat("%d", Player2_D), (Vector2){(float)player2X + 240, (float)player2Y}, Main_Font.baseSize , spacing, BLACK);
}


// Display the menu menux = 100   menuy = 300
void Display_Menu ( Font Main_Font )
{
    Rectangle New_Game_Botton = {100 , 350 , 200 , 50};
    bool Mouse_On_New_Game_Botton = false;
    if (CheckCollisionPointRec(GetMousePosition(), New_Game_Botton ) ) 
    Mouse_On_New_Game_Botton = true;
    else Mouse_On_New_Game_Botton = false;
    DrawRectangleRec ( New_Game_Botton , Mouse_On_New_Game_Botton ? GREEN : WHITE );

    Rectangle Exit_Botton = {100 , 400 , 200 , 50};
    bool Mouse_On_Exit_Botton = false;
    if (CheckCollisionPointRec(GetMousePosition(), Exit_Botton ) ) 
    Mouse_On_Exit_Botton = true;
    else Mouse_On_New_Game_Botton = false;
    DrawRectangleRec ( Exit_Botton , Mouse_On_Exit_Botton ? GREEN : WHITE );
    for ( int i = 0 ; i < 4 ; i++ )
    {
        DrawLine ( Menu_X , Menu_Y + ( 50 * i ) , Menu_X + 200 , Menu_Y + ( 50 * i ) , BLACK );
        if ( i < 2 ) DrawLine ( Menu_X + ( 200 * i ) , Menu_Y , Menu_X + ( 200 * i ) , Menu_Y + 150 , BLACK );
    }
    DrawTextEx ( Main_Font , "Menu" , (Vector2){ 160 , 310 } , Main_Font.baseSize , spacing , BLACK );
    DrawTextEx ( Main_Font , "Exit" , (Vector2){ 174 , 410 } , Main_Font.baseSize , spacing , BLACK );
    DrawTextEx ( Main_Font , "New Game" , (Vector2){ 126 , 360 } , Main_Font.baseSize , spacing , BLACK );
    
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

void ResetGame() // bool& start, bool& gameOver, int& winner, bool& playerOneTurn
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
    Possible_To_Reset = false;
    for ( int i = 0 ; i < 16 ; i++ )  Player_One_Name[i] = '\0';
    for ( int i = 0 ; i < 16 ; i++ )  Player_Two_Name[i] = '\0';
    Active_Text_Box = 1;
    Player1_W = 0; Player2_W = 0; Player1_D = 0; Player2_D = 0; Player1_L = 0; Player2_L = 0;
}

int main()
{
    const char* text1 = "Enter players names (Maximum 15 characters)";
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
            if (CheckCollisionPointRec(GetMousePosition(), Reset_Botton) )
            {
                Mouse_On_Reset_Botton = true;
                if( IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && Possible_To_Reset)
                    ResetGame(); //start, gameOver, winner, playerOneTurn);
            }
            else Mouse_On_Reset_Botton = false;
            // Reset button functionality
            if (CheckCollisionPointRec(GetMousePosition(), Start_Botton) )
            Mouse_On_Start_Botton = true;
            else Mouse_On_Start_Botton = false;

            if (strcmp(Player_One_Name, Player_Two_Name) == 0 && strlen(Player_One_Name) > 0) Error = true;
            else Error = false;
            if (strlen(Player_One_Name) > 0 && strlen(Player_Two_Name) > 0 && !Error) Possible_To_Start = true;
            else Possible_To_Start = false;
            if (CheckCollisionPointRec(GetMousePosition(), Start_Botton) && Possible_To_Start)
            {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    start = true;
                    Possible_To_Reset = true;
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
            DrawTextEx(Main_Font, text1, (Vector2){(screenWidth - MeasureTextEx(Main_Font, text1, Main_Font.baseSize, spacing).x) / 2, (screenHeight - MeasureTextEx(Main_Font, text1, Main_Font.baseSize, spacing).y) / 2}, Main_Font.baseSize, spacing, WHITE);
            DrawRectangle(500, 450, 400, 50, Active_Text_Box == 1 ? LIGHTGRAY : WHITE);
            DrawRectangleLines(500, 450, 400, 50, Active_Text_Box == 1 ? BLUE : DARKGRAY);
            DrawTextEx(Main_Font, Player_One_Name, (Vector2){510, 465}, Main_Font.baseSize, spacing, BLACK);
            DrawRectangle(500, 520, 400, 50, Active_Text_Box == 2 ? LIGHTGRAY : WHITE);
            DrawRectangleLines(500, 520, 400, 50, Active_Text_Box == 2 ? BLUE : DARKGRAY);
            DrawTextEx(Main_Font, Player_Two_Name, (Vector2){510, 535}, Main_Font.baseSize, spacing, BLACK);
            DrawRectangleRec(Start_Botton, Mouse_On_Start_Botton ? GREEN : WHITE);
            DrawTextEx(Main_Font, "Start" , (Vector2){120, 710}, Main_Font.baseSize, spacing, Possible_To_Start ? BLACK : LIGHTGRAY );
            DrawRectangleRec(Reset_Botton, Mouse_On_Reset_Botton ? GREEN : WHITE);
            DrawTextEx(Main_Font, "Reset", (Vector2){1212, 710}, Main_Font.baseSize, spacing, Possible_To_Reset ? BLACK : LIGHTGRAY );
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
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !End_Game)
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
            if (CheckCollisionPointRec(GetMousePosition(), Reset_Botton) )
            {
                Mouse_On_Reset_Botton = true;
                if( IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && Possible_To_Reset && !gameOver )
                    ResetGame(); //start, gameOver, winner, playerOneTurn);
            }
            else Mouse_On_Reset_Botton = false;
            // Start button functionality
            if (CheckCollisionPointRec(GetMousePosition(), Start_Botton) )
            Mouse_On_Start_Botton = true;
            else Mouse_On_Start_Botton = false;

            BeginDrawing();
            ClearBackground(RAYWHITE);

            // Draw game background
            Rectangle source = {0, 0, (float)gameBackground.width, (float)gameBackground.height};
            Rectangle dest = {0, 0, (float)screenWidth, (float)screenHeight};
            Vector2 origin = {0, 0};
            DrawTexturePro(gameBackground, source, dest, origin, 0.0f, WHITE);

            DrawTextEx(Main_Font, Player_One_Name, (Vector2){200 - (MeasureTextEx(Main_Font, Player_One_Name, Main_Font.baseSize, spacing).x) / 2, 150}, Main_Font.baseSize, spacing, BLACK);
            DrawTextEx(Main_Font, Player_Two_Name, (Vector2){1200 - (MeasureTextEx(Main_Font, Player_Two_Name, Main_Font.baseSize, spacing).x) / 2, 150}, Main_Font.baseSize, spacing, BLACK);
            DrawTextEx(Main_Font, "Tic Tac Toe", (Vector2){620, 12}, Main_Font.baseSize, spacing, BLACK);
            if ( !End_Game ) Display_Board();

            Display_Menu( Main_Font );
            // new game ( 100 , 350 , 100 , 50 )
            // exit ( 100 , 400 , 100 , 450 )
            if ( IsMouseButtonPressed(MOUSE_LEFT_BUTTON) )
            {
                if ( CheckCollisionPointRec ( GetMousePosition() , {100 , 350 , 200 , 50} ) && Possible_To_Reset )
                {
                    First_Time = true;
                    ResetGame();
                }
                else if ( CheckCollisionPointRec ( GetMousePosition() , {100, 400, 200, 50} ) )
                {
                    return 0;
                }
            }

            // Draw the score table
            Score_Board( Main_Font );

            // Draw Win/Draw text on the left side of the game field
            if (gameOver)
            {
                if (winner == 0 && !End_Game)
                {
                    DrawTextEx(Main_Font, "Draw!", (Vector2){(screenWidth - MeasureTextEx(Main_Font, "Draw!", Main_Font.baseSize, spacing).x) / 2, 560}, Main_Font.baseSize, spacing, BLACK);
                }
                else if ( !End_Game )
                {
                    char Win_Message[20]; 
                    strcpy(Win_Message, winner == 1 ? Player_One_Name : Player_Two_Name); 
                    strcat(Win_Message, " Won!"); 
                    DrawTextEx(Main_Font, Win_Message, (Vector2){(screenWidth - MeasureTextEx(Main_Font, Win_Message, Main_Font.baseSize, spacing).x) / 2, 560}, Main_Font.baseSize, spacing, BLACK);
                    playerOneTurn ? playerOneTurn = true : playerOneTurn = false;
                }
                // Display message to continue
                DrawTextEx( Main_Font , "Would you like to continue?" , (Vector2){980 , 260} , Main_Font.baseSize , spacing , BLACK );
                // DrawRectangle (1050 , 310 , MeasureTextEx(Main_Font , "YES" , Main_Font.baseSize , spacing).x , 30 , WHITE);
                DrawTextEx(Main_Font , "YES" , (Vector2){1050 , 310}, Main_Font.baseSize, spacing, BLACK);
                // DrawRectangle (1240 , 310 , MeasureTextEx(Main_Font , "NO" , Main_Font.baseSize , spacing).x , 30 , WHITE);
                DrawTextEx(Main_Font , "NO" , (Vector2){1240 , 310}, Main_Font.baseSize, spacing, BLACK);
                if (CheckCollisionPointRec ( GetMousePosition() , {1050 , 310 , MeasureTextEx(Main_Font , "YES" , Main_Font.baseSize , spacing).x , 30 } ) && !End_Game )
                {
                    Mouse_On_Yes = true;
                    if ( IsMouseButtonPressed(MOUSE_LEFT_BUTTON) )
                    {
                        First_Time = false;
                        for (int i = 0; i < BOARD_SIZE; i++)
                        {
                            for (int j = 0; j < BOARD_SIZE; j++)
                            {
                                board[i][j] = 0;
                            }
                        }
                        gameOver = false;
                    }
                }
                else Mouse_On_Yes = false;
                if ( CheckCollisionPointRec ( GetMousePosition() , {1240 , 310 , MeasureTextEx(Main_Font , "NO" , Main_Font.baseSize , spacing).x , 30} )  && !End_Game)
                {
                    Mouse_On_No = true;
                    if ( IsMouseButtonPressed(MOUSE_LEFT_BUTTON) ) End_Game = true;
                }
                else Mouse_On_No = false;
                if ( Mouse_On_Yes ) DrawLine ( 1050 , 340 , 1050 + MeasureTextEx(Main_Font , "YES" , Main_Font.baseSize , spacing).x , 340 , BLACK );
                if ( Mouse_On_No ) DrawLine ( 1240 , 340 , 1240 + MeasureTextEx(Main_Font , "NO" , Main_Font.baseSize , spacing).x , 340 , BLACK );

            }
            else
            {
            // Display turn
            char Turn_Message[20]; 
            strcpy ( Turn_Message , playerOneTurn ? Player_One_Name : Player_Two_Name ); 
            strcat ( Turn_Message , "'s turn"); 
            DrawTextEx (Main_Font , Turn_Message , (Vector2){( screenWidth - MeasureTextEx ( Main_Font , Turn_Message , Main_Font.baseSize , spacing ).x ) / 2, 560} , Main_Font.baseSize , spacing , BLACK );
            }
            
            // Strat and Reset buttons
            DrawRectangleRec(Start_Botton, Mouse_On_Start_Botton ? GREEN : WHITE );
            DrawTextEx(Main_Font, First_Time ? "Start" : "Again" , (Vector2){120, 710}, Main_Font.baseSize, spacing, ( Possible_To_Start && !gameOver ) ? BLACK : LIGHTGRAY);
            DrawRectangleRec(Reset_Botton, Mouse_On_Reset_Botton ? GREEN : WHITE );
            DrawTextEx(Main_Font, "Reset", (Vector2){1212, 710}, Main_Font.baseSize, spacing, ( Possible_To_Reset && !gameOver ) ? BLACK : LIGHTGRAY);
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