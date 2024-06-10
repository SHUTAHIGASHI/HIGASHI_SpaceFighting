#pragma once
#include <unordered_map>
#include <string>

enum class SoundType
{
	transition,
	select,
	itemGet,
	boost,
	damage,
	shot,
	dead
};

enum class MusicType
{
	title,
	main,
	over
};

/// <summary>
/// SEの管理を行うシングルトンクラス
/// </summary>
class SoundManager
{
public:
	~SoundManager();
	/// <summary>
	/// SoundManager使用者はGetInstance()を通した参照からしか利用できない
	/// </summary>
	/// <returns></returns>
	static SoundManager& GetInstance()
	{
		// 唯一の実態
		static SoundManager instance;

		// それの参照を返す
		return instance;
	}

	/// <summary>
	/// 指定のサウンドを鳴らす
	/// </summary>
	/// <param name="name">サウンド名</param>
	void PlaySE(SoundType sound);
	void PlayMusic(MusicType music);
	void RestartCurrentBGM();
	int IsPlayingMusic();

	/// <summary>
	/// SEのボリュームを設定する
	/// </summary>
	/// <param name="volume"></param>
	void SetSEVolume(int volume);
	int GetSEVolume() const;

	/// <summary>
	/// BGMのボリュームを設定する
	/// </summary>
	/// <param name="volume"></param>
	void SetBGMVolume(int volume);
	int GetBGMVolume() const;

	/// <summary>
	/// BGMの現在の鳴らす音量の割合
	/// </summary>
	/// <param name="rate">音量の割合(一時的なもの0.0～1.0)</param>
	void SetBGMRate(float rate);

	// サウンドを全部一時停止する
	void StopBGM();

	// サウンド情報をセーブ
	void SaveSoundConfig();
private:
	// 変更したサウンド情報をファイルに書き込む
	struct SoundConfigInfo
	{
		char signature[5];			// "SND_"
		float version;				// 1.0
		unsigned short volumeSE;	// 0～255	
		unsigned short volumeBGM;	// 0～255
	};

	// 効果音と音楽の音量
	int m_volumeSE = 100;
	int m_volumeBGM = 100;

	// シングルトンパターンなのでコンストラクタはprivateに置く
	SoundManager();
	// コピーも代入も禁止する
	SoundManager(const SoundManager&) = delete;			// コピーコンストラクタ
	void operator = (const SoundManager&) = delete;		// 代入も禁止

	// 選択したサウンドタイプのハンドルを返す
	int GetCurrentSoundHandle(SoundType sound);
	void SetCurrentMusic(MusicType music);

	// 音データ読込
	void LoadData();

	// 音の設定を読込
	void LoadSoundConfig();

private:
	// 現在再生中の音楽
	int m_hCurrentMusic;

	// サウンドデータハンドル
	int m_hTransition;
	int m_hSelectSound;
	int m_hItemGetSound;
	int m_hBoostSound;
	int m_hDamageSound;
	int m_hShotSound;
	int m_hDeadSound;

	// ミュージックデータハンドル
	int m_hNormalMusic;
	int m_hMainMusic;
};