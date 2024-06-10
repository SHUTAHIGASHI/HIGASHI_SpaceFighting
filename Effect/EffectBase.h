#pragma once
#include "Game.h"

// エフェクトの種類
enum class EffectType
{
	EngineFire,
	Dead,
	Damage,
	BossDamage,
	Shot,
	EnemyShot,
	Charge,
	ChargeShot,
	Heal,
	Avoid
};

// エフェクトベース
class EffectBase
{
public:
	// コンストラクタ
	EffectBase(EffectType type, int handle, bool loop);
	EffectBase(EffectType type, int handle, bool loop, class ObjectBase* obj);
	// デストラクタ
	virtual ~EffectBase();

	// 更新処理
	void Update();
	// エフェクト停止
	void StopCurrentEffect();
	// エフェクト作成
	void CreateEffect();

	// エフェクトの各種設定
	void SetPosition(VECTOR pos);
	void SetPositionOnlyZ(float posZ);
	void SetEffectPlayingSpeed(float speed);
	void SetEffectColor(int r, int g, int b, int a);
	void SetTracking(bool isTracking) { m_isTracking = isTracking; }
	void SetRota(VECTOR rota);
	void SetScale(float scale);
	void SetEnable(bool isEnable) { m_isEnable = isEnable; }

	// エフェクト有効判定取得
	bool IsEnable() { return m_isEnable; }
	// エフェクトタイプ取得
	EffectType GetEffectType() { return m_type; }
	// 追尾中のオブジェクト取得
	class ObjectBase* GetTrackingObject() 
	{ 
		// 追尾中のオブジェクトが存在する場合はそのオブジェクトを返す
		if(m_trackingObject != nullptr) return m_trackingObject; 
		else return nullptr;
	}

protected:
	// エフェクトが有効かどうか
	bool m_isEnable;
	// エフェクトが追尾するかどうか
	bool m_isTracking;
	// エフェクトがループするかどうか
	bool m_isLoop;
	// エフェクトタイプ
	EffectType m_type;
	// エフェクトのハンドル
	int m_hEffectResource;
	int m_currentPlayingEffect;
	// 描画座標
	VECTOR m_pos;
	// 追尾するオブジェクト
	ObjectBase* m_trackingObject = nullptr;
};

