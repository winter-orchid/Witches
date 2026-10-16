#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "header.h"

int main(void) {
    bool isOpen = false;
    bool isItemsOpen = false;
    char infoFormat[100];
    int frameCountd = 0;
    Texture2D icon;
    Texture2D enIcon;

    InitWindow(1920, 1080, "raylib + raygui test");
    SetTargetFPS(24);

    int winY = GetScreenHeight();
    int winX = GetScreenWidth();
    int btnSX = winX / 5;
    int btnSY = winY / 13;
    int btnGap = 30;
    enemyT newEn = {"Dark Wizard", 10, 50};
    int attacked = false;
    int turning = false;
    int multi = 1;


    if (!RunMainMenu()) {
        CloseWindow();
        return 0;
    }


    //initialization
    inventoryT Inventory[] = {
        {"Health Pot", 1, 0},
        {"Mana Pot", 2, 0},
        {"Strength Pot", 3, 0}
    };

    static const int itemCount = sizeof(Inventory) / sizeof(Inventory[0]);

    spells Spells[] = {
        {"...", 20, -5},
        {"...", 50, 15},
    };

    int spellCount = sizeof(Spells) / sizeof(Spells[0]);

    if (Player.classID == 1) {
        Spells[0].name = "Ice Spike";
        Spells[1].name = "Snow Storm";
        icon = LoadTexture("Assets/IW.png");
    } else if (Player.classID == 2) {
        Spells[0].name = "Flame Shot";
        Spells[1].name = "Fire Typhoon";
        icon = LoadTexture("Assets/FW.png");
    }

    enIcon = LoadTexture("Assets/DW.png");

    turn GameState = PLAYER_TURN;

    while (!WindowShouldClose()) {
        switch (GameState) {
            case PLAYER_TURN:
                //drawing
                BeginDrawing();
                ClearBackground(RAYWHITE);
                //Game state
                if (newEn.hp <= 0) {
                    GameState = NO_ENEMY;
                    frameCountd = 50;
                    EndDrawing();
                    break;
                }


                //Draw chars & UI
                DrawText("Choose Action", 20, 20, 60, DARKGRAY);
                DrawTexture(icon, 40, winY / 2 - 200, RAYWHITE);
                if (strcmp(newEn.name, "Dark Wizard") == 0 && newEn.hp > 0) {
                    DrawTexture(enIcon, winX - 600, winY / 2 - 200, RAYWHITE);
                    DrawText(TextFormat("Hp: %d", newEn.hp), winX - 600, winY / 2 + 280, 60, DARKGRAY);
                }

                GuiSetStyle(DEFAULT, TEXT_SIZE, 50);
                if (frameCountd <= 0) {
                    //buttons/actions
                    if (GuiButton((Rectangle){20, 110, btnSX, btnSY}, "Attacks")) {
                        TraceLog(LOG_INFO, "Button clicked");
                        isOpen = true;
                        isItemsOpen = false;
                    }
                    if (GuiButton((Rectangle){20, 240, btnSX, btnSY}, "Items")) {
                        TraceLog(LOG_INFO, "Button clicked");
                        isOpen = false;
                        isItemsOpen = true;
                    }


                    if (isOpen == true) {
                        DrawRectangle(20 + btnSX, 100 + btnSY / 2, 2 * btnGap, 20, LIGHTGRAY);
                        DrawRectangle(btnSX + 2 * btnGap, 120, btnGap, (spellCount) * btnSY, LIGHTGRAY);
                        for (int i = 0; i < spellCount; i++) {
                            if (GuiButton((Rectangle){
                                              btnSX + 3 * btnGap, i * btnSY + btnGap + 110, btnSX - btnGap,
                                              btnSY - btnGap
                                          }, Spells[i].name)) {
                                isOpen = false;
                                if (Player.mana < Spells[i].mana) {
                                    snprintf(infoFormat, sizeof(infoFormat), "Not Enough mana");
                                    frameCountd = 48;
                                } else {
                                    snprintf(infoFormat, sizeof(infoFormat), "You used %s", Spells[i].name);
                                    frameCountd = 40;
                                    Player.mana -= Spells[i].mana;
                                    newEn.hp -= Spells[i].dmg*multi;
                                    multi = 1;
                                }
                            }
                        }
                    }
                    if (isItemsOpen == true) {
                        DrawRectangle(20 + btnSX, 230 + btnSY / 2, 2 * btnGap, 20, LIGHTGRAY);
                        DrawRectangle(btnSX + 2 * btnGap, 120, btnGap, (itemCount) * btnSY, LIGHTGRAY);
                        for (int i = 0; i < itemCount; i++) {
                            if (GuiButton((Rectangle){
                                              btnSX + 3 * btnGap, i * btnSY + btnGap + 110, btnSX - btnGap,
                                              btnSY - btnGap
                                          }, Inventory[i].itemName)) {
                                isItemsOpen = false;
                                if (Inventory[i].count == 0) {
                                    snprintf(infoFormat, sizeof(infoFormat), "None held!");
                                    frameCountd = 48;
                                }
                                if (Inventory[i].count > 0) {
                                    if (Inventory[i].itemID == 1) {
                                        Player.hp += 30;
                                    } else if (Inventory[i].itemID == 2) {
                                        Player.mana += 20;
                                    } else if (Inventory[i].itemID == 3) {
                                        multi = 2;
                                    }
                                    Inventory[i].count--;
                                    frameCountd = 40;
                                    snprintf(infoFormat, sizeof(infoFormat), "You used a potion.");
                                }
                            }
                        }
                    }

                    //frame animations
                } else if (frameCountd > 0) {
                    DrawText(infoFormat, 20, winY - 200, 60, DARKGRAY);
                    if (frameCountd == 1) {
                        GameState = ENEMY_TURN;
                        frameCountd = 30;
                        EndDrawing();
                        break;
                    }

                    frameCountd--;
                }
                //statuses

                DrawText(TextFormat("Hp: %d Mp: %d", Player.hp, Player.mana), 20, winY - 100, 60, BLACK);

                EndDrawing();
                break;
            case ENEMY_TURN:
                if (frameCountd == 0) {
                    GameState = PLAYER_TURN;
                    attacked = false;
                    break;
                }
                if (!attacked) {
                    attacked = true;
                    Player.hp -= newEn.dmg;
                }
                BeginDrawing();
                ClearBackground(RAYWHITE);
                printf("Enemy turn\n");
                DrawText("You are attacked", winX/2 - btnSX, winY / 2, 100, DARKGRAY);
                frameCountd --;
                EndDrawing();
                break;
            case NO_ENEMY:
                if (frameCountd == 0) {
                    newEn = (enemyT){"Dark Wizard", 10, 50};
                    GameState = PLAYER_TURN;
                    turning = false;
                    break;
                }
                if (!turning) {
                    int gen = rand() % 5;
                    for (int i = 0; i < itemCount; i++) {
                        if (Inventory[i].itemID == gen) {
                            Inventory[i].count++;
                            snprintf(infoFormat, sizeof(infoFormat), "You pick up a %s from him", Inventory[i].itemName);
                        }
                    }
                    turning = true;
                }
                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawText(infoFormat, winX/2 - btnSX*1.5, winY / 2, winX/30, DARKGRAY);
                frameCountd --;
                EndDrawing();
                break;
        }
    }

    UnloadTexture(icon);
    CloseWindow();
    return 0;
}
