// g++ editor.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

// TODO
//    ZOOM
//    Verzweigungen
//    Loop Fuss & Kopf

#include <raylib.h>
#include <stdio.h>
#include <vector>

#define DEBUG 0
#define MENUSPACE 10
#define FPS 60

class Seqfield {
public:
  Rectangle rec;
  std::vector<char> text;

  Seqfield(Rectangle rec) { this->rec = rec; }

  void UpdatePos(Vector2 Mousepos) {
    this->rec.x = Mousepos.x;
    this->rec.y = Mousepos.y;
  }

  void UpdateText(char Char) { text.push_back(Char); }

  void Draw() {
    // char Vector to char[]
    char TexttoDraw[text.size() + 1];
    for (int i = 0; i < text.size(); i++) {
      TexttoDraw[i] = text.at(i);
    }

    TexttoDraw[text.size()] = 0;

    if (MeasureText(TexttoDraw, 20) + 8 * 2 > rec.width) {
      rec.width = MeasureText(TexttoDraw, 20) + 8 * 2;
    }

    DrawRectangle(rec.x, rec.y, rec.width, rec.height,
                  (Color){170, 170, 170, 255});
    DrawText(TexttoDraw, rec.x + 8, rec.y + 8, 20, BLACK);
  }
};

enum HoverState { AWAY = 0, HOVERING = 1 };

enum Mousehold {
  NONE = 0,
  SEQ = 1,
};

int main() {
  InitWindow(0, 0, "tmp_win");
  Vector2 Winsize = {GetMonitorWidth(0) / 2.0f, GetMonitorHeight(0) / 2.0f};
  CloseWindow();

  InitWindow(Winsize.x, Winsize.y, "Struktogramme");
  SetTargetFPS(FPS);

  Rectangle Menurec;
  Vector2 MousePos;
  Color SeqButtonColor = (Color){180, 180, 180, 255};
  HoverState SeqButtonHover = AWAY;
  Mousehold Holding = NONE;
  Seqfield Possable_Seqfield((Rectangle){0, 0, 0, 0});
  bool entermode = false;
  int SeqIndexenter;
  char key;

  std::vector<Seqfield> placedSeqs;

  while (!WindowShouldClose()) {
    Winsize = {
        (float)GetRenderWidth(),
        (float)
            GetRenderHeight()}; // Winsize is 30px different after ~33msek ???
    Menurec = {MENUSPACE, MENUSPACE, Winsize.x / 4, Winsize.y - MENUSPACE * 2};
    MousePos = GetMousePosition();

    if (DEBUG) {
      printf("WINSIZE:%f\t%d\n", Winsize.x, GetRenderHeight());
      printf("MOUSEPOS:%f\t%f\n", MousePos.x, MousePos.y);
      printf("HOLD:%d\n", Holding);
      if (entermode) {
        printf("ENTERMODE\n");
      }

      printf("__\n");
    }

    // Seqbutton
    if (CheckCollisionRecs((Rectangle){(Menurec.x + Menurec.width) / 4,
                                       MENUSPACE * 3 + 40, Menurec.width / 2,
                                       35},
                           (Rectangle){MousePos.x, MousePos.y, 1, 1})) {
      SeqButtonHover = HOVERING;
      if (IsMouseButtonPressed(0) && !entermode) {
        Possable_Seqfield.rec = (Rectangle){0, 0, Winsize.x / 8, 35};
        Holding = SEQ;
      }
    } else {
      SeqButtonHover = AWAY;
    }

    if (IsMouseButtonReleased(0)) {
      if (Holding != NONE) {
        placedSeqs.push_back(Possable_Seqfield);
      }
      Holding = NONE;
    } else if (IsMouseButtonPressed(0)) {
      entermode = false;

      // Check wich Seq is beeing clicked
      if (placedSeqs.size() > 0) {
        short SeqIndex = -1;
        std::vector<char> Text;
        // Find corresponding Seqfield
        for (int i = placedSeqs.size(); i >= 0; i--) {
          if (CheckCollisionRecs(placedSeqs[i].rec,
                                 (Rectangle){MousePos.x, MousePos.y, 1, 1})) {
            SeqIndex = i;
            break;
          }
        }
        if (SeqIndex > -1) {
          entermode = true;
          SeqIndexenter = SeqIndex;
        }
      }
    }

    // Seq handling
    key = GetCharPressed();
    if (IsKeyPressed(KEY_ENTER)) {
      entermode = false;
    } else if (IsKeyPressed(KEY_BACKSPACE)) {
      placedSeqs[SeqIndexenter].text.pop_back();
    } else if (entermode && key > 31 && key < 127) {
      placedSeqs[SeqIndexenter].UpdateText(key);
    } else if (IsKeyPressed(KEY_DELETE)) {
      placedSeqs.erase(
          placedSeqs.begin() +
          SeqIndexenter); // Doesnt work properly bc possable_seqfield
      entermode = false;
    }

    if (Holding != NONE && !entermode) {
      switch (Holding) {
      case SEQ:
        Possable_Seqfield.UpdatePos(MousePos);
        break;
      }
    }

    BeginDrawing();
    ClearBackground(LIGHTGRAY);

    // Menu field
    DrawRectangleRounded(Menurec, 0.05, 20, (Color){150, 150, 150, 255});
    DrawText("Blocks",
             (Menurec.x + Menurec.width + MENUSPACE) / 2 -
                 (MeasureText("Blocks", 40) / 2.0),
             20, 40, BLACK);

    // SeqButton
    if (SeqButtonHover && !entermode) {
      SeqButtonColor = {135, 135, 135, 255};
      DrawRectangleRoundedLinesEx(
          (Rectangle){(Menurec.x + Menurec.width + MENUSPACE) / 4,
                      MENUSPACE * 3 + 40, Menurec.width / 2, 35},
          0.05, 20, 2, BLACK);
    } else {
      SeqButtonColor = (Color){180, 180, 180, 255};
    }
    DrawRectangleRounded(
        (Rectangle){(Menurec.x + Menurec.width + MENUSPACE) / 4,
                    MENUSPACE * 3 + 40, Menurec.width / 2, 35},
        0.15, 20, SeqButtonColor);
    DrawText("Sequenz",
             (MENUSPACE * 2 + Menurec.width) / 2 -
                 (MeasureText("Sequenz", 19) / 2.0),
             MENUSPACE * 4 + 40, 19, BLACK);

    // Seq following cursor
    if (Possable_Seqfield.rec.x != 0 && Holding == SEQ) {
      Possable_Seqfield.Draw();
    }

    for (int i = 0; i < placedSeqs.size(); i++) {
      placedSeqs[i].Draw();
      if (entermode) {
        DrawRectangleLinesEx(placedSeqs[SeqIndexenter].rec, 2, BLACK);
      }
    }
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
