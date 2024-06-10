#pragma once
#include "Scene.h"
#include <memory>
#include <string>

class SceneAllClear :
    public Scene
{
public:
    SceneAllClear(SceneManager& manager);
    virtual ~SceneAllClear();

    // ������
    void Init() {}
    // �X�V
    void Update(const InputState& input);
    // �`��
    void Draw();
    // �I��
    void End() {}

private:
    // �����o�֐��|�C���^(�X�V)
    using m_tUpdateFunc = void (SceneAllClear::*) (const InputState& input);
    m_tUpdateFunc m_updateFunc = nullptr;
    void StartUpdate(const InputState& input);      // �J�n���̍X�V����
    void NormalUpdate(const InputState& input);     // �ʏ�̍X�V����
    void EndUpdate(const InputState& input);        // �I�����̍X�V����

private: // �ϐ�
    // �w�i�摜
    int m_hBgImg;
    // �n�C�X�R�A
    int m_highScore;
    std::string m_rank;
    // �X�R�A�`��p���W
    float m_scorePosY;
    // �I�����j���[
    std::shared_ptr<class SelectMenuBase> m_pSelectMenu;
};

