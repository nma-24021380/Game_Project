#ifndef SCENEMANAGER_H_INCLUDED
#define SCENEMANAGER_H_INCLUDED

enum class SceneType
{
    ADVENTURE,
    BATTLE
};

SceneType getScene();
void setScene(SceneType scene);

void renderAdventure();
void renderBattle();

#endif // SCENEMANAGER_H_INCLUDED
