#include "ObjectRock.h"
#include "Game.h"
#include "Model.h"

namespace
{
	// 拡大率
	constexpr float kScale = 1.0f;
}

ObjectRock::ObjectRock() :
	StageObjectBase(StageObjType::Rock, "rock")
{
	// 拡大率設定
	m_status.scale = kScale;
	// モデルのローカル座標を調整
	m_pModel->SetModelLocalPos(VGet(0.0f, -144.0f, 0.0f));
	// ステージ背景オブジェクトの拡大率設定
	m_pModel->SetScale(m_status.scale);
}

ObjectRock::~ObjectRock()
{ 
}
