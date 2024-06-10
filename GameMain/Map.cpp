#include "Map.h"
#include "Model.h"
#include "SkyDome.h"
#include "Load.h"

namespace
{
	// 背景オブジェクトの最大生成数
	constexpr int kMaxStageBackModel = 60;
	// 背景オブジェクトの生成座標範囲
	constexpr float kCreateRange = 20000.0f;
	// 背景オブジェクトの生成Z軸範囲
	constexpr float kCreateZRange = 30000.0f;
	// 背景オブジェクトの削除距離
	constexpr float kDeleteRange = 1000.0f;
	constexpr float kDeleteRangeAfterEnd = 12000.0f;
	// 背景オブジェクトの生成禁止範囲
	constexpr float kCreateStopRange = 1500.0f;
	// スカイドームの拡大率
	constexpr float kSkyDomeScale = 1.0f;
}

Map::Map() :
	m_hStageBackModelHandleBase(-1),
	m_stageBackModel(),
	m_basePos(Game::kVecZero),
	m_deleteRange(kDeleteRange),
	m_pSkyDome(std::make_shared<SkyDome>())
{
}

Map::~Map()
{
	// ステージモデルの削除
	MV1DeleteModel(m_hStageBackModelHandleBase);
	// ステージ背景オブジェクトの削除
	for (auto& model : m_stageBackModel)
	{
		delete model;
	}
}

void Map::Init()
{
	// ステージ背景オブジェクト読み込み
	m_hStageBackModelHandleBase = MV1DuplicateModel(Load::GetInstance().GetHandle("rock"));
	// スカイドームの初期化処理
	m_pSkyDome->Init(m_basePos);
	m_pSkyDome->SetScale(kSkyDomeScale);
	// ステージ背景オブジェクトの生成
	CreateStageBackModel();
}

void Map::Update()
{
	// ステージ背景オブジェクトの座標更新
	for (auto& model : m_stageBackModel)
	{
		if (model->GetPos().z < m_basePos.z - m_deleteRange)
		{
			model->SetPosition(GetRandStageBackPos());
		}
	}

	// スカイドームの更新処理
	m_pSkyDome->Update(m_basePos);
}

void Map::Draw()
{
	// ステージ背景オブジェクト描画
	for (auto& model : m_stageBackModel)
	{
		model->Draw();
		//DrawLine3D(m_basePos, model->GetPos(), 0xff0000);
	}

	//スカイドーム描画
	m_pSkyDome->Draw();
}

void Map::OnStageEnd()
{
	// 背景オブジェクトの削除範囲を変更
	m_deleteRange = kDeleteRangeAfterEnd;
}

void Map::CreateStageBackModel()
{
	for (int i = 0; i < kMaxStageBackModel; i++)
	{
		// ステージ背景オブジェクト生成
		m_stageBackModel.push_back(new Model(MV1DuplicateModel(m_hStageBackModelHandleBase)));
		// モデルのローカル座標を調整
		m_stageBackModel.back()->SetModelLocalPos(VGet(0.0f, -144.0f, 0.0f));
		// ステージ背景オブジェクトの拡大率設定
		m_stageBackModel.back()->SetScale(4.0f);

		// 指定した範囲でランダムに座標を生成
		VECTOR createPos = Game::kVecZero;
		createPos.x = static_cast<float>(GetRand(static_cast<int>(kCreateRange)) - kCreateRange / 2);
		createPos.y = static_cast<float>(GetRand(static_cast<int>(kCreateRange)) - kCreateRange / 2);
		createPos.z = m_basePos.z + static_cast<float>(GetRand(static_cast<int>(kCreateZRange)) - static_cast<int>(kCreateZRange) / 2);
		// 生成禁止範囲内の座標を生成した場合、範囲内に収める
		if (createPos.x > -kCreateStopRange && createPos.x < kCreateStopRange)
		{
			createPos.x = (createPos.x > 0) ? kCreateStopRange : -kCreateStopRange;
		}
		if (createPos.y > -kCreateStopRange && createPos.y < kCreateStopRange)
		{
			createPos.y = (createPos.y > 0) ? kCreateStopRange : -kCreateStopRange;
		}

		// ステージ背景オブジェクトの座標設定
		m_stageBackModel.back()->SetPosition(createPos);
	}
}

VECTOR Map::GetRandStageBackPos()
{
	// 生成する座標
	VECTOR result = Game::kVecZero;
	// 指定した範囲でランダムに座標を生成
	result.x = static_cast<float>(GetRand(static_cast<int>(kCreateRange)) - kCreateRange / 2);
	result.y = static_cast<float>(GetRand(static_cast<int>(kCreateRange)) - kCreateRange / 2);
	result.z = m_basePos.z + static_cast<float>(GetRand(static_cast<int>(kCreateZRange)));
	// 生成禁止範囲内の座標を生成した場合、範囲内に収める
	if (result.x > -kCreateStopRange && result.x < kCreateStopRange)
	{
		result.x = (result.x > 0) ? kCreateStopRange : -kCreateStopRange;
	}
	if (result.y > -kCreateStopRange && result.y < kCreateStopRange)
	{
		result.y = (result.y > 0) ? kCreateStopRange : -kCreateStopRange;
	}

	return result;
}
