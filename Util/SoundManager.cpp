#include "SoundManager.h"
#include "Game.h"
#include <cassert>

namespace
{
	constexpr char sound_file_signature[] = "SND_";
	constexpr char sound_config_file_path[] = "./sound.conf";
	constexpr float sound_config_version = 1.0f;
}

void SoundManager::LoadSoundConfig()
{
	SoundConfigInfo conf = {};
	FILE* fp = nullptr;
	fopen_s(&fp, sound_config_file_path, "rb");
	if (fp)
	{
		fread(&conf, sizeof(conf), 1, fp);
		fclose(fp);
		m_volumeBGM = conf.volumeBGM;
		m_volumeSE = conf.volumeSE;
	}
}

void SoundManager::SaveSoundConfig()
{
	SoundConfigInfo conf = {};
	memcpy_s(conf.signature, sizeof(conf.signature), sound_file_signature, sizeof(sound_file_signature));

	conf.version = sound_config_version;
	conf.volumeSE = m_volumeSE;
	conf.volumeBGM = m_volumeBGM;

	FILE* fp = nullptr;
	fopen_s(&fp, sound_config_file_path, "wb");
	if (fp)
	{
		fwrite(&conf, sizeof(conf), 1, fp);
		fclose(fp);
	}
}

SoundManager::~SoundManager()
{
	if (CheckSoundMem(m_hCurrentMusic) == 1)
	{
		StopSoundMem(m_hCurrentMusic);
	}

	SaveSoundConfig();

	DeleteSoundMem(m_hCurrentMusic);
	DeleteSoundMem(m_hTransition);
	DeleteSoundMem(m_hSelectSound);
	DeleteSoundMem(m_hItemGetSound);
	DeleteSoundMem(m_hBoostSound);
	DeleteSoundMem(m_hDamageSound);
	DeleteSoundMem(m_hShotSound);
	DeleteSoundMem(m_hDeadSound);
	DeleteSoundMem(m_hMainMusic);
	DeleteSoundMem(m_hNormalMusic);
}

void SoundManager::PlaySE(SoundType sound)
{
	int volumePal = (255 / 100) * m_volumeSE;
	ChangeVolumeSoundMem(m_volumeSE, GetCurrentSoundHandle(sound));
	PlaySoundMem(GetCurrentSoundHandle(sound), DX_PLAYTYPE_BACK);
}

void SoundManager::PlayMusic(MusicType music)
{
	SetCurrentMusic(music);
	int volumePal = (255 / 100) * m_volumeBGM;
	ChangeVolumeSoundMem(volumePal, m_hCurrentMusic);
	PlaySoundMem(m_hCurrentMusic, DX_PLAYTYPE_BACK);
}

void SoundManager::RestartCurrentBGM()
{
	PlaySoundMem(m_hCurrentMusic, DX_PLAYTYPE_BACK, false);
}

int SoundManager::IsPlayingMusic()
{
	return CheckSoundMem(m_hCurrentMusic);
}

void SoundManager::SetSEVolume(int volume)
{
	m_volumeSE = volume;
}

int SoundManager::GetSEVolume() const
{
	return m_volumeSE;
}

void SoundManager::SetBGMVolume(int volume)
{
	m_volumeBGM = volume;
	int volumePal = (255 / 100) * m_volumeBGM;
	ChangeVolumeSoundMem(volumePal, m_hCurrentMusic);
}

int SoundManager::GetBGMVolume() const
{
	return m_volumeBGM;
}

void SoundManager::SetBGMRate(float rate)
{
	assert(0.0f <= rate && rate <= 1.0f);
	ChangeVolumeSoundMem(static_cast<int>(static_cast<float>(m_volumeBGM) * rate), m_hCurrentMusic);
}

void SoundManager::StopBGM()
{
	StopSoundMem(m_hCurrentMusic);
}

SoundManager::SoundManager() :
	m_hCurrentMusic(NULL),
	m_hSelectSound(-1),
	m_hItemGetSound(-1),
	m_hDamageSound(-1),
	m_hMainMusic(-1)
{
	LoadSoundConfig();
	LoadData();
}

int SoundManager::GetCurrentSoundHandle(SoundType sound)
{
	int currentSound = -1;

	if (sound == SoundType::transition)
	{
		currentSound = m_hTransition;
	}
	else if (sound == SoundType::select)
	{
		currentSound = m_hSelectSound;
	}
	else if (sound == SoundType::itemGet)
	{
		currentSound = m_hItemGetSound;
	}
	else if (sound == SoundType::boost)
	{
		currentSound = m_hBoostSound;
	}
	else if (sound == SoundType::damage)
	{
		currentSound = m_hDamageSound;
	}
	else if (sound == SoundType::shot)
	{
		currentSound = m_hShotSound;
	}
	else if (sound == SoundType::dead)
	{
		currentSound = m_hDeadSound;
	}

	return currentSound;
}

void SoundManager::SetCurrentMusic(MusicType music)
{
	if (music == MusicType::title)
	{
		m_hCurrentMusic = m_hNormalMusic;
	}
	else if (music == MusicType::main)
	{
		m_hCurrentMusic = m_hMainMusic;
	}
	else if (music == MusicType::over)
	{
		m_hCurrentMusic = m_hNormalMusic;
	}
}

void SoundManager::LoadData()
{
	// サウンド
	m_hTransition = LoadSoundMem("Data/SoundData/Transition.mp3");
	m_hSelectSound = LoadSoundMem("Data/SoundData/Select.wav");
	m_hItemGetSound = LoadSoundMem("Data/SoundData/ItemGet.wav");
	m_hBoostSound = LoadSoundMem("Data/SoundData/Boost.wav");
	m_hDamageSound = LoadSoundMem("Data/SoundData/Damage.wav");
	m_hShotSound = LoadSoundMem("Data/SoundData/Shoot.wav");
	m_hDeadSound = LoadSoundMem("Data/SoundData/Dead.wav");
	// ミュージック
	m_hMainMusic = LoadSoundMem("Data/SoundData/MainMusic_Ret.mp3");
	m_hNormalMusic = LoadSoundMem("Data/SoundData/rankMusic.mp3");
}
