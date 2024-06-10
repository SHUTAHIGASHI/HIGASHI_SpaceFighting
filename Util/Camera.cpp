#include "Camera.h"

Camera::Camera():
	m_cameraPos(VGet(0.0f, 0.0f, 0.0f)),
	m_targetPos(VGet(0.0f, 0.0f, 0.0f)),
	m_cameraUpVec(VGet(0.0f, 0.0f, 0.0f)),
	m_fov(0.0f),
	m_changeFrame(0),
	m_quakeScale(0)
{
}

Camera::~Camera()
{
}

void Camera::Init(float near_, float far_, float fov)
{
	// Zバッファを有効にする
	SetUseZBuffer3D(true);
	// Zバッファへの書き込みを行う
	SetWriteZBuffer3D(true);
	// 裏面ポリゴンを表示しない
	SetUseBackCulling(true);
	// ニア/ファーの設定
	SetCameraNearFar(near_, far_);
	// 視野角初期化
	SetCameraFov(fov);
}

void Camera::Update()
{
	// カメラ移動のフレーム数
	if (m_changeFrame > 0) m_changeFrame--;

	// カメラ揺れの更新処理
	CameraQuakeUpdate();
}

void Camera::SetPosAndTarget(VECTOR cameraPos, VECTOR targetPos)
{
	// 座標設定
	m_cameraPos = cameraPos;
	m_targetPos = targetPos;
	// カメラに位置を反映
	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_targetPos);
}

void Camera::SetPosAndTargetAndUpVec(VECTOR cameraPos, VECTOR targetPos, VECTOR cameraUpVec)
{
	// 座標設定
	m_cameraPos = cameraPos;
	m_targetPos = targetPos;
	m_cameraUpVec = cameraUpVec;
	// カメラに位置を反映
	SetCameraPositionAndTargetAndUpVec(m_cameraPos, m_targetPos, cameraUpVec);
}

void Camera::SetNextCameraPosAndFrame(VECTOR cameraPos, int frame)
{
	m_changeFrame = frame;
}

void Camera::SetNextTargetPosAndFrame(VECTOR targetPos, int frame)
{
	m_changeFrame = frame;
}

void Camera::SetNextCameraPosAndTargetPosAndFrame(VECTOR cameraPos, VECTOR targetPos, int frame)
{
	m_changeFrame = frame;
}

void Camera::SetCameraFov(float fov)
{
	// 視野角の設定
	m_fov = fov;
	SetupCamera_Perspective(m_fov * DX_PI_F / 180.0f);
}

void Camera::CameraQuakeUpdate()
{
	// 画面揺れの数値を代入
	m_cameraPos.x += m_quakeScale;
	m_cameraPos.y += m_quakeScale;
	m_targetPos.x += m_quakeScale;
	m_targetPos.y += m_quakeScale;

	// 画面揺れの数値が0以外の場合
	if (m_quakeScale != 0)
	{
		// 揺れの大きさを減らす
		if (m_quakeScale > 0)
		{
			m_quakeScale--;
		}
		m_quakeScale *= -1;
	}
}
