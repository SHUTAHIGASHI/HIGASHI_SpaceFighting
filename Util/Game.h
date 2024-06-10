#pragma once
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "InputState.h"

namespace Game
{
	// システム基本設定 //
#ifdef _DEBUG
	// ウィンドウモード設定
	constexpr bool kWindowMode = true;
#else
	constexpr bool kWindowMode = false;
#endif
	// ウィンドウ名
	const char* const kTitleText = "SpaceFighting";
	// ウィンドウサイズ
	constexpr int kScreenWidth = 1920;
	constexpr int kScreenHeight = 1080;
	constexpr int kScreenWidthHalf = kScreenWidth / 2;
	constexpr int kScreenHeightHalf = kScreenHeight / 2;
	constexpr int kScreenWidthTriple = kScreenWidth * 3;
	constexpr int kScreenHeightTriple = kScreenHeight * 3;

	// ゲームで使う色コード
	constexpr int kColorOrange = 0xff7800;
	constexpr int kColorRed = 0xdc143c;
	constexpr int kColorGreen = 0xadff2f;
	constexpr int kColorBlue = 0x4f7fee;
	constexpr int kColorBlack = 0x0a192c;
	constexpr int kColorWhite = 0xffffff;
	constexpr int kColorGray = 0xa9a9a9;

	// ０のベクトル
	static VECTOR kVecZero = VGet(0.0f, 0.0f, 0.0f);

	// ステージデータファイル
	const char* const kStageDataFileName = "Data/StageData.csv";

	/* フォント */
	// フォントファイル
	const char* const kFontFileName = "Data/fontData/851Gkktt_005.ttf";
	// フォントサイズ
	constexpr int kFontSize = 40;

	// カラーモード
	constexpr int kColorDepth = 32;
	
	// ゲームの基本設定 //
	constexpr int kBoostLevelMax = 3;
	// プレイヤーの基本設定
	constexpr float kChipSize = 32.0f;

	// 基本移動速度
	constexpr float kMoveSpeed = 12.0f;

	// ゲームのステージサイズ
	constexpr int kStageWidth = 1400;
	constexpr int kStageHeight = 700;

	// パーティクル用
	constexpr int kParticleNum = 8000;
}