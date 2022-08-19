#pragma once

#include "Client_Defines.h"
#include "LandObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CCollider;
class CModel;
END

BEGIN(Client)

class CRoad final : public CLandObject
{
public:
	typedef struct tagRoadPosition
	{
		_float4		vPosition;
		_float3		vAxis;
		_float		fAngle;
		_float3		vScale;
	}RoadPosition;

private:
	CRoad(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CRoad(const CRoad& rhs);
	virtual ~CRoad() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModelCom = nullptr;
	CCollider*				m_pAABBCom = nullptr;
	CCollider*				m_pOBBCom = nullptr;
	CCollider*				m_pSphereCom = nullptr;

private:
	RoadPosition			m_tRoadPos;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();

public:
	static CRoad* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END