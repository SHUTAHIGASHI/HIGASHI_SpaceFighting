#pragma once
#include "ObjectBase.h"
#include <DxLib.h>

class Shot : public ObjectBase
{
public:
	Shot(VECTOR pos, VECTOR target);
	~Shot();

	// XV
	void Update();
	// •`‰æ
	void Draw();

	// ’…’e
	void OnHit();

	// ’e‘¬İ’è
	void SetSpeed(float speed) { m_status.moveSpeed = speed; }
	// ’e‚Ì‘å‚«‚³İ’è
	void SetScale(float scale);
private:
	// ’e‚Ì—LŒøŠÔ
	int m_shotTime = 60;
};

