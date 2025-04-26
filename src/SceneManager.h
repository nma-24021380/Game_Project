#ifndef SCENEMANAGER_H_INCLUDED
#define SCENEMANAGER_H_INCLUDED

enum class SceneState {ADVENTURE, BATTLE, GAME_OVER, WIN};

SceneState getScene();
void changeScene(SceneState scene);

void renderGameWin();
void renderGameOver();

void cleanScene();

#endif // SCENEMANAGER_H_INCLUDED
