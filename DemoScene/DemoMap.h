#pragma once
#include <vector>
#include <memory>
#include "game.h"

class DemoMap
{
public:
	// コンストラクタ
	DemoMap();
	// デストラクタ
	~DemoMap();

	// 初期化
	void Init();
	// 更新
	void Update();
	// 描画
	void Draw();

	// 基準座標の設定
	void SetBasePos(const VECTOR pos) { m_basePos = pos; }

private:
	// ステージ背景オブジェクトの生成
	void CreateStageBackModel();
	// ステージ背景オブジェクトの座標生成
	VECTOR GetRandStageBackPos();

private:
	// ステージ背景オブジェクトハンドル
	int m_hStageBackModelHandleBase;
	std::vector<class Model*> m_stageBackModel;

	// 背景オブジェクトを配置する基準座標
	VECTOR m_basePos;

private:
	// スカイドーム
	std::shared_ptr<class SkyDome> m_pSkyDome;
};

