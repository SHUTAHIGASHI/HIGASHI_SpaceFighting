#include "DemoCamera.h"
#include "Camera.h"
#include "DemoPlayer.h"

namespace
{
	// カメラ位置の定数
	constexpr float kCameraPosX = 600.0f;
	constexpr float kCameraPosY = 200.0f;
	constexpr float kCameraPosZ = 800.0f;
	// 注視点までの移動速度
	constexpr float kTargetTrackingSpeed = 10.0f;
}

// コンストラクタ
DemoCamera::DemoCamera() :
	m_targetPos(Game::kVecZero),
	m_pDemoPlayer(nullptr),
	m_pCamera(std::make_shared<Camera>())
{
}

// デストラクタ
DemoCamera::~DemoCamera()
{
	// 処理なし
}

// 初期化
void DemoCamera::Init()
{
	// カメラ設定初期化
	m_pCamera->Init(CameraSetting::kCameraNear, CameraSetting::kCameraFar * 2, CameraSetting::kFovNum);
	// カメラ位置設定
	UpdateCameraPos();
}

// 更新
void DemoCamera::Update()
{
	// カメラ位置更新
	UpdateCameraPos();
	// カメラ更新
	m_pCamera->Update();
}

void DemoCamera::UpdateCameraPos()
{
	// プレイヤーの位置を追従
	VECTOR pos = m_pDemoPlayer->GetPos();
	pos.x = kCameraPosX; pos.y = kCameraPosY; pos.z += kCameraPosZ;
	// カメラの注視点設定
	VECTOR targetDir = VSub(m_pDemoPlayer->GetPos(), m_targetPos);
	if (VSize(targetDir) > 0) targetDir = VNorm(targetDir);
	targetDir = VScale(targetDir, kTargetTrackingSpeed);
	m_targetPos = VAdd(m_targetPos, targetDir);
	// カメラ座標設定
	m_pCamera->SetPosAndTarget(pos, m_targetPos);
}
