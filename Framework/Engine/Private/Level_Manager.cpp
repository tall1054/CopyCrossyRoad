#include "..\Public\Level_Manager.h"
#include "Level.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CLevel_Manager)

CLevel_Manager::CLevel_Manager()
{
}

HRESULT CLevel_Manager::OpenLevel(_uint iLevelIndex, CLevel * pNextLevel)
{
	if (nullptr == pNextLevel || 0 > iLevelIndex)
	{
		MSGBOX("CLevel_Manager::Open_Level : OpenLevel is failed");
		return E_FAIL;
	}

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (nullptr != m_pCurrentLevel)
	{
		if (m_iPreLevelindex != iLevelIndex)
		{
			if (FAILED(pGameInstance->Clear_LevelResource(m_iCurrentLevelIndex)))
			{
				MSGBOX("CLevel_Manager::Open_Level : Failed to Clear_LevelResource");
				return E_FAIL;
			}
		}
		else if (m_iPreLevelindex == iLevelIndex)
		{
			if (FAILED(pGameInstance->Clear_LevelResource_For_ReStart(m_iCurrentLevelIndex)))
			{
				MSGBOX("CLevel_Manager::Open_Level : Failed to Clear_LevelResource");
				return E_FAIL;
			}
		}
	}

	Safe_Release(m_pCurrentLevel);
	m_pCurrentLevel = pNextLevel;
	m_iPreLevelindex = m_iCurrentLevelIndex;
	m_iCurrentLevelIndex = iLevelIndex;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CLevel_Manager::Tick(_double TimeDelta)
{
	if (nullptr == m_pCurrentLevel)
	{
#ifdef _DEBUG
		MSGBOX("CLevel_Manager::Tick : not find Current_Level");
#endif // _DEBUG
		return -1;
	}

	return m_pCurrentLevel->Tick(TimeDelta);
}

_int CLevel_Manager::LateTick(_double TimeDelta)
{
	if (nullptr == m_pCurrentLevel)
	{
#ifdef _DEBUG
		MSGBOX("CLevel_Manager::Late_Tick : not find Current_Level");
#endif // _DEBUG
		return -1;
	}

	return m_pCurrentLevel->LateTick(TimeDelta);
}

HRESULT CLevel_Manager::Render()
{
	if (nullptr == m_pCurrentLevel)
	{
#ifdef _DEBUG
		MSGBOX("CLevel_Manager::Render: not find Current_Level");
#endif // _DEBUG
		return E_FAIL;
	}

	return m_pCurrentLevel->Render();
}

void CLevel_Manager::Free()
{
	Safe_Release(m_pCurrentLevel);
}
