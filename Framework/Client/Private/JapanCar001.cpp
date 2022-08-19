#include "stdafx.h"
#include "../Public/JapanCar001.h"
#include "GameInstance.h"

CJapanCar001::CJapanCar001(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CLandObject(pDevice, pDeviceContext)
{
}

CJapanCar001::CJapanCar001(const CJapanCar001 & rhs)
	: CLandObject(rhs)
{
}

HRESULT CJapanCar001::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to CJapanCar001 NativeConstruct_Prototype");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CJapanCar001::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(&CTransform::TRANSFORMDESC(3.f, XMConvertToRadians(90.f)))))
	{
		MSGBOX("Failed to CJapanCar001 NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
	{
		MSGBOX("Failed to CJapanCar001 SetUp_Components");
		return E_FAIL;
	}

	_float3		fJapanCar001Scale = _float3(0.1f, 0.1f, 0.1f);
	m_pTransform->Scaled(XMLoadFloat3(&fJapanCar001Scale));

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.f, 16.f, 1.f));
	return S_OK;
}

_int CJapanCar001::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	m_fTimeFrame += TimeDelta;

	m_pAABBCom->Update_Transform(m_pTransform->Get_WorldMatrix());

	m_pTransform->Go_Right(TimeDelta);
	//while문으로 무한 반복처리해두며 반복조건으로 일정시간에 도달하면
	while (m_fTimeFrame > 16.f)
	{
		//탈출처리를 해두게 두며
		if (m_bExitCheck = true)
		{
			//탈추위치는 원상복구가 된 위치로 바꾸어 둔다.
			//그리고 그에 대한 시간을 0으로 초기화해둔다.
			m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.f, 16.f, 1.f));
			m_fTimeFrame = 0.f;
		}
		break;
	}
	return _int();
}

_int CJapanCar001::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	__super::SetUp_OnTerrain(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), 0, TEXT("Com_VIBuffer"));

	m_pAABBCom->Collision_AABB((CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_AABB")));



	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

HRESULT CJapanCar001::Render()
{
	if (nullptr == m_pModelCom)
	{
		MSGBOX("Failed to CJapanCar001 Render_Model");
		return E_FAIL;
	}

	if (FAILED(SetUp_ConstantTable()))
	{
		MSGBOX("Failed to CJapanCar001 SetUp_ConstantTable");
		return E_FAIL;
	}
	_uint		iNumMatrials = m_pModelCom->Get_NumMaterials();

	for (_uint i = 0; i < iNumMatrials; ++i)
	{
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture");
		m_pModelCom->Render(m_pShaderCom, 0, i, "g_BoneMatrices");
	}

#ifdef _DEBUG
	m_pAABBCom->Render();
#endif // _DEBUG

	return S_OK;
}

void CJapanCar001::Clear_Monster_Car()
{
}

HRESULT CJapanCar001::SetUp_Components()
{
	//렌더러 컴포넌트
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("Failed to CMonster_Car Prototype_Component_Renderer");
		return E_FAIL;
	}

	//쉐이더 컴포넌트?? 렌더러는 스테틱영역이고 쉐이더는 레벨플레이이다. 그 차이 알아보자
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_NonAnimModel"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
	{
		MSGBOX("Failed to CMonster_Car Prototype_Component_Shader_NonAnimModel");
		return E_FAIL;
	}

	//모델 컴포넌트
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_CJapanCar001"),
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
	{
		MSGBOX("Failed to CMonster_Car Prototype_Component_Model_CJapanCar001");
		return E_FAIL;
	}

	CCollider::COLLIDERDESC		ColliderDesc;
	//AABB충돌 컴포넌트
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(10.f, 10.f, 5.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float4(0.f, 0.7f, 0.f, 1.f);


	//다른 애드컴포넌트와 다르게 콜라이더는 인자: 콜라이더 디스크립션 더 추가해서 애드 컴포넌트를 완성시킨다 이유는??
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CJapanCar001::SetUp_ConstantTable()
{
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

	//빛 구조체
	const LIGHTDESC* pLightDesc = pGameInstance->Get_LightDesc(0);

	if (nullptr == pLightDesc)
		return E_FAIL;

	//쉐이더에서 빛의 위치를 얻어낸다.
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

CJapanCar001 * CJapanCar001::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CJapanCar001*	pInstance = new CJapanCar001(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CJapanCar001");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CJapanCar001::Clone(void * pArg)
{
	CJapanCar001*	pInstance = new CJapanCar001(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CJapanCar001");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CJapanCar001::Free()
{
	__super::Free();

	Safe_Release(m_pAABBCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
