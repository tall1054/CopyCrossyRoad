#pragma once

#include "Client_Defines.h"
#include "LandObject.h"

BEGIN(Engine)
class CTransform;
class CShader;
class CRenderer;
class CCollider;
class CModel;//CVIBufferRect�� ������ ���� ����
END

BEGIN(Client)

class CPlayer_Koala final : public CLandObject
{
public:
	typedef struct tagPlayer_KoalaPosition
	{
		_float3		vAxis;
		_float		fAngle;
		_float4		vPosition;
		_float3		vScale;
	}Player_KoalaPosition;

private:
	CPlayer_Koala(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CPlayer_Koala(const CPlayer_Koala& rhs);
	virtual ~CPlayer_Koala() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

private:
	CTransform*				m_pTransformCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModelCom = nullptr;
	CCollider*				m_pAABBCom = nullptr;
	CCollider*				m_pOBBCom = nullptr;
	CCollider*				m_pSphereCom = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();

private:
	HRESULT			CollisionCheck();

private:
	_uint			m_iWalkCut = 0;
	_float			fTimeAcc = 0.f;
	_bool			Jump = false;

	Player_KoalaPosition		m_tPlayer_KoalaPos;

public:
	static CPlayer_Koala* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END