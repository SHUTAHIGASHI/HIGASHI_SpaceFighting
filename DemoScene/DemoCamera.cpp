#include "DemoCamera.h"
#include "Camera.h"
#include "DemoPlayer.h"

namespace
{
	// �J�����ʒu�̒萔
	constexpr float kCameraPosX = 600.0f;
	constexpr float kCameraPosY = 200.0f;
	constexpr float kCameraPosZ = 800.0f;
	// �����_�܂ł̈ړ����x
	constexpr float kTargetTrackingSpeed = 10.0f;
}

// �R���X�g���N�^
DemoCamera::DemoCamera() :
	m_targetPos(Game::kVecZero),
	m_pDemoPlayer(nullptr),
	m_pCamera(std::make_shared<Camera>())
{
}

// �f�X�g���N�^
DemoCamera::~DemoCamera()
{
	// �����Ȃ�
}

// ������
void DemoCamera::Init()
{
	// �J�����ݒ菉����
	m_pCamera->Init(CameraSetting::kCameraNear, CameraSetting::kCameraFar * 2, CameraSetting::kFovNum);
	// �J�����ʒu�ݒ�
	UpdateCameraPos();
}

// �X�V
void DemoCamera::Update()
{
	// �J�����ʒu�X�V
	UpdateCameraPos();
	// �J�����X�V
	m_pCamera->Update();
}

void DemoCamera::UpdateCameraPos()
{
	// �v���C���[�̈ʒu��Ǐ]
	VECTOR pos = m_pDemoPlayer->GetPos();
	pos.x = kCameraPosX; pos.y = kCameraPosY; pos.z += kCameraPosZ;
	// �J�����̒����_�ݒ�
	VECTOR targetDir = VSub(m_pDemoPlayer->GetPos(), m_targetPos);
	if (VSize(targetDir) > 0) targetDir = VNorm(targetDir);
	targetDir = VScale(targetDir, kTargetTrackingSpeed);
	m_targetPos = VAdd(m_targetPos, targetDir);
	// �J�������W�ݒ�
	m_pCamera->SetPosAndTarget(pos, m_targetPos);
}
