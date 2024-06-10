#include "TitleDemo.h"
#include "DemoCamera.h"
#include "DemoPlayer.h"
#include "DemoMap.h"
#include "EffekseerManager.h"

TitleDemo::TitleDemo():
	m_pDemoCamera(std::make_shared<DemoCamera>()),
	m_pDemoPlayer(std::make_shared<DemoPlayer>()),
	m_pDemoMap(std::make_shared<DemoMap>())
{
}

TitleDemo::~TitleDemo()
{
}

void TitleDemo::Init()
{
	// カメラの初期化処理
	m_pDemoCamera->SetPlayer(m_pDemoPlayer.get());
	m_pDemoCamera->Init();
	// プレイヤーの初期化処理
	m_pDemoPlayer->Init();
	// マップの初期化処理
	m_pDemoMap->Init();
	// エフェクトの初期化処理
	EffekseerManager::GetInstance().CreateEffect(EffectType::EngineFire, true, m_pDemoPlayer.get());
}

void TitleDemo::Update()
{
	// カメラの更新処理
	m_pDemoCamera->Update();
	// プレイヤーの更新処理
	m_pDemoPlayer->Update();
	// マップの更新処理
	m_pDemoMap->SetBasePos(m_pDemoPlayer->GetPos());
	m_pDemoMap->Update();
	// エフェクトの更新処理
	EffekseerManager::GetInstance().Update();
}

void TitleDemo::Draw()
{
	// プレイヤー描画
	m_pDemoPlayer->Draw();
	// マップ描画
	m_pDemoMap->Draw();
	// エフェクト描画
	EffekseerManager::GetInstance().Draw();
}

void TitleDemo::End()
{
	// 再生中のエフェクト停止
	EffekseerManager::GetInstance().StopAllEffect();
}
