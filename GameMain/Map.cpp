#include "Map.h"
#include "Model.h"
#include "SkyDome.h"
#include "Load.h"

namespace
{
	// �w�i�I�u�W�F�N�g�̍ő吶����
	constexpr int kMaxStageBackModel = 60;
	// �w�i�I�u�W�F�N�g�̐������W�͈�
	constexpr float kCreateRange = 20000.0f;
	// �w�i�I�u�W�F�N�g�̐���Z���͈�
	constexpr float kCreateZRange = 30000.0f;
	// �w�i�I�u�W�F�N�g�̍폜����
	constexpr float kDeleteRange = 1000.0f;
	constexpr float kDeleteRangeAfterEnd = 12000.0f;
	// �w�i�I�u�W�F�N�g�̐����֎~�͈�
	constexpr float kCreateStopRange = 1500.0f;
	// �X�J�C�h�[���̊g�嗦
	constexpr float kSkyDomeScale = 1.0f;
}

Map::Map() :
	m_hStageBackModelHandleBase(-1),
	m_stageBackModel(),
	m_basePos(Game::kVecZero),
	m_deleteRange(kDeleteRange),
	m_pSkyDome(std::make_shared<SkyDome>())
{
}

Map::~Map()
{
	// �X�e�[�W���f���̍폜
	MV1DeleteModel(m_hStageBackModelHandleBase);
	// �X�e�[�W�w�i�I�u�W�F�N�g�̍폜
	for (auto& model : m_stageBackModel)
	{
		delete model;
	}
}

void Map::Init()
{
	// �X�e�[�W�w�i�I�u�W�F�N�g�ǂݍ���
	m_hStageBackModelHandleBase = MV1DuplicateModel(Load::GetInstance().GetHandle("rock"));
	// �X�J�C�h�[���̏���������
	m_pSkyDome->Init(m_basePos);
	m_pSkyDome->SetScale(kSkyDomeScale);
	// �X�e�[�W�w�i�I�u�W�F�N�g�̐���
	CreateStageBackModel();
}

void Map::Update()
{
	// �X�e�[�W�w�i�I�u�W�F�N�g�̍��W�X�V
	for (auto& model : m_stageBackModel)
	{
		if (model->GetPos().z < m_basePos.z - m_deleteRange)
		{
			model->SetPosition(GetRandStageBackPos());
		}
	}

	// �X�J�C�h�[���̍X�V����
	m_pSkyDome->Update(m_basePos);
}

void Map::Draw()
{
	// �X�e�[�W�w�i�I�u�W�F�N�g�`��
	for (auto& model : m_stageBackModel)
	{
		model->Draw();
		//DrawLine3D(m_basePos, model->GetPos(), 0xff0000);
	}

	//�X�J�C�h�[���`��
	m_pSkyDome->Draw();
}

void Map::OnStageEnd()
{
	// �w�i�I�u�W�F�N�g�̍폜�͈͂�ύX
	m_deleteRange = kDeleteRangeAfterEnd;
}

void Map::CreateStageBackModel()
{
	for (int i = 0; i < kMaxStageBackModel; i++)
	{
		// �X�e�[�W�w�i�I�u�W�F�N�g����
		m_stageBackModel.push_back(new Model(MV1DuplicateModel(m_hStageBackModelHandleBase)));
		// ���f���̃��[�J�����W�𒲐�
		m_stageBackModel.back()->SetModelLocalPos(VGet(0.0f, -144.0f, 0.0f));
		// �X�e�[�W�w�i�I�u�W�F�N�g�̊g�嗦�ݒ�
		m_stageBackModel.back()->SetScale(4.0f);

		// �w�肵���͈͂Ń����_���ɍ��W�𐶐�
		VECTOR createPos = Game::kVecZero;
		createPos.x = static_cast<float>(GetRand(static_cast<int>(kCreateRange)) - kCreateRange / 2);
		createPos.y = static_cast<float>(GetRand(static_cast<int>(kCreateRange)) - kCreateRange / 2);
		createPos.z = m_basePos.z + static_cast<float>(GetRand(static_cast<int>(kCreateZRange)) - static_cast<int>(kCreateZRange) / 2);
		// �����֎~�͈͓��̍��W�𐶐������ꍇ�A�͈͓��Ɏ��߂�
		if (createPos.x > -kCreateStopRange && createPos.x < kCreateStopRange)
		{
			createPos.x = (createPos.x > 0) ? kCreateStopRange : -kCreateStopRange;
		}
		if (createPos.y > -kCreateStopRange && createPos.y < kCreateStopRange)
		{
			createPos.y = (createPos.y > 0) ? kCreateStopRange : -kCreateStopRange;
		}

		// �X�e�[�W�w�i�I�u�W�F�N�g�̍��W�ݒ�
		m_stageBackModel.back()->SetPosition(createPos);
	}
}

VECTOR Map::GetRandStageBackPos()
{
	// ����������W
	VECTOR result = Game::kVecZero;
	// �w�肵���͈͂Ń����_���ɍ��W�𐶐�
	result.x = static_cast<float>(GetRand(static_cast<int>(kCreateRange)) - kCreateRange / 2);
	result.y = static_cast<float>(GetRand(static_cast<int>(kCreateRange)) - kCreateRange / 2);
	result.z = m_basePos.z + static_cast<float>(GetRand(static_cast<int>(kCreateZRange)));
	// �����֎~�͈͓��̍��W�𐶐������ꍇ�A�͈͓��Ɏ��߂�
	if (result.x > -kCreateStopRange && result.x < kCreateStopRange)
	{
		result.x = (result.x > 0) ? kCreateStopRange : -kCreateStopRange;
	}
	if (result.y > -kCreateStopRange && result.y < kCreateStopRange)
	{
		result.y = (result.y > 0) ? kCreateStopRange : -kCreateStopRange;
	}

	return result;
}
