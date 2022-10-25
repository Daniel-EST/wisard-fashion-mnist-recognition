// #include <raylib.h>
// #include "canvas.h"

// void start_canvas(wisardpkg::Wisard* w)
// {
//     int counter{};
//     std::vector<std::string> out;

//     std::vector<int> painting(IMAGE_WIDTH * IMAGE_HEIGHT, 0);
//     std::vector<std::vector<int>> values;
//     values.push_back(painting);

//     int index;
//     Color pixel;

//     InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Doodle Canvas");
//     SetConfigFlags(FLAG_VSYNC_HINT);
//     SetTargetFPS(60);

//     RenderTexture2D target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
//     Rectangle CanvasArea{
//         200, (float) (GetScreenHeight() / 2) - (IMAGE_HEIGHT / 2), 
//         IMAGE_WIDTH, IMAGE_HEIGHT
//     };

//     BeginTextureMode(target);
//         DrawRectangleRec(CanvasArea, BLACK);
//     EndTextureMode();

//     while(!WindowShouldClose())
//     {
//          Vector2 mousePos = GetMousePosition();

//         if(CheckCollisionPointRec(GetMousePosition(), CanvasArea))
//         {
//             if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
//             {
//                 BeginTextureMode(target);
//                     DrawCircle(mousePos.x, mousePos.y, LINE_THICKNESS, WHITE);
//                     DrawLineEx((Vector2){ mousePos.x, mousePos.y }, (Vector2){ GetMouseDelta().x, GetMouseDelta().y }, LINE_THICKNESS, WHITE);
//                 EndTextureMode();
//             }
//         }

//         // if (IsKeyPressed(KEY_F5))
//         // {
//         //     ExportImage(image, "cat.png");
//         //     UnloadImage(image);
//         // }

//         if((counter % 120) == 0)
//         {
//             Image image = LoadImageFromTexture(target.texture);
//             ImageFlipVertical(&image);
//             ImageCrop(&image, CanvasArea);
//             Color *colors = LoadImageColors(image);

//             for(int y{}; y < image.height; y++)
//             {
//                 for(int x{}; x < image.width; x++)
//                 {
//                     index = (y * image.width) + x;
//                     pixel = colors[index];     
//                     if(
//                         pixel.r == 0 ||
//                         pixel.g == 0 ||
//                         pixel.b == 0 
//                     )
//                     {
//                         values.at(0)[index] = 1;
//                     }
//                     else
//                     {
//                         values.at(0)[index] = 0;
//                     }
//                 }
//             }
//             UnloadImage(image);
//             out = w->classify(values);
            
//             counter = 0;
//         }

//         BeginDrawing();
//             ClearBackground(SKYBLUE);
//             DrawRectangleRec(CanvasArea, BLACK);
//             DrawTextureRec(target.texture, 
//                 (Rectangle) { 
//                     0, 0, 
//                     (float)target.texture.width, 
//                     (float)-target.texture.height 
//                 }, (Vector2) { 0, 0 }, WHITE);
//             if(CheckCollisionPointRec(GetMousePosition(), CanvasArea))
//                 DrawCircle(
//                     GetMouseX(), GetMouseY(), 
//                     LINE_THICKNESS, 
//                     ColorAlpha(WHITE, 0.75)
//                 );

//             DrawRectangle(750, 75, 500, 575, GRAY);
//             DrawText(TextFormat("%s 70%", out.at(0).c_str()), 775, 100, 32, RAYWHITE);

//             DrawRectangleLinesEx(
//                 (Rectangle)
//                 {
//                     CanvasArea.x - LINE_THICKNESS,
//                     CanvasArea.y - LINE_THICKNESS,
//                     CanvasArea.width + LINE_THICKNESS * 2,
//                     CanvasArea.height + LINE_THICKNESS * 2,
//                 }, 
//                 LINE_THICKNESS, 
//                 SKYBLUE
//             );

//         EndDrawing();
//     }

//     UnloadRenderTexture(target);
//     CloseWindow();
// }