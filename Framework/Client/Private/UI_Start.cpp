#include "stdafx.h"
#include "..\Public\UI_Start.h"


CUI_Start::CUI_Start(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CUI_Start::CUI_Start(const CUI_Start & rhs)
	: CGameObject(rhs)
{
}

HRESULT CUI_Start::NativeConstruct_Prototype()
{
	return E_NOTIMPL;
}

HRESULT CUI_Start::NativeConstruct(void * pArg)
{
	return E_NOTIMPL;
}

_int CUI_Start::Tick(_double TimeDelta)
{
	return _int();
}

_int CUI_Start::LateTick(_double TimeDelta)
{
	return _int();
}

HRESULT CUI_Start::Render()
{
	return E_NOTIMPL;
}

HRESULT CUI_Start::SetUp_Components()
{
	return E_NOTIMPL;
}

CUI_Start * CUI_Start::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	return nullptr;
}

CGameObject * CUI_Start::Clone(void * pArg)
{
	return nullptr;
}

void CUI_Start::Free()
{
}
