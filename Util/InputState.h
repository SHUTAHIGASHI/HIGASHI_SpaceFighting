#pragma once
#include <EffekseerForDXLib.h>

enum class InputType
{
	// システム系ボタン
	all,	// 全入力
	pause,	// ポーズ
	select,	// 決定
	back,	// 戻る
	space,	// スペース
	up,		// 上
	down,	// 下
	right,	// 右
	left,	// 左

	// ゲーム系ボタン
	lookUp,			// 視点上
	lookDown,		// 視点下
	lookRight,		// 視点右
	lookLeft,		// 視点左
	moveForward,	// 前方移動
	moveBehind,		// 後方移動
	moveRight,		// 右移動
	moveLeft,		// 左移動
	avoid,			// 回避
	shot,			// 攻撃
};

struct MouseInputState
{
	// マウスカーソル座標
	int x = 0;
	int y = 0;
	// マウス入力状態
	int keyMouseState = 0;
	int lastKeyMouseState = 0;
	// マウスホイール入力状態
	int mouseWheelState = 0;
};

class InputState
{
public:
	// トリガー判定
	bool IsTriggered(InputType type)const;
	// 押し込み判定
	bool IsPressed(InputType type)const;
	// 入力状態更新
	void Update();
	// マウスホイールの入力状態取得
	bool IsMouseWheel(bool isPlus)const;
	// パッドスティックの入力状態取得
	bool IsStickInput(int stickType, bool isPlus)const;
	// マウスカーソルの座標取得
	int GetMousePosX()const { return mouseState.x; }
	int GetMousePosY()const { return mouseState.y; }

private:
	int lastPadState = 0;
	int padState = 0;
	char keyState[256];
	char lastKeyState[256];
	// マウスの入力状態
	MouseInputState mouseState;
	// パッドの入力状態
	DINPUT_JOYSTATE dinputPadState;
};