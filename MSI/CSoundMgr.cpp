#include "global.h"
#include "CCore.h"
#include "CSoundMgr.h"
#include "CSound.h"

#include "fmod.hpp"
#include "CKeyMgr.h"

CSoundMgr::CSoundMgr()
{
}

CSoundMgr::~CSoundMgr()
{
	m_pSystem->release();
}


Sound* CSoundMgr::CreateSound(wstring _strSoundName, string _strFileName, bool _bLoop)
{
	if (FindSound(_strSoundName) == nullptr)
	{
		std::pair<wstring, Sound*> pairSound;
		pairSound.first = _strSoundName;
		m_pSystem->createSound(_strFileName.c_str(), _bLoop == true ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF, 0, &pairSound.second);
		m_mapSound.insert(pairSound);
		return pairSound.second;
	}
	else return FindSound(_strSoundName);
}

Sound* CSoundMgr::FindSound(wstring _strSoundName)
{
	map<wstring, Sound*>::iterator iter = m_mapSound.find(_strSoundName);
	if (iter == m_mapSound.end()) return nullptr;
	else return iter->second;
}

void CSoundMgr::Play(wstring _strSoundName, SOUND_TYPE _eType)
{
	Sound* pSound = FindSound(_strSoundName);
	if (pSound == nullptr)
	{
		string fileName(_strSoundName.begin(), _strSoundName.end());
		fileName += ".mp3";
		CreateSound(_strSoundName, fileName.c_str(), false);
		pSound = FindSound(_strSoundName);
	}
	m_eResult = m_pSystem->playSound(pSound, 0, false, &m_Channel[(unsigned long long)_eType]);
}

bool CSoundMgr::isPlaying(SOUND_TYPE _eSoundType)
{
	bool bPlaying = false;
	m_Channel[(unsigned long long)_eSoundType]->isPlaying(&bPlaying);
	return bPlaying;
}

int CSoundMgr::init()
{
	m_eResult = FMOD::System_Create(&m_pSystem);
	if (m_eResult != FMOD_OK) return false;

	m_eResult = m_pSystem->getVersion(&m_iVersion);
	if (m_eResult != FMOD_OK) return false;

	m_eResult = m_pSystem->init(32, FMOD_INIT_NORMAL, extradriverdata);
	if (m_eResult != FMOD_OK) return false;

	for (int i = 0; i < (int)SOUND_TYPE::END; i++)
	{
		m_Channel[i] = nullptr;
	}

	return true;
}

void CSoundMgr::update()
{
	m_eResult = m_pSystem->update();

	m_Channel[(unsigned long long)SOUND_TYPE::BGM]->setVolume(0.1f);
	m_Channel[(unsigned long long)SOUND_TYPE::BGM]->setPaused(false);

	m_Channel[(unsigned long long)SOUND_TYPE::EFFECT1]->setVolume(0.2f);
	m_Channel[(unsigned long long)SOUND_TYPE::EFFECT1]->setPaused(false);

	m_Channel[(unsigned long long)SOUND_TYPE::EFFECT2]->setVolume(0.2f);
	m_Channel[(unsigned long long)SOUND_TYPE::EFFECT2]->setPaused(false);

	m_Channel[(unsigned long long)SOUND_TYPE::EFFECT3]->setVolume(0.3f);
	m_Channel[(unsigned long long)SOUND_TYPE::EFFECT3]->setPaused(false);
}

