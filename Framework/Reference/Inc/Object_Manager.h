#pragma once

#include "Base.h"
#include "GameObject.h"
#include "Layer.h"

/* 객체들의 원형을 생성해서 모아놓는다. */
/* 원형을 찾아서 복제한후, 모아서 관리한다. */

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
	//사이즈티로 반환되는 겟사이즈는 레벨위치와 레이어태그를 인자(제료로)받아 사용하여 객체의 사이즈를 리턴하여 보냄
	size_t			Get_Size(_uint iLevelIndex, const _tchar* pLayerTag) { return Find_Layer(iLevelIndex, pLayerTag)->Get_Objects().size(); }
	CLayer*			Get_Layer(_uint iLevelIndex, const _tchar* pLayerTag);
	CGameObject*	Get_Front(_uint iLevelIndex, const _tchar* pLayerTag);
	CGameObject*	Get_Back(_uint iLevelIndex, const _tchar* pLayerTag);
	CGameObject*	Get_ListOfGameObject(_uint iLevelIndex, const _tchar* pLayerTag, int iterNum);
	CGameObject*	Get_Clone(const _tchar* pPrototypeTag, void* pArg);
	//Get_Layer와 동일한 기능이다. 정리해두자
	CLayer*			Get_ObjectLayer(_uint iLevelIndex, const _tchar* pLayerTag);
	HRESULT			Destory_Front(_uint iLevelIndex, const _tchar* pLayerTag);
	HRESULT			Destory_Last(_uint iLevelIndex, const _tchar* pLayerTag);

public:
	HRESULT Reserve_Container(_uint iNumLevels);
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype); /* 원형객체를 추가한다. */
	HRESULT Add_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, void* pArg); /* 사본(게임내에 출현해야할 객체) 객체를 추가한다. */

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