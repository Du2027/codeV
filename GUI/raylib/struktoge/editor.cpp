#include <raylib.h>
#include <stdio.h>
#include <vector>

#define DEBUG 1
#define FPS 120

enum statementType {
  NONE = 0,
  SEQ = 1,
  VER = 2,
  SCH = 3
};

class Field {
public:
  statementType type;
  Rectangle rec;

  Field(Rectangle rec, statementType type) {
    this->rec = rec;
    this->type = type;
  }

  void updatePos(Vector2 pos) {
    this->rec.x = pos.x;
    this->rec.y = pos.y;
  }

  void draw() {
    DrawRectangleRec(rec, (Color){135, 135, 135, 200});
  }
  Field() {}
};

class ManagerBlock {
public:
  Vector2 pos;
  std::vector<Field> fields;

  ManagerBlock(Vector2 pos) {
    this->pos = pos;
    this->fields = std::vector<Field>();
  }

  void appendField(Field field) {
    fields.push_back(field); // Need to implement!
  }

  void drawAllFields() {
    for (int i = 0; i < fields.size(); i++) {
      fields.at(i).draw();
    }
  }

  float getWidth() {
    float biggestWidth = 0;
    for (int i = 0; i < fields.size(); i++) {
      if (fields.at(i).rec.width > biggestWidth) {
        biggestWidth = fields.at(i).rec.width;
      }
    }
    return biggestWidth;
  }

  float getHeight() {
    float height = 0;
    for (int i = 0; i < fields.size(); i++) {
      height = height + fields.at(i).rec.height;
    }
    return height;
  }
};

bool doesMFCollide(Rectangle managerRec, Rectangle fieldRec) {
  return CheckCollisionRecs(managerRec, fieldRec);
}

int main() {
  InitWindow(0, 0, "tmp_win");
  Vector2 winSize = {GetMonitorWidth(0) / 2.0f, GetMonitorHeight(0) / 2.0f};
  CloseWindow();

  InitWindow(winSize.x, winSize.y, "Struktogramme");
  SetTargetFPS(FPS);

  Rectangle menuRec;
  Rectangle seqRec;
  Color seqButtonColor = (Color){180, 180, 180, 255};
  Color menuColor = (Color){150, 150, 150, 255};

  int menuPadding = 10;
  Vector2 mousePos;
  statementType holding = NONE;
  Field mouseField;
  std::vector<ManagerBlock> managers = {};

  while (!WindowShouldClose()) {
    winSize = (Vector2){(float)GetRenderWidth(), (float)GetRenderHeight()}; // Winsize is 30px different after ~33msek ???
    menuRec = (Rectangle){(float)menuPadding, (float)menuPadding, winSize.x / 4, winSize.y - menuPadding * 2};
    seqRec = (Rectangle){(menuRec.x + menuRec.width + menuPadding) / 4, (float)menuPadding * 3 + 40, menuRec.width / 2, 35};

    if (IsMouseButtonPressed(0)) {
      mousePos = GetMousePosition();

      // Click on seqRec
      if (CheckCollisionRecs(seqRec, (Rectangle){mousePos.x, mousePos.y, 1, 1})) {
        mouseField = Field((Rectangle){mousePos.x, mousePos.y, winSize.x / 8, winSize.y / 20}, SEQ);
        holding = SEQ;
      } else if (mousePos.x > menuRec.width + menuRec.x && holding != NONE) {
        bool doesCollide = false;
        short collidingManager = 0;
        // Does Field collides with a ManagerBlock
        for (int i = 0; i < managers.size(); i++) {
          if (doesMFCollide((Rectangle){managers.at(i).pos.x, managers.at(i).pos.y, managers.at(i).getWidth(), managers.at(i).getHeight()}, mouseField.rec)) {
            doesCollide = true;
            collidingManager = i;
          }
        }
        if (managers.size() < 1 || !doesCollide) {
          ManagerBlock tmpManager = ManagerBlock(mousePos);
          tmpManager.appendField(mouseField);
          managers.push_back(tmpManager);
        } else if (doesCollide) {
          mouseField.rec.x = managers.at(collidingManager).pos.x;
          mouseField.rec.y = managers.at(collidingManager).pos.y + managers.at(collidingManager).getHeight();
          managers.at(collidingManager).appendField(mouseField);
          holding = NONE;
        }
        holding = NONE;
      } else if (holding == NONE) {
        bool found = false;

        for (int i = 0; i < managers.size(); i++) {
          for (int n = 0; n < managers.at(i).fields.size(); n++) {
            if (CheckCollisionRecs((Rectangle){mousePos.x, mousePos.y, 1, 1}, managers.at(i).fields.at(n).rec)) {
              mouseField = managers.at(i).fields.at(n);
              holding = SEQ;
              managers.at(i).fields.erase(managers.at(i).fields.begin() + n); // need to move other elements to front
            }
            if (found) {
              break;
            }
          }
          if (found) {
            break;
          }
        }
      }
    }

    BeginDrawing();
    ClearBackground(LIGHTGRAY);

    // Menu field
    DrawRectangleRounded(menuRec, 0.05, 20, menuColor);
    DrawText("Blocks", (menuRec.x + menuRec.width + menuPadding) / 2 - (MeasureText("Blocks", 40) / 2.0), 20, 40, BLACK);

    if (holding != NONE) {
      mouseField.updatePos(GetMousePosition());
      mouseField.draw();
    }

    // SeqButton
    DrawRectangleRounded(seqRec, 0.15, 20, seqButtonColor);
    DrawText("Sequenz", (menuPadding * 2 + menuRec.width) / 2 - (MeasureText("Sequenz", 19) / 2.0), menuPadding * 4 + 40, 19, BLACK);

    for (int i = 0; i < managers.size(); i++) {
      managers.at(i).drawAllFields();
    }
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
