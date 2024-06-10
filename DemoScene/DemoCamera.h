#pragma once
#include "Game.h"
#include <memory>

// カメラ
class DemoCamera
{
public:
	// コンストラクタ
	DemoCamera();
	// デストラクタ
	virtual ~DemoCamera();

	// プレイヤーのポインタセット
	void SetPlayer(class DemoPlayer* player) { m_pDemoPlayer = player; }

	// カメラ初期化
	void Init();
	// カメラ更新
	void Update();

private:
	// カメラの位置更新
	void UpdateCameraPos();

private:
	// カメラターゲット座標
	VECTOR m_targetPos;

	// プレイヤーポインタ
	class DemoPlayer* m_pDemoPlayer;
	// カメラ
	std::shared_ptr<class Camera> m_pCamera;
};
