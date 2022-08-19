#include "..\Public\Layer.h"
#include "GameObject.h"

CLayer::CLayer()
{
}
//������Ʈ�� ���� �Լ�(������Ʈ �±׿� �ε���(��ü���� ������ ���� ǥ��)�� ���ڷ� �Ѵ�.)
CComponent * CLayer::Get_Component(const _tchar * pComponentTag, _uint iIndex)
{
	//�ε����� ��ü�� ũ�⺸�� ũ�� return length of sequence
	if (iIndex >= m_Objects.size())
	{
		MSGBOX("CLayer::Get_Component : Out of range");
		return nullptr;
	}
	//��ü���� ���ۺ��� �ݺ��ڷ� �����ؼ�
	auto	iter = m_Objects.begin();
	//�ݺ��ڵ��� �ݺ����� ����ؼ� �־��ְ�
	for (_uint i = 0; i < iIndex; ++i)
		++iter;
	//�� �ݺ��ڵ��� ������Ʈ�� ���Լ��� �̿��ؼ� �־��ش�.
	return (*iter)->Get_Component(pComponentTag);
}
//���Ӱ�ü�� ���ڷ��ϴ� �ֵ� ���Ӱ�ü�Լ�
HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	//��ü���� ������ �־�д�.insert element at end
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
//���̾������Ʈ�� ��ʳ��� �Լ�
CGameObject * CLayer::Get_LayerObject(_uint iIndex)
{
	//��ü�� ���� 0�������ų� �ƴϸ� �� ������ �̻��� ���
	if (0 > iIndex || iIndex >= m_Objects.size())
	{
		MSGBOX("CLayer::Get_LayerObject : Out of range");
		return nullptr;
	}
	//��ü�� ó�� �ּҰ��� �ݺ��ڷ� �����Ѵ�.
	auto&	iter = m_Objects.begin();
	for (_uint i = 0; i < iIndex; ++i)
		++iter;
	//�ݺ����� �ּҸ� ��ȯ�Ѵ�.
	return *iter;
}
//���̾ ����� �Լ�
CLayer * CLayer::Create()
{
	//�����Ҵ��Ѵ�.
	CLayer* pLayer = new CLayer();
	//��ȯ��.
	return pLayer;
}

void CLayer::Free()
{
	for (auto& iter : m_Objects)
		Safe_Release(iter);

	m_Objects.clear();
}


