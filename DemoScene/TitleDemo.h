#pragma once
#include <memory>

class TitleDemo
{
public:
	// コンストラクタ
	TitleDemo();
	// デストラクタ
	~TitleDemo();
	// 初期化
	void Init();
	// 更新
	void Update();
	// 描画
	void Draw();
	// 終了
	void End();

private:
	// デモカメラ
	std::shared_ptr<class DemoCamera> m_pDemoCamera;
	// デモプレイヤー
	std::shared_ptr<class DemoPlayer> m_pDemoPlayer;
	// デモステージ
	std::shared_ptr<class DemoMap> m_pDemoMap;
};

