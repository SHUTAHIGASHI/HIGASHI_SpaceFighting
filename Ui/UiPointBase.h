#pragma once
#include "Game.h"
#include <string>

namespace
{
	// 文字移動速度
	constexpr float kTextMoveSpeed = 2.0f;

	// 文字描画時間
	constexpr int kDrawDelay = 30;
}

class UiPointBase
{
public:
	// コンストラクタ
	UiPointBase();
	// デストラクタ
	virtual ~UiPointBase();

	// 更新
	virtual void Update() = 0;
	// 描画
	virtual void Draw();

	// UIに表示する情報の設定
	void SetUIInfo(int tData);

	// UI表示を終了するかどうか
	bool IsDelete() { return m_isDelete; }

protected:
	// 画像描画位置
	float m_posX;
	float m_posY;
protected:
	// 有効かどうか
	bool m_isDelete;
	// 文字停止カウント
	int m_countDelay;
	// 描画する数値データ
	int m_numData;
	// 描画する文字データ
	std::string m_textData;
	// 描画する文字の色
	int m_textColor;
};

