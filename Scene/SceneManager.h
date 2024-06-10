#pragma once
#include <deque>

//プロトタイプ宣言
class Scene;
class SceneFade;
class InputState;

//シーン監視クラス
class SceneManager
{
public:

	/// <summary>
	/// シーンの切り替えを行う
	/// </summary>
	/// <param name="scene">切り替えたい次シーンのアドレス</param>
	void ChangeScene(Scene* scene);

	/// <summary>
	/// シーンを上に積む
	/// Updateでは全てのシーンが更新される
	/// </summary>
	/// <param name="scene"></param>
	void PushSceneAllUpdate(Scene* scene);
	/// <summary>
	/// シーンを上に積む
	/// Updateで実行されるのは上につまれたシーンのみ
	/// </summary>
	/// <param name="scene"></param>
	void PushScene(Scene* scene);
	/// <summary>
	/// 一番上のシーンを削除する
	/// ただし、その結果スタックが空になることのないように
	/// </summary>
	void PopScene();
	/// <summary>
	/// 各シーンのUpdateを行う
	/// </summary>
	/// <param name="input">入力ステート</param>
	void Update(const InputState& input);
	/// <summary>
	/// 各シーンの描画を行う
	/// </summary>
	void Draw();
	/// <summary>
	/// シーンをすべて削除し、すべての処理を終了させる
	/// </summary>
	void GameEnd();
	/// <summary>
	/// ゲームの終了するかどうかを返す
	/// </summary>
	bool IsGameEnd() { return m_isEnd; }

private:
	// シーンを全てクリアする
	void ClearAllScene();

	//メンバ関数ポインタ
	using m_tUpdateFunc = void (SceneManager::*) (const InputState& input);
	m_tUpdateFunc m_updateFunc = nullptr;
	// 通常の更新処理
	void SceneNormalUpdate(const InputState& input);
	// フェード処理
	void FadeCloseUpdate(const InputState& input);
	void FadeOpenUpdate(const InputState& input);

private:
	//今実行中のシーンを切り替えていきたいので	
	//参照ではなくポインタとして宣言
	std::deque<Scene*> m_Scene;
	// 次に実行するシーンを一時的に保存する
	Scene* m_NextScene;
	// フェード処理のシーン宣言
	SceneFade* m_Fade;

	// 処理を終了するか
	bool m_isEnd = false;

	// 更新処理を全て行うかどうか
	bool m_isAllUpdate = false;
};

