#include "stdafx.h"
#include "..\Public\Interaction_Object.h"
#include "GameInstance.h"

CInteraction_Object::CInteraction_Object(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CInteraction_Object::CInteraction_Object(const CInteraction_Object & pArg)
	:CGameObject(pArg)
{
}

HRESULT CInteraction_Object::NativeConstruct_Prototype()
{
	return E_NOTIMPL;
}

HRESULT CInteraction_Object::NativeConstruct(void * pArg)
{
	return E_NOTIMPL;
}

_int CInteraction_Object::Tick(_double TimeDelta)
{
	return _int();
}

_int CInteraction_Object::LateTick(_double timeDelta)
{
	return _int();
}

HRESULT CInteraction_Object::Render()
{
	return E_NOTIMPL;
}

_float CInteraction_Object::Distance_Player()
{
	return _float();
}

CGameObject * CInteraction_Object::Clone(void * pArg)
{
	return nullptr;
}

void CInteraction_Object::Free()
{
}
