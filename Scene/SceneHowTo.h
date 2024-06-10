#pragma once
#include "Scene.h"
#include "Game.h"

class SceneHowTo :
    public Scene
{
public:
    // コンストラクタ
    SceneHowTo(SceneManager& manager);
    // デストラクタ
    virtual ~SceneHowTo();
    // 初期化
    void Init() override {}
    // 更新
    void Update(const InputState& input) override;
    // 描画
    void Draw();
    // 終了
    void End() {}

private:
    // 毎フレームカウント
    int m_countFrame;
    // 画像ハンドル
    int m_hImg;
    // 描画座標
    VECTOR m_pos;
};

