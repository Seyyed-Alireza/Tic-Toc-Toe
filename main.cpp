#include <iostream>
#include <raylib.h>
#include <cstring>

void Display_Board()
{
    for ( int i = 0 ; i <= 5 ; i++ )
    {
        DrawLine ( 450 + i * 100 , 50 , 450 + i * 100 , 550 , BLACK );
        DrawLine ( 450 , 50 + i * 100 , 950 , 50 + i * 100 , BLACK );
    }
}
void Score_Board()
{
    for ( int i = 0 ; i <= 3 ; i++ )
    {
        DrawLine ( 550 + i * 100 , 600 , 550 + i * 100 , 750 , BLACK );
    }
    for ( int i = 0 ; i <= 4 ; i++ )
    {
        DrawLine ( 550 , 600 + i * 37.5 , 850 , 600 + i * 37.5 , BLACK );
    }
}


int main()
{
    const char* text1 = "Enter players names (Maximum 15 characters)";
    int screenWidth = 1400;
    int screenHeight = 800;
    InitWindow ( screenWidth , screenHeight , "Tic Tac Toe" );
    Texture2D background = LoadTexture ( "images/background.png" );
    Font Main_Font = LoadFont ( "fonts/calibri.ttf" );      
    SetTextureFilter(Main_Font.texture, TEXTURE_FILTER_POINT);    
    float spacing = 1.5f;
    SetTargetFPS (60);

    bool Player_One_Input_Finished = false;
    bool Player_Two_Input_Finished = false;
    bool start = false;
    bool Error = false;
    bool Possible_To_Start = false;
    char Player_One_Name[16] = "";
    char Player_Two_Name[16] = "";
    Rectangle Start_Botton = { 50 , 700 , 200 , 50 };
    Rectangle Reset_Botton = { 1150 , 700 , 200 , 50 };
    int Active_Text_Box = 1; 
    int key1 , key2; 
    int Player1_W = 3 , Player2_W = 0 , Player1_D = 0 , Player2_D = 0 , Player1_L = 0 , Player2_L = 0;
    while ( !WindowShouldClose() )
    {
        if ( !start )
        {
            if ( IsMouseButtonPressed ( MOUSE_LEFT_BUTTON ) ) 
            {
                Vector2 mousePos = GetMousePosition();
                if (CheckCollisionPointRec(mousePos, {500, 450, 400, 50})) {
                    Active_Text_Box = 1; 
                    Player_One_Input_Finished = false;
                    Player_Two_Input_Finished = true;
                } else if (CheckCollisionPointRec(mousePos, {500, 520, 400, 50})) {
                    Active_Text_Box = 2; 
                    Player_One_Input_Finished = true;
                    Player_Two_Input_Finished = false;
                }
            }

            if (Active_Text_Box == 1) 
            {
                key1 = GetCharPressed();
                while ( key1 > 0 ) 
                {
                    if ( strlen ( Player_One_Name ) < 15 )  
                    {
                        Player_One_Name[strlen ( Player_One_Name )] = static_cast<char>(key1);
                        Player_One_Name[strlen ( Player_One_Name ) + 1] = '\0';
                    }
                    key1 = GetCharPressed();
                }
                if ( IsKeyPressed ( KEY_BACKSPACE ) && strlen ( Player_One_Name ) > 0) 
                {
                    Player_One_Name[strlen ( Player_One_Name ) - 1] = '\0'; 
                }
                if ( IsKeyPressed ( KEY_ENTER ) ) 
                {
                    Player_One_Input_Finished = true;
                    Active_Text_Box = 2; 
                }
            }
            else if ( !Player_Two_Input_Finished && Active_Text_Box == 2 && Player_One_Input_Finished) 
            {
                key2 = GetCharPressed();
                while ( key2 > 0 ) 
                {
                    if ( strlen ( Player_Two_Name ) < 15 )  
                    {
                        Player_Two_Name[strlen ( Player_Two_Name )] = static_cast<char>(key2);
                        Player_Two_Name[strlen ( Player_Two_Name ) + 1] = '\0';
                    }
                    key2 = GetCharPressed();
                }
                if ( IsKeyPressed ( KEY_BACKSPACE ) && strlen ( Player_Two_Name ) > 0) 
                {
                    Player_Two_Name[strlen ( Player_Two_Name ) - 1] = '\0'; 
                }
                if ( IsKeyPressed ( KEY_ENTER ) ) 
                {
                    Player_Two_Input_Finished = true;
                }
            }
            if ( strcmp ( Player_One_Name , Player_Two_Name ) == 0 && strlen ( Player_One_Name ) > 0 ) Error = true;
            else Error = false;
            if ( strlen ( Player_One_Name ) > 0 && strlen ( Player_Two_Name ) > 0 && !Error ) Possible_To_Start = true;
            else Possible_To_Start = false;
            if ( CheckCollisionPointRec ( GetMousePosition(), Start_Botton ) && Possible_To_Start ) 
            {
                if ( IsMouseButtonPressed ( MOUSE_LEFT_BUTTON ))  
                {
                    start = true; 
                }
            }
            BeginDrawing();
            ClearBackground ( RAYWHITE );
            Rectangle source = { 0, 0, (float)background.width, (float)background.height };
            Rectangle dest = { 0, 0, (float)screenWidth, (float)screenHeight };
            Vector2 origin = { 0, 0 }; 
            DrawTexturePro(background, source, dest, origin, 0.0f, WHITE);
            DrawTextEx ( Main_Font , text1 , (Vector2){ (screenWidth - MeasureTextEx(Main_Font , text1 , Main_Font.baseSize , spacing).x) / 2 , (screenHeight - MeasureTextEx(Main_Font , text1 , Main_Font.baseSize , spacing).y) / 2 } , Main_Font.baseSize , spacing , BLACK );
            DrawRectangle ( 500 , 450 , 400 , 50 , Active_Text_Box == 1 ? LIGHTGRAY : WHITE );
            DrawRectangleLines(500, 450, 400, 50, Active_Text_Box == 1 ? BLUE : DARKGRAY);
            DrawTextEx( Main_Font , Player_One_Name , (Vector2){ 510 , 465} , Main_Font.baseSize , spacing , BLACK);
            DrawRectangle ( 500 , 520 , 400 , 50 , Active_Text_Box == 2 ? LIGHTGRAY : WHITE );
            DrawRectangleLines(500, 520, 400, 50, Active_Text_Box == 2 ? BLUE : DARKGRAY);
            DrawTextEx( Main_Font , Player_Two_Name , (Vector2){ 510 , 535} , Main_Font.baseSize , spacing , BLACK);
            DrawRectangleRec ( Start_Botton , Possible_To_Start ? GREEN : LIGHTGRAY );
            DrawTextEx( Main_Font , "Start" , (Vector2){ 120 , 710} , Main_Font.baseSize , spacing , BLACK);
            DrawRectangleRec ( Reset_Botton , GREEN );
            DrawTextEx( Main_Font , "Reset" , (Vector2){ 1212 , 710} , Main_Font.baseSize , spacing , BLACK);
            if ( Error )
            {
                DrawTextEx( Main_Font , "Names can't be same" , (Vector2){ 550 , 585} , Main_Font.baseSize , spacing , BLACK);
            }
            EndDrawing();
        }
        else 
        {
            BeginDrawing();
            ClearBackground ( RAYWHITE );
            Rectangle source = { 0, 0, (float)background.width, (float)background.height };
            Rectangle dest = { 0, 0, (float)screenWidth, (float)screenHeight };
            Vector2 origin = { 0, 0 }; 
            DrawTexturePro(background, source, dest, origin, 0.0f, WHITE);
            DrawTextEx( Main_Font , Player_One_Name , (Vector2){ 20 , 20 } , Main_Font.baseSize , spacing , BLACK);
            // DrawTextEx( Main_Font , "Wins: " , (Vector2){ 20 , 50 } , Main_Font.baseSize , spacing , BLACK);
            // DrawTextEx( Main_Font , "Draws: " , (Vector2){ 20 , 80 } , Main_Font.baseSize , spacing , BLACK);
            DrawTextEx( Main_Font , Player_Two_Name , (Vector2){ 1200 , 20 } , Main_Font.baseSize , spacing , BLACK);
            // DrawTextEx( Main_Font , "Wins: " , (Vector2){ 1200 , 50 } , Main_Font.baseSize , spacing , BLACK);
            // DrawTextEx( Main_Font , "Draws: " , (Vector2){ 1200 , 80 } , Main_Font.baseSize , spacing , BLACK);
            DrawTextEx( Main_Font , "Tic Toc Toe" , (Vector2){ 620 , 12 } , Main_Font.baseSize , spacing , BLACK);
            Display_Board();
            DrawTextEx( Main_Font , "Score Board" , (Vector2){ 620 , 560 } , Main_Font.baseSize , spacing , BLACK);
            Score_Board();
            DrawTextEx( Main_Font , "Wins" , (Vector2){ 577 , 645 } , 20 , spacing , BLACK);
            DrawTextEx( Main_Font , "Loses" , (Vector2){ 575 , 683 } , 20 , spacing , BLACK);
            DrawTextEx( Main_Font , "Draws" , (Vector2){ 572 , 721 } , 20 , spacing , BLACK);
            DrawTextEx( Main_Font , Player_One_Name , (Vector2){ 700 - ( MeasureTextEx( Main_Font , Player_One_Name , strlen ( Player_One_Name ) < 8 ? 20 : 12 , spacing).x) / 2 , 610 } , strlen ( Player_One_Name ) < 8 ? 20 : 12 , spacing , BLACK);
            DrawTextEx( Main_Font , Player_Two_Name , (Vector2){ 800 - ( MeasureTextEx( Main_Font , Player_Two_Name , strlen ( Player_One_Name ) < 8 ? 20 : 12 , spacing).x) / 2 , 610 } , strlen ( Player_One_Name ) < 8 ? 20 : 12 , spacing , BLACK);

            DrawRectangleRec ( Start_Botton , Possible_To_Start ? GREEN : LIGHTGRAY );
            DrawTextEx( Main_Font , "Start" , (Vector2){ 120 , 710} , Main_Font.baseSize , spacing , BLACK);
            DrawRectangleRec ( Reset_Botton , GREEN );
            DrawTextEx( Main_Font , "Reset" , (Vector2){ 1212 , 710} , Main_Font.baseSize , spacing , BLACK);
            EndDrawing();
        }
    }

    UnloadTexture(background);
    UnloadFont(Main_Font);
    CloseWindow();

    return 0;
}