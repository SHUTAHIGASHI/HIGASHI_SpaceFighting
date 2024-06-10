#pragma once
#include "ObjectBase.h"
#include "Game.h"
#include <memory>
#include <string>

// オブジェクトの種類
enum class StageObjType
{
	Item,
	Rock,
	Enemy
};

class StageObjectBase : public ObjectBase
{
public:
	// コンストラクタ
	StageObjectBase(StageObjType type, std::string name);
	// デストラクタ
	virtual ~StageObjectBase();

	// 更新処理
	virtual void Update();
	// 描画処理
	void Draw();

	// 衝突時の処理
	virtual void OnCollision();

	// 座標設定
	void SetPos(VECTOR pos);
	// アウトライン設定
	void SetOutline();
	// 無効化
	virtual void SetDisable() { m_status.isEnabled = false; }

	// オブジェクトの種類取得
	StageObjType GetType() const { return m_type; }
protected:
	// オブジェクトの種類
	StageObjType m_type;
	// モデル
	std::shared_ptr<class Model> m_pModel;
};

