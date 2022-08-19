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
	//����Ʈȭ�� ��ü�� �ٸ� Ŭ������ �ٸ� ������ ���� ���ؼ� get�Լ� �̿��Ѵ�.
	//list<class CGameObject*>		Get_Object() { return m_Objects; }
	const list<CGameObject*>&		Get_Objects() const { return m_Objects; }
	CGameObject*						Get_LayerObject(_uint iIndex);
	/*void		pop() { m_Objects.pop_back(); }*/

private:
	//��ü���� ����Ʈ �����̳ʸ� �̿��ؼ� �����Ѵ�.
	list<CGameObject*>			m_Objects;
	typedef list<CGameObject*>	OBJECTS;

public:
	static CLayer* Create();
	virtual void Free() override;
};

END