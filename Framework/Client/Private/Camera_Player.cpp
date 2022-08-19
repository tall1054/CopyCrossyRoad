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

	////1.카메라_플레이어 틱함수인 이곳에 하는게 맞는가??
	////2.라룩업 선언은 했으나 잡는건??
	////3.픞레이어 선언은 이렇게 하는게 맞는가?? 명시적형변환을 통해서 
	//CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	//
	////위치얻고
	//_vector vPosition = m_pPlayerPosition_TransformCom->Get_State(CTransform::STATE_POSITION);
	/*_vector	vRight = m_pPlayerPosition_TransformCom->Get_State(CTransform::STATE_RIGHT);
	//_vector vLook = m_pPlayerPosition_TransformCom->Get_State(CTransform::STATE_LOOK);
	//_vector vUp = m_pPlayerPosition_TransformCom->Get_State(CTransform::STATE_UP);*/
	////위치를 잡으면 어떻게 하면 좋을까 참고할 만한 클래스나 코드는 무엇인가??
	////그러면 lookat은 어떻게쓰면 좋을까??
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/*CPsy*			Player_Pos = CPsy::Create(m_pDevice, m_pDeviceContext);*/

	//플레이어 위치를 얻어온거다

	CTransform* m_pPlayerPosition_TransformCom = static_cast<CTransform*>(pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform")));
	_vector vPosition1 = m_pPlayerPosition_TransformCom->Get_State(CTransform::STATE_POSITION);
	//vPosition1.m128_f32[0] = vPosition1.m128_f32[0] + 2.5f;//X축
	vPosition1.m128_f32[1] = vPosition1.m128_f32[1] + 10.f;//Y축
	vPosition1.m128_f32[2] = vPosition1.m128_f32[2] - 5.f;//Z축
	//_vector vPosition = Player_Pos->Player_Position();
	/*int iii = 10;*/
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition1);
	//LOOKAT함수를 얻어내려면 어떻게 해야하는가

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
