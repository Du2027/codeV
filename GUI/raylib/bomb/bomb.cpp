#include <raylib.h>
#include <stdio.h>
#include <stdbool.h>

int fpsc = 60;

class Bomb{
   public:
      Vector2 pos;
      double size;
      bool exploded;

      Bomb(Vector2 win_size){
         size = GetRandomValue(20, 50);

         pos.x = GetRandomValue(10, win_size.x - 10 - size);
         pos.y = GetRandomValue(10, win_size.y - 10 - size);
      }

      void draw(Texture textur){
         //DrawRectangle(pos.x, pos.y + 5, size + 5, size + 5, WHITE);
         DrawTextureEx(textur, pos, 0, size / 145, WHITE);
      }
};

int main(){
   Vector2 window_size = {700,700};
   Color bg_color = {100,100,100,255};
   int bombc = 0;

   InitWindow(window_size.x, window_size.y, "cpptry");
   SetTargetFPS(fpsc);

   Image bombs_imgs[5] = {LoadImage("bomb_v2.png"), LoadImage("bomb_v2_2.png"),LoadImage("bomb_v2_3.png"),LoadImage("bomb_v2_4.png"),LoadImage("bomb_v2_final.png")};
   Texture bombs[5] = {LoadTextureFromImage(bombs_imgs[0]),LoadTextureFromImage(bombs_imgs[1]),LoadTextureFromImage(bombs_imgs[2]),LoadTextureFromImage(bombs_imgs[3]),LoadTextureFromImage(bombs_imgs[4])};

   Bomb bomb1 = Bomb(window_size);
   char bombc_str[50] = {0};
   Vector2 mousepos;
   short framec = 0;
   short frame_to_load = 0;
   bool explosion = false;

   while(!WindowShouldClose() && explosion == false){
      if(framec >= 300){
         framec = 0;
         frame_to_load = 0;
      }
      framec++;
      switch (framec) {
         case 0:
            frame_to_load = 0;
            break;
         case 60:
            frame_to_load = 1;
            break;
         case 120:
            frame_to_load = 2;
            break;
         case 180:
            frame_to_load = 3;
            break;
         case 280:
            frame_to_load = 4;
      }

      if(IsMouseButtonPressed(0)){
         mousepos = GetMousePosition();
         if(mousepos.x >= bomb1.pos.x && mousepos.x <= bomb1.pos.x + bomb1.size + 5){
            if(mousepos.y >= bomb1.pos.y && mousepos.y + 5 <= bomb1.pos.y + bomb1.size + 5 + 5){
               framec = 300;
               bomb1 = Bomb(window_size);
               bombc++;
            }
         }
      }
      else if(IsKeyPressed(KEY_N)){
         framec = 300;
         bomb1 = Bomb(window_size);
         bombc++;
      }


      BeginDrawing();
         ClearBackground(bg_color);
         sprintf(bombc_str, "%d", bombc);
         DrawText(bombc_str, window_size.x - 25, 20, 20, WHITE);  // Handler to move text to left if text > 10 and > 100
         bomb1.draw(bombs[frame_to_load]);
      EndDrawing();

      if(framec == 299){
         explosion = true;
      }
   }

   CloseWindow();
   return 0;
}
