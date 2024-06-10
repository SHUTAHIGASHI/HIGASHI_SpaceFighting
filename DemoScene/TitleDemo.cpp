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
	// �J�����̏���������
	m_pDemoCamera->SetPlayer(m_pDemoPlayer.get());
	m_pDemoCamera->Init();
	// �v���C���[�̏���������
	m_pDemoPlayer->Init();
	// �}�b�v�̏���������
	m_pDemoMap->Init();
	// �G�t�F�N�g�̏���������
	EffekseerManager::GetInstance().CreateEffect(EffectType::EngineFire, true, m_pDemoPlayer.get());
}

void TitleDemo::Update()
{
	// �J�����̍X�V����
	m_pDemoCamera->Update();
	// �v���C���[�̍X�V����
	m_pDemoPlayer->Update();
	// �}�b�v�̍X�V����
	m_pDemoMap->SetBasePos(m_pDemoPlayer->GetPos());
	m_pDemoMap->Update();
	// �G�t�F�N�g�̍X�V����
	EffekseerManager::GetInstance().Update();
}

void TitleDemo::Draw()
{
	// �v���C���[�`��
	m_pDemoPlayer->Draw();
	// �}�b�v�`��
	m_pDemoMap->Draw();
	// �G�t�F�N�g�`��
	EffekseerManager::GetInstance().Draw();
}

void TitleDemo::End()
{
	// �Đ����̃G�t�F�N�g��~
	EffekseerManager::GetInstance().StopAllEffect();
}
