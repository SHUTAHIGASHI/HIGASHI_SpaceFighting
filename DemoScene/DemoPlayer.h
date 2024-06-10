#pragma once
#include "ObjectBase.h"
#include <memory>

class DemoPlayer : public ObjectBase
{
public:
	// コンストラクタ
	DemoPlayer();
	// デストラクタ
	~DemoPlayer();

	// 更新
	void Update();
	// 描画
	void Draw();

private:
	// 回転処理
	void UpdateRot();
	// 移動処理
	void UpdateMove();

private:
	// 回転速度
	float m_rotSpeed;
	// 移動速度
	float m_heightSpeed;
	// 上下移動判定
	bool m_isUp;
	// 左右移動判定
	bool m_isRight;
	// 左右移動のフレームカウント
	int m_moveFrameCount;
	// プレイヤーモデルハンドル
	int m_hModel;
	// プレイヤーのモデル
	std::shared_ptr<class Model> m_pModel;
};

