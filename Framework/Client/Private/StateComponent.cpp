#include "stdafx.h"
#include "..\Public\StateComponent.h"

CStateComponent::CStateComponent(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{
}

CStateComponent::CStateComponent(const CStateComponent & pArg)
	: CComponent(pArg)
	, m_tState(pArg.m_tState)
{
}

HRESULT CStateComponent::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to CStateComponent::NativeConstructPrototype");
		return E_FAIL;
	}
		
	return S_OK;
}

HRESULT CStateComponent::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("Failed to CStateComponent::NativeConstruct");
		return E_FAIL;
	}

	if (nullptr != pArg)
	{
		memcpy(&m_tState, pArg, sizeof(tagState));
	}
	else
	{
		m_tState.m_fMax_Coin = 1000.f;
		m_tState.m_fCurrent_Coin = m_tState.m_fMax_Coin;
	}
	return S_OK;
}

void CStateComponent::Set_MaxCoin(_float fNewMaxCoin)
{
	m_tState.m_fMax_Coin = fNewMaxCoin;
}

void CStateComponent::Set_CurrentCoin(_float fNewCurCoin)
{
	m_tState.m_fCurrent_Coin = fNewCurCoin;
}

void CStateComponent::Add_CurrentCoin(_float fAddCurrentCoin)
{
	m_tState.m_fCurrent_Coin += fAddCurrentCoin;
}

_float CStateComponent::Get_MaxCoin()
{
	return m_tState.m_fMax_Coin;
}

_float CStateComponent::Get_CurrentCoin()
{
	return m_tState.m_fCurrent_Coin;
}

_float CStateComponent::Get_CoinRatio()
{
	return m_tState.m_fCurrent_Coin / m_tState.m_fMax_Coin;
}


_float CStateComponent::Add_Coin(_float fAddedCoin)
{
	m_tState.m_fCurrent_Coin += fAddedCoin;

	if (m_tState.m_fCurrent_Coin > m_tState.m_fMax_Coin)
		m_tState.m_fCurrent_Coin = m_tState.m_fMax_Coin;

	if (0.f >= m_tState.m_fCurrent_Coin)
	{
		m_tState.m_fCurrent_Coin = 0.f;
		return 0.f;
	}

	return m_tState.m_fCurrent_Coin;
}

CStateComponent * CStateComponent::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CStateComponent*	pInstance = new CStateComponent(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CStateComponent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CStateComponent::Clone(void * pArg)
{
	CStateComponent*	PInstnace = new CStateComponent(*this);
	if (FAILED(PInstnace->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CStateComponent Clone");
		Safe_Release(PInstnace);
	}

	return PInstnace;
}

void CStateComponent::Free()
{
	__super::Free();
}
