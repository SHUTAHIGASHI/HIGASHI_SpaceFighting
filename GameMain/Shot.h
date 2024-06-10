#pragma once
#include "ObjectBase.h"
#include <DxLib.h>

class Shot : public ObjectBase
{
public:
	Shot(VECTOR pos, VECTOR target);
	~Shot();

	// 更新
	void Update();
	// 描画
	void Draw();

	// 着弾時
	void OnHit();

	// 弾速設定
	void SetSpeed(float speed) { m_status.moveSpeed = speed; }
	// 弾の大きさ設定
	void SetScale(float scale);
private:
	// 弾の有効時間
	int m_shotTime = 60;
};

