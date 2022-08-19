#include "..\Public\Layer.h"
#include "GameObject.h"

CLayer::CLayer()
{
}
//컴폰너트를 얻어내는 함수(컴포넌트 태그와 인덱스(객체들의 개수를 수로 표현)를 인자로 한다.)
CComponent * CLayer::Get_Component(const _tchar * pComponentTag, _uint iIndex)
{
	//인덱스가 객체의 크기보다 크면 return length of sequence
	if (iIndex >= m_Objects.size())
	{
		MSGBOX("CLayer::Get_Component : Out of range");
		return nullptr;
	}
	//객체들의 시작부터 반복자로 지정해서
	auto	iter = m_Objects.begin();
	//반복자들을 반복문을 사용해서 넣어주고
	for (_uint i = 0; i < iIndex; ++i)
		++iter;
	//이 반복자들을 컴포넌트를 겟함수를 이용해서 넣어준다.
	return (*iter)->Get_Component(pComponentTag);
}
//게임객체를 인자로하는 애드 게임객체함수
HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	//객체들을 끝부터 넣어둔다.insert element at end
	m_Objects.push_back(pGameObject);

	return S_OK;
}

_int CLayer::Tick(_double TimeDelta)
{
	/*for (auto& pGameObject : m_Objects)
	{
		if (0 > pGameObject->Tick(TimeDelta))
			return -1;
	}*/

	for (auto& iter = m_Objects.begin(); iter != m_Objects.end();)
	{
		_int	iResult = 0;
		iResult = (*iter)->Tick(TimeDelta);
		if (-2 == iResult)
		{
			return -2;
		}
		if (0 > iResult)
		{
			return -1;
		}

		if ((*iter)->Get_Delete())
		{
			Safe_Delete(*iter);
			iter = m_Objects.erase(iter);
			continue;
		}
		++iter;
	}
	return 0;
}

_int CLayer::LateTick(_double TimeDelta)
{
	/*for (auto& pGameObject : m_Objects)
	{
		if (0 > pGameObject->LateTick(TimeDelta))
			return -1;
	}*/
	for (auto& iter = m_Objects.begin(); iter != m_Objects.end();)
	{

		if (!(*iter)->Get_Delete() && 0 > (*iter)->LateTick(TimeDelta))
			return -1;

		if ((*iter)->Get_Delete())
		{
			Safe_Release(*iter);
			iter = m_Objects.erase(iter);
			continue;
		}
		++iter;
	}
	return 0;
}
//레이어오브젝트를 얻너내는 함수
CGameObject * CLayer::Get_LayerObject(_uint iIndex)
{
	//객체의 수가 0보다적거나 아니면 그 사이즈 이상일 경우
	if (0 > iIndex || iIndex >= m_Objects.size())
	{
		MSGBOX("CLayer::Get_LayerObject : Out of range");
		return nullptr;
	}
	//객체의 처음 주소값을 반복자로 선언한다.
	auto&	iter = m_Objects.begin();
	for (_uint i = 0; i < iIndex; ++i)
		++iter;
	//반복자의 주소를 반환한다.
	return *iter;
}
//레이어를 만드는 함수
CLayer * CLayer::Create()
{
	//동적할당한다.
	CLayer* pLayer = new CLayer();
	//반환다.
	return pLayer;
}

void CLayer::Free()
{
	for (auto& iter : m_Objects)
		Safe_Release(iter);

	m_Objects.clear();
}


