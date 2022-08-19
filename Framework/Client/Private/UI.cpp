#include "stdafx.h"
#include "..\Public\UI.h"


CUI::CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
	
}

CUI::CUI(const CUI & rhs)
	: CGameObject(rhs)
{
}

HRESULT CUI::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI::NativeConstruct(void * pArg)
{
	return S_OK;
}

_int CUI::Tick(_double TimeDelta)
{
	return _int();
}

_int CUI::Late_Tick(_double TimeDelta)
{
	return _int();
}

HRESULT CUI::Render()
{
	return S_OK;
}

void CUI::Free()
{
	__super::Free();
}