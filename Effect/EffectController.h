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
	// �G�t�F�N�g�n���h��
	int m_hShootingStarEffect;
	int m_hEngineEffect;
	int m_hDeathEffect;
	int m_hExplosionEffect;

	// ��݂���G�t�F�N�g
	std::shared_ptr<EffectBase> m_ShootingStarEffect;
	std::vector<std::shared_ptr<EffectBase>> m_Effects;
	// �ꎞ�I�ȃG�t�F�N�g
	std::vector<std::shared_ptr<EffectBase>> m_TempEffects;
};

