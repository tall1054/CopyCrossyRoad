#include "..\Public\GameObject.h"
#include "GameInstance.h"
#include "Component.h"

const _tchar* CGameObject::m_pTransformTag = TEXT("Com_Transform");

CGameObject::CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

CGameObject::CGameObject(const CGameObject & rhs)
	: m_pDevice(rhs.m_pDevice)
	, m_pDeviceContext(rhs.m_pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

CComponent * CGameObject::Get_Component(const _tchar * pComponentTag)
{
	return Find_Component(pComponentTag);
}

HRESULT CGameObject::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::NativeConstruct(const CTransform::TRANSFORMDESC* pTransformDesc)
{
	m_pTransform = CTransform::Create(m_pDevice, m_pDeviceContext);
	if (nullptr == m_pTransform)
		return E_FAIL;

	if (nullptr != pTransformDesc)
		m_pTransform->Set_TransformDesc(*pTransformDesc);

	if (nullptr != Find_Component(m_pTransformTag))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_Components.emplace(m_pTransformTag, m_pTransform);

	Safe_AddRef(m_pTransform);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CGameObject::Tick(_double TimeDelta)
{
	return _int();
}

_int CGameObject::LateTick(_double TimeDelta)
{
	return _int();
}

HRESULT CGameObject::Render()
{
	return S_OK;
}

HRESULT CGameObject::Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pComponentTag, CComponent** ppOut, void* pArg)
{
	if (0 > iLevelIndex)
	{
		MSGBOX("CGameObject::Add_Component : Out of range");
		return E_FAIL;
	}

	if (nullptr != Find_Component(pComponentTag))
	{
#ifdef _DEBUG
		MSGBOX("CGameObject::Add_Component : Already existed component");
#endif // _DEBUG
		return E_FAIL;
	}

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CComponent*			pComponent = pGameInstance->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
	if (nullptr == pComponent)
	{
		MSGBOX("CGameObject::Add_Component : Failed to Clone");
		return E_FAIL;
	}

	m_Components.emplace(pComponentTag, pComponent);
	*ppOut = pComponent;
	Safe_AddRef(pComponent);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CComponent * CGameObject::Find_Component(const _tchar * pComponentTag)
{
	auto	iter = find_if(m_Components.begin(), m_Components.end(), CTagFinder(pComponentTag));

	if (iter == m_Components.end())
		return nullptr;

	return iter->second;
}

//HRESULT CGameObject::Compute_CamDistance(const CTransform * pTransform, const CPipeLine* pPipeLine)
//{
//	
//	return S_OK;
//}

void CGameObject::Free()
{
	Safe_Release(m_pTransform);

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);

	for (auto& Pair : m_Components)
		Safe_Release(Pair.second);
	m_Components.clear();
}

