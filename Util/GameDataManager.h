#pragma once
#include <unordered_map>
#include "Stage.h"

namespace ScoreInfo
{
	// �X�R�A���
	static const int ROCK_SCORE = 30;
	static const int ENEMY_SCORE = 100;
	static const int BOSS_SCORE = 5000;
	static const int STAGECLEAR_SCORE = 10000;
	static const int HEAL_SCORE = 50;
	static const int MISS_SCORE = 100;
	static const int TIME_SCORE = 10;
}

/// <summary>
/// SE�̊Ǘ����s���V���O���g���N���X
/// </summary>
class GameDataManager
{
public:
	~GameDataManager();
	/// <summary>
	/// SoundManager�g�p�҂�GetInstance()��ʂ����Q�Ƃ��炵�����p�ł��Ȃ�
	/// </summary>
	/// <returns></returns>
	static GameDataManager& GetInstance()
	{
		// �B��̎���
		static GameDataManager instance;
		// ����̎Q�Ƃ�Ԃ�
		return instance;
	}

	/* �X�e�[�W�֌W���� */
	// ���݂̃X�e�[�W�ԍ��擾
	int GetCurrentStageNum() { return m_currentStageNum + 1; }
	// �X�e�[�W�N���A����
	void OnStageClear();
	// ���̃X�e�[�W��
	void OnNextStage();
	// �X�e�[�W���Z�b�g
	void ResetStage() { m_currentStageNum = 0; }
	// �X�e�[�W�f�[�^�擾
	StageData GetStageData(int stageNum)
	{
		return m_stageData[stageNum];
	}
	StageData GetStageData()
	{
		return m_stageData[m_currentStageNum];
	}

	/* �|�C���g�֌W���� */
	void SetGamePoint(int point) { m_score = point; }
	// �|�C���g���Z�b�g
	void ResetPoint();
	// �|�C���g����
	void AddPoint(int point);
	// �|�C���g����
	void SubPoint(int point);

	// ����v���C����Z�b�g
	void SetFirstPlay(bool isFirstPlay) { m_isFirstPlay = isFirstPlay; }

	/* �e����擾 */
	// �|�C���g�擾
	int GetPoint() { return m_score; }
	// �����N�擾
	std::string GetRank() { ConvertRank(); return m_rank; }
	// �S�X�e�[�W�N���A����
	const bool IsAllStageClear() { return m_isAllStageClear; }
	// ����v���C����
	const bool IsFirstPlay() { return m_isFirstPlay; }

private:
	// �V���O���g���p�^�[���Ȃ̂ŃR���X�g���N�^��private�ɒu��
	GameDataManager();
	// �R�s�[��������֎~����
	GameDataManager(const GameDataManager&) = delete;		// �R�s�[�R���X�g���N�^
	void operator = (const GameDataManager&) = delete;		// ������֎~

	// �X�e�[�W�f�[�^�ǂݍ���
	void LoadStageData();

	// �X�R�A�������N�ɕϊ�
	void ConvertRank();

private:
	// �X�R�A�f�[�^
	int m_score;
	std::string m_rank;
	// �X�e�[�W�f�[�^
	std::unordered_map<int, StageData> m_stageData;
	// ���݂̃X�e�[�W�ԍ�
	int m_currentStageNum;
	// �X�e�[�W�N���A���J�E���g
	int m_stageClearCount;
	// �S�X�e�[�W�N���A����
	bool m_isAllStageClear;
	// ����v���C����
	bool m_isFirstPlay;
};