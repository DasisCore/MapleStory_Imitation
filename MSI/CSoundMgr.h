#pragma once

#include "fmod.h"
#include "fmod.hpp"

using namespace FMOD;

enum class SOUND_TYPE
{
	BGM,
	EFFECT1,
	EFFECT2,
	EFFECT3,
	MOUSE,
	KEYBOARD,
	ETC,
	END = 32,
};

class CSound;
class CSoundMgr
{
	SINGLE(CSoundMgr);

private:
	System* m_pSystem;
	map<wstring, Sound*> m_mapSound;
	
	Sound* m_Sound[(unsigned long long)SOUND_TYPE::END];
	Channel* m_Channel[32];

	FMOD_RESULT m_eResult;
	unsigned int m_iVersion;
	void* extradriverdata;

public:
	// 사운드를 생성하는 함수
	Sound* CreateSound(wstring _strSoundName, string _strFileName, bool _bLoop);

	// 사운드를 찾는 함수
	Sound* FindSound(wstring _strSoundName);

	void Play(wstring _strSoundName, SOUND_TYPE _eType);

	bool isPlaying(SOUND_TYPE _eSoundType);

public:
	int init(void);
	void update();

};


