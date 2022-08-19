#pragma once

#include "Base.h"
#include "Transform.h"

BEGIN(Engine)

class CComponent;
class ENGINE_DLL CGameObject abstract : public CBase
{
public:
	explicit CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;
	
//==================add==================//
public:
	class CComponent*	Get_Component(const _tchar* pComponentTag);
	//is_destory
	_bool				Get_Delete() { return m_bCheckObjectDelete; }
	void				Set_Delete() { m_bCheckObjectDelete = true; }
	class CTransform*	Get_Transform() { return m_pTransform; }

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(const CTransform::TRANSFORMDESC* pTransformDesc = nullptr);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

public:
	HRESULT Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pComponentTag, CComponent** ppOut, void* pArg = nullptr);
	static const _tchar*	m_pTransformTag;

protected:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;
	class CTransform*		m_pTransform = nullptr;
	//µ“¿« m_bDestoryøÕ µø¿œ
	_bool					m_bCheckObjectDelete = false;

protected:
	CComponent*	Find_Component(const _tchar* pComponentTag);
	//HRESULT		Compute_CamDistance(const CTransform* pTransform, const CPipeLine* pPipeLine);

protected:
	map<const _tchar*, CComponent*>			m_Components;
	typedef map<const _tchar*, CComponent*>	COMPONENTS;


public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END