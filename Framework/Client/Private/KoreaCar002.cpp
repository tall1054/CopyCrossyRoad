#include "stdafx.h"
#include "../Public/KoreaCar002.h"
#include "GameInstance.h"

CKoreaCar002::CKoreaCar002(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CLandObject(pDevice, pDeviceContext)
{
}

CKoreaCar002::CKoreaCar002(const CKoreaCar002 & rhs)
	: CLandObject(rhs)
{
}

HRESULT CKoreaCar002::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to CKoreaCar002 NativeConstruct_Prototype");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CKoreaCar002::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(&CTransform::TRANSFORMDESC(3.f, XMConvertToRadians(90.f)))))
	{
		MSGBOX("Failed to CKoreaCar002 NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
	{
		MSGBOX("Failed to CKoreaCar002 SetUp_Components");
		return E_FAIL;
	}

	/*_float3		fKoreaCar002Scale = _float3(0.1f, 0.1f, 0.1f);
	m_pTransform->Scaled(XMLoadFloat3(&fKoreaCar002Scale));*/

	if (nullptr != pArg)
		memcpy(&m_tKorCar2Pos, pArg, sizeof(KorCar2Position));

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_tKorCar2Pos.vPosition.x, m_tKorCar2Pos.vPosition.y, m_tKorCar2Pos.vPosition.z, m_tKorCar2Pos.vPosition.w));
	m_pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(m_tKorCar2Pos.fAngle));
	m_pTransform->Scaled(XMLoadFloat3(&m_tKorCar2Pos.vScale));
	m_pTransform->Set_TransformDesc(CTransform::TRANSFORMDESC(m_tKorCar2Pos.fSpeed));

	return S_OK;
}

_int CKoreaCar002::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	m_fTimeFrame += TimeDelta;

	m_pAABBCom->Update_Transform(m_pTransform->Get_WorldMatrix());

	m_pTransform->Go_Right(TimeDelta);

	while (m_fTimeFrame > 20.f)
	{
		if (m_bExitCheck = true)
		{
			m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_tKorCar2Pos.vPosition.x, m_tKorCar2Pos.vPosition.y, m_tKorCar2Pos.vPosition.z, m_tKorCar2Pos.vPosition.w));
			m_fTimeFrame = 0.f;
		}
		break;
	}

	return _int();
}

_int CKoreaCar002::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	//__super::SetUp_OnTerrain(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), 0, TEXT("Com_VIBuffer"));

	m_pAABBCom->Collision_AABB((CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_AABB")));

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

HRESULT CKoreaCar002::Render()
{
	if (nullptr == m_pModelCom)
	{
		MSGBOX("Failed to CKoreaCar002 Render_Model");
		return E_FAIL;
	}

	if (FAILED(SetUp_ConstantTable()))
	{
		MSGBOX("Failed to CKoreaCar002 SetUp_ConstantTable");
		return E_FAIL;
	}
	_uint		iNumMatrials = m_pModelCom->Get_NumMaterials();

	for (_uint i = 0; i < iNumMatrials; ++i)
	{
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture");
		m_pModelCom->Render(m_pShaderCom, 0, i, "g_BoneMatrices");
	}

#ifdef _DEBUG
	//m_pAABBCom->Render();
#endif // _DEBUG

	return S_OK;
}

void CKoreaCar002::Clear_Monster_Car()
{
}

HRESULT CKoreaCar002::SetUp_Components()
{
	//렌더러 컴포넌트
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("Failed to CKoreaCar002 Prototype_Component_Renderer");
		return E_FAIL;
	}

	//쉐이더 컴포넌트?? 렌더러는 스테틱영역이고 쉐이더는 레벨플레이이다. 그 차이 알아보자
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_NonAnimModel"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
	{
		MSGBOX("Failed to CKoreaCar002 Prototype_Component_Shader_NonAnimModel");
		return E_FAIL;
	}

	//모델 컴포넌트
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_CKoreaCar002"),
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
	{
		MSGBOX("Failed to CMonster_Car Prototype_Component_Model_CKoreaCar002");
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

HRESULT CKoreaCar002::SetUp_ConstantTable()
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransform->Bind_OnShader(m_pShaderCom, "g_WorldMatrix")))
	{
		MSGBOX("Failed to CKoreaCar002 Render_Bind_OnShader(World)");
		return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
	{
		MSGBOX("Failed to CKoreaCar002  Render_Set_RawValue(View)");
		return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
	{
		MSGBOX("Failed to CKoreaCar002 Render_Set_RawValue(Proj)");
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

CKoreaCar002 * CKoreaCar002::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CKoreaCar002*	pInstance = new CKoreaCar002(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CKoreaCar002");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CKoreaCar002::Clone(void * pArg)
{
	CKoreaCar002*	pInstance = new CKoreaCar002(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CKoreaCar002");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CKoreaCar002::Free()
{
	__super::Free();

	Safe_Release(m_pAABBCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
