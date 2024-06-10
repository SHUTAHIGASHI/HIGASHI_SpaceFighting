#pragma once
#include <memory>

class TitleDemo
{
public:
	// �R���X�g���N�^
	TitleDemo();
	// �f�X�g���N�^
	~TitleDemo();
	// ������
	void Init();
	// �X�V
	void Update();
	// �`��
	void Draw();
	// �I��
	void End();

private:
	// �f���J����
	std::shared_ptr<class DemoCamera> m_pDemoCamera;
	// �f���v���C���[
	std::shared_ptr<class DemoPlayer> m_pDemoPlayer;
	// �f���X�e�[�W
	std::shared_ptr<class DemoMap> m_pDemoMap;
};

