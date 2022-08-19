#include "stdafx.h"
#include "..\Public\SendStage.h"
#include "GameInstance.h"
#include "Level_Loading.h"


CSendStage::CSendStage(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CSendStage::CSendStage(const CSendStage & rhs)
	: CGameObject(rhs)
{
}

HRESULT CSendStage::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSendStage::NativeConstruct(void * pArg)
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(__super::NativeConstruct(&CTransform::TRANSFORMDESC(3.f, XMConvertToRadians(90.0f)))))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;


	if (nullptr != pArg)
		memcpy(&m_tSendStage, pArg, sizeof(SendStage));

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_tSendStage.vPosition.x, m_tSendStage.vPosition.y, m_tSendStage.vPosition.z, m_tSendStage.vPosition.w));
	m_pTransform->Scaled(XMLoadFloat3(&m_tSendStage.vScale));



	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CSendStage::Tick(_double TimeDelta)
{
	return _int();
}

_int CSendStage::Late_Tick(_double TimeDelta)
{
	return _int();
}

HRESULT CSendStage::Render()
{
	return E_NOTIMPL;
}

HRESULT CSendStage::SetUp_Components()
{
	return E_NOTIMPL;
}

HRESULT CSendStage::SetUp_ConstantTable()
{
	return E_NOTIMPL;
}

HRESULT CSendStage::Send_StageKorea()
{
	return E_NOTIMPL;
}

CSendStage * CSendStage::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	return nullptr;
}

CGameObject * CSendStage::Clone(void * pArg)
{
	return nullptr;
}

void CSendStage::Free()
{
}
