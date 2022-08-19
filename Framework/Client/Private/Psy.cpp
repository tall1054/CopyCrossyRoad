#include "stdafx.h"
#include "..\Public\Psy.h"
#include "GameInstance.h"
#include "Item.h"
#include "StateComponent.h"
#include "Level_Loading.h"

CPsy::CPsy(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CLandObject(pDevice, pDeviceContext)
{

}

CPsy::CPsy(const CPsy & rhs)
	: CLandObject(rhs)
{
}

HRESULT CPsy::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to CPsy NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CPsy::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(&CTransform::TRANSFORMDESC(5.f, XMConvertToRadians(180.0f)))))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	CPsy::PsyPosition	Psypos;
	ZeroMemory(&Psypos, sizeof(CPsy::PsyPosition));
	Psypos.vPosition = _float4(25.f, 0.7f, 1.f, 1.f);

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(Psypos.vPosition.x, Psypos.vPosition.y, Psypos.vPosition.z, Psypos.vPosition.w));

	return S_OK;
}

_int CPsy::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	fTimeAcc += (_float)TimeDelta;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			TransformMatrix;

	//Ű �ѹ� ���� ���� ó��
	//�����鼭 �����ϸ� �۵������� ��Ű�� ���������� Y�� ���̰� �̻�����
	if (pGameInstance->Key_Pressing(VK_UP))
	{
		m_pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(0.f));
		//�����Ҷ�
		if (true == Jump)
		{
			if (fTimeAcc <= 0.2f)
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
			if (fTimeAcc <= 0.2f)
			{
				m_pTransform->Go_Down(TimeDelta);
			}
			else
			{
				fTimeAcc = 0.f;
				Jump = true;
				++m_iWalkCut;
			}
		}
		m_pTransform->Go_Straight(TimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_DOWN) & 0x80)
	{
		m_pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));
		if (Jump == true)
		{
			if (fTimeAcc <= 0.2f)
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
			if (fTimeAcc <= 0.2f)
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
			if (fTimeAcc <= 0.2f)
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
			if (fTimeAcc <= 0.2f)
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
			if (fTimeAcc <= 0.2f)
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
			if (fTimeAcc <= 0.2f)
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

	RELEASE_INSTANCE(CGameInstance);

	m_pAABBCom->Update_Transform(m_pTransform->Get_WorldMatrix());
	m_pOBBCom->Update_Transform(m_pTransform->Get_WorldMatrix());

	//??
	//������ ���� ���´�
	//��ġ �������
	if (0.7f > XMVectorGetY(m_pTransform->Get_State(CTransform::STATE_POSITION)))
	{
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pTransform->Get_State(CTransform::STATE_POSITION), 0.7f));
	}

	if (1.4f < XMVectorGetY(m_pTransform->Get_State(CTransform::STATE_POSITION)))
	{
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pTransform->Get_State(CTransform::STATE_POSITION), 1.4f));
	}
	//��ġ �����
	return _int();
}

_int CPsy::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	//===========collision_structure======//
	//�÷��̾�� ��ü���� �浹��
	//�÷��̾��� ��ġ, �÷��̾��� �浹��ü(�ݸ���), �浹���
	CLayer*	pLayer = pGameInstance->Get_ObjectLayer(LEVEL_GAMEPLAY, TEXT("Layer_Structure"));

	if (pLayer != nullptr)
	{
		list<class CGameObject*> pGameObject = pLayer->Get_Objects();
		for (auto iter : pGameObject)
		{
			if (m_pAABBCom->Collision_AABB(static_cast<CCollider*>(iter->Get_Component(TEXT("Com_AABB")))))
			{
				CTransform::TRANSFORMDESC		pTransformDesc;
				ZeroMemory(&pTransformDesc, sizeof(CTransform::TRANSFORMDESC));
				pTransformDesc.fSpeedPerSec = 0.f;
				m_pTransform->Go_Straight(-TimeDelta);
			}
		}
		//�ڷ� ��������.
	}

	//===========collision_Item=========//
	//�÷��̾�� ������ �浹��
	//�������� ������ ������Ű��
	//�������� ������� �ϴ� ��� = complete

	//������ ���̾ ����.
	CLayer*	pLayerItem = pGameInstance->Get_ObjectLayer(LEVEL_GAMEPLAY, TEXT("Layer_Item"));

	CItem::ITEMDESC		tItemDesc;
	ZeroMemory(&tItemDesc, sizeof(CItem::ITEMDESC));
	tItemDesc.fItemValue = 0;
	//�� ���̾ ���� ���� ���
	if (pLayerItem != nullptr)
	{
		//����Ʈ ���ӿ�����Ʈ�� ������Ʈ�� ���� �׾Ƶд�.
		list<class CGameObject*> pGameObject = pLayerItem->Get_Objects();

		for (auto iter : pGameObject)
		{
			if (m_pAABBCom->Collision_AABB(static_cast<CCollider*>(iter->Get_Component(TEXT("Com_AABB")))))
			{
				Safe_Release(iter);
				tItemDesc.fItemValue = tItemDesc.fItemValue + 100;
				m_fCoinValue += tItemDesc.fItemValue;
				m_fCoinValue = Get_CoinValue();
			}
			
		}
	}

	//===========collision_Monster===========//
	//�÷��̾�� ���� �浹��
	//�÷��̾�� ������� �ϰų� �ٸ� ���·� �����д�.

	CLayer*	pLayerMon = pGameInstance->Get_ObjectLayer(LEVEL_GAMEPLAY, TEXT("Layer_Monster"));

	if (pLayerMon != nullptr)
	{
		list<class CGameObject*> pGameObject = pLayerMon->Get_Objects();
		for (auto iter : pGameObject)
		{
			if (m_pAABBCom->Collision_AABB(static_cast<CCollider*>(iter->Get_Component(TEXT("Com_AABB")))))
			{
				Safe_Release(iter);

				continue;
				/*pGameInstance->Destory_Front(LEVEL_GAMEPLAY, TEXT("Layer_Player"));
				return 3;*/
			}
		}
	}
	/*if(this!=nullptr)*/
	//�ݸ����� �̿��� ���� �¿�� �Ϸ�� ����
	//�޸� ������ �κ�
	//__super::SetUp_OnTerrain(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), 0, TEXT("Com_AABB"));

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

HRESULT CPsy::Render()
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


HRESULT CPsy::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_NonAnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Psy"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
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

HRESULT CPsy::SetUp_ConstantTable()
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

HRESULT CPsy::Take_Item(ITEM eItem, _float fItemValue)
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);
	CLayer*	pUI = pGameInstance->Get_ObjectLayer(LEVEL_STATIC, TEXT("Layer_UI"));

	switch (eItem)
	{
	case ITEM_COIN:
		if (1000.f < m_pStateCom->Get_MaxCoin())
			break;
		m_pStateCom->Add_Coin(fItemValue);
		break;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}


CPsy * CPsy::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CPsy*	pInstance = new CPsy(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CPsy");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPsy::Clone(void * pArg)
{
	CPsy*	pInstance = new CPsy(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CPsy");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPsy::Free()
{
	__super::Free();

	Safe_Release(m_pStateCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pSphereCom);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pAABBCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
