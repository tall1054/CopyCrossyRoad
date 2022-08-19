#include "..\Public\Object_Manager.h"
//#include "GameObject.h"
//#include "Layer.h"

IMPLEMENT_SINGLETON(CObject_Manager)

CObject_Manager::CObject_Manager()
{
}
//������Ʈ�� ��� �Լ�(���: ������ġ, ���̾��±�(�������� �ִ� ���̾ü�ǹ�), ������Ʈ�±�, �ε���(��))
CComponent * CObject_Manager::Get_Component(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pComponentTag, _uint iIndex)
{
	if (iLevelIndex >= m_iNumLevels)//������ġ�� ��緹���� ������Ŭ���
	{
		MSGBOX("Failed to CObject_Manager::Get_Component");
		return nullptr;//���� ������ ����Ѵ�.
	}
	//���̾ ����=���̾ ã�� �Լ�(�𸨴ϴ� : ������������ ����)(�����ε�����, ���̾��±׷�)
	CLayer*		pLayer = Find_Layer(iLevelIndex, pLayerTag);
	if (nullptr == pLayer)
	{
		MSGBOX("Failed to CObject_Manager::Find_Layer");
		return nullptr;//������ ���̾ ���ٰ� ������
	}
	//������ �������� ������Ʈ�� ���� �Լ��� ó���Ѵ�.
	return pLayer->Get_Component(pComponentTag, iIndex);
}
//���̾ ���� �Լ�(���: �����ε���, ���̾��±�)
CLayer * CObject_Manager::Get_Layer(_uint iLevelIndex, const _tchar * pLayerTag)
{
	//�ݺ��ڸ� ����ؼ� ���̾��� ����(������ġ�� �ִ�)���ͳ����� �ݺ��ڷ� ����
	auto	iter = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTagFinder(pLayerTag));
	//�� ���� �����Ͽ� 
	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;//���ٰ� ��ȯ���ش�.
	//�׸��� �� ���̾��� �������� ��ȯ���ش�.
	return iter->second;
}
//���� ��ü�� ���� �Լ�(���: ������ġ�� ���̾��±�)
CGameObject * CObject_Manager::Get_Front(_uint iLevelIndex, const _tchar * pLayerTag)
{
	//�ݺ��ڸ� �̿��ؼ� ���ۺ��� ������ ������ġ���� �±׸� ����.
	auto	iter = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTagFinder(pLayerTag));
	//�� ���̾ ���� ��� �η� �����Ѵ�.
	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;
	//������� �������� �װ�ü�� ó������ ��ȯ��Ų��.
	return iter->second->Get_Objects().front();
}
//�Ĺ水ü�� ���� �Լ�
CGameObject * CObject_Manager::Get_Back(_uint iLevelIndex, const _tchar * pLayerTag)
{
	//�ݺ��ڸ� �̿��ؼ� ���ۺ��� ������ ������ġ���� �±׸� ����.
	auto	iter = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTagFinder(pLayerTag));
	//�� ���̾ ���� ��� �η� �����Ѵ�.
	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;
	//������� �������� �װ�ü�� ���������� ��ȯ��Ų��.
	return iter->second->Get_Objects().back();
}
//���Ӱ�ü�� ����Ʈ�� ���� �Լ�(��� : ������ġ, ���̾��±�, �ݺ����� ��(��:))
CGameObject * CObject_Manager::Get_ListOfGameObject(_uint iLevelIndex, const _tchar * pLayerTag, int iterNum)
{
	//�ݺ��ڸ� �̿��ؼ� �±׸� ã�Ƴ���.
	auto	iter = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTagFinder(pLayerTag));
	//�±׷� �Ҹ��� ģ���� ������ �η� ��ȯ�Ѵ�.
	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;
	//�Ǵٸ� �ݺ��������� ����� �ݺ��ڸ� ���� = �ݺ��ڰ� �������� ���� �� �������� ó����ġ�̴�.
	auto	List_iter = iter->second->Get_Objects().front();
	//�ݺ����� �̿��ؼ�
	for (int i = 0; i < iterNum; ++i)
	{
		//�ݺ����� ����� �߰��߰����ش�.
		++List_iter;
	}

	return List_iter;
}
//Ŭ���� ���� �Լ�(������ü�±�, �ݺ����� ���̵�������)
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
//��ü���̾��Լ�(������ġ, ���̾��±�)
CLayer * CObject_Manager::Get_ObjectLayer(_uint iLevelIndex, const _tchar * pLayerTag)
{
	auto	iter = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTagFinder(pLayerTag));

	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;

	return iter->second;
}
//���������Լ�(������ġ, ���̾��±�)
HRESULT CObject_Manager::Destory_Front(_uint iLevelIndex, const _tchar * pLayerTag)
{
	//�ݺ��� �����صξ ó������ ������ ã�Ƴ���.
	auto	iter = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTagFinder(pLayerTag));
	//�����(������ġ�� �ִ�)�� ���� ���
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
	// ��ü�� ������ �������� ��������.erase element at beginning
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
	// ��ü�� ������ ������ ��������.erase element at end
	/*iter->second->Get_Objects().pop_back();*/

	return S_OK;
}
//�����̳ʿ� ��°�(���: ������ȣ�� ����)
HRESULT CObject_Manager::Reserve_Container(_uint iNumLevels)
{
	m_iNumLevels = iNumLevels;
	//���̾ �����Ҵ��ؼ�[������ȣ��]
	m_pLayers = new LAYERS[m_iNumLevels];

	return S_OK;
}
//������ü�� �߰��ϴ� ��(���(����): �����±�, ���ఴü)
HRESULT CObject_Manager::Add_Prototype(const _tchar * pPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == pPrototype ||
		nullptr != Find_Prototype(pPrototypeTag))
	{//���ⰴü�� �����±׸� ã�� �۾��� �̷������ ���� ���
		MSGBOX("Failed to CObject_Manager::Add_Prototype");
		return E_FAIL;
	}
	//������ü�� ������ �������̳ʿ� ��Ƴ���. Ű������ �±�, ���������� ��ü
	m_Prototypes.insert(PROTOTYPES::value_type(pPrototypeTag, pPrototype));

	return S_OK;
}
//���Ӱ�ü�� �߰��ϴ°�(���: ������ġ, ���̾��±�, �����±�, ���̵�������)
HRESULT CObject_Manager::Add_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pPrototypeTag, void* pArg)
{//������ġ�� �ٸ���ġ���� ũ�� 
	if (iLevelIndex >= m_iNumLevels)
	{//�ƿ��߰� ���ϰ� ���Ƶΰ�
		MSGBOX("Failed to CObject_Manager::Add_GameObject");
		return E_FAIL;
	}
	//���ఴü�� �����ؼ� ������ü���� ã�Ƴ���(��Ḧ �±׸� ����.)
	CGameObject*	pPrototype = Find_Prototype(pPrototypeTag);
	if (nullptr == pPrototype)
	{
		MSGBOX("Failed to CObject_Manager::Find_Prototype");
		return E_FAIL;
	}
	//�纻��ü�� �����ؼ� �纻��ü�� ���̵������͸� Ȱ���� Ŭ���Լ��� �����ؼ� ����.
	CGameObject*	pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
	{
		MSGBOX("Failed to CObject_Manager::Clone");
		return E_FAIL;
	}
	//���̾ ã�Ƴ���.(�����ε����� ������±׸� ���ڷ��ϴ�)
	CLayer*		pLayer = Find_Layer(iLevelIndex, pLayerTag);
	//���̾ �������
	if (nullptr == pLayer)
	{
		//���� �����
		pLayer = CLayer::Create();
		if (nullptr == pLayer)
		{
			MSGBOX("Failed to CObject_Manager::CLayer::Create()");
			return E_FAIL;
		}
		//���̾ ���Ӱ�ü�߰��Լ��� ����Ѵ�.
		if (FAILED(pLayer->Add_GameObject(pGameObject)))
		{
			MSGBOX("Failed to CObject_Manager::CLayer::Add_GameObject(pGameObject)");
			return E_FAIL;
		}
		//������ġ�� ���� ���̾���� �־��ش�.
		m_pLayers[iLevelIndex].insert(LAYERS::value_type(pLayerTag, pLayer));
	}
	else
	{
		//������� ���̾ ���Ӱ�ü�� �߰��صд�.
		if (FAILED(pLayer->Add_GameObject(pGameObject)))
		{
			MSGBOX("Failed to CObject_Manager::CLayer::Add_GameObject(pGameObject)");
			return E_FAIL;
		}
	}

	return S_OK;
}
//�����ε����� ���ڷ� �޴� Ŭ�������ü�Լ��̴�.
HRESULT CObject_Manager::Clear_LevelObject(_uint iLevelIndex)
{
	//�����ε����� (����)�������� ũ��
	if (iLevelIndex >= m_iNumLevels)
	{
		MSGBOX("Failed to CObject_Manager::Clear_LevelObject");
		return E_FAIL;
	}
	//�ݺ����� ����Ѵ�.�׸��� �ش緹���� �ִ� �������̳ʷ�ȭ�� ���̾��� ���������� �������Դ�.
	for (auto& Pair : m_pLayers[iLevelIndex])
		Safe_Release(Pair.second);
	//�׸��� �� ���̾���� Ŭ����� �޸� // erase all
	m_pLayers[iLevelIndex].clear();

	return S_OK;
}
//���̾������Ʈ�� �������Ѵ�.()
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
//���ఴü�� ã�� �Լ�(��� : �±׸� �̿���)
CGameObject * CObject_Manager::Find_Prototype(const _tchar * pPrototypeTag)
{//�ݺ��ڸ� ����ؼ� ó������ ������ ã�Ƴ���.
	auto	iter = find_if(m_Prototypes.begin(), m_Prototypes.end(), CTagFinder(pPrototypeTag));
	//�� ���� �����Ұ��(���ϴ� ��ü�� ������ ���� ���) �η� ��ȯ
	if (iter == m_Prototypes.end())
		return nullptr;
	//������ �� ������(��ü)�� ��ȯ�Ѵ�.
	return iter->second;
}
//���ε巹�̾�(������ġ, ���̾��±�)
CLayer * CObject_Manager::Find_Layer(_uint iLevelIndex, const _tchar * pLayerTag)
{
	auto	iter = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTagFinder(pLayerTag));
	//�� ���̾� �±׸� ó������ ������ ã�Ƴ��� �ݺ��ڸ� �����Ѵ�.
	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;//�ݺ��ڰ� ��������ġ��� ���³����� �����Ѵ�.
	//�������̳ʿ����� Ű���� �������� �����µ� Ű���� �����ϰ� ������ first, �������� �����ϰ� ������ secoud�� ó���Ѵ�.
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


