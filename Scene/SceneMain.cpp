#include "SceneMain.h"
// �V�[���֌W
#include "SceneManager.h"
#include "SceneTitleMenu.h"
#include "ScenePauseMenu.h"
#include "SceneGameOver.h"
#include "SceneClear.h"
#include "SceneAllClear.h"
#include "SceneHowTo.h"
#include "SceneFade.h"
// �Q�[���֌W
#include "Game.h"
#include "CameraController.h"
#include "Player.h"
#include "StageManager.h"
#include "UiManager.h"
#include "EffekseerManager.h"
#include "SoundManager.h"
#include "GameDataManager.h"

namespace
{
	// �Q�[���I�������̎��s�t���[����
	constexpr int kGameEndTimeCountMax = 120;
}

SceneMain::SceneMain(SceneManager& manager) :
	Scene(manager),
	m_updateFunc(&SceneMain::StartUpdate),
	m_drawFunc(&SceneMain::StartDraw),
	m_pCamera(std::make_shared<CameraController>()),
	m_pPlayer(std::make_shared<Player>()),
	m_pStage(std::make_shared<StageManager>()),
	m_pUi(std::make_shared<UiManager>()),
	m_isEnd(false),
	m_isDrawOperation(false),
	m_endTimeCount(0)
{
}

SceneMain::~SceneMain()
{
	// �Đ����̉��y���~
	SoundManager::GetInstance().StopBGM();
}

void SceneMain::Init()
{
	// �Q�[���|�C���g���Z�b�g
	GameDataManager::GetInstance().ResetPoint();

	// �v���C���[�̏���������
	m_pPlayer->SetCamera(m_pCamera.get());
	m_pPlayer->SetUi(m_pUi.get());
	m_pPlayer->Init();

	// �X�e�[�W�̏���������
	m_pStage->SetPlayer(m_pPlayer.get());
	m_pStage->SetUi(m_pUi.get());
	m_pStage->Init();

	// �J����������
	m_pCamera->SetPlayer(m_pPlayer.get());
	m_pCamera->Init();

	// ��������`��t���O
	if (GameDataManager::GetInstance().IsFirstPlay())
	{
		m_isDrawOperation = true;
		GameDataManager::GetInstance().SetFirstPlay(false);
	}
}

void SceneMain::Update(const InputState& input)
{
	// Update�����̃����o�֐��|�C���^
	(this->*m_updateFunc)(input);

	// UI�X�V����
	m_pUi->Update();

	// �G�t�F�N�g�̍X�V����
	EffekseerManager::GetInstance().Update();
}

void SceneMain::Draw()
{
	// �X�e�[�W�`��
	m_pStage->Draw();
	// �v���C���[�`��
	m_pPlayer->Draw();

	// Draw�����̃����o�֐��|�C���^
	(this->*m_drawFunc)();

	// �G�t�F�N�g�`��
	EffekseerManager::GetInstance().Draw();
}

void SceneMain::End()
{
	// �Đ����̃G�t�F�N�g��~
	EffekseerManager::GetInstance().StopAllEffect();
}

void SceneMain::CheckStageEnd()
{
	// �X�e�[�W�I������
	if (m_pStage->IsClear() || m_pPlayer->IsDead())
	{
		// �X�e�[�W�I�����̏���
		m_pStage->OnStageEnd();
		// �Q�[���I�����̏���
		OnGameEnd();
		return;
	}
}

void SceneMain::ChangeNextScene()
{
	// �X�e�[�W�N���A���肾������
	if (m_pStage->IsClear())
	{
		// �S�X�e�[�W�N���A���Ă�����
		if (GameDataManager::GetInstance().IsAllStageClear())
		{
			// �I�[���N���A�V�[���ֈڍs
			m_Manager.PushSceneAllUpdate(new SceneAllClear(m_Manager));
		}
		else
		{
			// �ʏ�N���A�V�[���ֈڍs
			m_Manager.PushSceneAllUpdate(new SceneClear(m_Manager));
		}
	}
	else 
	{
		// �Q�[���I�[�o�[�V�[���ֈڍs
		m_Manager.PushSceneAllUpdate(new SceneGameOver(m_Manager)); 
	}
}

void SceneMain::OnGameStart()
{
	// �J��������ʒu�ɂ�����ʏ�̏����ւƈڍs
	m_updateFunc = &SceneMain::NormalUpdate;
	m_drawFunc = &SceneMain::NormalDraw;
	// �N���X�w�A�\��
	m_pPlayer->SetCrosshair(true);
}

void SceneMain::OnGameEnd()
{
	// �Q�[���I�������̎��s�t���[������ݒ�
	m_endTimeCount = kGameEndTimeCountMax;
	// �Q�[���I�������ֈڍs
	m_updateFunc = &SceneMain::EndUpdate;
	m_drawFunc = &SceneMain::EndDraw;
	// �N���X�w�A��\��
	m_pPlayer->SetCrosshair(false);
}

void SceneMain::NormalUpdate(const InputState& input)
{
	// �|�[�Y���j���[�ֈڍs
	if (input.IsTriggered(InputType::pause))
	{
		// BGM��~
		SoundManager::GetInstance().StopBGM();
		// �|�[�Y���j���[�V�[���ֈڍs
		m_Manager.PushScene(new ScenePauseMenu(m_Manager));
		return;
	}

	// ���y�Đ��J�n
	if (SoundManager::GetInstance().IsPlayingMusic() != 1)
	{
		SoundManager::GetInstance().PlayMusic(MusicType::main);
	}

	// �v���C���[�X�V����
	m_pPlayer->Update(input);
	// �X�e�[�W�X�V����
	m_pStage->Update(input);
	// �J�����X�V����
	m_pCamera->Update();

	// �X�e�[�W�I������
	CheckStageEnd();
}

void SceneMain::StartUpdate(const InputState& input)
{
	// �J�����X�V����
	m_pCamera->StartUpdate();

	// �J�����̊J�n���̏������s��
	if (m_pCamera->IsCameraSet())
	{
		// �Q�[���J�n���̏���
		OnGameStart();
	}

	// ��������`��t���O�������Ă�����
	if (m_isDrawOperation)
	{
		// ��������V�[�������s
		m_Manager.PushScene(new SceneHowTo(m_Manager));
		// ��������`��t���O��������
		m_isDrawOperation = false;
		return;
	}
}

void SceneMain::EndUpdate(const InputState& input)
{
	// �Q�[���I�������̎��s�t���[�������J�E���g
	m_endTimeCount--;
	if (m_endTimeCount < 0)
	{
		// �V�[���I��
		ChangeNextScene();
		// ���U���g�\�����̍X�V�����ֈڍs
		m_updateFunc = &SceneMain::ResultUpdate;
		// �J�����������ʒu��
		m_pCamera->Init();
		return;
	}

	// �v���C���[�X�V����
	m_pPlayer->Update(input);
	// �X�e�[�W�X�V����
	m_pStage->Update(input);
	// �J�����X�V����
	m_pCamera->Update();
}

void SceneMain::ResultUpdate(const InputState& input)
{
	// �v���C���[�X�V����
	m_pPlayer->Update(input);
	// �X�e�[�W�X�V����
	m_pStage->Update(input);
	// �J�����X�V����
	m_pCamera->ResultUpdate();
}

void SceneMain::NormalDraw()
{
	// UI�`��
	m_pUi->Draw();
}

void SceneMain::StartDraw()
{
	// �����Ȃ�
}

void SceneMain::EndDraw()
{
	// �����Ȃ�
}
