#pragma once
#include "../../fmod/fmod.h"
#include "../../fmod/fmod.hpp"
#include "Engine_Defines.h"
#include "Base.h"


#define SOUND_MAX 1.0f
#define SOUND_MIN 0.0f
#define SOUND_DEFAULT 0.5f
#define SOUND_WEIGHT 0.1f

class CSoundMgr : public CBase
{
	DECLARE_SINGLETON(CSoundMgr)

public:
	enum CHANNELID {
		BGM, PLAYER, PLAYER2, CAR, TRAIN, ITEM, UI, MAXCHANNEL};

private:
	CSoundMgr();
	virtual ~CSoundMgr() = default;

public:
	HRESULT NativeConstruct();

public:
	int  VolumeUp(CHANNELID eID, _float _vol);
	int  VolumeDown(CHANNELID eID, _float _vol);
	int  BGMVolumeUp(_float _vol);
	int  BGMVolumeDown(_float _vol);
	int  Pause(CHANNELID eID);
	void Play_Sound(TCHAR* pSoundKey, CHANNELID eID, _float _vol);
	void Play_Sound_Loop(TCHAR* pSoundKey, CHANNELID eID, _float _vol);
	void PlayBGM(TCHAR* pSoundKey);
	void StopSound(CHANNELID eID);
	void StopAll();

private:
	float m_volume = SOUND_DEFAULT;
	float m_BGMvolume = SOUND_DEFAULT;
	FMOD_BOOL m_bool;

public:
	virtual void Free() override;

private:
	void LoadSoundFile();

private:
	// ���� ���ҽ� ������ ���� ��ü 
	map<TCHAR*, FMOD_SOUND*> m_mapSound;
	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];
	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM* m_pSystem  = nullptr;
	_bool		m_bPause = false;
};

