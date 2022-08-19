//==========================������Ʈ�Ŵ����ش�==========================//
#pragma once
#include "Base.h"
//��ü���� ������ �����Ѵ����� �����ϴ� �����̴�.
//������ ã�Ƽ� ������ ������ ��Ƽ� �����Ѵ�.
BEGIN(Engine)
class CObject_Manager final : public CBase
{
	//�ݺ��ؼ� ����� �� �ֱ⿡ �̱������� ���Ѵ�.
	DECLARE_SINGLETON(CObject_Manager)
public:
	CObject_Manager();
	virtual ~CObject_Manager() = default;
public:
	//������Ʈ�� ���� �Լ�(���ڷ� �����ε�����, ���̾��±�, ������Ʈ�±�, ������Ʈ�� ��Ƶδ� �ε���)
	class CComponent* Get_Component(_uint iLevelIndex, const _tchar* pLayertag, const _tchar* pComponentTag, _uint iIndex);
public:
	//==============�����̳� ���� �Լ�, ������ü �߰�, ������Ʈ��ü�߰�, �� ������ �ִ� ������Ʈ Ŭ����================//
	HRESULT Reserve_Container(_uint iNumLevels);
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, void* pArg);
	HRESULT Clear_LevelObject(_uint iLevelIndex);
public:
	//============�ð��� �ݺ�ó������=================//
	_int Tick(_double TimeDelta);
	_int LateTick(_double TimeDelta);
private:
	//============��STL�����̳ʷ� �� ������ü�� ���̾���� ��´�.==================//
	map<const _tchar*, class CGameObject*>			m_Prototypes;
	typedef map<const _tchar*, class CGameObject*>	PROTOTYPES;
private:
	map<const _tchar*, class CLayer*>*				m_pLayers = nullptr;
	typedef map<const _tchar*, class CLayer*>		LAYERS;
	//=======�������� ���� ��������==============//
	_uint		m_iNumLevels = 0;
private:
	//==========� ���ӿ�����Ʈ, � ���̾����� Ȯ���ϴ� �۾�===============//
	class CGameObject*	Find_Prototype(const _tchar* pPrototypeTag);
	class CLayer*		Find_Layer(_uint iLevelIndex, const _tchar* pLayerTag);
public:
	virtual void Free() override;
};
//=====================������Ʈ�Ŵ���Ŭ�������γ���=======================//
#include "Object_Manager.h"
#include "GameObject.h"
#include "Layer.h"
//================���ø���Ʈ �̱������� ���ֻ���ϰ� ��������============//
IMPLEMENT_SINGLETON(CObject_Manager)
CObject_Manager::CObject_Manager()
{
}
CComponent* CObject_Manager::Get_Component(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex)
{
	//============������Ʈ�� ����� �۾��̱⿡ �����ϴ� �����ε������� �����ѷ����ε����� �۾ƾ� ������ �����ϴ�.==========//
	if (iLevelIndex >= m_iNumLevels)
		return nullptr;
	//======���̾ ���� �۾��� ������ �´�.=============//
	CLayer*	pLayer = Find_Layer(iLevelIndex, pLayerTag);
	if (nullptr == pLayer)
		return nullptr;
	//=========�ε����� ������Ʈ �±װ� ��� ���̾�� ���Ͻ�Ų��.
	return pLayer->Get_Component(pComponentTag, iIndex);
}
HRESULT CObject_Manager::Reserve_Container(_uint iNumLevels)
{
	//======
	m_iNumLevels = iNumLevels;
	m_pLayers = new LAYERS[m_iNumLevels];
	return S_OK;
}
HRESULT CObject_Manager::Add_Prototype(const _tchar* pPrototypeTag, CGameObject* pPrototype)
{
	if (nullptr == pPrototype || nullptr != Find_Prototype(pPrototypeTag))
		return E_FAIL;
	m_Prototypes.insert(PROTOTYPES::value_type(pPrototypeTag, pPrototype));
	return S_OK;
}
HRESULT CObject_Manager::Add_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, void* pArg)
{
	if (iLevelIndex >= m_iNumLevels)
		return E_FAIL;
	CGameObject*	pPrototype = Find_Prototype(pPrototypeTag);
	if (nullptr == pPrototype)
		return E_FAIL;
	CGameObject*	pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
		return E_FAIL;
	CLayer*	pLayer = Find_Layer(iLevelIndex, pLayerTag);
	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();
		if (nullptr == pLayer)
			return E_FAIL;
		if (FAILED(pLayer->Add_Gameobject(pGameObject)))
			return E_FAIL;
		m_pLayers[iLevelIndex].insert(LAYERS::value_type(pLayerTag, pLayer));
	}
	else
	{
		if (FAILED(pLayer->Add_GameObject(pGameObject)))
			return E_FAIL;
	}
	return S_OK;
}
HRESULT CObject_Manager::Clear_LevelObject(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return E_FAIL;
	for (auto& Pair : m_pLayers[iLevelIndex])
		Safe_Release(Pair.second);
	m_pLayers[iLevelIndex].clear();
	return S_OK;
}
_int CObject_Manager::Tick(_double Timedelta)
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			if (0 > Pair.second->Tick(Timedelta))
				if (3 == Pair.second->Tick(Timedelta))
					safe_release(Pair.second);
			m_pLayers.er

				return -1;
		}
	}
	return _int();
}
_int CObject_Manager::LateTick(_double TimeDelta)
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			if (0 > Pair.second->LateTick(TimeDelta))
				return -1;
		}
	}
	return _int();
}
CGameObject* CObject_Manager::Find_Prototype(const _tchar* pPrototypeTag)
{
	auto	iter = find_if(m_Prototypes.begin(), m_Prototypes.end(), CTagFinder(pPrototypeTag));
	if (iter == m_Prototypes.end())
		return nullptr;
	return iter->second;
}
CLayer* CObject_Manager::Find_Layer(_uint iLevelIndex, const _tchar* pLayerTag)
{
	auto	iter = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTagFinder(pLayerTag));
	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;
	return iter->second;
}
void CObject_Manager::Free()
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
			Safe_Release(Pair.second);
		m_pLayers[i].clear();
	}
	Safe_Delete_Array(m_pLayers);
	for (auto& Pair : m_Prototypes)
		Safe_Release(Pair.second);
	m_Prototypes.clear();
}
//===========���̾��ش�==============//
#pragma once
#include "Base.h"
BEGIN(Engine)
class CLayer final : public CBase
{
public:
	CLayer();
	virtual ~CLayer() = default;
public:
	class CComponent*	Get_Component(const _tchar* pComponentTag, _uint iIndex);
public:
	HRESULT Add_GameObject(class CGameObject* pGameObject);
	_int Tick(_double TimeDelta);
	_int LateTick(_double TimeDelta);
private:
	list<class CGameobject*>			m_Objects;
	typedef list<class CGameobject*>	OBJECTS;
public:
	static CLayer*	Create();
	virtual void Free();
};
//===============���̾�Ŭ���� �����Լ�=================//
#include "Layer.h"
#include "GameObject.h"
CLayer::CLayer()
{
}
CComponent* CLayer::Get_Component(const _tchar* pComponentTag, _uint iIndex)
{
	if (iIndex >= m_Objects.size())
		return nullptr;
	auto	iter = m_Objects.begin();
	for (_uint i = 0; i < iIndex; ++i)
		++iter;
	return (*iter)->Get_Component(pComponentTag);
}
HRESULT CLayer::Add_GameObject(CGameObject* pGameObject)
{
	m_Objects.push_back(pGameObject);
	return S_OK;
}
_int CLayer::Tick(_double TimeDelta)
{
	for (auto& pGameObject : m_Objects)
	{
		if (0 > pGameObject->Tick(TimeDelta))
			return -1;
	}
	return 0;
}
_int CLayer::LateTick(_double TimeDelta)
{
	for (auto& pGameObject : m_Objects)
	{
		if (0 > pGameObject->LateTick(TimeDelta))
			return -1;
	}
	return 0;
}
CLayer*	CLayer::Create()
{
	CLayer* pLayer = new CLayer();
	return pLayer;
}
void CLayer::Free()
{
	for (auto& pGameObject : m_Objects)
		Safe_Release(pGameObject);
	m_Objects.clear();
}