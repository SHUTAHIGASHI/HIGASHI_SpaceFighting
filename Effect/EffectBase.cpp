#include "EffectBase.h"
#include "ObjectBase.h"

EffectBase::EffectBase(EffectType type, int handle, bool loop) :
	m_isEnable(true),
	m_type(type),
	m_isLoop(loop),
	m_isTracking(false),
	m_hEffectResource(handle),
	m_currentPlayingEffect(-1),
	m_pos(Game::kVecZero),
	m_trackingObject(nullptr)
{
	// エフェクトの生成
	CreateEffect();
}

EffectBase::EffectBase(EffectType type, int handle, bool loop, ObjectBase* obj) :
	m_isEnable(true),
	m_type(type),
	m_isLoop(loop),
	m_isTracking(true),
	m_hEffectResource(handle),
	m_currentPlayingEffect(-1),
	m_pos(obj->GetPos()),
	m_trackingObject(obj)
{
	// エフェクトの生成
	CreateEffect();
}

EffectBase::~EffectBase()
{
	// エフェクトの停止
	StopCurrentEffect();
}

void EffectBase::SetPosition(VECTOR pos)
{
	m_pos = pos;
}

void EffectBase::SetPositionOnlyZ(float posZ)
{
	m_pos.z = posZ;
}

void EffectBase::SetEffectPlayingSpeed(float speed)
{
	SetSpeedPlayingEffekseer3DEffect(m_currentPlayingEffect, speed);
}

void EffectBase::SetEffectColor(int r, int g, int b, int a)
{
	SetColorPlayingEffekseer3DEffect(m_currentPlayingEffect, r, g, b, a);
}

void EffectBase::SetRota(VECTOR rota)
{
	// エフェクトの回転
	SetRotationPlayingEffekseer3DEffect(m_currentPlayingEffect, rota.x, rota.y, rota.z);
}

void EffectBase::SetScale(float scale)
{
	if(scale <= 0.0f) return;
	SetScalePlayingEffekseer3DEffect(m_currentPlayingEffect, scale, scale, scale);
}

void EffectBase::Update()
{
	// エフェクトが再生中かどうか
	if (IsEffekseer3DEffectPlaying(m_currentPlayingEffect) != 0)
	{
		if (m_isLoop)
		{
			// ループ再生
			m_currentPlayingEffect = PlayEffekseer3DEffect(m_hEffectResource);
		}
		else
		{
			// 無効化
			m_isEnable = false;
		}
	}

	// 追尾オブジェクトが有効かどうか
	if (m_isTracking)
	{
		if (m_trackingObject != nullptr)
		{
			m_pos = m_trackingObject->GetPos();
		}
	}

	// エフェクトの座標設定
	SetPosPlayingEffekseer3DEffect(m_currentPlayingEffect, m_pos.x, m_pos.y, m_pos.z);
}

void EffectBase::StopCurrentEffect()
{
	// すでに停止している場合は何もしない
	if(!m_isEnable) m_isEnable = true;
	// エフェクトの停止
	int result = 0;
	if (IsEffekseer3DEffectPlaying(m_currentPlayingEffect) == 0)
	{
		result = StopEffekseer3DEffect(m_currentPlayingEffect);
	}
	assert(result == 0);
}

void EffectBase::CreateEffect()
{
	// エフェクトの再生
	m_currentPlayingEffect = PlayEffekseer3DEffect(m_hEffectResource);
	assert(m_hEffectResource != -1);
	// エフェクトの座標設定
	SetPosPlayingEffekseer3DEffect(m_currentPlayingEffect, m_pos.x, m_pos.y, m_pos.z);
}

