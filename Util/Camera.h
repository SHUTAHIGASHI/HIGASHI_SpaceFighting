#pragma once
#include "Game.h"

namespace CameraSetting
{
	// Near/Far
	constexpr float kCameraNear = 100.0f;
	constexpr float kCameraFar = 12800.0f;
	// 視野角定数
	constexpr float kFovNum = 80.0f;
}

class Camera
{
public:
	// コンストラクタ
	Camera();
	// デストラクタ
	virtual ~Camera();

	// カメラの初期化
	void Init(float near_, float far_, float fov);
	// 更新処理
	void Update();

	// カメラ位置とターゲット位置の設定
	void SetPosAndTarget(VECTOR cameraPos, VECTOR targetPos);
	// カメラ位置とターゲット位置の設定
	void SetPosAndTargetAndUpVec(VECTOR cameraPos, VECTOR targetPos, VECTOR cameraUpVec);

	// 指定したフレームでカメラ位置を変更
	void SetNextCameraPosAndFrame(VECTOR cameraPos, int frame);
	// 指定したフレームでターゲット位置を変更
	void SetNextTargetPosAndFrame(VECTOR targetPos, int frame);
	// 指定したフレームでカメラ位置とターゲット位置を変更
	void SetNextCameraPosAndTargetPosAndFrame(VECTOR cameraPos, VECTOR targetPos, int frame);

	// カメラの視野角変更
	void SetCameraFov(float fov);
	// カメラの揺れ
	void SetCameraQuake(int quakeScale) { m_quakeScale = quakeScale; }

private:
	// カメラ揺れの更新処理
	void CameraQuakeUpdate();

private:
	// カメラ座標
	VECTOR m_cameraPos;
	// ターゲット座標
	VECTOR m_targetPos;
	// カメラ上方向のベクトル
	VECTOR m_cameraUpVec;
	// カメラの視野角
	float m_fov;

	// 座標変換のフレーム数
	int m_changeFrame;
	// カメラ揺れの大きさ
	int m_quakeScale;
};

