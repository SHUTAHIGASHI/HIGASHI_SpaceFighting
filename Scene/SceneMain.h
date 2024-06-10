#pragma once
#include "Scene.h"
#include <memory>

class SceneMain :
    public Scene
{
public:
    // �R���X�g���N�^
    SceneMain(SceneManager& manager);
    // �f�X�g���N�^
    virtual ~SceneMain();

    // �V�[��������
    void Init();
    // �X�V����
    void Update(const InputState& input);
    // �`�揈��
    void Draw();
    // �I��
    void End();

private:
    // �X�e�[�W�I������̃`�F�b�N
	void CheckStageEnd();
    // �Q�[���N���A�V�[���ֈڍs
    void ChangeNextScene();

    // �V�[���I������
    void OnSceneEnd() { m_isEnd = true; }
    // �Q�[���X�^�[�g��
    void OnGameStart();
    // �Q�[���I����
    void OnGameEnd();

    // �����o�֐��|�C���^(�X�V)
    using m_tUpdateFunc = void (SceneMain::*) (const InputState& input);
    m_tUpdateFunc m_updateFunc = nullptr;
    void NormalUpdate(const InputState& input);     // �ʏ�̍X�V����
    void StartUpdate(const InputState& input);      // �Q�[���X�^�[�g���̍X�V����
    void EndUpdate(const InputState& input);        // �Q�[���I�[�o�[���̍X�V����
    void ResultUpdate(const InputState& input);     // ���U���g�`�掞�̍X�V����

    // �����o�֐��|�C���^(�`��)
    using m_tDrawFunc = void (SceneMain::*) ();
    m_tDrawFunc m_drawFunc = nullptr;
    void NormalDraw();      // �ʏ�̕`�揈��
    void StartDraw();       // �V�[���J�n���̕`�揈��
    void EndDraw();         // �V�[���I�����̕`�揈��

private:
    std::shared_ptr<class CameraController> m_pCamera;     // �J����
    std::shared_ptr<class Player> m_pPlayer;               // �v���C���[
    std::shared_ptr<class StageManager> m_pStage;          // �X�e�[�W
    std::shared_ptr<class UiManager> m_pUi;                // UI

private:
    // �V�[�����I�����邩�ǂ���
    bool m_isEnd;
    // ��������`��t���O
    bool m_isDrawOperation;
    // �Q�[���I�������̎��s�t���[����
    int m_endTimeCount;
};

