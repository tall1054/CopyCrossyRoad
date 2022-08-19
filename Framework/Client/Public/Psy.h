#pragma once

#include "Client_Defines.h"
#include "LandObject.h"

BEGIN(Engine)
class CTransform;
class CShader;
class CRenderer;
class CCollider;
class CModel;//CVIBufferRect客 悼老茄 开且 荐青
END

BEGIN(Client)
class CStateComponent;

class CPsy final : public CLandObject
{
public:
	enum { PLAYER_IDLE, PLAYER_COLL ,PLAYER_DEAD, PLAYER_END };

public:
	typedef struct tagPsyPosition
	{
		_float3		vAxis;
		_float		fAngle;
		_float4		vPosition;
		_float3		vScale;
	}PsyPosition;

private:
	CPsy(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CPsy(const CPsy& rhs);
	virtual ~CPsy() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

public:
	_uint	Get_WalkCount() const { return m_iWalkCut; }
	_uint	Get_CoinValue() const { return m_fCoinValue; }

private:
	CTransform*				m_pTransformCom = nullptr;//
	CShader*				m_pShaderCom = nullptr;//
	CRenderer*				m_pRendererCom = nullptr;//
	CModel*					m_pModelCom = nullptr;//
	CCollider*				m_pAABBCom = nullptr;//
	CCollider*				m_pOBBCom = nullptr;//
	CCollider*				m_pSphereCom = nullptr;//
	CStateComponent*		m_pStateCom = nullptr;//

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();

private:
	HRESULT			Take_Item(ITEM eItem, _float fItemValue);

private:
	_uint			m_iWalkCut = 0;
	_float			fTimeAcc = 0.f;
	_bool			Jump = false;
	PsyPosition		m_tPsyPos;
	_uint			m_fCoinValue = 0;

public:
	static CPsy* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END