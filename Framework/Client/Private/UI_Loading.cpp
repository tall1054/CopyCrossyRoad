#include "stdafx.h"
#include "..\Public\UI_Loading.h"
#include "GameInstance.h"

CUI_Loading::CUI_Loading(ID3D11Device * pDevice, ID3D11DeviceContext * pDEviceContext)
	: CUI(pDevice, pDEviceContext)
{
}

CUI_Loading::CUI_Loading(const CUI_Loading & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Loading::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to CUI_Loading NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_Loading::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("Failed to CUI_Loading NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
	{
		MSGBOX("Failed to CUI_Loading Setup components");
		return E_FAIL;
	}

	
	return S_OK;
}

_int CUI_Loading::Tick(_double TimeDelta)
{
	return _int();
}

_int CUI_Loading::Late_Tick(_double TimeDelta)
{
	return _int();
}

HRESULT CUI_Loading::Render()
{
	return E_NOTIMPL;
}

HRESULT CUI_Loading::SetUp_Components()
{
	return E_NOTIMPL;
}

HRESULT CUI_Loading::SetUp_RenderState()
{
	return E_NOTIMPL;
}

HRESULT CUI_Loading::Release_RenderState()
{
	return E_NOTIMPL;
}

CUI_Loading * CUI_Loading::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDEviceContext)
{
	return nullptr;
}

CGameObject * CUI_Loading::Clone(void * pArg)
{
	return nullptr;
}

void CUI_Loading::Free()
{
}
