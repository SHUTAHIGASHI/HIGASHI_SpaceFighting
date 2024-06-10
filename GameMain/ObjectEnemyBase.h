#pragma once
#include "StageObjectBase.h"
#include "Game.h"
#include <vector>
#include <memory>

namespace
{
    // �g�嗦
    constexpr float kScale = 0.3f;
    // �V���b�g�����Ԋu
    constexpr int kShotDelay = 60 * 4;
    // �V���b�g�𐶐����鋗��
    constexpr float kShotDistanceMax = 9000.0f;
    constexpr float kShotDistanceMin = 1000.0f;
    // �V���b�g�̑��x
    constexpr float kShotSpeed = 48.0f;
}

class ObjectEnemyBase :
    public StageObjectBase
{
public:
    // �R���X�g���N�^
    ObjectEnemyBase();
    // �f�X�g���N�^
    virtual ~ObjectEnemyBase();
    // �X�V
    virtual void Update();

    // �^�[�Q�b�g���W�ݒ�
    void SetTargetPos(VECTOR targetPos) { m_targetPos = targetPos; }

    // �V���b�g�擾
    std::vector<std::shared_ptr<class Shot>> GetShots() const { return m_pShots; }

protected:
    // �V���b�g����
    void CreateShot();
    // �s�v�ȃV���b�g�폜
    void DeleteDisableShot();
    // �S�V���b�g�폜
    void DeleteAllShot();

protected:
    // �^�[�Q�b�g���W
    VECTOR m_targetPos;
    // �V���b�g�����Ԋu
    int m_shotDelay;
    // �V���b�g
    std::vector<std::shared_ptr<class Shot>> m_pShots;
};

