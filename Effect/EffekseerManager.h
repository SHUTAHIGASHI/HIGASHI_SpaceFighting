#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include "Game.h"
#include "EffectBase.h"

class EffectBase;

/// <summary>
/// エフェクシアの管理を行うシングルトンクラス
/// </summary>
class EffekseerManager
{
public:
	// デストラクタ
	~EffekseerManager();
	// EffekseerManager使用者はGetInstance()を通した参照からしか利用できない
	static EffekseerManager& GetInstance()
	{
		// 唯一の実態
		static EffekseerManager instance;
		// それの参照を返す
		return instance;
	}

	// 更新
	void Update();
	// 描画
	void Draw();
	// 各種エフェクトの生成
	void CreateEffect(EffectType type, bool loop, class ObjectBase* obj);
	void CreateEffectAndSetScale(EffectType type, bool loop, class ObjectBase* obj, float scale);
	void CreateEffect(EffectType type, bool loop, VECTOR pos);
	// すべてのエフェクトの停止
	void StopAllEffect();
	// 選んだエフェクトの停止
	void StopEffect(EffectType type);
	void StopEffectTargetObj(class ObjectBase* obj);
	// 無効化されているエフェクトの削除
	void DeleteDisableEffect();
	// 指定したエフェクトの座標を設定
	void SetEffectPosition(EffectType type, VECTOR pos);
	// 指定したエフェクトの回転を設定
	void SetEffectRota(EffectType type, VECTOR rota);
	// 指定したエフェクトが再生中かどうか
	bool IsPlayingEffect(EffectType type);
	// 指定したエフェクト取得
	EffectBase* GetEffect(EffectType type);

private:
	// エフェクトの読み込み
	void LoadData();

private:
	// シングルトンパターンなのでコンストラクタはprivateに置く
	EffekseerManager();
	// コピーも代入も禁止する
	EffekseerManager(const EffekseerManager&) = delete;		// コピーコンストラクタ
	void operator = (const EffekseerManager&) = delete;		// 代入も禁止

private:
	// エフェクトハンドルのマップ
	std::unordered_map<EffectType, int> m_EffectHandle;
	// エフェクト
	std::vector<std::shared_ptr<EffectBase>> m_pEffects;
};