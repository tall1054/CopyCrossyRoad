#pragma once

#include "Client_Defines.h"
#include "LandObject.h"

BEGIN(Engine)
class CShader;//컴
class CRenderer;//컴
class CCollider;//컴포넌트패턴
class CModel;//컴가ㅓ지고오기 위함
END

BEGIN(Client)

class CTree final : public CLandObject
{
public:
	typedef struct tagTreePosition
	{
		_float3		vAxis;
		_float		fAngle;
		_float4		vPosition;
		_float3		vScale;
	}TreePosition;

private:
	CTree(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CTree(const CTree& rhs);
	virtual ~CTree() = default;

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
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();
	TreePosition			m_tTreePos;

public:
	static CTree* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END