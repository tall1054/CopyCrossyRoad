#include "..\Public\Object_Manager.h"
//#include "GameObject.h"
//#include "Layer.h"

IMPLEMENT_SINGLETON(CObject_Manager)

CObject_Manager::CObject_Manager()
{
}
//컴포넌트를 얻는 함수(재료: 레벨위치, 레이어태그(각레벨에 있는 레이어객체의미), 컴포넌트태그, 인덱스(모름))
CComponent * CObject_Manager::Get_Component(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pComponentTag, _uint iIndex)
{
	if (iLevelIndex >= m_iNumLevels)//레벨위치가 모든레벨의 끝보다클경우
	{
		MSGBOX("Failed to CObject_Manager::Get_Component");
		return nullptr;//없는 놈으로 취급한다.
	}
	//레이어를 선언=레이어를 찾는 함수(모릅니다 : 최종벨류값을 얻어낸다)(레벨인덱스와, 레이어태그로)
	CLayer*		pLayer = Find_Layer(iLevelIndex, pLayerTag);
	if (nullptr == pLayer)
	{
		MSGBOX("Failed to CObject_Manager::Find_Layer");
		return nullptr;//없을시 레이어가 없다고 생각함
	}
	//레이의 벨류값은 컴포넌트를 얻어내는 함수로 처리한다.
	return pLayer->Get_Component(pComponentTag, iIndex);
}
//레이어를 얻어내는 함수(재료: 레벨인덱스, 레이어태그)
CLayer * CObject_Manager::Get_Layer(_uint iLevelIndex, const _tchar * pLayerTag)
{
	//반복자를 사용해서 레이어의 시작(레벨위치에 있는)부터끝까지 반복자로 대입
	auto	iter = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTagFinder(pLayerTag));
	//그 끝에 도달하염 
	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;//없다고 반환해준다.
	//그리고 얻어낸 레이어의 벨류값을 반환해준다.
	return iter->second;
}
//전방 객체를 얻어내는 함수(재료: 레벨위치랑 레이어태그)
CGameObject * CObject_Manager::Get_Front(_uint iLevelIndex, const _tchar * pLayerTag)
{
	//반복자를 이용해서 시작부터 끝까지 레벨위치에서 태그를 얻어낸다.
	auto	iter = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTagFinder(pLayerTag));
	//그 레이어가 없을 경우 널로 리턴한다.
	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;
	//있을경우 벨류값을 그객체를 처음으로 반환시킨다.
	return iter->second->Get_Objects().front();
}
//후방객체를 얻어내는 함수
CGameObject * CObject_Manager::Get_Back(_uint iLevelIndex, const _tchar * pLayerTag)
{
	//반복자를 이용해서 시작부터 끝까지 레벨위치에서 태그를 얻어낸다.
	auto	iter = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTagFinder(pLayerTag));
	//그 레이어가 없을 경우 널로 리턴한다.
	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;
	//있을경우 벨류값을 그객체를 마지막으로 반환시킨다.
	return iter->second->Get_Objects().back();
}
//게임객체의 리스트를 얻어내는 함수(재료 : 레벨위치, 레이어태그, 반복자의 수(모름:))
CGameObject * CObject_Manager::Get_ListOfGameObject(_uint iLevelIndex, const _tchar * pLayerTag, int iterNum)
{
	//반복자를 이용해서 태그를 찾아낸다.
	auto	iter = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTagFinder(pLayerTag));
	//태그로 할만한 친구가 없으면 널로 반환한다.
	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;
	//또다른 반복자이지만 명단인 반복자를 선언 = 반복자가 벨류값을 얻어내고 그 벨류값은 처음위치이다.
	auto	List_iter = iter->second->Get_Objects().front();
	//반복문을 이요해서
	for (int i = 0; i < iterNum; ++i)
	{
		//반복자의 명단을 추가추가해준다.
		++List_iter;
	}

	return List_iter;
}
//클론을 얻어내는 함수(원형객체태그, 반복자의 보이드포인터)
CGameObject * CObject_Manager::Get_Clone(const _tchar * pPrototypeTag, void* pArg)
{
	CGameObject* pGameObject = Find_Prototype(pPrototypeTag);
	if (pGameObject == nullptr)
	{
		//MSGBOX("CObject_Manager :: Get_Clone :: Not found CGameObject");
		return nullptr;
	}

	return pGameObject->Clone(pArg);
}
//객체레이어함수(레벨위치, 레이어태그)
CLayer * CObject_Manager::Get_ObjectLayer(_uint iLevelIndex, const _tchar * pLayerTag)
{
	auto	iter = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTagFinder(pLayerTag));

	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;

	return iter->second;
}
//전방제거함수(레벨위치, 레이어태그)
HRESULT CObject_Manager::Destory_Front(_uint iLevelIndex, const _tchar * pLayerTag)
{
	//반복자 선언해두어서 처음부터 끝까지 찾아낸다.
	auto	iter = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTagFinder(pLayerTag));
	//레어어(레벨위치에 있는)가 없을 경우
	if (iter == m_pLayers[iLevelIndex].end())
	{
		MSGBOX("Failed to CObject_Manager::Destory_Front");
		return 0;
	}


	/*for (auto& obj : iter->second->Get_Objects())
	{
		Safe_Release(obj);
	}
	auto a = iter->second->Get_Objects();
	a.clear();*/
	

	/*m_pLayers[iLevelIndex].clear();*/
	// 객체를 얻어낸다음 시작주터 지워낸다.erase element at beginning
	/*iter->second->pop();
	Safe_Release(iter->second);

	iter->second->Get_Object().pop_front();*/

	return S_OK;
}

HRESULT CObject_Manager::Destory_Last(_uint iLevelIndex, const _tchar * pLayerTag)
{
	auto	iter = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTagFinder(pLayerTag));

	if (iter == m_pLayers[iLevelIndex].end())
	{
		MSGBOX("Failed to CObject_Manager::Destory_Last");
		return 0;
	}
	// 객체를 얻어낸다음 끝부터 지워낸다.erase element at end
	/*iter->second->Get_Objects().pop_back();*/

	return S_OK;
}
//컨테이너에 담는것(재료: 레벨번호에 따라)
HRESULT CObject_Manager::Reserve_Container(_uint iNumLevels)
{
	m_iNumLevels = iNumLevels;
	//레이어에 동적할당해서[레벨번호에]
	m_pLayers = new LAYERS[m_iNumLevels];

	return S_OK;
}
//원형객체를 추가하는 것(재료(인자): 원형태그, 원행객체)
HRESULT CObject_Manager::Add_Prototype(const _tchar * pPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == pPrototype ||
		nullptr != Find_Prototype(pPrototypeTag))
	{//원향객체나 원형태그를 찾는 작업이 이루어지지 않을 경우
		MSGBOX("Failed to CObject_Manager::Add_Prototype");
		return E_FAIL;
	}
	//원형객체를 선언한 맵컨테이너에 담아낸다. 키값으로 태그, 벨류값으로 객체
	m_Prototypes.insert(PROTOTYPES::value_type(pPrototypeTag, pPrototype));

	return S_OK;
}
//게임객체를 추가하는것(재료: 레벨위치, 레이어태그, 원행태그, 보이드포인터)
HRESULT CObject_Manager::Add_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pPrototypeTag, void* pArg)
{//레벨위치가 다른위치보다 크면 
	if (iLevelIndex >= m_iNumLevels)
	{//아예추가 못하게 막아두고
		MSGBOX("Failed to CObject_Manager::Add_GameObject");
		return E_FAIL;
	}
	//원행객체를 선언해서 원형객체들을 찾아낸고(재료를 태그를 쓴다.)
	CGameObject*	pPrototype = Find_Prototype(pPrototypeTag);
	if (nullptr == pPrototype)
	{
		MSGBOX("Failed to CObject_Manager::Find_Prototype");
		return E_FAIL;
	}
	//사본객체를 선언해서 사본객체는 보이드포인터를 활용한 클론함수를 선언해서 얻어낸다.
	CGameObject*	pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
	{
		MSGBOX("Failed to CObject_Manager::Clone");
		return E_FAIL;
	}
	//레이어를 찾아낸다.(레벨인덱스와 레어어태그를 인자로하는)
	CLayer*		pLayer = Find_Layer(iLevelIndex, pLayerTag);
	//레이어가 없을경우
	if (nullptr == pLayer)
	{
		//레어어를 만들고
		pLayer = CLayer::Create();
		if (nullptr == pLayer)
		{
			MSGBOX("Failed to CObject_Manager::CLayer::Create()");
			return E_FAIL;
		}
		//레이어에 게임객체추가함수를 사용한다.
		if (FAILED(pLayer->Add_GameObject(pGameObject)))
		{
			MSGBOX("Failed to CObject_Manager::CLayer::Add_GameObject(pGameObject)");
			return E_FAIL;
		}
		//레벨위치에 따른 레이어들을 넣어준다.
		m_pLayers[iLevelIndex].insert(LAYERS::value_type(pLayerTag, pLayer));
	}
	else
	{
		//있을경우 레이어에 게임객체를 추가해둔다.
		if (FAILED(pLayer->Add_GameObject(pGameObject)))
		{
			MSGBOX("Failed to CObject_Manager::CLayer::Add_GameObject(pGameObject)");
			return E_FAIL;
		}
	}

	return S_OK;
}
//레벨인덱스를 인자로 받는 클리어레벨객체함수이다.
HRESULT CObject_Manager::Clear_LevelObject(_uint iLevelIndex)
{
	//레벨인덱스가 (기준)레벨보다 크면
	if (iLevelIndex >= m_iNumLevels)
	{
		MSGBOX("Failed to CObject_Manager::Clear_LevelObject");
		return E_FAIL;
	}
	//반복문을 사용한다.그리고 해당레벨에 있는 맵컨테이너로화한 레이어의 벨류값들을 릴리즈함다.
	for (auto& Pair : m_pLayers[iLevelIndex])
		Safe_Release(Pair.second);
	//그리고 그 레이어들을 클리어로 메모리 // erase all
	m_pLayers[iLevelIndex].clear();

	return S_OK;
}
//레이어오브젝트를 릴리즈한다.()
HRESULT CObject_Manager::Release_LayerObject(_uint iLevelIndex, const _tchar * pLayerTag)
{
	if (iLevelIndex >= m_iNumLevels)
	{
		MSGBOX("Failed to Release_LayerObject");
		return E_FAIL;
	}
	
	auto	iter = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTagFinder(pLayerTag));
	Safe_AddRef(iter->second);
	if (iter == m_pLayers[iLevelIndex].end())
		return E_FAIL;

	Safe_Release(iter->second);

	return S_OK;
}

_int CObject_Manager::Tick(_double TimeDelta)
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			_int	iTickResult = 0;
			iTickResult = Pair.second->Tick(TimeDelta);
			if (-2 >= iTickResult)
			{
				break;
			}
			else if (0 > iTickResult)
			{
				MSGBOX("CObject_Manager::Tick : Failed to Gameobject Tick");
				return -1;
			}
		}
	}
	return 0;
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
//원행객체를 찾는 함수(재료 : 태그를 이용한)
CGameObject * CObject_Manager::Find_Prototype(const _tchar * pPrototypeTag)
{//반복자를 사용해서 처음부터 끝까지 찾아낸다.
	auto	iter = find_if(m_Prototypes.begin(), m_Prototypes.end(), CTagFinder(pPrototypeTag));
	//그 끝에 도달할경우(원하는 객체가 나오지 않을 경우) 널로 반환
	if (iter == m_Prototypes.end())
		return nullptr;
	//나오면 그 벨류값(객체)를 반환한다.
	return iter->second;
}
//파인드레이어(레벨위치, 레이어태그)
CLayer * CObject_Manager::Find_Layer(_uint iLevelIndex, const _tchar * pLayerTag)
{
	auto	iter = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTagFinder(pLayerTag));
	//각 레이어 태그를 처음부터 끝까지 찾아내는 반복자를 선언한다.
	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;//반복자가 끝나는위치라면 없는놈으로 선언한다.
	//맵컨테이너에서는 키값과 벨류값이 있으는데 키값을 접근하고 싶으면 first, 벨류값을 접근하고 싶으면 secoud로 처리한다.
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


