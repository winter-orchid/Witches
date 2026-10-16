#include "raylib.h"
#include "raygui.h"

#include "header.h"

playerT Player = {"", 0, 0};

static const playerT classes[] = {
    { "Frost Witch", 1,100, 50 },
    { "Flame Witch", 2,150, 10 },
};
#define CLASS_COUNT ((int)(sizeof(classes) / sizeof(classes[0])))

typedef enum {
    SCREEN_MAIN,
    SCREEN_CLASS_SELECT
} menuScreen;


bool RunMainMenu(void)
{

    menuScreen screen = SCREEN_MAIN;
    bool started = false;
    bool quit = false;
    float btnSizeX = 420, btnSizeY = 130;
    float mid = GetScreenWidth()/2 -btnSizeX/2;
    float midY = GetScreenHeight()/2 -btnSizeY/2;

    GuiSetStyle(DEFAULT, TEXT_SIZE, 50);

    while (!started && !quit && !WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch (screen) {
            case SCREEN_MAIN:
                DrawText("Witches", mid-100, 20, 160, DARKGRAY);
                if (GuiButton((Rectangle){ mid, midY, btnSizeX, btnSizeY }, "Play")) {
                    screen = SCREEN_CLASS_SELECT;
                }
                if (GuiButton((Rectangle){ mid, midY+btnSizeY+20, btnSizeX, btnSizeY }, "Quit")) {
                    quit = true;
                }
                break;
            case SCREEN_CLASS_SELECT:
                DrawText("Witches", mid-100, 20, 160, DARKGRAY);
                DrawText("Choose Your class", mid-100, 200, 65, DARKGRAY);
                for (int i = 0; i < CLASS_COUNT; i++) {
                    if (GuiButton((Rectangle){ mid, 1.3*i*110+410, btnSizeX, btnSizeY }, classes[i].class)) {
                        Player.class = classes[i].class;
                        Player.classID = classes[i].classID;
                        Player.hp = classes[i].hp;
                        Player.mana = classes[i].mana;
                        Player.exp = 0;
                        started = true;
                    }
                }
                if (GuiButton((Rectangle){ 20, GetScreenHeight()-200, btnSizeX, btnSizeY }, "Back")) {
                    screen = SCREEN_MAIN;
                }
                break;
        }

        EndDrawing();
    }

    return started;
}