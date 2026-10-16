
//
// Created by winterorchid on 10/12/26.
//

#ifndef MYGAME_HEADER_H
#define MYGAME_HEADER_H

#include <stdbool.h>

typedef struct {
    char *class;
    int classID;
    int hp;
    int mana;
}playerT;

extern playerT Player;

typedef struct {
    char* itemName;
    int itemID;
    int count;
}inventoryT;

extern inventoryT Inventory;

typedef struct {
    char *name;
    int dmg;
    int mana;
}spells;

static spells Spells;

typedef struct {
    char *name;
    int dmg;
    int hp;
}enemyT;

typedef enum {
    PLAYER_TURN,
    ENEMY_TURN,
    NO_ENEMY
}turn;

bool RunMainMenu(void);

#endif //MYGAME_HEADER_H
