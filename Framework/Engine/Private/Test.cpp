#pragma once
#include "Transform.h"
#include "GameInstance.h"
#include "Component.h"

BEGIN(Engine)

class CComponent;

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	explicit CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	class CComponent* Get_Component(const _tchar* pComponentTag);

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(const CTransform::TRANSFORMDESC* pTransformDesc = nullptr);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

public:
	HRESULT Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar*pComponentTag, CComponent** ppOut, void* pArg = nullptr);

public:
	static const _tchar* m_pTransformTag;

protected:
	ID3D11Device*					m_pDevice = nullptr;
	ID3D11DeviceContext*			m_pDeviceContext = nullptr;

protected:
	class CTransform*				m_pTransform = nullptr;

protected:
	CComponent*						Find_Component(const _tchar* pComponentTag);

protected:
	map<const _tchar*, CComponent*>					m_Components;
	typedef map<const _tchar*, CComponent*>		COMPONENTS;

public:
	virtual CGameObject*	Clone(void* pArg) = 0;
	virtual void Free() override;
}

END

const _tchar* CGameObject::m_pTransformTag = TEXT("Com_Transform");

CGameObject::CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

CGameObject::CGameObject(const CGameObject& rhs)
	: m_pDevice(rhs.m_pDevice)
	, m_pDeviceContext(rhs.m_pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

CComponent* CGameObject::Get_Component(const _tchar* pComponentTag)
{
	return Find_Component(pComponentTag);
}

HRESULT CGameObject::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::NativeConstruct(const CTransform::TRANSFORMDESC* pTransformDesc)
{
	//트랜스폼 을 만들어둔다.
	m_pTransform = CTransform::Create(m_pDevice, m_pDeviceContext);

	if (nullptr == m_pTransform)
		return E_FAIL;

	//트랜스폼 디스크립션이 있을 경우
	//트램스폼 디스크립션(인자로 트랜스폼 디스크립션으로)을 세팅해둔다.
	if (nullptr != pTransformDesc)
		m_pTransform->Set_TransformDesc(*pTransformDesc);

	if (nullptr != Find_Component(m_pTransformTag))
		return E_FAIL;

	CGameInstance*		pGameinstance = GET_INSTANCE(CGameInstance);

	m_Components.emplace(m_pTransformTag, m_pTransform);

	Safe_AddRef(m_pTransform);

	RELEASE_INSTANCE(CGameInstance);
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

HRESULT CGameObject::Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pComponentTag, CComponent**ppOut, void* pArg)
{
	//게임오브젝트에서 컴포넌트를 추가하는 작업 함수다. 레벨인덱스와, 프로토타입태그, 컴포넌트태그, 컴포넌트포인터를 내보내는 것
	//컴포넌트를 찾는 작업을 수행한다. 각 컴포넌트 태그가 없을 경우 fail 수행한다.
	if (nullptr != Find_Component(pComponentTag))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CComponent*			pComponent = pGameInstance->Clone_Component(iLevelIndex, pPrototypeTag, pArg);

	if (nullptr == pComponent)
		return E_FAIL;

	m_Components.emplace(pComponentTag, pComponent);

	*ppOut = pComponent;

	Safe_AddRef(pComponent);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CComponent* CGameObject::Find_Component(const _tchar* pComponentTag)
{
	auto	iter = find_if(m_Components.begin(), m_Components.end(), CTagFinder(pComponentTag));

	if (iter == m_Components.end())
		return nullptr;

	return iter->second;
}

void CGameObject::Free()
{
	Safe_Release(m_pTransform);

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);

	for (auto& Pair : m_Components)
		Safe_Release(Pair.second);
	m_Components.clear();
}