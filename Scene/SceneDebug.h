#pragma once
#include "Scene.h"
#include "Game.h"
#include <memory>
#include <string>

class SceneDebug :
    public Scene
{
public:
    SceneDebug(SceneManager& manager);
    virtual ~SceneDebug();

    // シーン初期化
    void Init() {}
    // 更新処理
    void Update(const InputState& input);
    // 描画処理
    void Draw();
    // 終了
    void End() {}

private:
    void UpdateText();

private:
    // 画像ハンドル
    int m_hImg;
    // 選択中の項目
    int m_selectedPos;
    // 選択中の項目名
    std::string m_selectedItemName;
};

