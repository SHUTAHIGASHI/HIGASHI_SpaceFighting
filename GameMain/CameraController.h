#pragma once
#include "Game.h"
#include <deque>
#include <memory>

class Camera;
class Player;

// カメラ
class CameraController
{
public:
	// コンストラクタ
	CameraController();
	// デストラクタ
	virtual ~CameraController();

	// プレイヤーのポインタセット
	void SetPlayer(Player* player) { m_pPlayer = player; }

	// カメラ初期化
	void Init();
	// カメラ更新
	void Update();
	// カメラ位置更新
	void UpdateCameraPos();
	// カメラの揺れ有効時の更新
	void UpdateCameraQuake();
	// シーンメイン開始時の演出用更新処理
	void StartUpdate();
	// シーンメイン終了時の演出用更新処理
	void EndUpdate();
	// シーンメイン開始時の演出用更新処理
	void ResultUpdate();
	// 画面揺れ
	void OnScreenQuake();
	// ポジションのGetter
	const VECTOR& GetPos() const { return m_pos; }
	// カメラ回転数取得
	const float GetRotaAxisY() const { return m_rotaAxisY; }
	// スタート演出終了判定
	const bool IsCameraSet() const { return m_isCameraSet; }

private:
	// カメラ座標
	VECTOR m_dir;
	VECTOR m_pos;
	VECTOR m_lookPos;
	// カメラ視野角
	float m_fov;
	// カメラ回転
	float m_rotaAxisY;
	// 開始演出時のカメラY軸
	float m_startPosY;
	// 画面揺れの大きさ
	int m_screenQuakeScale;
	// スタート演出終了判定
	bool m_isCameraSet;
	// カメラ
	std::shared_ptr<Camera> m_pCamera;

private:
	// プレイヤーポインタ
	Player* m_pPlayer;
};
