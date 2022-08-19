#include "stdafx.h"
#include "..\Public\Player_Korea_Drummer.h"
#include "GameInstance.h"

CPlayer_Korea_Drummer::CPlayer_Korea_Drummer(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CLandObject(pDevice, pDeviceContext)
{

}

CPlayer_Korea_Drummer::CPlayer_Korea_Drummer(const CPlayer_Korea_Drummer & rhs)
	: CLandObject(rhs)
{
}

HRESULT CPlayer_Korea_Drummer::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to CPlayer_Korea_Drummer NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CPlayer_Korea_Drummer::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(&CTransform::TRANSFORMDESC(5.f, XMConvertToRadians(180.0f)))))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	CPlayer_Korea_Drummer::Player_Korea_DrummerPosition	Player_Korea_Drummerpos;
	ZeroMemory(&Player_Korea_Drummerpos, sizeof(CPlayer_Korea_Drummer::Player_Korea_DrummerPosition));
	Player_Korea_Drummerpos.vPosition = _float4(25.f, 0.5f, 0.3f, 1.f);

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(Player_Korea_Drummerpos.vPosition.x, Player_Korea_Drummerpos.vPosition.y, Player_Korea_Drummerpos.vPosition.z, Player_Korea_Drummerpos.vPosition.w));

	return S_OK;
}

_int CPlayer_Korea_Drummer::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	fTimeAcc += (_float)TimeDelta;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			TransformMatrix;

	if (pGameInstance->Key_Pressing(VK_UP))
	{
		m_pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(0.f));
		if (true == Jump)
		{
			if (fTimeAcc <= 0.18f)
			{
				m_pTransform->Go_Up(TimeDelta);
			}
			else
			{
				fTimeAcc = 0.f;
				Jump = false;
			}
		}
		else
		{
			if (fTimeAcc <= 0.18f)
			{
				m_pTransform->Go_Down(TimeDelta);
			}
			else
			{
				fTimeAcc = 0.f;
				Jump = true;
			}
		}
		m_pTransform->Go_Straight(TimeDelta);
		++m_iWalkCut;
	}



	/*if (pGameInstance->Key_Down(VK_UP) && Jump == false)
	{
	m_pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(0.f));
	if (Jump == true)
	{
	m_pTransform->Go_Up(TimeDelta);
	}
	Jump = false;
	m_pTransform->Go_Straight(TimeDelta);
	}

	if (pGameInstance->Key_Up(VK_UP) && Jump == false)
	{
	m_pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(0.f));
	if (Jump = true)
	{
	m_pTransform->Go_Down(TimeDelta);
	}
	Jump = false;
	m_pTransform->Go_Straight(TimeDelta);
	}*/




	if (pGameInstance->Get_DIKeyState(DIK_DOWN) & 0x80)
	{
		m_pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));
		if (Jump == true)
		{
			if (fTimeAcc <= 0.18f)
			{
				m_pTransform->Go_Up(TimeDelta);
			}
			else
			{

				Jump = false;
				fTimeAcc = 0.f;
			}
		}
		else
		{
			if (fTimeAcc <= 0.18f)
			{
				m_pTransform->Go_Down(TimeDelta);
			}
			else
			{
				Jump = true;
				fTimeAcc = 0.f;
			}
		}
		m_pTransform->Go_Straight(TimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_LEFT) & 0x80)
	{
		m_pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(270.f));
		if (Jump == true)
		{
			if (fTimeAcc <= 0.18f)
			{
				m_pTransform->Go_Up(TimeDelta);
			}
			else
			{
				Jump = false;
				fTimeAcc = 0.f;
			}
		}
		else
		{
			if (fTimeAcc <= 0.18f)
			{
				m_pTransform->Go_Down(TimeDelta);
			}
			else
			{
				Jump = true;
				fTimeAcc = 0.f;
			}
		}
		m_pTransform->Go_Straight(TimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_RIGHT) & 0x80)
	{
		m_pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));
		if (Jump == true)
		{
			if (fTimeAcc <= 0.18f)
			{
				m_pTransform->Go_Up(TimeDelta);
			}
			else
			{
				Jump = false;
				fTimeAcc = 0.f;
			}
		}
		else
		{
			if (fTimeAcc <= 0.18f)
			{
				m_pTransform->Go_Down(TimeDelta);
			}
			else
			{
				Jump = true;
				fTimeAcc = 0.f;
			}
		}
		m_pTransform->Go_Straight(TimeDelta);
	}

	/*if (pGameInstance->Get_DIKeyState(DIK_SPACE) & 0x80)
	{
	if (Test == true)
	{
	if (fTimeAcc <= 0.18f)
	{
	m_pTransform->Go_Up(TimeDelta);
	}
	else
	{
	Test = false;
	fTimeAcc = 0.f;
	}
	}
	else
	{
	if (fTimeAcc <= 0.18f)
	{
	m_pTransform->Go_Down(TimeDelta);
	}
	else
	{
	Test = true;
	fTimeAcc = 0.f;
	}
	}
	}*/

	RELEASE_INSTANCE(CGameInstance);

	m_pAABBCom->Update_Transform(m_pTransform->Get_WorldMatrix());
	m_pOBBCom->Update_Transform(m_pTransform->Get_WorldMatrix());

	//??
	//쓰래기 값이 나온다
	//위치 고정방법
	if (0.7f > XMVectorGetY(m_pTransform->Get_State(CTransform::STATE_POSITION)))
	{
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pTransform->Get_State(CTransform::STATE_POSITION), 0.7f));
	}

	if (1.4f < XMVectorGetY(m_pTransform->Get_State(CTransform::STATE_POSITION)))
	{
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pTransform->Get_State(CTransform::STATE_POSITION), 1.4f));
	}
	//위치 상승중
	return _int();
}

_int CPlayer_Korea_Drummer::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	//__super::SetUp_OnTerrain(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), 0, TEXT("Com_VIBuffer"));

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

	return _int();
}

HRESULT CPlayer_Korea_Drummer::Render()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		m_pModelCom->Bind_OnShader(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture");

		m_pModelCom->Render(m_pShaderCom, 0, i, "g_BoneMatrices");
	}

#ifdef _DEBUG
	m_pAABBCom->Render();
	m_pOBBCom->Render();
#endif // _DEBUG

	return S_OK;
}


HRESULT CPlayer_Korea_Drummer::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_NonAnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Player_Korea_Drummer"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;


	CCollider::COLLIDERDESC			ColliderDesc;
	/* For.Com_AABB */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(0.6f, 1.4f, 0.6f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float4(0.f, 0.7f, 0.f, 1.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_OBB */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(1.f, 1.f, 1.0f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float4(0.f, 0.5f, 0.f, 1.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;



	return S_OK;
}

HRESULT CPlayer_Korea_Drummer::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransform->Bind_OnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

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

HRESULT CPlayer_Korea_Drummer::CollisionCheck()
{

	return S_OK;
}

CPlayer_Korea_Drummer * CPlayer_Korea_Drummer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CPlayer_Korea_Drummer*	pInstance = new CPlayer_Korea_Drummer(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CPlayer_Korea_Drummer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPlayer_Korea_Drummer::Clone(void * pArg)
{
	CPlayer_Korea_Drummer*	pInstance = new CPlayer_Korea_Drummer(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CPlayer_Korea_Drummer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer_Korea_Drummer::Free()
{
	__super::Free();

	Safe_Release(m_pSphereCom);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pAABBCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
