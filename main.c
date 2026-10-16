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

    InitWindow(0, 0, "raylib + raygui test");
    SetTargetFPS(24);

    int winY = GetScreenHeight();
    int winX = GetScreenWidth();
    int btnSX = winX / 5;
    int btnSY = winY / 13;
    int btnGap = 30;
    enemyT newEn = {"Dark Wizard", 10, 50, 500};
    int attacked = false;
    int turning = false;
    int multi = 1;
    Color col;
    int level;
    int manaMax;
    Texture2D bg = LoadTexture("Assets/BG.png");

    if (!RunMainMenu()) {
        CloseWindow();
        return 0;
    }

    //initialization

    if (Player.classID == 1) {
        col = (Color){153, 186, 204, 255};
    }else if (Player.classID == 2) {
        col = (Color){204, 153, 153, 255};
    }

    inventoryT Inventory[] = {
        {"Health Pot", 1, 0},
        {"Mana Pot", 2, 0},
        {"Strength Pot", 3, 0}
    };

    static const int itemCount = sizeof(Inventory) / sizeof(Inventory[0]);

    spells Spells[] = {
        {"...", 15, -5},
        {"...", 50, 15},
    };

    int spellCount = sizeof(Spells) / sizeof(Spells[0]);

    if (Player.classID == 1) {
        Spells[0].name = "Ice Spike";
        Spells[1].name = "Snow Storm";
        icon = LoadTexture("Assets/IW.png");
        manaMax = 100;
    } else if (Player.classID == 2) {
        Spells[0].name = "Flame Shot";
        Spells[1].name = "Fire Typhoon";
        Spells[0].dmg = 20;
        Spells[1].mana = 20;
        icon = LoadTexture("Assets/FW.png");
        manaMax = 50;
    }

    enIcon = LoadTexture("Assets/DW.png");

    turn GameState = PLAYER_TURN;

    while (!WindowShouldClose()) {
        switch (GameState) {
            case PLAYER_TURN:
                //drawing
                BeginDrawing();
                ClearBackground(RAYWHITE);
                //BG
                Rectangle sourceRec = { 0.0f, 0.0f, (float)bg.width, (float)bg.height };
                Rectangle destRec = { 0.0f, 0.0f, winX, winY };
                Vector2 origin = { 0.0f, 0.0f };
                DrawTexturePro(bg, sourceRec, destRec, origin, 0.0f, WHITE);


                DrawTexture(bg, -500, -500, LIGHTGRAY);

                //Game state
                if (newEn.hp <= 0) {
                    GameState = NO_ENEMY;
                    frameCountd = 50;
                    EndDrawing();
                    break;
                }


                //Draw chars & UI
                DrawText("Choose Action", 20, 20, 60, col);
                DrawTexture(icon, 40, winY / 2 - 200, RAYWHITE);
                if (strcmp(newEn.name, "Dark Wizard") == 0 && newEn.hp > 0) {
                    DrawTexture(enIcon, winX - 600, winY / 2 + 200 , RAYWHITE);
                    DrawText(TextFormat("Hp: %d", newEn.hp), winX - 600, winY / 2 + 680, 60, RAYWHITE);
                }
                //gui style

                GuiSetStyle(DEFAULT, TEXT_SIZE, 50);
                GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt(col));
                GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(BLACK));

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
                            if (Spells[i].mana >= 0) {
                                DrawText(TextFormat("%d dmg/-%d mana",Spells[i].dmg, abs(Spells[i].mana)),
                                    2*btnSX + 3*btnGap,
                                    i * btnSY + btnGap + 110+ btnSY/2.3,
                                    40,
                                    DARKGRAY);
                            }else {
                                DrawText(TextFormat("%d dmg/+%d mana",Spells[i].dmg, abs(Spells[i].mana)),
                                    2*btnSX + 3*btnGap,
                                    i * btnSY + btnGap + 110+ btnSY/2.3,
                                    40,
                                    DARKGRAY);
                            }
                            if (GuiButton((Rectangle){
                                btnSX + 3 * btnGap,
                                i * btnSY + btnGap + 110,
                                btnSX - btnGap,
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
                                    if (Player.mana > manaMax) {
                                        Player.mana = manaMax;
                                    }
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
                            DrawText(TextFormat("%d held",Inventory[i].count),
                                2*btnSX + 3*btnGap,
                                i * btnSY + btnGap + 110+ btnSY/2.3,
                                40,
                                DARKGRAY);
                            if (GuiButton((Rectangle){
                                btnSX + 3 * btnGap,
                                i * btnSY + btnGap + 110,
                                btnSX - btnGap,
                                btnSY - btnGap
                                          },
                                          Inventory[i].itemName)) {
                                isItemsOpen = false;
                                if (Inventory[i].count == 0) {
                                    snprintf(infoFormat, sizeof(infoFormat), "None held!");
                                    frameCountd = 48;
                                }
                                if (Inventory[i].count > 0) {
                                    if (Inventory[i].itemID == 1) {
                                        Player.hp += 30;
                                        if (Player.hp > 100+level*10) {
                                            Player.hp = 100+level*10;
                                        }
                                    } else if (Inventory[i].itemID == 2) {
                                        Player.mana += 20;
                                        if (Player.mana > manaMax) {
                                            Player.mana = manaMax;
                                        }
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
                DrawText(TextFormat("Lvl: %d    Exp: %d/%d", level, Player.exp, level*500+500), 20, winY - 110, 50, RAYWHITE);
                DrawText(TextFormat("Hp: %d/%d    Mp: %d/%d", Player.hp, 100+level*10, Player.mana, manaMax), 20, winY - 60, 50, RAYWHITE);

                EndDrawing();
                break;
            case ENEMY_TURN:
                if (frameCountd == 0) {
                    GameState = PLAYER_TURN;
                    attacked = false;
                    Player.mana += 3;
                    if (Player.mana > manaMax) {
                        Player.mana = manaMax;
                    }
                    break;
                }
                if (!attacked) {
                    attacked = true;
                    Player.hp -= newEn.dmg;
                }
                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawTexture(bg, 0, 0, LIGHTGRAY);
                printf("Enemy turn\n");
                DrawText("You are attacked", winX/2 - btnSX, winY / 2, 100, DARKGRAY);
                frameCountd --;
                EndDrawing();
                break;
            case NO_ENEMY:
                if (frameCountd == 0) {
                    newEn = (enemyT){"Dark Wizard", 10, 50, 400};
                    GameState = PLAYER_TURN;
                    turning = false;
                    Player.mana += 5;
                    if (Player.mana > manaMax) {
                        Player.mana = manaMax;
                    }
                    break;
                }
                int temp;
                if (!turning) {
                    int gen = rand() % 5;
                    for (int i = 0; i < itemCount; i++) {
                        if (Inventory[i].itemID == gen) {
                            Inventory[i].count++;
                            snprintf(infoFormat, sizeof(infoFormat), "You pick up a %s from him", Inventory[i].itemName);
                        }
                    }
                    Player.exp += newEn.reward;
                    temp = level;
                    if (Player.exp >= level*500+500) {
                        level++;
                        Player.exp = 0;
                    }
                    turning = true;
                }
                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawTexture(bg, 0, 0, LIGHTGRAY);

                DrawText("Enemy defeated!", winX/2 - btnSX/1.5, 60, winX/26, DARKGRAY);
                DrawText(infoFormat, winX/2 - btnSX*1.5, winY / 2-120, winX/30, DARKGRAY);
                if (level > temp) {
                    DrawText("You leveled up!", winX/2 - btnSX/1.9, winY / 2, winX/30, DARKGRAY);
                }
                frameCountd --;
                EndDrawing();
                break;
        }
    }
    UnloadTexture(enIcon);
    UnloadTexture(icon);
    CloseWindow();
    return 0;
}
