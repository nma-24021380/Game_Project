#ifndef SCENEMANAGER_H_INCLUDED
#define SCENEMANAGER_H_INCLUDED

#include "Entity.h"

enum class SceneState {ADVENTURE, BATTLE};

SceneState getScene();
void setScene(SceneState scene);

void renderAdventure();
void renderBattle(Entity* player, Entity* enemy);

void renderHP(Entity* entity);
void renderText(const std::string text, int x, int y);

#endif // SCENEMANAGER_H_INCLUDED
