#pragma once

#include "Base.h"
#include "GameObject.h"
#include "Layer.h"

/* ��ü���� ������ �����ؼ� ��Ƴ��´�. */
/* ������ ã�Ƽ� ��������, ��Ƽ� �����Ѵ�. */

BEGIN(Engine)

class CObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CObject_Manager)
public:
	CObject_Manager();
	virtual ~CObject_Manager() = default;
public:
	class CComponent* Get_Component(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex);

public:
	//������Ƽ�� ��ȯ�Ǵ� �ٻ������ ������ġ�� ���̾��±׸� ����(�����)�޾� ����Ͽ� ��ü�� ����� �����Ͽ� ����
	size_t			Get_Size(_uint iLevelIndex, const _tchar* pLayerTag) { return Find_Layer(iLevelIndex, pLayerTag)->Get_Objects().size(); }
	CLayer*			Get_Layer(_uint iLevelIndex, const _tchar* pLayerTag);
	CGameObject*	Get_Front(_uint iLevelIndex, const _tchar* pLayerTag);
	CGameObject*	Get_Back(_uint iLevelIndex, const _tchar* pLayerTag);
	CGameObject*	Get_ListOfGameObject(_uint iLevelIndex, const _tchar* pLayerTag, int iterNum);
	CGameObject*	Get_Clone(const _tchar* pPrototypeTag, void* pArg);
	//Get_Layer�� ������ ����̴�. �����ص���
	CLayer*			Get_ObjectLayer(_uint iLevelIndex, const _tchar* pLayerTag);
	HRESULT			Destory_Front(_uint iLevelIndex, const _tchar* pLayerTag);
	HRESULT			Destory_Last(_uint iLevelIndex, const _tchar* pLayerTag);

public:
	HRESULT Reserve_Container(_uint iNumLevels);
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype); /* ������ü�� �߰��Ѵ�. */
	HRESULT Add_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, void* pArg); /* �纻(���ӳ��� �����ؾ��� ��ü) ��ü�� �߰��Ѵ�. */

public:
	HRESULT Clear_LevelObject(_uint iLevelIndex);
	HRESULT	Release_LayerObject(_uint iLevelIndex, const _tchar* pLayerTag);

public:
	_int Tick(_double TimeDelta);
	_int LateTick(_double TimeDelta);

private:
	map<const _tchar*, class CGameObject*>			m_Prototypes;
	typedef map<const _tchar*, class CGameObject*>	PROTOTYPES;

private:
	map<const _tchar*, class CLayer*>*			m_pLayers = nullptr;
	typedef map<const _tchar*, class CLayer*>	LAYERS;

	_uint			m_iNumLevels = 0;

private:
	class CGameObject* Find_Prototype(const _tchar* pPrototypeTag);
	class CLayer* Find_Layer(_uint iLevelIndex, const _tchar* pLayerTag);

public:
	virtual void Free() override;
};

END