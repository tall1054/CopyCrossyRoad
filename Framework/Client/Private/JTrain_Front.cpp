#include "stdafx.h"
#include "..\Public\JTrain_Front.h"
#include "GameInstance.h"

CJTrain_Front::CJTrain_Front(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CLandObject(pDevice, pDeviceContext)
{
}

CJTrain_Front::CJTrain_Front(const CJTrain_Front & rhs)
	: CLandObject(rhs)
{
}

HRESULT CJTrain_Front::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to CJTrain_Front NativeConstruct_Prototype");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CJTrain_Front::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(&CTransform::TRANSFORMDESC(30.f, XMConvertToRadians(270.0f)))))
	{
		MSGBOX("Failed to CJTrain_Front NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
	{
		MSGBOX("Failed to CJTrain_Front SetUp_Components");
		return E_FAIL;
	}

	//===============스케일 입력해서 크기를 알아낸다===================//
	//_float3		fScale00 = _float3(0.2f, 0.2f, 0.2f);
	//m_pTransform->Scaled(XMLoadFloat3(&fScale00));

	//====기차의 생성위치를 특정 지형에 생성되게끔 만들어 두자====//
	if (nullptr != pArg)
		memcpy(&m_tKFrontPos, pArg, sizeof(KTrainFront));

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_tKFrontPos.vPosition.x, m_tKFrontPos.vPosition.y, m_tKFrontPos.vPosition.z, m_tKFrontPos.vPosition.w));
	m_pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(m_tKFrontPos.fAngle));
	m_pTransform->Scaled(XMLoadFloat3(&m_tKFrontPos.vScale));

	return S_OK;
}

_int CJTrain_Front::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	m_fTimeFrame += TimeDelta;

	m_pAABBCom->Update_Transform(m_pTransform->Get_WorldMatrix());

	//========왼쪽으로 이동하기==============//
	m_pTransform->Go_Left(TimeDelta);

	//if (15.f < TimeDelta)
	//{
	//	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(45.f, 0.4f, 17.5f, 1.f));
	//}
	while (m_fTimeFrame > 5.f)
	{
		if (m_bExitCheck = true)
		{
			m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_tKFrontPos.vPosition.x, m_tKFrontPos.vPosition.y, m_tKFrontPos.vPosition.z, m_tKFrontPos.vPosition.w));
			m_fTimeFrame = 0.f;
		}
		break;
	}

	return _int();
}

_int CJTrain_Front::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	CGameInstance*	pGameInstnce = GET_INSTANCE(CGameInstance);

	//__super::SetUp_OnTerrain(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), 0, TEXT("Com_VIBuffer"));

	m_pAABBCom->Collision_AABB((CCollider*)pGameInstnce->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_AABB")));

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

	//=====지형 끝에 도달했을때 물체가 사라지게 만든다.========//

	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

HRESULT CJTrain_Front::Render()
{
	if (nullptr == m_pModelCom)
	{
		MSGBOX("Failed to CJTrain_Front Render_Model");
		return E_FAIL;
	}

	if (FAILED(SetUp_ConstantTable()))
	{
		MSGBOX("Failed to CJTrain_Front SetUp_ConstantTable");
		return E_FAIL;
	}

	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture");

		m_pModelCom->Render(m_pShaderCom, 0, i, "g_BoneMatrices");
	}

#ifdef _DEBUG
	m_pAABBCom->Render();
#endif // _DEBUG

	return S_OK;
}


HRESULT CJTrain_Front::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("Failed to CJTrain_Front Prototype_Component_Renderer");
		return E_FAIL;
	}

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_NonAnimModel"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
	{
		MSGBOX("Failed to CJTrain_Front Prototype_Component_Shader_AnimModel");
		return E_FAIL;
	}

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_JTrain_Front"),
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
	{
		MSGBOX("Failed to CJTrain_Front Prototype_Component_Model_JTrain_Front");
		return E_FAIL;
	}

	CCollider::COLLIDERDESC			ColliderDesc;
	/* For.Com_AABB */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(40.f, 10.f, 10.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float4(0.f, 5.f, 0.f, 1.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CJTrain_Front::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

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

	const LIGHTDESC* pLightDesc = pGameInstance->Get_LightDesc(0);

	if (nullptr == pLightDesc)
		return E_FAIL;

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

CJTrain_Front * CJTrain_Front::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CJTrain_Front*		pInstance = new CJTrain_Front(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CJTrain_Front");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CJTrain_Front::Clone(void * pArg)
{
	CJTrain_Front*	pInstance = new CJTrain_Front(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CJTrain_Front");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CJTrain_Front::Free()
{
	__super::Free();

	Safe_Release(m_pAABBCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
