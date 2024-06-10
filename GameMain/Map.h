#pragma once
#include <vector>
#include <memory>
#include "game.h"

class Map
{
public:
	// コンストラクタ
	Map();
	// デストラクタ
	~Map();

	// 初期化
	void Init();
	// 更新
	void Update();
	// 描画
	void Draw();

	// ステージ終了時の処理
	void OnStageEnd();

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
	// 背景オブジェクトを削除する範囲
	float m_deleteRange;

private:
	// スカイドーム
	std::shared_ptr<class SkyDome> m_pSkyDome;
};

