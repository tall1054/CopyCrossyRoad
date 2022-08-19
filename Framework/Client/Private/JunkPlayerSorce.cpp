//if (false == m_tJumpInfo.bJump)
//{
//	Jumping(TimeDelta);
//}
//m_tJumpInfo.fJumpPower = 5.f;

//if (true == m_tJumpInfo.bJump)
//{
//	_float3 vPosition = (_float3)XMStoreFloat3(&vPosition ,m_pTransformCom->Get_State(CTransform::STATE_POSITION));
//	vPosition.y = m_tJumpInfo.fJumpY + (m_tJumpInfo.fJumpPower * m_tJumpInfo.fTime - 9.8f * m_tJumpInfo.fTime * m_tJumpInfo.fTime * 0.5f);
//	m_tJumpInfo.fTime += 0.03f;
//	if (m_tJumpInfo.fY > vPosition.y)
//	{
//		m_tJumpInfo.bJump = false;
//		m_tJumpInfo.fTime = 0.f;
//	}

//	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
//}

//_vector vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);
///*_vector vUp = m_pTransform->Get_State(CTransform::STATE_UP);*/

/*return S_OK;*/

//HRESULT CPsy::Jumping(_double TimeDelta)
//{
//
//}

//typedef struct tagJumpInfo
//{
//	_bool	bJump;
//	_float	fY, fJumpY, fJumpPower;
//	_float	fTime;
//}JUMPINFO;

//private:
//	JUMPINFO			m_tJumpInfo;
//	_float				m_fTime = 0.f;
//	_float				m_fFrame = 0.f;
//
//private:
//	HRESULT		Jumping();
//	HRESULT		Key_Input(_double TimeDelta);
//	HRESULT		MoveKeyInput(CGameInstance* pGameInstance, _double Timedelta);

/*Test = true;*/

//if(pGameInstance->Key_Up(DIK_SPACE) & 0x80)
//	m_pTransform->Go_Down(fTimeAcc * 0.005f);

/*m_pTransform->Go_Up(fTimeAcc * 0.15f);*/
////올라가면서 전진한다.
//
//if (fTimeAcc > 0.3f)
//{
//	m_pTransform->Go_Down(TimeDelta);
//}

/*m_pTransform->Go_Straight(TimeDelta);*/


/* For.Com_SPHERE */
/*ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

ColliderDesc.vScale = _float3(0.3f, 0.3f, 0.3f);
ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
ColliderDesc.vTranslation = _float4(0.f, 0.5f, 0.f, 1.f);

if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), TEXT("Com_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
return E_FAIL;*/

/*_vector Player_Position()
{
return m_pTransform->Get_State(CTransform::STATE_POSITION);
}*/

//m_pSphereCom->Update_Transform(m_pTransform->Get_WorldMatrix());

/*CLayer*	pMonLayer = pGameInstance->Get_ObjectLayer(LEVEL_GAMEPLAY, TEXT("Layer_Monster"));

if (pMonLayer != nullptr)
{
CGameObject*	pMonGameObject = pMonLayer->Get_Object().front();
m_pAABBCom->Collision_AABB((CCollider*)pMonGameObject);
pGameInstance->Clear_LevelObject(LEVEL_GAMEPLAY);
}*/

/*if (m_pAABBCom->Collision_AABB((CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Com_AABB"))))
{
pGameInstance->Clear_LevelObject(LEVEL_GAMEPLAY);
}*/
//__super::SetUp_OnTerrain(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), 0, TEXT("Com_VIBuffer"));


/*CLayer*	pLayer = pGameInstance->Get_Object(ALL_eNextLevel, TEXT("Layer_Monster"));
list<class CGameObject*> Objects = pLayer->Get_Object();

for (auto iter : Objects)
{
CCollider* pCollider = (CCollider*)iter->Get_Component(TEXT("Com_SPH_BODY"));
if (m_pSphereCom02->Collision_Sphere(pCollider))
{
TransformDesc_Setting(1.f, 0.f, 0.f);
m_pTransform->Go_Straight(-TimeDelta);
}
}
CPsy*		pPsy = nullptr;
m_pLayer = m_pGameInstance->Get_ObjectLayer(LEVEL_GAMEPLAY, TEXT("Layer_Player"));
if (m_pLayer != nullptr)
{
CGameObject* cGameObject = m_pLayer->Get_Object().front();
if (cGameObject != nullptr) {
pPsy = (CPsy*)cGameObject;
wsprintf(m_szCross, TEXT("Cross Point : %d"), pPsy->Get_WalkCount());
m_pGameInstance->Render_Font(m_pDeviceContext, TEXT("8BIT_WONDER"), m_szCross, _float2(0.f, 0.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
}
}
*/

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



/*CTransform::TRANSFORMDESC	Transformdesc;

_double TrainTime = Transformdesc.fSpeedPerSec * TimeDelta;*/

//if (15.f < TimeDelta)
//{
//	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(54.f, 0.4f, 17.5f, 1.f));
//}
