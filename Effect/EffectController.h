#pragma once
#include "Game.h"
#include <vector>
#include <memory>
#include "Players.h"

class EffectBase;

class EffectController
{
public:
	EffectController();
	virtual ~EffectController();

	void Init();
	void Update(VECTOR pos);
	void Draw();

	void OnDead(VECTOR pos);
	void OnDamage(VECTOR pos);
	void OnBoost(BoostLevel level);

	void DeleteDisableEffect();
private:
	void LoadData();

private:
	// エフェクトハンドル
	int m_hShootingStarEffect;
	int m_hEngineEffect;
	int m_hDeathEffect;
	int m_hExplosionEffect;

	// 常設するエフェクト
	std::shared_ptr<EffectBase> m_ShootingStarEffect;
	std::vector<std::shared_ptr<EffectBase>> m_Effects;
	// 一時的なエフェクト
	std::vector<std::shared_ptr<EffectBase>> m_TempEffects;
};

