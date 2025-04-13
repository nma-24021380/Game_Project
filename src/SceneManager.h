#ifndef SCENEMANAGER_H_INCLUDED
#define SCENEMANAGER_H_INCLUDED

#include "Entity.h"
#include "TileSet.h"

enum class SceneState {ADVENTURE, BATTLE};

SceneState getScene();
void changeScene(SceneState scene);
void changeMap(TileSet* tileMap, int &playerX, int &playerY, int direction);

void renderAdventure(TileSet* tileMap);

#endif // SCENEMANAGER_H_INCLUDED
