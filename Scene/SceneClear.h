#pragma once
#include "Scene.h"
#include <memory>
#include <string>

class SceneClear :
    public Scene
{
public:
    // コンストラクタ
    SceneClear(SceneManager& manager);
    // デストラクタ
    virtual ~SceneClear();

    // 初期化
    void Init(){}
    // 更新
    void Update(const InputState& input);
    // 描画
    void Draw();
    // 終了
    void End() {}

private:
    // メンバ関数ポインタ(更新)
    using m_tUpdateFunc = void (SceneClear::*) (const InputState& input);
    m_tUpdateFunc m_updateFunc = nullptr;
    void StartUpdate(const InputState& input);      // 開始時の更新処理
    void NormalUpdate(const InputState& input);     // 通常の更新処理
    void EndUpdate(const InputState& input);        // 終了時の更新処理

private: // 変数
    // 背景画像
    int m_hBgImg;
    // ハイスコア
    int m_highScore;
    // スコア描画用座標
    float m_scorePosY;
    // ランク
    std::string m_rank;
    // 選択メニュー
    std::shared_ptr<class SelectMenuBase> m_pSelectMenu;
};

