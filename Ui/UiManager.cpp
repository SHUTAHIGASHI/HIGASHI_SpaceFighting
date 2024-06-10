#include "UiManager.h"
#include "SceneMain.h"
#include "UiBar.h"
#include "UiPlayerHpBar.h"
#include "UiGetPoint.h"
#include "UiLostPoint.h"
#include "Player.h"
#include "EnemyBoss.h"
#include "GameDataManager.h"

namespace
{
	// ステージ目標描画位置
	constexpr float kStageRateDrawPosX = 200.0f;
	constexpr float kStageRateDrawPosY = 60.0f;
	// ステージテキスト描画位置
	constexpr float kStageTextDrawPosX = 40.0f;
	constexpr float kStageTextDrawPosY = 20.0f;
	// 描画テキスト
	const char* const kStageRateText = "ボスを倒せ！";
	// ボス撃破テキスト描画位置
	constexpr float kBossTextDrawPosX = 65.0f;
	constexpr float kBossTextDrawPosY = 40.0f;
}

UiManager::UiManager():
	m_isBossRate(false),
	m_stageNum(0),
	m_hHpBarImg(-1),
	m_hBgImg(-1),
	m_playerHp(0),
	m_bossHp(0),
	m_enemyKillCount(0),
	m_enemyKillCountMax(0),
	m_stageRate(0),
	m_pPointUI(nullptr),
	m_pPlayerHp(nullptr),
	m_pBossHp(nullptr)
{
	m_stageNum = GameDataManager::GetInstance().GetCurrentStageNum();
	
	// 画像の読み込み
	m_hHpBarImg = LoadGraph("Data/ImageData/HpBarImg.png");
	m_hBgImg = LoadGraph("Data/ImageData/BoardImg.png");

	// UI生成
	m_pPlayerHp = new UiPlayerHpBar(m_hHpBarImg);
	if (m_pPlayerHp != nullptr)
	{
		m_pPlayerHp->Init(PlayerInfo::kHpMax);
	}
}

UiManager::~UiManager()
{
	delete m_pPlayerHp;
	delete m_pBossHp;
	// 画像の削除
	DeleteGraph(m_hHpBarImg);
	DeleteGraph(m_hBgImg);
}

void UiManager::Update()
{
	if (m_pPointUI != nullptr) m_pPointUI->Update();
	if (m_pPlayerHp != nullptr)
	{
		m_pPlayerHp->Update();
		m_pPlayerHp->UpdateUIData(m_playerHp);
	}
	if (m_pBossHp != nullptr)
	{
		m_pBossHp->Update();
		m_pBossHp->UpdateUIData(m_bossHp);
	}

	DeleteUI();
}

void UiManager::Draw()
{
	// ポイントUI描画
	if (m_pPointUI != nullptr) m_pPointUI->Draw();
	// プレイヤーHPバー描画
	if (m_pPlayerHp != nullptr) m_pPlayerHp->Draw();
	// ショットUI描画
	if (m_pChargeShot != nullptr) m_pChargeShot->Draw();
	// ボスHPバー描画
	if (m_pBossHp != nullptr) m_pBossHp->Draw();

	// ステージ目標描画
	DrawStageRate();

	// ステージ番号描画
	DrawStageNum();
}

void UiManager::DeleteUI()
{
	// 処理が一通り終わったら削除
	if (m_pPointUI != nullptr)
	{
		if (m_pPointUI->IsDelete())
		{
			delete m_pPointUI;
			m_pPointUI = nullptr;
		}
	}
}

void UiManager::CreateShotUI(int maxShot)
{
	m_pChargeShot = new UiBar();
	if (m_pChargeShot != nullptr)
	{
		m_pChargeShot->Init(maxShot);
		m_pChargeShot->SetDrawPos(Game::kScreenWidthHalf, Game::kScreenHeightHalf);
	}
}

void UiManager::DeleteShotUI()
{
	if (m_pChargeShot != nullptr) delete m_pChargeShot;
	m_pChargeShot = nullptr;
}

void UiManager::UpdateShotUI(int chargeNum)
{
	if (m_pChargeShot != nullptr)
	{
		m_pChargeShot->Update();
		m_pChargeShot->UpdateUIData(chargeNum);
	}
}

void UiManager::SetShotDrawPos(float x, float y)
{
	if (m_pChargeShot != nullptr)
	{
		m_pChargeShot->SetDrawPos(static_cast<int>(x), static_cast<int>(y));
	}
}

UiBar* UiManager::GetChargeShotUI()
{
	if (m_pChargeShot != nullptr)
	{
		return m_pChargeShot;
	}
	return nullptr;
}

void UiManager::CreateBossUI(int maxHp)
{
	m_pBossHp = new UiBar();
	if (m_pBossHp != nullptr)
	{
		m_pBossHp->Init(maxHp);
		m_pBossHp->SetDrawPos(Game::kScreenWidthHalf, Game::kScreenHeightHalf);
	}
}

void UiManager::DeleteBossUI()
{
	if (m_pBossHp != nullptr) delete m_pBossHp;
	m_pBossHp = nullptr;
}

void UiManager::CountEnemyKill()
{
	m_enemyKillCount++;
}

void UiManager::OnGetPoint(int point)
{
	if (m_pPointUI != nullptr)
	{
		delete m_pPointUI;
	}

	m_pPointUI = new UiGetPoint(point);
}

void UiManager::OnLosePoint(int point)
{
	if (m_pPointUI != nullptr)
	{
		delete m_pPointUI;
	}

	m_pPointUI = new UiLostPoint(point);
}

void UiManager::SetStageRate(int rate)
{
	m_stageRate = rate;
	if (m_stageRate >= 100)
	{
		m_stageRate = 100;
	}
}

void UiManager::SetBossHpDrawPos(VECTOR pos)
{
	if (m_pBossHp != nullptr)
	{
		VECTOR drawPos = ConvWorldPosToScreenPos(pos);
		m_pBossHp->SetDrawPos(static_cast<int>(drawPos.x), static_cast<int>(drawPos.y));
	}
}

void UiManager::DrawStageRate()
{
	// 背景描画
	DrawRotaGraphF(kStageRateDrawPosX, kStageRateDrawPosY, 0.5, 0.0, m_hBgImg, true);

	if (!m_isBossRate)
	{
		// 敵撃破数描画
		DrawFormatStringF(kStageTextDrawPosX, kStageTextDrawPosY, 0xffffff, "撃破数 : %d/%d", m_enemyKillCount, m_enemyKillCountMax);
		// ステージ進行率描画
		DrawFormatStringF(kStageTextDrawPosX, kStageTextDrawPosY + Game::kFontSize, 0xffffff, "進行度 : %d％", m_stageRate);
	}
	else
	{
		// ボス撃破率描画
		std::string drawText = kStageRateText;
		DrawFormatStringF(kBossTextDrawPosX, kBossTextDrawPosY, 0xffffff, "%s", drawText.c_str());
	}
}

void UiManager::DrawStageNum()
{
	// ステージ番号描画
	std::string drawText = "ステージ : " + std::to_string(m_stageNum) + " ";
	// テキストサイズ取得
	int textLength = GetDrawFormatStringWidth(drawText.c_str());
	DrawFormatStringF(static_cast<float>(Game::kScreenWidth - textLength), 10.0f, 0xffffff, "%s", drawText.c_str());
}
