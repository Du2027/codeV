/*
   g++ game.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
   BUGS:
      Gamesize ~32 pixels smaller than window???
*/

#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#define FPS 60
#define STATEFRAMES 25
#define OFFSET_DIVIDEND 2.5 // Offset between pipes as game_size. / _DIVIDEND
#define BIRD_SCALE 1.8

float biggest_scaling(int img_sizex, int img_sizey, int finalx, int finaly){
   float result;
   result = (float) finalx / img_sizex;

   if((float) finaly / img_sizey > result){
      result = (float) finaly / img_sizey;
   }
   return result;
}

enum Bird_state{
   FALLING = -1,
   FLOATING = 0,
   RISING = 1
};

class Roehre{
   public:
      Rectangle rorec;
      Vector2 game_size;
      Texture textures[2];
      int offset;

      Roehre(Vector2 game_size){
         this->offset = game_size.y / OFFSET_DIVIDEND;
         this->game_size = game_size;
         this->rorec = (Rectangle) {game_size.x, 0, game_size.x / 10.5f, (float) GetRandomValue(30, game_size.y - offset)};

         // Load texture for part
         Image img = LoadImage("assets/block.png");
         ImageResize(&img, rorec.width, rorec.height);
         textures[0] = LoadTextureFromImage(img);

         // Load texture for part2
         img = LoadImage("assets/block.png");
         ImageResize(&img, rorec.width, game_size.y - offset - rorec.height + 30);
         textures[1] = LoadTextureFromImage(img);
      }

      short naehern(float ppf){
         rorec.x = rorec.x - ppf;
         return 0;
      }

      void respawn(){
         rorec.x = game_size.x;
         rorec.height = GetRandomValue(30, game_size.y - offset);

         Image img = LoadImage("assets/block.png");
         ImageResize(&img, rorec.width, rorec.height);
         textures[0] = LoadTextureFromImage(img);

         img = LoadImage("assets/block.png");
         ImageResize(&img, rorec.width, game_size.y - offset - rorec.height + 30);
         textures[1] = LoadTextureFromImage(img);
      }

      void draw(){
         DrawTextureEx(textures[0], (Vector2) {rorec.x, rorec.y}, 0, 1, WHITE);
         DrawTextureEx(textures[1], (Vector2) {rorec.x,(float) rorec.height + offset}, 0, 1, WHITE);
      }

      ~Roehre(){
         UnloadTexture(textures[0]);
         UnloadTexture(textures[1]);
      }
};

int main() {
   InitWindow(0, 0, "tmp_win");  // tmp win to init main win dynamically

   const Vector2 win_size = {(float) GetMonitorWidth(0), (float) GetMonitorHeight(0)};    // Vector2 is defined as x float and y float :/
   Vector2 game_size = {win_size.x / 2.7f, win_size.y / 1.8f};
   if(win_size.x / 2.7f < 834){game_size.x = 834;}
   if(win_size.y / 1.8f < 469){game_size.y = 469;}

   CloseWindow();

   InitWindow(game_size.x, game_size.y, "Flappenbird");
   SetTargetFPS(FPS);
   float fps_diff = 60.0 / FPS;
   InitAudioDevice();

   Image settings_img = LoadImage("assets/settings.png");
   Image clicker = LoadImage("assets/click.png");
   Image bird_img = LoadImage("assets/flappenbitd.png");
   Image bird2_img = LoadImage("assets/flappenbitd2.png");
   Image bg_images[] = {
      LoadImage("background_ani/bg1.png"), LoadImage("background_ani/bg2-.png"), LoadImage("background_ani/bg3-.png"),
      LoadImage("background_ani/bg4-.png"), LoadImage("background_ani/bg5-.png"), LoadImage("background_ani/bg6-.png"),
      LoadImage("background_ani/bg7-.png"), LoadImage("background_ani/bg8-.png"),

      LoadImage("background_ani/bg2+.png"), LoadImage("background_ani/bg3+.png"), LoadImage("background_ani/bg4+.png"),
      LoadImage("background_ani/bg5+.png"), LoadImage("background_ani/bg6+.png"), LoadImage("background_ani/bg7+.png")
   };

   Texture bg_texture;
   Texture settings_texture = LoadTextureFromImage(settings_img);
   Texture click_please = LoadTextureFromImage(clicker);
   Texture bird_textures[2] = {LoadTextureFromImage(bird_img), LoadTextureFromImage(bird2_img)};
   Texture BirdText_to_load;
   Texture bg_textures[14];
   for (int i = 0; i < sizeof(bg_images) / sizeof(Image); i++) {
      ImageResize(&bg_images[i], game_size.x, game_size.y);
      bg_textures[i] = LoadTextureFromImage(bg_images[i]);
   }

   Sound flap = LoadSound("sounds/up.wav");
   Sound death = LoadSound("sounds/death.ogg");
   Music background_music = LoadMusicStream("sounds/background.mp3");
   Music menu_music = LoadMusicStream("sounds/menu.mp3");
   SetMusicVolume(menu_music, 0.5);
   SetMusicVolume(background_music, 0.5);

   Vector2 bird_pos = {(float) game_size.x / 4, (float) game_size.y / 2};
   enum Bird_state bird_state = FLOATING;
   Vector2 mousepos;

   bool menu = true;
   bool settings = false;
   bool escape = false;
   bool beginn = true;
   float ychange = game_size.y / 100;
   Roehre ro1 = Roehre(game_size);
   short score = 0;
   char score_str[10] = {0};

   double state_framec = 0;
   double framec = 0;
   short bg_imagec = 1;

   while (!WindowShouldClose()) {
      // Loop Music
      if(!IsMusicStreamPlaying(background_music) && menu == false){PlayMusicStream(background_music);}
      else if(!IsMusicStreamPlaying(menu_music) && menu == true){PlayMusicStream(menu_music);}
      if(menu == false){UpdateMusicStream(background_music);}
      else if(menu == true){UpdateMusicStream(menu_music);}

      // Bird animation with variable framerate
      if (framec == 60) {
         framec = 0;
         BirdText_to_load = bird_textures[0];
         if(menu == false){bg_imagec++;}
      }

      framec = framec + 1 * fps_diff;

      if(framec == 0.25 * FPS || framec == 0.75 * FPS){
         BirdText_to_load = bird_textures[1];
         if(menu == false){bg_imagec++;}
      }
      else if(framec == 0.5 * FPS){
         BirdText_to_load = bird_textures[0];
         if(menu == false){bg_imagec++;}
      }

      if(bg_imagec >= 14){bg_imagec = 0;}

      // Settings Menu
      if(menu == true){
         mousepos = GetMousePosition();

         if(IsMouseButtonPressed(0)){
            // Settings Button
            if(mousepos.x >= game_size.x / 1.112f  && mousepos.x <= game_size.x / 1.112f + 80 && settings == false){ // change if settings button is changed
               if(mousepos.y >= game_size.y / 1.1f && mousepos.y / 1.1f + 80){
                  settings = true;
               }
            }
            else if(settings){
               // Back Text
               if(mousepos.x >= game_size.x / 3.7 && mousepos.x <= game_size.x / 3.7 + 80){
                  if(mousepos.y >= (game_size.y + 32) / 1.2 && mousepos.y <= (game_size.y + 32) / 1.2 + 20){settings = false;}
               }
            }
            else{
               StopMusicStream(menu_music);
               PlaySound(flap);
               PlayMusicStream(background_music);
               menu = false;
               beginn = false;
               bird_state = RISING;
               state_framec = 0;
            }
         }
      }
      else {
         // Handling of Bird states
         if(IsMouseButtonPressed(0)){
            bird_state = RISING;
            state_framec = 0;
         }

         if(bird_state == RISING){
            bird_pos.y = bird_pos.y - ychange * fps_diff + (game_size.y / 700) * fps_diff;
         }

         if(bird_state == FALLING){
            bird_pos.y = bird_pos.y + pow(ychange, state_framec / 38) + 4 * fps_diff;
         }

         if(bird_state == RISING && state_framec == 25){
            state_framec = 0;
            bird_state = FLOATING;
         }

         if(bird_state == FLOATING && state_framec >= 6){
            state_framec = 0;
            bird_state = FALLING;
         }

         state_framec = state_framec + 1 * fps_diff;

         // Wall Collisions
         if(bird_pos.y < 0){
            bird_pos.y = 0;
            escape = true;
         }

         if(bird_pos.y > game_size.y - 15){
            bird_pos.y = game_size.y - 15;
            escape = true;
         }

         // Pipe Collision
         if(CheckCollisionRecs((Rectangle){bird_pos.x, bird_pos.y, 48 * BIRD_SCALE - 0.5, 30 * BIRD_SCALE - 0.5}, ro1.rorec)){escape = true;}
         if(CheckCollisionRecs((Rectangle){bird_pos.x, bird_pos.y, 48 * BIRD_SCALE - 0.5, 30 * BIRD_SCALE - 0.5}, (Rectangle){ro1.rorec.x, ro1.rorec.height + ro1.offset, ro1.rorec.width, game_size.y - ro1.offset - ro1.rorec.height + 30})){escape = true;}

         // Respawn pipe if out of screen
         ro1.naehern(3 * fps_diff);
         if(ro1.rorec.x < 0 - ro1.rorec.width){
            ro1.rorec.x = game_size.x;
            ro1.respawn();
            score++;
         }

         // On collision
         if(escape == true){
            menu = true;
            escape = false;
            sprintf(score_str, "%hd", score);
            score = 0;
            ro1.respawn();
            ro1.rorec.x = game_size.x;
            bird_pos = {(float) game_size.x / 4, (float) game_size.y / 2};

            StopMusicStream(background_music);
            PlaySound(death);
         }
      }

      BeginDrawing();
         bg_texture = bg_textures[bg_imagec];
         DrawTextureEx(bg_texture, (Vector2){0, 0}, 0, 1.1, WHITE);
         DrawTextureEx(BirdText_to_load, bird_pos, 0, BIRD_SCALE, WHITE);

         if(menu == true){
            if(!settings){
               DrawText("Flappenbitd", game_size.x / 3.7, game_size.y / 5, 60, YELLOW);
               DrawTextureEx(settings_texture, (Vector2){game_size.x / 1.112f, game_size.y / 1.1f}, 0, 1, WHITE);
               if(beginn == true){
                  DrawTextureEx(click_please, (Vector2){(float) game_size.x / 1.8f, (float) game_size.y / 2}, 0, 2, WHITE);
               }
               else{
                  DrawText(score_str, (float) game_size.x / 1.8f, (float) game_size.y / 2, 120, YELLOW);
               }
            }
            else {
               // shade background
               DrawRectangle(0, 0, game_size.x, game_size.y + 32, (Color){0,0,0,125});
               // Menu-background
               DrawRectangle(game_size.x / 2 - game_size.x / 4, (game_size.y + 32) / 2 - (game_size.y + 32) / 1.3 / 2, game_size.x / 2, (game_size.y + 32) / 1.3, (Color){0,0,0,220});
               // Settingstext
               DrawText("Settings", game_size.x / 2 - 42 , (game_size.y + 32) / 2 - (game_size.y + 32) / 1.3 / 2 + 30, 20, WHITE);
               DrawText("<- Back", game_size.x / 3.7, (game_size.y + 32) / 1.2, 20, WHITE);
            }
         }
         else {
            sprintf(score_str, "%hd", score);
            DrawText(score_str, 20, 20, 40, YELLOW);
         }
         ro1.draw();
      EndDrawing();
   }

   CloseAudioDevice();
   CloseWindow();
   return 0;
}
