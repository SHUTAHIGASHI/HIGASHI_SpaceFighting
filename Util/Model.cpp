#include "Model.h"

Model::Model(int handle):
	m_hModel(handle),
	m_AnimNo(-1),
	m_AnimIndex(0),
	m_AnimTotalTime(0.0f),
	m_AnimPlayTime(0.0f),
	m_localPos(Game::kVecZero),
	m_pos(Game::kVecZero),
	m_rotation(Game::kVecZero)
{
}

Model::~Model()
{
	// モデルハンドル削除
	MV1DeleteModel(m_hModel);
}

void Model::Update()
{
	// 再生時間を進める
	m_AnimPlayTime += 1.0f;

	// 再生時間がアニメーションの総再生時間に達したら再生時間を０に戻す
	if (m_AnimPlayTime > m_AnimTotalTime)
	{
		m_AnimPlayTime = 0.0f;
	}

	// 再生時間をセットする
	MV1SetAttachAnimTime(m_hModel, m_AnimIndex, m_AnimPlayTime);
}

void Model::Draw()
{
	// モデルの座標を設定
	MV1SetPosition(m_hModel, VAdd(m_pos, m_localPos));
	// モデル描画
	MV1DrawModel(m_hModel);
}

void Model::SetModelLocalPos(VECTOR pos)
{
	// モデルのローカル座標を設定
	m_localPos = pos;
}

void Model::SetPosition(VECTOR pos)
{
	// 座標設定
	m_pos = pos;
	MV1SetPosition(m_hModel, VAdd(m_pos, m_localPos));
}

void Model::SetScale(float size)
{
	// 拡大率設定
	MV1SetScale(m_hModel, VGet(size, size, size));
	m_localPos = VScale(m_localPos, size);
}

void Model::SetMotionType(int motionType)
{
	// 実行するモーション設定
	if (m_AnimNo == motionType) return;
	m_AnimNo = motionType;
	m_AnimIndex = MV1AttachAnim(m_hModel, motionType, -1);
	m_AnimTotalTime = MV1GetAttachAnimTotalTime(m_hModel, m_AnimIndex);
	m_AnimPlayTime = 0.0f;
}

void Model::SetDirectionAxisY(VECTOR lookPos, float rota)
{
	// 方向設定
	lookPos.y -= 500.0f;
	MV1SetRotationZYAxis(m_hModel, VGet(0.0f, 0.0f, 1.0f), VSub(m_pos, lookPos), rota);
}

void Model::SetDirectionAxisZ(VECTOR lookPos, float rota)
{
	// 上下は固定
	lookPos.y = m_pos.y;
	// 方向設定
	MV1SetRotationZYAxis(m_hModel, VSub(m_pos, lookPos), VGet(0.0f, 1.0f, 0.0f), rota);
}

void Model::SetRotation(VECTOR rota)
{
	// 回転方向設定
	m_rotation = rota;
	MV1SetRotationXYZ(m_hModel, m_rotation);
}

void Model::SetOutline(int materialIndex, float width, float r, float g, float b, float a)
{
	// 3Dオブジェクトのアウトラインを描画
	MV1SetMaterialOutLineWidth(m_hModel, materialIndex, width);
	MV1SetMaterialOutLineColor(m_hModel, materialIndex, GetColorF(r, g, b, a));
}
