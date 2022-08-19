#include "stdafx.h"
#include "..\Public\Monster_Train.h"
#include "GameInstance.h"

CMonster_Train::CMonster_Train(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CLandObject(pDevice, pDeviceContext)
{
}

CMonster_Train::CMonster_Train(const CMonster_Train & rhs)
	: CLandObject(rhs)
{
}

HRESULT CMonster_Train::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Cmonster_Train NativeConstruct_Prototype");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CMonster_Train::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(&CTransform::TRANSFORMDESC(30.f, XMConvertToRadians(270.0f)))))
	{
		MSGBOX("Failed to CMonster_Train NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
	{
		MSGBOX("Failed to CMonster_Train SetUp_Components");
		return E_FAIL;
	}

	//===============스케일 입력해서 크기를 알아낸다===================//
	//_float3		fScale00 = _float3(0.2f, 0.2f, 0.2f);
	//m_pTransform->Scaled(XMLoadFloat3(&fScale00));

	//====기차의 생성위치를 특정 지형에 생성되게끔 만들어 두자====//
	if (nullptr != pArg)
		memcpy(&m_tFrontPos, pArg, sizeof(TrainFront));

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_tFrontPos.vPosition.x, m_tFrontPos.vPosition.y, m_tFrontPos.vPosition.z, m_tFrontPos.vPosition.w));
	m_pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(m_tFrontPos.fAngle));
	m_pTransform->Scaled(XMLoadFloat3(&m_tFrontPos.vScale));
	m_pTransform->Set_TransformDesc(CTransform::TRANSFORMDESC(m_tFrontPos.fSpeed));

	return S_OK;
}

_int CMonster_Train::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	m_fTimeFrame += TimeDelta;

	m_pAABBCom->Update_Transform(m_pTransform->Get_WorldMatrix());

	//========왼쪽으로 이동하기==============//
	m_pTransform->Go_Left(TimeDelta);
	
	if (5.f < m_fTimeFrame)
	{
		CGameInstance*	pInstance = GET_INSTANCE(CGameInstance);

		pInstance->StopSound(CSoundMgr::CHANNELID::CAR);
		pInstance->Play_Sound(TEXT("train pass shorter.wav"), CSoundMgr::CHANNELID::CAR, 0.5f);

		RELEASE_INSTANCE(CGameInstance);
	}
	while (m_fTimeFrame > 5.f)
	{
		if (m_bExitCheck = true)
		{
			m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_tFrontPos.vPosition.x, m_tFrontPos.vPosition.y, m_tFrontPos.vPosition.z, m_tFrontPos.vPosition.w));
			m_fTimeFrame = 0.f;
		}
		break;
	}

	return _int();
}

_int CMonster_Train::LateTick(_double TimeDelta)
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

HRESULT CMonster_Train::Render()
{
	if (nullptr == m_pModelCom)
	{
		MSGBOX("Failed to CMonster_Train Render_Model");
		return E_FAIL;
	}

	if (FAILED(SetUp_ConstantTable()))
	{
		MSGBOX("Failed to CMonster_Train SetUp_ConstantTable");
		return E_FAIL;
	}

	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture");

		m_pModelCom->Render(m_pShaderCom, 0, i, "g_BoneMatrices");
	}

#ifdef _DEBUG
	//m_pAABBCom->Render();
#endif // _DEBUG

	return S_OK;
}


HRESULT CMonster_Train::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("Failed to CMonster_Train Prototype_Component_Renderer");
		return E_FAIL;
	}

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_NonAnimModel"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
	{
		MSGBOX("Failed to CMonster_Train Prototype_Component_Shader_AnimModel");
		return E_FAIL;
	}

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster_Train"),
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
	{
		MSGBOX("Failed to CMonster_Train Prototype_Component_Model_Monster_Train");
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

HRESULT CMonster_Train::SetUp_ConstantTable()
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

CMonster_Train * CMonster_Train::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CMonster_Train*		pInstance = new CMonster_Train(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CMonster_Train");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster_Train::Clone(void * pArg)
{
	CMonster_Train*	pInstance = new CMonster_Train(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Train");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Train::Free()
{
	__super::Free();

	Safe_Release(m_pAABBCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
