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
	// �X�e�[�W�ڕW�`��ʒu
	constexpr float kStageRateDrawPosX = 200.0f;
	constexpr float kStageRateDrawPosY = 60.0f;
	// �X�e�[�W�e�L�X�g�`��ʒu
	constexpr float kStageTextDrawPosX = 40.0f;
	constexpr float kStageTextDrawPosY = 20.0f;
	// �`��e�L�X�g
	const char* const kStageRateText = "�{�X��|���I";
	// �{�X���j�e�L�X�g�`��ʒu
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
	
	// �摜�̓ǂݍ���
	m_hHpBarImg = LoadGraph("Data/ImageData/HpBarImg.png");
	m_hBgImg = LoadGraph("Data/ImageData/BoardImg.png");

	// UI����
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
	// �摜�̍폜
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
	// �|�C���gUI�`��
	if (m_pPointUI != nullptr) m_pPointUI->Draw();
	// �v���C���[HP�o�[�`��
	if (m_pPlayerHp != nullptr) m_pPlayerHp->Draw();
	// �V���b�gUI�`��
	if (m_pChargeShot != nullptr) m_pChargeShot->Draw();
	// �{�XHP�o�[�`��
	if (m_pBossHp != nullptr) m_pBossHp->Draw();

	// �X�e�[�W�ڕW�`��
	DrawStageRate();

	// �X�e�[�W�ԍ��`��
	DrawStageNum();
}

void UiManager::DeleteUI()
{
	// ��������ʂ�I�������폜
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
	// �w�i�`��
	DrawRotaGraphF(kStageRateDrawPosX, kStageRateDrawPosY, 0.5, 0.0, m_hBgImg, true);

	if (!m_isBossRate)
	{
		// �G���j���`��
		DrawFormatStringF(kStageTextDrawPosX, kStageTextDrawPosY, 0xffffff, "���j�� : %d/%d", m_enemyKillCount, m_enemyKillCountMax);
		// �X�e�[�W�i�s���`��
		DrawFormatStringF(kStageTextDrawPosX, kStageTextDrawPosY + Game::kFontSize, 0xffffff, "�i�s�x : %d��", m_stageRate);
	}
	else
	{
		// �{�X���j���`��
		std::string drawText = kStageRateText;
		DrawFormatStringF(kBossTextDrawPosX, kBossTextDrawPosY, 0xffffff, "%s", drawText.c_str());
	}
}

void UiManager::DrawStageNum()
{
	// �X�e�[�W�ԍ��`��
	std::string drawText = "�X�e�[�W : " + std::to_string(m_stageNum) + " ";
	// �e�L�X�g�T�C�Y�擾
	int textLength = GetDrawFormatStringWidth(drawText.c_str());
	DrawFormatStringF(static_cast<float>(Game::kScreenWidth - textLength), 10.0f, 0xffffff, "%s", drawText.c_str());
}
