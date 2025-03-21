#ifndef SCENEMANAGER_H_INCLUDED
#define SCENEMANAGER_H_INCLUDED

enum class SceneState {ADVENTURE, BATTLE};

SceneState getScene();
void setScene(SceneState scene);

void renderAdventure();
void renderBattle();

#endif // SCENEMANAGER_H_INCLUDED
