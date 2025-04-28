// g++ editor.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

// TODO
//    ZOOM
//    Verzweigungen
//    Loop Fuss & Kopf

#include <raylib.h>
#include <stdio.h>
#include <vector>

#define DEBUG 1
#define MENUSPACE 10
#define FPS 120

class Field {
public:
  char type; // S,V,L
  Rectangle rec;
  float originalWidth;
  std::vector<char> text;

  Field(Rectangle rec, char type) {
    this->rec = rec;
    this->type = type;
    this->originalWidth = this->rec.width;
  }
  void updatePos(Vector2 newPos) {
    rec.x = newPos.x;
    rec.y = newPos.y;
  }
  void UpdateText(char Char) {
    text.push_back(Char);
  }
  void removeChar() {
    this->text.pop_back();
  }
  void Draw(Color color) {
    // char Vector to char[]
    char TexttoDraw[text.size() + 1];
    for (int i = 0; i < text.size(); i++) {
      TexttoDraw[i] = text.at(i);
    }

    TexttoDraw[text.size()] = 0;

    if (MeasureText(TexttoDraw, 20) + 8 * 2 > originalWidth) {
      rec.width = MeasureText(TexttoDraw, 20) + 8 * 2;
    } else {
      rec.width = originalWidth;
    }

    DrawRectangle(rec.x, rec.y, rec.width, rec.height, color);
    DrawText(TexttoDraw, rec.x + 8, rec.y + 8, 20, BLACK);
  }
};

class ManagerBlock {
public:
  std::vector<Field> fields;

  ManagerBlock() {
    this->fields = std::vector<Field>();
  }

  int IndexIfClick(Vector2 MousePos) {
    for (int i = 0; i < fields.size(); i++) {
      if (MousePos.x > fields[i].rec.x && MousePos.x < fields[i].rec.x + fields[i].rec.width) {
        if (MousePos.y > fields[i].rec.y && MousePos.y < fields[i].rec.y + fields[i].rec.height) {
          return i;
        }
      }
    }
    return -1;
  }

  void draw() {
    float biggestw = 0;
    float height = 0;

    for (int i = 0; i < fields.size(); i++) {
      if (fields.at(i).rec.width > biggestw) {
        biggestw = fields.at(i).rec.width;
      }
      height = height + fields[i].rec.height;
    }

    DrawRectangle(fields.at(fields.size() - 1).rec.x - 2, fields.at(fields.size() - 1).rec.y - 2, biggestw + 4, height + 2 * fields.size() + 2, BLUE);

    for (int n = 0; n < fields.size(); n++) {
      fields[n].Draw((Color){170, 170, 170, 255});
    }
  }
};

enum HoverState {
  AWAY = 0,
  HOVERING = 1
};

enum Mousehold {
  NONE = 0,
  SEQ = 1,
};

enum FocusMode {
  DEFAULT = 0,
  ENTER = 1,
};

struct FieldID {
  int ManagerIndex;
  int FieldIndex;
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
  FocusMode Mode = DEFAULT;

  FieldID WIP_Field;
  char key;
  std::vector<ManagerBlock> Managers = {};

  while (!WindowShouldClose()) {
    Winsize = {(float)GetRenderWidth(), (float)GetRenderHeight()}; // Winsize is 30px different after ~33msek ???
    Menurec = {MENUSPACE, MENUSPACE, Winsize.x / 4, Winsize.y - MENUSPACE * 2};
    MousePos = GetMousePosition();

    // Seqbutton
    if (CheckCollisionRecs((Rectangle){(Menurec.x + Menurec.width) / 4, MENUSPACE * 3 + 40, Menurec.width / 2, 35}, (Rectangle){MousePos.x, MousePos.y, 1, 1}) && Holding == NONE) {
      SeqButtonHover = HOVERING;
      if (IsMouseButtonDown(0) && Mode == DEFAULT) {
        ManagerBlock manBlock;
        Managers.push_back(manBlock);
        Field firstField = Field((Rectangle){0, 0, Winsize.x / 8, Winsize.y / 20}, 'S');
        Managers.back().fields.push_back(firstField);
        Holding = SEQ;
      }
    } else {
      SeqButtonHover = AWAY;
    }

    if (Holding != NONE && Managers.size() > 0) {
      Managers.back().fields.back().updatePos(MousePos);
    }

    if (IsMouseButtonReleased(0)) {
      Holding = NONE;
    } else if (IsMouseButtonPressed(0)) {

      // Check wich Seq is beeing clicked
      if (Managers.size() > 0) {
        // Find corresponding Seqfield
        for (int i = Managers.size() - 1; i >= 0; i--) {
          if (Managers.at(i).fields.size() > 1) {
            continue;
          }
          int IndexClick = Managers.at(i).IndexIfClick(MousePos);
          if (IndexClick != -1) {
            WIP_Field.ManagerIndex = i;
            WIP_Field.FieldIndex = IndexClick;
            Mode = ENTER;
            break;
          } else {
            Mode = DEFAULT;
          }
        }
      }
    }

    if (Mode == ENTER) {
      // Key presses
      key = GetCharPressed();
      if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
        Mode = DEFAULT;
        WIP_Field.ManagerIndex = -1;
        WIP_Field.FieldIndex = -1;
      } else if (IsKeyPressed(KEY_BACKSPACE)) {
        Managers[WIP_Field.ManagerIndex].fields[WIP_Field.FieldIndex].removeChar();
      } else if (Mode == ENTER && key > 31 && key < 127) {
        Managers[WIP_Field.ManagerIndex].fields[WIP_Field.FieldIndex].UpdateText(key);
      } else if (IsKeyPressed(KEY_DELETE)) {
        Managers.at(WIP_Field.ManagerIndex).fields.erase(Managers.at(WIP_Field.ManagerIndex).fields.begin() + WIP_Field.FieldIndex);
        Managers.erase(Managers.begin() + WIP_Field.ManagerIndex);
        Mode = DEFAULT;
        WIP_Field.ManagerIndex = -1;
        WIP_Field.FieldIndex = -1;
      } else if (IsMouseButtonDown(0)) { // If Mouse doesnt collide with Field, then updatePos()
        if (!CheckCollisionRecs((Rectangle){MousePos.x, MousePos.y, 1, 1}, (Rectangle){Managers.at(WIP_Field.ManagerIndex).fields.at(WIP_Field.FieldIndex).rec.x, Managers.at(WIP_Field.ManagerIndex).fields.at(WIP_Field.FieldIndex).rec.y, Managers.at(WIP_Field.ManagerIndex).fields.at(WIP_Field.FieldIndex).rec.width, Managers.at(WIP_Field.ManagerIndex).fields.at(WIP_Field.FieldIndex).rec.height})) {
          Managers.at(WIP_Field.ManagerIndex).fields.at(WIP_Field.FieldIndex).updatePos(MousePos);
        }
      }
    }

    // Add Field
    bool already = false;
    if (Managers.size() > 1 && Holding == NONE && Mode == DEFAULT) {
      for (int i = 0; i < Managers.size(); i++) {
        for (int n = 0; n < Managers.at(i).fields.size(); n++) {
          for (int i2 = 0; i2 < Managers.size(); i2++) {
            for (int n2 = 0; n2 < Managers.at(i2).fields.size(); n2++) {
              if (CheckCollisionRecs((Rectangle){Managers.at(i).fields.at(n).rec.x, Managers.at(i).fields.at(n).rec.y, Managers.at(i).fields.at(n).rec.width, Managers.at(i).fields.at(n).rec.height}, (Rectangle){Managers.at(i2).fields.at(n2).rec.x - 10, Managers.at(i2).fields.at(n2).rec.y - 10, Managers.at(i2).fields.at(n2).rec.width + 20, Managers.at(i2).fields.at(n2).rec.height + 20}) && i != i2) {
                Managers.at(i).fields.at(n).rec.x = Managers.at(i2).fields.front().rec.x;
                Managers.at(i).fields.at(n).rec.y = Managers.at(i2).fields.front().rec.y + Managers.at(i2).fields.size() * Managers.at(i2).fields.front().rec.height + (Managers.at(i2).fields.size() - 0) * 2;
                Managers.at(i2).fields.push_back(Managers.at(i).fields.at(n));
                Managers.at(i).fields.erase(Managers.at(i).fields.begin() + n);
                if (Managers.at(i).fields.size() == 0) {
                  Managers.erase(Managers.begin() + i);
                }
                already = true;
                break;
              }
            }
            if (already) {
              break;
            }
          }
          if (already) {
            break;
          }
        }
        if (already) {
          break;
        }
      }
    }
    printf("it\n");
    BeginDrawing();
    ClearBackground(LIGHTGRAY);

    // Menu field
    DrawRectangleRounded(Menurec, 0.05, 20, (Color){150, 150, 150, 255});
    DrawText("Blocks", (Menurec.x + Menurec.width + MENUSPACE) / 2 - (MeasureText("Blocks", 40) / 2.0), 20, 40, BLACK);

    // SeqButton
    if (SeqButtonHover && Mode == DEFAULT) {
      SeqButtonColor = {135, 135, 135, 255};
      DrawRectangleRoundedLinesEx((Rectangle){(Menurec.x + Menurec.width + MENUSPACE) / 4, MENUSPACE * 3 + 40, Menurec.width / 2, 35}, 0.05, 20, 2, BLACK);
    } else {
      SeqButtonColor = (Color){180, 180, 180, 255};
    }
    DrawRectangleRounded((Rectangle){(Menurec.x + Menurec.width + MENUSPACE) / 4, MENUSPACE * 3 + 40, Menurec.width / 2, 35}, 0.15, 20, SeqButtonColor);
    DrawText("Sequenz", (MENUSPACE * 2 + Menurec.width) / 2 - (MeasureText("Sequenz", 19) / 2.0), MENUSPACE * 4 + 40, 19, BLACK);

    for (int i = 0; i < Managers.size(); i++) {
      Managers.at(i).draw();
    }

    if (Mode == ENTER) {
      DrawRectangleLinesEx(Managers[WIP_Field.ManagerIndex].fields[WIP_Field.FieldIndex].rec, 2, BLACK);
    }
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
