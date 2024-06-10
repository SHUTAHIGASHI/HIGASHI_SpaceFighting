#include "EffectController.h"
#include "EffectType.h"
#include "TempEffectBase.h"
#include "LoopEffectBase.h"

EffectController::EffectController():
	m_hShootingStarEffect(-1),
	m_hEngineEffect(-1),
	m_hDeathEffect(-1),
	m_hExplosionEffect(-1),
	m_Effects()
{
}

EffectController::~EffectController()
{
	m_ShootingStarEffect->StopCurrentEffect();
	for (auto& effects : m_Effects)
	{
		effects->StopCurrentEffect();
	}
	for (auto& effects : m_TempEffects)
	{
		effects->StopCurrentEffect();
	}

	m_Effects.clear();

	DeleteEffekseerEffect(m_hShootingStarEffect);
	DeleteEffekseerEffect(m_hEngineEffect);
	DeleteEffekseerEffect(m_hDeathEffect);
	DeleteEffekseerEffect(m_hExplosionEffect);
}

void EffectController::Init()
{
	LoadData();
	
	m_ShootingStarEffect =  std::make_shared<LoopEffectBase>(EffectType::ShootingStar, m_hShootingStarEffect);
	m_Effects.push_back(std::make_shared<LoopEffectBase>(EffectType::EngineFireShip, m_hEngineEffect));
}

void EffectController::Update(VECTOR pos)
{
	Effekseer_Sync3DSetting();

	m_ShootingStarEffect->SetPositionOnlyZ(pos.z);
	m_ShootingStarEffect->Update();

	for (auto& effects : m_Effects)
	{
		effects->SetPosition(pos);
		effects->Update();
	}

	for (auto& effects : m_TempEffects)
	{
		effects->SetPosition(pos);
		effects->Update();
		if (effects->IsDelete())
		{
			DeleteDisableEffect();
		}
	}

	UpdateEffekseer3D();
}

void EffectController::Draw()
{
	DrawEffekseer3D();
}

void EffectController::OnDead(VECTOR pos)
{
	for (auto& efc : m_TempEffects)
	{
		if (efc->GetEffectType() == EffectType::Death)
		{
			return;
		}
	}

	m_TempEffects.push_back(std::make_shared<TempEffectBase>(EffectType::Death, m_hDeathEffect));
	m_TempEffects.back()->SetPosition(pos);
}

void EffectController::OnDamage(VECTOR pos)
{
	m_TempEffects.push_back(std::make_shared<TempEffectBase>(EffectType::Explosion, m_hExplosionEffect));
	m_TempEffects.back()->SetPosition(pos);
	m_ShootingStarEffect->SetEffectPlayingSpeed(1.0f);
}

void EffectController::OnBoost(BoostLevel level)
{
	m_ShootingStarEffect->SetEffectPlayingSpeed(static_cast<float>(level) + 1);
}

void EffectController::DeleteDisableEffect()
{
	// ïKóvÇ»Ç≠Ç»Ç¡ÇΩUIÇÕèáî‘Ç…çÌèú
	auto rmIt = std::remove_if(m_TempEffects.begin(), m_TempEffects.end(),
		[](const std::shared_ptr<EffectBase>& efc)
		{
			return efc->IsDelete();
		});

	// é¿ç€Ç…îÕàÕÇéwíËÇµÇƒçÌèú
	m_TempEffects.erase(rmIt, m_TempEffects.end());
}

void EffectController::LoadData()
{
	m_hShootingStarEffect = LoadEffekseerEffect("Data/EffectData/StarParticle.efkefc", 10.0f);
	m_hEngineEffect = LoadEffekseerEffect("Data/EffectData/EngineFire.efkefc", 10.0f);
	m_hDeathEffect = LoadEffekseerEffect("Data/EffectData/DeadGameOverEffect.efkefc", 10.0f);
	m_hExplosionEffect = LoadEffekseerEffect("Data/EffectData/Explosion.efkefc", 100.0f);
}
