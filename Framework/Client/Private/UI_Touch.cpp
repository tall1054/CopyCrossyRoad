#include "stdafx.h"
#include "..\Public\UI_Touch.h"

CUI_Touch::CUI_Touch(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CUI(pDevice, pDeviceContext)
{
}

CUI_Touch::CUI_Touch(const CUI_Touch & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Touch::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to CUI_Touch NativeConstruct_Prototype");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CUI_Touch::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("Failed to CUI_Touch NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
	{
		MSGBOX("Failed to CUI_Touch SetUp components");
		return E_FAIL;
	}

	m_fSizeX = g_iWinCX;
	m_fSizeY = g_iWinCY;
	m_fX = g_iWinCX *0.5f;
	m_iCount = 0;

	return S_OK;
}

_int CUI_Touch::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
	{
		MSGBOX("");
		return -1;
	}
	return _int();
}

_int CUI_Touch::LateTick(_double TimeDelta)
{
	return _int();
}

HRESULT CUI_Touch::Render()
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	//pGameInstance->Render_Font(m_pDeviceContext, TEXT("8BIT_WONDER"), m_iCount, _float2(0.f, 0.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CUI_Touch::SetUp_Components()
{
	return S_OK;
}

CUI_Touch * CUI_Touch::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CUI_Touch*	pInstance = new CUI_Touch(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CUI_Touch");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUI_Touch::Clone(void * pArg)
{
	CUI_Touch*	pInstance = new CUI_Touch(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CUI_Touch");
		Safe_Release(pInstance);
	}

	return pInstance;;
}

void CUI_Touch::Free()
{
	__super::Free();
}
