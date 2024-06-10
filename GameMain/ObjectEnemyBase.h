#pragma once
#include "StageObjectBase.h"
#include "Game.h"
#include <vector>
#include <memory>

namespace
{
    // 拡大率
    constexpr float kScale = 0.3f;
    // ショット生成間隔
    constexpr int kShotDelay = 60 * 4;
    // ショットを生成する距離
    constexpr float kShotDistanceMax = 9000.0f;
    constexpr float kShotDistanceMin = 1000.0f;
    // ショットの速度
    constexpr float kShotSpeed = 48.0f;
}

class ObjectEnemyBase :
    public StageObjectBase
{
public:
    // コンストラクタ
    ObjectEnemyBase();
    // デストラクタ
    virtual ~ObjectEnemyBase();
    // 更新
    virtual void Update();

    // ターゲット座標設定
    void SetTargetPos(VECTOR targetPos) { m_targetPos = targetPos; }

    // ショット取得
    std::vector<std::shared_ptr<class Shot>> GetShots() const { return m_pShots; }

protected:
    // ショット生成
    void CreateShot();
    // 不要なショット削除
    void DeleteDisableShot();
    // 全ショット削除
    void DeleteAllShot();

protected:
    // ターゲット座標
    VECTOR m_targetPos;
    // ショット生成間隔
    int m_shotDelay;
    // ショット
    std::vector<std::shared_ptr<class Shot>> m_pShots;
};

