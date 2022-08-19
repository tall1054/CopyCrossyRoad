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

class CPlayer_Chicken final : public CLandObject
{
public:
	typedef struct tagPlayer_ChickenPosition
	{
		_float3		vAxis;
		_float		fAngle;
		_float4		vPosition;
		_float3		vScale;
	}Player_ChickenPosition;

private:
	CPlayer_Chicken(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CPlayer_Chicken(const CPlayer_Chicken& rhs);
	virtual ~CPlayer_Chicken() = default;

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
	CTransform*				m_pTransformCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModelCom = nullptr;
	CCollider*				m_pAABBCom = nullptr;
	CCollider*				m_pOBBCom = nullptr;
	CCollider*				m_pSphereCom = nullptr;
	CStateComponent*		m_pStateCom = nullptr;//

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();

private:
	HRESULT			CollisionCheck();

private:
	HRESULT			Take_Item(ITEM eItem, _float fItemValue);

private:
	_uint			m_iWalkCut = 0;
	_float			fTimeAcc = 0.f;
	_bool			Jump = false;
	_uint			m_fCoinValue = 0;

	Player_ChickenPosition		m_tPlayer_ChickenPos;

public:
	static CPlayer_Chicken* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END