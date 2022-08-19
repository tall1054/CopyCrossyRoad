//==========================오브젝트매니져해더==========================//
#pragma once
#include "Base.h"
//객체들의 원형을 생성한다음에 보관하는 과정이다.
//원형을 찾아서 복제한 다음에 모아서 보관한다.
BEGIN(Engine)
class CObject_Manager final : public CBase
{
	//반복해서 사용할 수 있기에 싱글톤으로 감싼다.
	DECLARE_SINGLETON(CObject_Manager)
public:
	CObject_Manager();
	virtual ~CObject_Manager() = default;
public:
	//컴포넌트를 얻어내는 함수(인자로 레벨인덱스와, 레이어태그, 컴포넌트태그, 컴포넌트를 모아두는 인덱스)
	class CComponent* Get_Component(_uint iLevelIndex, const _tchar* pLayertag, const _tchar* pComponentTag, _uint iIndex);
public:
	//==============컨테이너 담은 함수, 원형객체 추가, 오브젝트객체추가, 각 레벨당 있는 오브젝트 클리어================//
	HRESULT Reserve_Container(_uint iNumLevels);
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, void* pArg);
	HRESULT Clear_LevelObject(_uint iLevelIndex);
public:
	//============시간당 반복처리구문=================//
	_int Tick(_double TimeDelta);
	_int LateTick(_double TimeDelta);
private:
	//============맵STL컨테이너로 각 원형객체와 레이어들을 담는다.==================//
	map<const _tchar*, class CGameObject*>			m_Prototypes;
	typedef map<const _tchar*, class CGameObject*>	PROTOTYPES;
private:
	map<const _tchar*, class CLayer*>*				m_pLayers = nullptr;
	typedef map<const _tchar*, class CLayer*>		LAYERS;
	//=======각레벨을 담을 변수선언==============//
	_uint		m_iNumLevels = 0;
private:
	//==========어떤 게임오브젝트, 어떤 레이어인지 확인하는 작업===============//
	class CGameObject*	Find_Prototype(const _tchar* pPrototypeTag);
	class CLayer*		Find_Layer(_uint iLevelIndex, const _tchar* pLayerTag);
public:
	virtual void Free() override;
};
//=====================오브젝트매니져클래스세부내용=======================//
#include "Object_Manager.h"
#include "GameObject.h"
#include "Layer.h"
//================인플리맨트 싱글톤으로 자주사용하게 만들어두자============//
IMPLEMENT_SINGLETON(CObject_Manager)
CObject_Manager::CObject_Manager()
{
}
CComponent* CObject_Manager::Get_Component(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex)
{
	//============컴포넌트을 얻오는 작업이기에 존재하는 레벨인덱스보다 선언한레벨인덱스가 작아야 수행이 가능하다.==========//
	if (iLevelIndex >= m_iNumLevels)
		return nullptr;
	//======레이어를 얻어내는 작업을 가지고 온다.=============//
	CLayer*	pLayer = Find_Layer(iLevelIndex, pLayerTag);
	if (nullptr == pLayer)
		return nullptr;
	//=========인덱스와 컴포넌트 태그가 담긴 레이어로 리턴시킨다.
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
//===========레이어해더==============//
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
//===============레이어클래스 세부함수=================//
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