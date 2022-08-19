#include "..\Public\Timer_Manager.h"
#include "Timer.h"

IMPLEMENT_SINGLETON(CTimer_Manager)

CTimer_Manager::CTimer_Manager()
{
}

_double CTimer_Manager::Get_TimeDelta(const _tchar * pTimerTag)
{
	CTimer*	pTimer = Find_Timer(pTimerTag);

	if (nullptr == pTimer)
		return 0.f;

	return pTimer->Get_TimeDelta();
}

HRESULT CTimer_Manager::Add_Timer(const _tchar * pTimerTag)
{
	if (nullptr != Find_Timer(pTimerTag))
	{
#ifdef _DEBUG
		MSGBOX("CTimer_Manager::Add_Timer : Duplicate Timer Tag");
#endif // _DEBUG
		return E_FAIL;
	}

	CTimer*	pTimer = CTimer::Create();
	if (nullptr == pTimer)
	{
		MSGBOX("CTimer_Manager::Add_Timer : Failed create new Timer");
		return E_FAIL;
	}

	m_Timers.insert(TIMERS::value_type(pTimerTag, pTimer));

	return S_OK;
}

CTimer * CTimer_Manager::Find_Timer(const _tchar * pTimerTag)
{
	auto	iter = find_if(m_Timers.begin(), m_Timers.end(), CTagFinder(pTimerTag));
	if (iter == m_Timers.end())
		return nullptr;

	return iter->second;
}

void CTimer_Manager::Free()
{
	for (auto& Pair : m_Timers)
		Safe_Release(Pair.second);

	m_Timers.clear();
}
