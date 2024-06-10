#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include "Game.h"

class StageObjectManager
{
public:
	// コンストラクタ
	StageObjectManager();
	// デストラクタ
	virtual ~StageObjectManager();

	// ステージ管理クラスセット
	void SetStage(class Stage* stage) { m_pStage = stage; }
	// UIポインタセット
	void SetUi(class UiManager* ui) { m_pUi = ui; }

	// 初期化
	void Init();
	// 更新
	void Update();
	// 描画
	void Draw();
	// プレイヤーとの当たり判定
	void CheckPlayerCollision(class Player* player);
	// 指定オブジェクト生成
	void CreateObject(std::string objName, VECTOR pos);

	// プレイヤー座標設定
	void SetPlayerPos(VECTOR pos) { m_playerPos = pos; }

	// オブジェクト情報取得
	std::vector<std::shared_ptr<class StageObjectBase>> GetStageObject() const { return m_pObjects; }

private:
	// オブジェクト削除
	void DeleteDisableObject();

private:
	// プレイヤー座標
	VECTOR m_playerPos;
	// オブジェクト配列
	std::vector<std::shared_ptr<class StageObjectBase>> m_pObjects;
	// アイテム生成座標
	std::vector<VECTOR> m_itemCreatePos;

private:
	// ステージ管理
	class Stage* m_pStage;
	// UI
	class UiManager* m_pUi;
};

