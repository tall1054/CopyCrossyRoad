#include "stdafx.h"
#include "..\Public\Monster_Car.h"
#include "GameInstance.h"

CMonster_Car::CMonster_Car(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CLandObject(pDevice, pDeviceContext)
{
}

CMonster_Car::CMonster_Car(const CMonster_Car & rhs)
	: CLandObject(rhs)
{
}

HRESULT CMonster_Car::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to CMonster_Car NativeConstruct_Prototype");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CMonster_Car::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(&CTransform::TRANSFORMDESC(3.f, XMConvertToRadians(90.f)))))
	{
		MSGBOX("Failed tor CMonster_Car NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
	{
		MSGBOX("Failed to CMonster_Car SetUP_Components");
		return E_FAIL;
	}

	//===========�ڵ����� ũ��� �� Ư�� ��ġ�� �������ش�.================//
	//_float3		fCarScale = _float3(0.15f, 0.1f, 0.15f);
	//m_pTransform->Scaled(XMLoadFloat3(&fCarScale));

	if (nullptr != pArg)
		memcpy(&m_tCarPos, pArg, sizeof(CarPosition));

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_tCarPos.vPosition.x, m_tCarPos.vPosition.y, m_tCarPos.vPosition.z, m_tCarPos.vPosition.w));
	m_pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(m_tCarPos.fAngle));
	m_pTransform->Scaled(XMLoadFloat3(&m_tCarPos.vScale));
	m_pTransform->Set_TransformDesc(CTransform::TRANSFORMDESC(m_tCarPos.fSpeed));


	return S_OK;
}

_int CMonster_Car::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	m_fTimeFrame += (_float)TimeDelta;

	m_pAABBCom->Update_Transform(m_pTransform->Get_WorldMatrix());

	m_pTransform->Go_Right(TimeDelta);
	//while������ ���� �ݺ�ó���صθ� �ݺ��������� �����ð��� �����ϸ�
	while (m_fTimeFrame > 15.f)
	{
		//Ż��ó���� �صΰ� �θ�
		if (m_bExitCheck = true)
		{
			//Ż����ġ�� ���󺹱��� �� ��ġ�� �ٲپ� �д�.
			//�׸��� �׿� ���� �ð��� 0���� �ʱ�ȭ�صд�.
			m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_tCarPos.vPosition.x, m_tCarPos.vPosition.y, m_tCarPos.vPosition.z, m_tCarPos.vPosition.w));
			m_fTimeFrame = 0.f;
		}
		break;
	}
	return _int();
}

_int CMonster_Car::LateTick(_double TimedDelta)
{
	if (0 > __super::LateTick(TimedDelta))
		return -1;

	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	//__super::SetUp_OnTerrain(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), 0, TEXT("Com_VIBuffer"));

	m_pAABBCom->Collision_AABB((CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_AABB")));

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

	RELEASE_INSTANCE(CGameInstance);

	//�ð��� ������ ��ü�� ������� �ٽ� ����ġ�� �����.
	return _int();
}

HRESULT CMonster_Car::Render()
{
	if (nullptr == m_pModelCom)
	{
		MSGBOX("Failed to Cmonster_Car Render_Model");
		return E_FAIL;
	}

	if (FAILED(SetUp_ConstantTable()))
	{
		MSGBOX("Failed to Cmonster_Car SetUp_constantTable");
		return E_FAIL;
	}

	_uint	iNumMaterials = m_pModelCom->Get_NumMaterials();

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture");
		m_pModelCom->Render(m_pShaderCom, 0, i, "g_BoneMatrices");
	}

	//������϶� �浹ü �����д�.
#ifdef _DEBUG
	//m_pAABBCom->Render();
#endif // _DEBUG

	return S_OK;
}

void CMonster_Car::Clear_Monster_Car()
{
	
}

HRESULT CMonster_Car::SetUp_Components()
{
	//������ ������Ʈ
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("Failed to CMonster_Car Prototype_Component_Renderer");
		return E_FAIL;
	}

	//���̴� ������Ʈ?? �������� ����ƽ�����̰� ���̴��� �����÷����̴�. �� ���� �˾ƺ���
	if(FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_NonAnimModel"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
	{
		MSGBOX("Failed to CMonster_Car Prototype_Component_Shader_NonAnimModel");
		return E_FAIL;
	}

	//�� ������Ʈ
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster_Car"),
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
	{
		MSGBOX("Failed to CMonster_Car Prototype_Component_Model_Monster_Car");
		return E_FAIL;
	}

	CCollider::COLLIDERDESC		ColliderDesc;
	//AABB�浹 ������Ʈ
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(10.f, 10.f, 5.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f ,1.f);
	ColliderDesc.vTranslation = _float4(0.f, 0.7f, 0.f, 1.f);


	//�ٸ� �ֵ�������Ʈ�� �ٸ��� �ݶ��̴��� ����: �ݶ��̴� ��ũ���� �� �߰��ؼ� �ֵ� ������Ʈ�� �ϼ���Ų�� ������??
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster_Car::SetUp_ConstantTable()
{
	//�� �Լ��� ����� �˾ƺ��� �Ѵ�.
	//�����ϴ� ���߿� �ν��Ͻ��� ������ ���� ��찡 �߻��ߴ�. �׷� ������ ���ΰ�?
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransform->Bind_OnShader(m_pShaderCom, "g_WorldMatrix")))
	{
		MSGBOX("Failed to CPlayer Render_Bind_OnShader(World)");
		return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
	{
		MSGBOX("Failed to CPlayer  Render_Set_RawValue(View)");
		return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
	{
		MSGBOX("Failed to CPlayer Render_Set_RawValue(Proj)");
		return E_FAIL;
	}

	//�� ����ü�� �����̴�. ���� ����ü�� 0��°�� ����ü�� ���� ����������� �����Ѵ�. �ڼ��Ѱ� �𸣴� �����
	const LIGHTDESC* pLightDesc = pGameInstance->Get_LightDesc(0);

	if (nullptr == pLightDesc)
		return E_FAIL;

	//���̴����� ���� ��ġ�� ����.
	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightPos", &pLightDesc->vPosition, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vCamPosition", &pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CMonster_Car * CMonster_Car::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CMonster_Car*	pInstance = new CMonster_Car(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CMonster_Car");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster_Car::Clone(void * pArg)
{
	CMonster_Car*	pInstance = new CMonster_Car(*this);

	if(FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CMonster_Car");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Car::Free()
{
	__super::Free();

	Safe_Release(m_pAABBCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
