#include "stdafx.h"
#include "..\Public\Camera_Player.h"
#include "GameInstance.h"
#include "Psy.h"

CCamera_Player::CCamera_Player(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CCamera(pDevice, pDeviceContext)
{
}

CCamera_Player::CCamera_Player(const CCamera_Player & rhs)
	:CCamera(rhs)
{
}

HRESULT CCamera_Player::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to CCamera_Player NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CCamera_Player::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSGBOX("Failed to CCamera_Player NativeConstruct");
		return E_FAIL;
	}

	//m_pTransform->Set_State(CTransform::STATE_RIGHT, m_pPlayerTransform->Get_State(CTransform::STATE_RIGHT));
	//m_pTransform->Set_State(CTransform::STATE_LOOK, m_pPlayerTransform->Get_State(CTransform::STATE_LOOK));
	//m_pTransform->Set_State(CTransform::STATE_UP, m_pPlayerTransform->Get_State(CTransform::STATE_UP));

	//_vector	vPos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	/*vPos = XMStoreFloat3((_float3*), XMVectorGetY(vPos));
	XMVectorSet(0.f, vPos, 0.f, 1.f);*/

	return S_OK;
}

_int CCamera_Player::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	////1.ī�޶�_�÷��̾� ƽ�Լ��� �̰��� �ϴ°� �´°�??
	////2.���� ������ ������ ��°�??
	////3.�a���̾� ������ �̷��� �ϴ°� �´°�?? ���������ȯ�� ���ؼ� 
	//CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	//
	////��ġ���
	//_vector vPosition = m_pPlayerPosition_TransformCom->Get_State(CTransform::STATE_POSITION);
	/*_vector	vRight = m_pPlayerPosition_TransformCom->Get_State(CTransform::STATE_RIGHT);
	//_vector vLook = m_pPlayerPosition_TransformCom->Get_State(CTransform::STATE_LOOK);
	//_vector vUp = m_pPlayerPosition_TransformCom->Get_State(CTransform::STATE_UP);*/
	////��ġ�� ������ ��� �ϸ� ������ ������ ���� Ŭ������ �ڵ�� �����ΰ�??
	////�׷��� lookat�� ��Ծ��� ������??
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/*CPsy*			Player_Pos = CPsy::Create(m_pDevice, m_pDeviceContext);*/

	//�÷��̾� ��ġ�� ���°Ŵ�

	CTransform* m_pPlayerPosition_TransformCom = static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform")));
	_vector vPosition1 = m_pPlayerPosition_TransformCom->Get_State(CTransform::STATE_POSITION);
	//vPosition1.m128_f32[0] = vPosition1.m128_f32[0] + 2.5f;//X��
	vPosition1.m128_f32[1] = vPosition1.m128_f32[1] + 10.f;//Y��
	vPosition1.m128_f32[2] = vPosition1.m128_f32[2] - 5.f;//Z��
	//_vector vPosition = Player_Pos->Player_Position();
	/*int iii = 10;*/
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition1);
	//LOOKAT�Լ��� ������ ��� �ؾ��ϴ°�

	RELEASE_INSTANCE(CGameInstance);
	
	return _int();
}

_int CCamera_Player::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	return _int();
}

HRESULT CCamera_Player::Render()
{
	return S_OK;
}

CCamera_Player * CCamera_Player::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CCamera_Player*	pInstance = new CCamera_Player(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CCamera_Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Player::Clone(void * pArg)
{
	CCamera_Player*	pInstance = new CCamera_Player(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CCamera_Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Player::Free()
{
	__super::Free();
}
