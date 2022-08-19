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

class CMonster_OilTruck final : public CLandObject
{
public:
	enum CARSTATE { STATE_NONE, STATE_IDLE, STATE_EXIT };

public:
	typedef struct tagOilTruck
	{
		_float3		vAxis;
		_float		fAngle;
		_float4		vPosition;
		_float3		vScale;
		_float		fSpeed;
	}OilTruck;

private:
	//생성자와 소멸자이며
	CMonster_OilTruck(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CMonster_OilTruck(const CMonster_OilTruck& rhs);
	virtual ~CMonster_OilTruck() = default;

public:
	virtual HRESULT	NativeConstruct_Prototype();//원형객체이기에 상속관계 명확하게 하면 따로 할 것 없다
	virtual HRESULT NativeConstruct(void* pArg);//사본객체는 필요한 스테이지에 등장해서 어떻게 생성하는 방법에 가깝다
	virtual _int Tick(_double TimeDelta);//그 물체가 매 프레임을 돌때마다 어떤행동을 취해주면 좋은가
	virtual _int LateTick(_double TimedDelta);//매 프레임마다 어떤영향??을 주고 받는지 표현하면 된다.
	virtual HRESULT	Render();//우리가 그리고자하는 물체가 어떤형식으로 그려지면 좋은가

private:
	//그리고자하는 물체의 그리는 도구들은 컴포넌트형식을 가지고 있다
	//클래스로 포인터 줘서 변수처럼 사용한다.
	CShader*		m_pShaderCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CModel*			m_pModelCom = nullptr;
	CCollider*		m_pAABBCom = nullptr;

public:
	//내가 쓰고자하는 함수들이다.
	void		Clear_Monster_Car();
private:
	//==========컴포넌트셋팅,쉐이더셋팅=============//
	HRESULT		SetUp_Components();
	HRESULT		SetUp_ConstantTable();

private:
	//내가 쓰고자하는 함수에서 쓰이는 변수들을 모아둔 곳이다.
	_bool		m_bExitCheck = false;
	_double		m_fTimeFrame = 0.f;
	CTimer*		m_pTimeExit = nullptr;
	_double		m_dCoolTimeToStartPostition = 0.f;
	_double		m_dMaxTimeToStartPostition = 0.f;
	OilTruck	m_tOilTruck;

public:
	static CMonster_OilTruck*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void Free() override;
};

END