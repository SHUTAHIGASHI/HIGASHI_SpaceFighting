#include "CameraController.h"
#include "Camera.h"
#include "Player.h"

namespace
{
	// スタート時のカメラ演出
	// Y軸を中心としたカメラの移動速度
	constexpr float kMoveSpeedAxisY = 0.03f;
	// スタート演出のY軸
	constexpr float kStartPosY = 0.0f;
	// カメラ位置調整数値
	constexpr float kReposCameraY = 300.0f;
	constexpr float kReposCameraZ = 1000.0f;
	// カメラ位置までの速度
	constexpr float kCameraMoveSpeed = 92.0f;
	// 視野角変動速度
	constexpr float kFovExpandSpeed = 0.5f;
	constexpr float kFovReductionSpeed = -0.5f;
	// 画面揺れの大きさ
	constexpr int kQuakeScaleNum = 10;
	// 保存する注視点のフレーム数
	constexpr int kSaveLookPosNum = 5;
}

// コンストラクタ
CameraController::CameraController():
	m_dir(Game::kVecZero),
	m_pos(Game::kVecZero),
	m_lookPos(Game::kVecZero),
	m_fov(0.0f),
	m_rotaAxisY(0.0f),
	m_startPosY(kStartPosY),
	m_screenQuakeScale(0),
	m_isCameraSet(false),
	m_pCamera(std::make_shared<Camera>()),
	m_pPlayer(nullptr)
{
}

// デストラクタ
CameraController::~CameraController()
{
	// 処理なし
}

// 初期化
void CameraController::Init()
{
	// カメラの視野角設定
	m_fov = CameraSetting::kFovNum;
	// カメラの回転設定
	m_rotaAxisY = DX_PI_F;
	// カメラ設定初期化
	m_pCamera->Init(CameraSetting::kCameraNear, CameraSetting::kCameraFar, m_fov);

	// カメラの位置設定
	// プレイヤーの位置を追従
	m_pos = VGet(m_pPlayer->GetPos().x * 0.3f,
		m_pPlayer->GetPos().y * 0.3f,
		m_pPlayer->GetPos().z - kReposCameraZ);
	// カメラの向きをY軸を中心に反転させる
	MATRIX mtxRotaY = MGetRotY(m_rotaAxisY);
	m_pos = VTransform(m_pos, mtxRotaY);
	// カメラの注視点設定
	m_lookPos = m_pPlayer->GetPos();
	// カメラ座標設定
	m_pCamera->SetPosAndTarget(m_pos, m_lookPos);
}

// 更新
void CameraController::Update()
{
	// 数フレーム前までのカメラ座標の保存と削除を行う
	UpdateCameraPos();
	
	// 画面揺れの処理
	if (m_screenQuakeScale != 0)
	{
		UpdateCameraQuake();
	}

	// カメラの向きをZ軸を中心に反転させる
	MATRIX mtx = MGetRotZ(DX_PI_F);
	VECTOR cameraUpVec = VTransform(m_pos, mtx);

	// カメラに位置を反映
	m_pCamera->SetPosAndTarget(m_pos, m_lookPos);
}

void CameraController::UpdateCameraPos()
{
	// カメラ位置の設定
	VECTOR cameraPos = VGet(m_pPlayer->GetPos().x, m_pPlayer->GetPos().y + 300.0f, m_pPlayer->GetPos().z);
	m_dir = VSub(cameraPos, m_pos);
	m_dir = VNorm(m_dir);
	m_dir = VScale(m_dir, kCameraMoveSpeed);
	m_pos = VAdd(m_pos, m_dir);
	// Z座標を調整
	m_pos.z = m_pPlayer->GetPos().z - kReposCameraZ;

	// 注視点の設定
	m_lookPos = m_pPlayer->GetPos();
	m_lookPos.y += kReposCameraY;
}

void CameraController::UpdateCameraQuake()
{
	// 画面揺れの数値を代入
	m_lookPos.x += m_screenQuakeScale;
	m_lookPos.y += m_screenQuakeScale;
	m_pos.x += m_screenQuakeScale;
	m_pos.y += m_screenQuakeScale;

	// 画面揺れの数値が0以外の場合
	if (m_screenQuakeScale != 0)
	{
		// 揺れの大きさを減らす
		if (m_screenQuakeScale > 0)
		{
			m_screenQuakeScale--;
		}
		m_screenQuakeScale *= -1;
	}
}

// シーンメイン開始時の演出用更新処理
void CameraController::StartUpdate()
{
	// 毎フレームカメラ回転
	m_rotaAxisY -= kMoveSpeedAxisY;
	if (m_rotaAxisY < 0.0f)
	{
		m_rotaAxisY = 0.0f;
		m_isCameraSet = true;
	}
	// プレイヤーの位置を追従
	m_pos = VGet(m_pPlayer->GetPos().x * 0.3f,
		m_pPlayer->GetPos().y * 0.3f,
		m_pPlayer->GetPos().z - kReposCameraZ);
	// カメラの向きをY軸を中心に反転させる
	MATRIX mtxRotaY = MGetRotY(m_rotaAxisY);
	m_pos = VTransform(m_pos, mtxRotaY);

	// プレイヤー上の位置を注視する
	m_startPosY += 5.0f;
	if (m_startPosY > kReposCameraY) m_startPosY = kReposCameraY;
	m_lookPos = m_pPlayer->GetPos();
	m_lookPos.y += m_startPosY;

	// カメラに位置を反映.
	m_pCamera->SetPosAndTarget(m_pos, m_lookPos);
}

void CameraController::EndUpdate()
{
	// 毎フレームカメラ回転
	m_rotaAxisY += kMoveSpeedAxisY;
	if (m_rotaAxisY > DX_PI_F)
	{
		m_rotaAxisY = DX_PI_F;
	}
	// プレイヤーの位置を追従
	/*m_pos = VGet(m_pPlayer->GetPos().x * kCameraCenterDistance,
		m_pPlayer->GetPos().y * kCameraCenterDistance,
		m_pPlayer->GetPos().z - kReposCameraZ);*/
	// カメラの向きをY軸を中心に反転させる
	MATRIX mtxRotaY = MGetRotY(DX_PI_F);
	m_pos = VTransform(m_pos, mtxRotaY);

	// 注視点設定
	m_lookPos = m_pPlayer->GetPos();

	// カメラに位置を反映.
	m_pCamera->SetPosAndTarget(m_pos, m_lookPos);
}

void CameraController::ResultUpdate()
{
	// プレイヤーの位置を追従
	m_pos = VGet(m_pPlayer->GetPos().x + 300.0f,
		m_pPlayer->GetPos().y + kReposCameraY,
		m_pPlayer->GetPos().z + 1000.0f);

	// 注視点設定
	m_lookPos = m_pPlayer->GetPos();

	// カメラに位置を反映.
	m_pCamera->SetPosAndTarget(m_pos, m_lookPos);
}

// 画面揺れ
void CameraController::OnScreenQuake()
{
	// 揺れの大きさを設定
	m_screenQuakeScale = kQuakeScaleNum;
}
