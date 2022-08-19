#pragma once

#include "Client_Defines.h"
#include "LandObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CCollider;
class CModel;
class CTimer;
END

BEGIN(Client)

class CKoreaCar001 final : public CLandObject
{
public:
	enum CARSTATE { STATE_NONE, STATE_IDLE, STATE_EXIT };

public:
	typedef struct tagKorCar1Position
	{
		_float		fAngle;
		_float4		vPosition;
		_float3		vScale;
		_float		fSpeed;
	}KorCar1Position;

private:
	CKoreaCar001(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CKoreaCar001(const CKoreaCar001& rhs);
	virtual ~CKoreaCar001() = default;

public:
	virtual HRESULT	NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT	Render();

private:
	CShader*		m_pShaderCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CModel*			m_pModelCom = nullptr;
	CCollider*		m_pAABBCom = nullptr;

public:
	void		Clear_Monster_Car();
private:
	//==========컴포넌트셋팅,쉐이더셋팅=============//
	HRESULT		SetUp_Components();
	HRESULT		SetUp_ConstantTable();
	//

private:
	_bool		m_bExitCheck = false;

	_double		m_fTimeFrame = 0.f;

	CTimer*		m_pTimeExit = nullptr;
	_double		m_dCoolTimeToStartPostition = 0.f;
	_double		m_dMaxTimeToStartPostition = 0.f;

	KorCar1Position		m_tKorCar1Pos;

public:
	static CKoreaCar001*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void Free() override;
};

END