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
	//�����ڿ� �Ҹ����̸�
	CMonster_OilTruck(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CMonster_OilTruck(const CMonster_OilTruck& rhs);
	virtual ~CMonster_OilTruck() = default;

public:
	virtual HRESULT	NativeConstruct_Prototype();//������ü�̱⿡ ��Ӱ��� ��Ȯ�ϰ� �ϸ� ���� �� �� ����
	virtual HRESULT NativeConstruct(void* pArg);//�纻��ü�� �ʿ��� ���������� �����ؼ� ��� �����ϴ� ����� ������
	virtual _int Tick(_double TimeDelta);//�� ��ü�� �� �������� �������� ��ൿ�� �����ָ� ������
	virtual _int LateTick(_double TimedDelta);//�� �����Ӹ��� �����??�� �ְ� �޴��� ǥ���ϸ� �ȴ�.
	virtual HRESULT	Render();//�츮�� �׸������ϴ� ��ü�� ��������� �׷����� ������

private:
	//�׸������ϴ� ��ü�� �׸��� �������� ������Ʈ������ ������ �ִ�
	//Ŭ������ ������ �༭ ����ó�� ����Ѵ�.
	CShader*		m_pShaderCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CModel*			m_pModelCom = nullptr;
	CCollider*		m_pAABBCom = nullptr;

public:
	//���� �������ϴ� �Լ����̴�.
	void		Clear_Monster_Car();
private:
	//==========������Ʈ����,���̴�����=============//
	HRESULT		SetUp_Components();
	HRESULT		SetUp_ConstantTable();

private:
	//���� �������ϴ� �Լ����� ���̴� �������� ��Ƶ� ���̴�.
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