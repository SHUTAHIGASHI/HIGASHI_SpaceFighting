#include "StageObjectBase.h"
#include "Model.h"
#include "EffekseerManager.h"
#include "SoundManager.h"
#include "Load.h"

namespace
{
	// 当たり判定調整数値
	constexpr float kCollRadius = 128.0f;
	// モデルのアウトラインサイズ
	constexpr float kOutlineSize = 0.5f;
	// モデル回転速度
	constexpr float kRotaSpeed = 0.01f;
}

StageObjectBase::StageObjectBase(StageObjType type, std::string name) :
	m_type(type),
	m_pModel(nullptr)
{
	// 当たり判定設定
	m_status.radius = kCollRadius;
	// ステータス初期化
	m_status.hModel = MV1DuplicateModel(Load::GetInstance().GetHandle(name));
	// モデル生成
	m_pModel = std::make_shared<Model>(m_status.hModel);
}

StageObjectBase::~StageObjectBase()
{
	// モデルの削除
	MV1DeleteModel(m_status.hModel);
}

void StageObjectBase::Update()
{
	// 回転角度の更新
	m_status.rota.y += kRotaSpeed;
	if(m_status.rota.y > DX_PI_F) m_status.rota.y = -DX_PI_F;

	// モデルの更新
	m_pModel->SetPosition(m_status.pos);
	m_pModel->SetRotation(m_status.rota);
	m_pModel->Update();
}

void StageObjectBase::Draw()
{
	// モデルの描画
	m_pModel->Draw();
#ifdef _DEBUG
	DrawSphere3D(m_status.pos, m_status.radius, 16.0f, 0xffffff, 0xffffff, false);
#endif
}

void StageObjectBase::SetPos(VECTOR pos)
{
	// 座標の設定
	m_status.pos = pos;
	// モデルの座標設定
	m_pModel->SetPosition(m_status.pos);
}

void StageObjectBase::SetOutline()
{
	// アウトラインの設定
	m_pModel->SetOutline(1, kOutlineSize, 255, 0, 0, 255);
}

void StageObjectBase::OnCollision()
{
	// オブジェクトの無効化
	m_status.isEnabled = false;
	// ダメージ音再生
	SoundManager::GetInstance().PlaySE(SoundType::damage);
	// ダメージエフェクト生成
	EffekseerManager::GetInstance().CreateEffect(EffectType::Damage, false, m_status.pos);
}
