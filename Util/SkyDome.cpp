#include "SkyDome.h"
#include "Model.h"
#include "Load.h"

namespace
{
	// スカイドームの半径
	constexpr float kSkyDomeRadius = 10.0f;
	// スカイドームの回転速度
	constexpr float kSkyDomeRotSpeed = 0.0003f;
	// 0 のベクトル
	constexpr VECTOR kZero = { 0.0f, 0.0f, 0.0f };
}

SkyDome::SkyDome() :
	m_pModel(nullptr),
	m_scale(kSkyDomeRadius),
	m_angle(0.0f),
	m_dir(Game::kVecZero)
{
	// モデルクラス初期化
	m_pModel = std::make_shared<Model>(MV1DuplicateModel(Load::GetInstance().GetHandle("skydome")));
	// スカイドームのスケールを設定
	m_pModel->SetScale(m_scale);
}

SkyDome::~SkyDome()
{
}

void SkyDome::Init(VECTOR playerPos)
{
	m_dir = playerPos;
	//X軸
	m_dir.x += 1.0f * cosf(m_angle);
	//Z軸
	m_dir.z += 1.0f * sinf(m_angle);
	// モデル情報更新
	m_pModel->SetPosition(playerPos);
	m_pModel->SetDirectionAxisZ(m_dir, 0.0f);
}

void SkyDome::Update(VECTOR playerPos)
{
	m_dir = playerPos;
	//X軸
	m_dir.x += 1.0f * cosf(m_angle);
	//Z軸
	m_dir.z += 1.0f * sinf(m_angle);
	// モデル情報更新
	m_pModel->SetPosition(playerPos);
	m_pModel->SetDirectionAxisZ(m_dir, 0.0f);
}

void SkyDome::Draw()
{
	m_pModel->Draw();
}

void SkyDome::SetScale(float scale)
{
	m_pModel->SetScale(m_scale * scale);
}
