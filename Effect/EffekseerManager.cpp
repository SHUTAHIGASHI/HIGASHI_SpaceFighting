#include "EffekseerManager.h"
#include "EffectBase.h"

EffekseerManager::EffekseerManager() :
	m_EffectHandle(),
	m_pEffects()
{
	// エフェクトの読み込み
	LoadData();
}

EffekseerManager::~EffekseerManager()
{
	// エフェクトの削除
	m_pEffects.clear();
	// エフェクトの破棄
	for (auto& efc : m_EffectHandle)
	{
		// エフェクトデータの削除
		DeleteEffekseerEffect(efc.second);
	}
	// エフェクトハンドルのクリア
	m_EffectHandle.clear();
}

void EffekseerManager::Update()
{
	// エフェクシアの3D設定を更新
	Effekseer_Sync3DSetting();
	// エフェクシアの更新
	for (auto& effects : m_pEffects)
	{
		effects->Update();
	}
	// 無効になったエフェクトは削除
	DeleteDisableEffect();
	// エフェクトの更新
	UpdateEffekseer3D();
}

void EffekseerManager::Draw()
{
	// エフェクトの描画
	DrawEffekseer3D();
}

void EffekseerManager::CreateEffect(EffectType type, bool loop, ObjectBase* obj)
{
	// エフェクトの生成
	m_pEffects.push_back(std::make_shared<EffectBase>(type, m_EffectHandle[type], loop, obj));
}

void EffekseerManager::CreateEffectAndSetScale(EffectType type, bool loop, ObjectBase* obj, float scale)
{
	// エフェクトの生成
	m_pEffects.push_back(std::make_shared<EffectBase>(type, m_EffectHandle[type], loop, obj));
	// エフェクトの拡大率設定
	m_pEffects.back()->SetScale(scale);
}

void EffekseerManager::CreateEffect(EffectType type, bool loop, VECTOR pos)
{
	// エフェクトの生成
	m_pEffects.push_back(std::make_shared<EffectBase>(type, m_EffectHandle[type], loop));
	// エフェクトの座標設定
	m_pEffects.back()->SetPosition(pos);
}

void EffekseerManager::StopAllEffect()
{
	for (auto& effects : m_pEffects)
	{
		// エフェクトの無効化
		effects->SetEnable(false);
	}
	// 無効になったエフェクトは削除
	DeleteDisableEffect();
	// エフェクトの更新
	UpdateEffekseer3D();
}

void EffekseerManager::StopEffect(EffectType type)
{
	for (auto& efc : m_pEffects)
	{
		// エフェクトの種類が一致したら
		if (efc->GetEffectType() == type)
		{
			// エフェクトの停止
			efc->StopCurrentEffect();
		}
	}
}

void EffekseerManager::StopEffectTargetObj(class ObjectBase* obj)
{
	for (auto& efc : m_pEffects)
	{
		// 追尾中のオブジェクトが一致したら
		if (efc->GetTrackingObject() == obj)
		{
			// エフェクトの停止
			efc->StopCurrentEffect();
		}
	}
}

void EffekseerManager::DeleteDisableEffect()
{
	// 必要なくなったエフェクトは順番に削除
	auto rmIt = std::remove_if(m_pEffects.begin(), m_pEffects.end(),
		[](const std::shared_ptr<EffectBase>& efc)
		{
			return !efc->IsEnable();
		});

	// 実際に範囲を指定して削除
	m_pEffects.erase(rmIt, m_pEffects.end());
}

void EffekseerManager::SetEffectPosition(EffectType type, VECTOR pos)
{
	for (auto& efc : m_pEffects)
	{
		// エフェクトの種類が一致したら
		if (efc->GetEffectType() == type)
		{
			// エフェクトの座標設定
			efc->SetPosition(pos);
		}
	}
}

void EffekseerManager::SetEffectRota(EffectType type, VECTOR rota)
{
	for (auto& efc : m_pEffects)
	{
		// エフェクトの種類が一致したら
		if (efc->GetEffectType() == type)
		{
			// エフェクトの回転設定
			efc->SetRota(rota);
		}
	}
}

bool EffekseerManager::IsPlayingEffect(EffectType type)
{
	for (auto& efc : m_pEffects)
	{
		// エフェクトの種類が一致したら
		if (efc->GetEffectType() == type)
		{
			// エフェクトが再生中かどうか
			return true;
		}
	}
	return false;
}

EffectBase* EffekseerManager::GetEffect(EffectType type)
{
	for (auto& efc : m_pEffects)
	{
		// エフェクトの種類が一致したら
		if (efc->GetEffectType() == type)
		{
			// エフェクトを返す
			return efc.get();
		}
	}
	return nullptr;
}

void EffekseerManager::LoadData()
{
	// エフェクトの読み込み
	m_EffectHandle[EffectType::EngineFire] = LoadEffekseerEffect("Data/EffectData/EngineFire.efkefc", 10.0f);
	m_EffectHandle[EffectType::Dead] = LoadEffekseerEffect("Data/EffectData/DeadGameOverEffect.efkefc", 100.0f);
	m_EffectHandle[EffectType::Damage] = LoadEffekseerEffect("Data/EffectData/Explosion.efkefc", 100.0f);
	m_EffectHandle[EffectType::BossDamage] = LoadEffekseerEffect("Data/EffectData/BossExplosion.efkefc", 100.0f);
	m_EffectHandle[EffectType::Shot] = LoadEffekseerEffect("Data/EffectData/Shot.efkefc", 10.0f);
	m_EffectHandle[EffectType::EnemyShot] = LoadEffekseerEffect("Data/EffectData/EnemyShot.efkefc", 10.0f);
	m_EffectHandle[EffectType::Heal] = LoadEffekseerEffect("Data/EffectData/Heal.efkefc", 25.0f);
	m_EffectHandle[EffectType::Charge] = LoadEffekseerEffect("Data/EffectData/Charge.efkefc", 25.0f);
	m_EffectHandle[EffectType::ChargeShot] = LoadEffekseerEffect("Data/EffectData/ChargeShot.efkefc", 10.0f);
	m_EffectHandle[EffectType::Avoid] = LoadEffekseerEffect("Data/EffectData/Avoid.efkefc", 25.0f);
}
