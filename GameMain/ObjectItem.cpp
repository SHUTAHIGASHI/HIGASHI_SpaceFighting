#include "ObjectItem.h"
#include "Game.h"
#include "Model.h"

namespace
{
	// 拡大率
	constexpr float kScale = 0.5f;
}

ObjectItem::ObjectItem() :
	StageObjectBase(StageObjType::Item, "item")
{
	// 拡大率設定
	m_status.scale = kScale;
	// ステージ背景オブジェクトの拡大率設定
	m_pModel->SetScale(m_status.scale);
}

ObjectItem::~ObjectItem()
{
}

void ObjectItem::OnCollision()
{
	// オブジェクトを無効にする
	m_status.isEnabled = false;
}
