#include "GameDataManager.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace
{
	// �|�C���g�ő�l
	constexpr int kScoreMax = 999999;
}

GameDataManager::~GameDataManager()
{
}

void GameDataManager::OnStageClear()
{
	// �X�e�[�W�N���A�J�E���g�𑝂₷
	m_stageClearCount++;
	if (m_stageClearCount >= static_cast<int>(m_stageData.size()))
	{
		// �S�X�e�[�W�N���A����
		m_isAllStageClear = true;
	}
}

void GameDataManager::OnNextStage()
{
	// ���̃X�e�[�W��
	m_currentStageNum++;
	if (m_currentStageNum >= static_cast<int>(m_stageData.size()))
	{
		// �ŏI�X�e�[�W�̏ꍇ�͍ŏ��̃X�e�[�W��
		m_currentStageNum = 0;
	}
}

void GameDataManager::ResetPoint()
{
	// �|�C���g���Z�b�g
	m_score = 0;
}

void GameDataManager::AddPoint(int point)
{
	// �|�C���g���Z
	m_score += point;
	if (m_score > kScoreMax)
	{
		// �|�C���g�ő�l�𒴂����ꍇ�͍ő�l�ɐݒ�
		m_score = kScoreMax;
	}
}

void GameDataManager::SubPoint(int point)
{
	// �|�C���g���Z
	m_score -= point;
	if (m_score < 0)
	{
		// �|�C���g���}�C�i�X�ɂȂ����ꍇ��0�ɐݒ�
		m_score = 0;
	}
}

GameDataManager::GameDataManager() :
	m_score(0),
	m_rank(""),
	m_stageData(),
	m_currentStageNum(0),
	m_stageClearCount(0),
	m_isAllStageClear(false),
	m_isFirstPlay(true)
{
	// �X�e�[�W�f�[�^�ǂݍ���
	LoadStageData();
}

void GameDataManager::LoadStageData()
{
	std::ifstream file(Game::kStageDataFileName);	// CSV�t�@�C�����J��
	std::string line;								// 1�s���̕�����
	std::vector<std::string> tempStageData;			// �ꎞ�X�e�[�W�f�[�^
	int lineCount = 0;								// �s��
	int strFirst = 0, strLast = 0;					// ������̍ŏ��ƍŌ�

	// �t�@�C�����J�������m�F
	if (file.is_open())
	{
		// �t�@�C������s��ǂݍ���
		while (getline(file, line)) 
		{
			// 1�s�ڂ̓X�L�b�v
			if (lineCount != 0)
			{
				// ��؂蕶���ŕ�����𕪊�
				strFirst = 0, strLast = 0;
				// �ꎞ�X�e�[�W�f�[�^���Z�b�g
				tempStageData.clear();
				for (int i = 0; i < line.size(); i++)
				{
					if (line[i] == ',')
					{
						// ��؂蕶���ŕ�����𕪊�
						strLast = i;
						// ��������擾
						std::string str;
						for (int j = strFirst; j < strLast; j++)
						{
							// ��������擾
							str += line[j];
						}
						// �ꎞ�X�e�[�W�f�[�^�ɒǉ�
						tempStageData.push_back(str);
						// ���̕�����
						strFirst = strLast + 1;
					}
				}
				
				// �X�e�[�W�f�[�^�ɕϊ�
				StageData stageData;
				stageData.isBoss = static_cast<bool>(std::stoi(tempStageData[0]));
				stageData.bossHp = std::stoi(tempStageData[1]);
				stageData.rockNum = std::stoi(tempStageData[2]);
				stageData.enemyNum = std::stoi(tempStageData[3]);
				stageData.length = static_cast<float>(std::stoi(tempStageData[4]));
				// �X�e�[�W�f�[�^�ɒǉ�
				m_stageData[lineCount - 1] = stageData;
			}
			// �s�����J�E���g
			lineCount++;
		}
		// �t�@�C�������
		file.close();
	}
	else
	{
		// �X�e�[�W�f�[�^�ǂݍ��݃G���[����
		MessageBox(NULL, "�X�e�[�W�f�[�^�ǂݍ��ݎ��s", "", MB_OK);
	}
}

void GameDataManager::ConvertRank()
{
	// �����N�ϊ�
	if (m_score >= 12000)
	{
		m_rank = "S!!";
	}
	else if (m_score >= 6000)
	{
		m_rank = "A";
	}
	else if (m_score >= 3000)
	{
		m_rank = "B";
	}
	else if (m_score >= 1000)
	{
		m_rank = "C";
	}
	else if (m_score >= 500)
	{
		m_rank = "D";
	}
	else
	{
		m_rank = "E";
	}
}
