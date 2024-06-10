#pragma once

// #includeではなくプロトタイプ宣言を使用する
class SceneManager; // シーンマネージャー
class InputState;	// 入力ステート
// この時点ではSceneManagerとInputStateは
// ポインタもしくは参照としてしか扱えない

/// <summary>
/// シーン基底クラス
/// </summary>

class Scene
{
public:
	Scene(SceneManager& manager) : m_Manager(manager) {}
	virtual ~Scene() {};

	// シーンを初期化する
	virtual void Init() = 0;
	// シーンを更新する
	virtual void Update(const InputState& input) = 0;
	// シーンを描画する
	virtual void Draw() = 0;
	// シーンを終了する
	virtual void End() = 0;
protected:
	SceneManager& m_Manager; // シーンマネージャーへの参照
};

