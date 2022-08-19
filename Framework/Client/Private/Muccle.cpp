#include "stdafx.h"
#include "../Public/Muccle.h"
#include "GameInstance.h"
#include "SoundMgr.h"

CMuccle::CMuccle(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CLandObject(pDevice, pDeviceContext)
{
}

CMuccle::CMuccle(const CMuccle & rhs)
	: CLandObject(rhs)
{
}

HRESULT CMuccle::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to CMuccle NativeConstruct_Prototype");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CMuccle::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(&CTransform::TRANSFORMDESC(3.f, XMConvertToRadians(90.f)))))
	{
		MSGBOX("Failed to CMuccle NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
	{
		MSGBOX("Failed to CMuccle SetUp_Components");
		return E_FAIL;
	}

	/*_float3		fMuccleScale = _float3(0.1f, 0.1f, 0.1f);
	m_pTransform->Scaled(XMLoadFloat3(&fMuccleScale));*/

	if (nullptr != pArg)
		memcpy(&m_tMucclePos, pArg, sizeof(MucclePosition));

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_tMucclePos.vPosition.x, m_tMucclePos.vPosition.y, m_tMucclePos.vPosition.z, m_tMucclePos.vPosition.w));
	m_pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(m_tMucclePos.fAngle));
	m_pTransform->Scaled(XMLoadFloat3(&m_tMucclePos.vScale));
	m_pTransform->Set_TransformDesc(CTransform::TRANSFORMDESC(m_tMucclePos.fSpeed));




	return S_OK;
}

_int CMuccle::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	m_fTimeFrame += TimeDelta;

	m_pAABBCom->Update_Transform(m_pTransform->Get_WorldMatrix());

	m_pTransform->Go_Left(TimeDelta);
	//while������ ���� �ݺ�ó���صθ� �ݺ��������� �����ð��� �����ϸ�
	while (m_fTimeFrame > 10.f)
	{
		//Ż��ó���� �صΰ� �θ�
		if (m_bExitCheck = true)
		{
			//Ż����ġ�� ���󺹱��� �� ��ġ�� �ٲپ� �д�.
			//�׸��� �׿� ���� �ð��� 0���� �ʱ�ȭ�صд�.
			m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_tMucclePos.vPosition.x, m_tMucclePos.vPosition.y, m_tMucclePos.vPosition.z, m_tMucclePos.vPosition.w));
			m_fTimeFrame = 0.f;
		}
		break;
	}

	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	pGameInstance->StopSound(CSoundMgr::CHANNELID::BGM);
	pGameInstance->Play_Sound(L"car-engine-loop-deep.wav", CSoundMgr::CHANNELID::PLAYER2, 0.5f);

	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

_int CMuccle::LateTick(_double TimeDelta)
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

HRESULT CMuccle::Render()
{
	if (nullptr == m_pModelCom)
	{
		MSGBOX("Failed to CMuccle Render_Model");
		return E_FAIL;
	}

	if (FAILED(SetUp_ConstantTable()))
	{
		MSGBOX("Failed to CMuccle SetUp_ConstantTable");
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

void CMuccle::Clear_Monster_Car()
{
}

HRESULT CMuccle::SetUp_Components()
{
	//������ ������Ʈ
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("Failed to CMonster_Car Prototype_Component_Renderer");
		return E_FAIL;
	}

	//���̴� ������Ʈ?? �������� ����ƽ�����̰� ���̴��� �����÷����̴�. �� ���� �˾ƺ���
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_NonAnimModel"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
	{
		MSGBOX("Failed to CMonster_Car Prototype_Component_Shader_NonAnimModel");
		return E_FAIL;
	}

	//�� ������Ʈ
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_CMuccle"),
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
	{
		MSGBOX("Failed to CMonster_Car Prototype_Component_Model_CMuccle");
		return E_FAIL;
	}

	CCollider::COLLIDERDESC		ColliderDesc;
	//AABB�浹 ������Ʈ
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(10.f, 10.f, 5.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vTranslation = _float4(0.f, 0.7f, 0.f, 1.f);


	//�ٸ� �ֵ�������Ʈ�� �ٸ��� �ݶ��̴��� ����: �ݶ��̴� ��ũ���� �� �߰��ؼ� �ֵ� ������Ʈ�� �ϼ���Ų�� ������??
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMuccle::SetUp_ConstantTable()
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

	//�� ����ü
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

CMuccle * CMuccle::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CMuccle*	pInstance = new CMuccle(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CMuccle");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMuccle::Clone(void * pArg)
{
	CMuccle*	pInstance = new CMuccle(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CMuccle");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMuccle::Free()
{
	__super::Free();

	Safe_Release(m_pAABBCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
