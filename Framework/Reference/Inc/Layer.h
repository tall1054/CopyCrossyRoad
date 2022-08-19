#pragma once

#include "Base.h"

BEGIN(Engine)

class CGameObject;
class CComponent;
class CLayer final : public CBase
{
public:
	CLayer();
	virtual ~CLayer() = default;

public:
	HRESULT Add_GameObject(CGameObject* pGameObject);
	_int Tick(_double TimeDelta);
	_int LateTick(_double TimeDelta);

public:
	CComponent*							Get_Component(const _tchar* pComponentTag, _uint iIndex);
	//리스트화한 객체를 다른 클래스나 다른 곳에서 얻어내기 위해서 get함수 이용한다.
	//list<class CGameObject*>		Get_Object() { return m_Objects; }
	const list<CGameObject*>&		Get_Objects() const { return m_Objects; }
	CGameObject*						Get_LayerObject(_uint iIndex);
	/*void		pop() { m_Objects.pop_back(); }*/

private:
	//객체들을 리스트 컨터이너를 이용해서 정렬한다.
	list<CGameObject*>			m_Objects;
	typedef list<CGameObject*>	OBJECTS;

public:
	static CLayer* Create();
	virtual void Free() override;
};

END