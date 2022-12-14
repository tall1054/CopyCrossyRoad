#pragma once

#include "Base.h"

/* 현재 화면에 보여줘야할 레벨의 주소를 보관한다. */
/* 레벨 교체시에 기존 레벨을 정리하고(삭제하고), 새로운 레벨로 저장한다. */

BEGIN(Engine)

class CLevel_Manager final : public CBase
{
	DECLARE_SINGLETON(CLevel_Manager)
public:
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;

public:
	HRESULT OpenLevel(_uint iLevelIndex, class CLevel* pNextLevel);
	_int Tick(_double TimeDelta);
	_int LateTick(_double TimeDelta);
	HRESULT Render();

public:
	const _uint&	Get_CurrentLevelIndex() const { return m_iCurrentLevelIndex; }

private:
	_uint				m_iCurrentLevelIndex = 0;
	class CLevel*		m_pCurrentLevel = nullptr;

	_uint				m_iPreLevelindex = 0;

public:
	virtual void Free() override;
};

END