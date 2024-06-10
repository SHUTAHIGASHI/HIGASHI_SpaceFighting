#pragma once
#include "Game.h"

class Model
{
public:
	// コンストラクタ
	Model(int handle);
	// デストラクタ
	virtual ~Model();

	// 更新(アニメーション使用時のみ)
	void Update();
	// 描画
	void Draw();

	// 各情報の設定
	void SetModelLocalPos(VECTOR pos);					// モデルのローカル座標調整
	void SetPosition(VECTOR pos);								// 座標
	void SetScale(float size);									// 拡大
	void SetMotionType(int);									// モーションタイプ
	void SetDirectionAxisY(VECTOR lookPos, float rota);			// 向き
	void SetDirectionAxisZ(VECTOR lookPos, float rota);			// 向き
	void SetRotation(VECTOR);									// 回転
	void SetOutline(int materialIndex, float width, float r, float g, float b, float a);	// 輪郭線

	/* getter */
	VECTOR GetPos() { return m_pos; }// 座標

private:
	// モデルハンドル
	int m_hModel;

	// アニメーション
	int m_AnimNo;
	int m_AnimIndex;
	float m_AnimTotalTime;
	float m_AnimPlayTime;

	// モデルのローカル座標の調整座標
	VECTOR m_localPos;
	// 表示位置座標
	VECTOR m_pos;
	// 回転
	VECTOR m_rotation;
};

