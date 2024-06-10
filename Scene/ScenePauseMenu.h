#pragma once
#include "Scene.h"
#include <memory>
#include <string>

class SelectMenuBase;

class ScenePauseMenu :
    public Scene
{
public:
    ScenePauseMenu(SceneManager& manager);
    virtual ~ScenePauseMenu();

    void Init() {}
    void Update(const InputState& input);
    void Draw();
    void End() {}

private: 

private:
    std::shared_ptr<SelectMenuBase> m_pSelectMenu;
};

