#include "stdafx.h"
#include "..\Public\Loader.h"
#include "GameInstance.h"
#include "Fork.h"
#include "Player.h"
#include "Sky.h"
#include "Camera_Dynamic.h"
#include "Camera_Player.h"
#include "Terrain.h"
#include "Monster.h"
//====for loading=====//

//=====train======//
#include "Monster_Train.h"
#include "Monster_TrainMiddle.h"
#include "Monster_TrainEnd.h"
#include "KTrain_Front.h"
#include "KTrain_Middle.h"
#include "KTrain_End.h"
#include "JTrain_Front.h"
#include "JTrain_Middle.h"
#include "JTrain_End.h"
//=====car======//
#include "Monster_Car.h"
#include "KoreaCar001.h"
#include "KoreaCar002.h"
#include "KoreaCar003.h"
#include "JapanCar001.h"
#include "JapanCar002.h"
#include "JapanCar003.h"
#include "Muccle.h"
//=====taxi=====//
#include "Monster_Taxi.h"

//=====truck=====//
#include "Monster_Truck.h"
#include "Monster_Truck2.h"
#include "Monster_OilTruck.h"
#include "KoreaTruck001.h"
#include "KoreaTruck002.h"

//=====objects====//
//======korea=======//
#include "Apartment.h"
#include "BBQ.h"

//======japan=====//
#include "Buliding.h"
#include "Sign.h"
#include "Castle.h"
#include "Shrine.h"

//========tree========//
#include "FineTreeA.h"
#include "FineTreeB.h"
#include "FineTreeC.h"
#include "Tree.h"
#include "TreeA.h"
#include "TreeB.h"
#include "TreeC.h"

//======terrain=======//
#include "Grass.h"
#include "Road.h"
#include "Water.h"
#include "Train.h"

//=====item=======//
#include "Item.h"

//======player=====//
#include "Player_Chicken.h"
#include "Player_Koala.h"
#include "Player_Korea_Drummer.h"
#include "Player_Samurai.h"
#include "Psy.h"
#include "Sword.h"

//=======UI============//
#include "UI_Money.h"

CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

unsigned int APIENTRY ThreadFunction(void* pArg)
{
	/* CLoader객체안에 로딩에 필요한 데이터들을 담아놨는데 그걸 써야혀? */
	/* CLoader객체의 주소를 가지고있으면 좋을것같다. */
	CLoader*		pLoader = (CLoader*)pArg;

	EnterCriticalSection(&pLoader->Get_CS());

	switch (pLoader->Get_LevelID())
	{
	case LEVEL_GAMEPLAY:
		pLoader->Loading_ForGamePlay();
		break;
	case LEVEL_KOREA:
		pLoader->Loading_ForKoreaLevel();
		break;
	case LEVEL_JAPAN:
		pLoader->Loading_ForJapanLevel();
		break;
	}

	LeaveCriticalSection(&pLoader->Get_CS());

	return 0;
}

HRESULT CLoader::NativeConstruct(LEVEL eNextLevel)
{
	InitializeCriticalSection(&m_CS);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunction, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	m_eLevel = eNextLevel;

	return S_OK;
}


HRESULT CLoader::Loading_ForGamePlay()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	
	/* 원형컴포넌트를 생성한다. */
#pragma region PROTOTYPE_COMPONENT
	pGameInstance->Set_CurrentLevelIndexForPrototype(LEVEL_GAMEPLAY);
	//==============================BasicOffer : Prototype Component Model=======================//
	Create_BasicOffer(LEVEL_GAMEPLAY);
	//==============================Directly Extracting : Prototype Component Model=======================//
	_matrix			TransformMatrix;
	//==============================Player : Prototype Component Model=======================//
	Create_PlayerPrototype(LEVEL_GAMEPLAY);
	//==============================Terrain : Prototype Component Model=======================//
	Create_TerrainPrototype(LEVEL_GAMEPLAY);
	//==============================Objects : Prototype Component Model=======================//
	Create_StructurePrototype(LEVEL_GAMEPLAY);
	//==============================Car, Train, Truck, Taxi : Prototype Component Model=======================//
	Create_MonsterPrototype(LEVEL_GAMEPLAY);
	//==============================Item : Prototype Component Model=======================//
	TransformMatrix = XMMatrixScaling(0.02f, 0.02f, 0.02f) * XMMatrixRotationY(XMConvertToRadians(0.0f));
	/* For.Prototype_Component_Model_Item */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Item"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Object/Coin/", "coin_optimised.fbx", TransformMatrix))))
	{
		MSGBOX("Failed to Prototype_Component_Model_Item");
		return E_FAIL;
	}
	//==============================UI : Prototype Component Model=======================//
	//==============================Shader : Prototype Component Model=======================//
	Create_Shader(LEVEL_GAMEPLAY);
#pragma endregion

#pragma  region PROTOTYPE_GAMEOBJECT
	/* 원형객체를 생성한다. */

	//===========================BasicOffer_Prototype_GameObject===========================//
	/* For.Prototype_GameObject_Player */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Player");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Terrain");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Monster */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster"),
		CMonster::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Monster");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Camera_Dynamic */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Dynamic"),
		CCamera_Dynamic::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Camera_Dynamic");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Fork */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Fork"),
		CFork::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Fork");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Sword */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sword"),
		CSword::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Sword");
		return E_FAIL;
	}
	//===========except basic prototype gameobjects================//
	/* For.Prototype_GameObject_Camera_Player */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Player"),
		CCamera_Player::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Camera_Player");
		return E_FAIL;
	}
	
	//================Directly Extracting Prototype Objects======================//
	//===================Terrain====================//
	//(임시로 오브젝트형식 빌려와서 사용)
	/* For.Prototype_GameObject_Grass */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Grass"),
		CGrass::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Grass");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Train */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Train"),
		CTrain::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Train");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Road */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Road"),
		CRoad::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Road");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Water */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Water"),
		CWater::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Water");
		return E_FAIL;
	}
	//=============================Objects=============================//
	/* For.Prototype_GameObject_Tree */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tree"),
		CTree::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Tree");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_TreeA */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TreeA"),
		CTreeA::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_TreeA");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_TreeB */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TreeB"),
		CTreeB::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_TreeB");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_TreeC */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TreeC"),
		CTreeC::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_TreeC");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_FineTreeA */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FineTreeA"),
		CFineTreeA::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_FineTreeA");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_FineTreeB */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FineTreeB"),
		CFineTreeB::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_FineTreeB");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_FineTreeC */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FineTreeC"),
		CFineTreeC::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_FineTreeC");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Buliding */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Buliding"),
		CBuliding::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Buliding");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Castle */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Castle"),
		CCastle::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Castle");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Shrine */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Shrine"),
		CShrine::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Shrine");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Sign */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sign"),
		CSign::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Sign");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Apartment */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Apartment"),
		CApartment::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Apartment");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_BBQ */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BBQ"),
		CBBQ::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_BBQ");
		return E_FAIL;
	}
	//=============================Player===============================//
	/* For.Prototype_GameObject_Psy */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Psy"),
		CPsy::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Psy");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Player_Chicken */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_Chicken"),
		CPlayer_Chicken::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Player_Chicken");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Player_Koala */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_Koala"),
		CPlayer_Koala::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Player_Koala");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Player_Korea_Drummer */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_Korea_Drummer"),
		CPlayer_Korea_Drummer::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Player_Korea_Drummer");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Player_Samurai */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_Samurai"),
		CPlayer_Samurai::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Player_Samurai");
		return E_FAIL;
	}

	//======================Car, Truck, Taxi, Train(Front, Middle, End)==================//
	//==========================Train============================//
	/* For.Prototype_GameObject_Monster_Train */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Train"),
		CMonster_Train::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Monster_Train");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Monster_TrainMiddle */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_TrainMiddle"),
		CMonster_TrainMiddle::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Monster_TrainMiddle");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Monster_TrainEnd */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_TrainEnd"),
		CMonster_TrainEnd::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Monster_TrainEnd");
		return E_FAIL;
	}
	//============================korea_train======================//
	/* For.Prototype_GameObject_Monster_Train */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_KTrain_Front"),
		CKTrain_Front::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Monster_KTrain_Front");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Monster_TrainMiddle */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_KTrain_Middle"),
		CKTrain_Middle::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Monster_KTrain_Middle");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Monster_TrainEnd */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_KTrain_End"),
		CKTrain_End::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Monster_KTrain_End");
		return E_FAIL;
	}
	//===========================Car============================//
	/* For.Prototype_GameObject_Monster_Car */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Car"),
		CMonster_Car::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Monster_Car");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_KoreaCar001 */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_KoreaCar001"),
		CKoreaCar001::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_KoreaCar001");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_KoreaCar002 */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_KoreaCar002"),
		CKoreaCar002::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_KoreaCar002");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_KoreaCar003 */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_KoreaCar003"),
		CKoreaCar003::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_KoreaCar003");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Muccle */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Muccle"),
		CMuccle::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Muccle");
		return E_FAIL;
	}
	//=====================Taxi========================//
	/* For.Prototype_GameObject_Monster_Taxi */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Taxi"),
		CMonster_Taxi::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Monster_Taxi");
		return E_FAIL;
	}
	//======================Truck========================//
	/* For.Prototype_GameObject_Monster_Truck */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Truck"),
		CMonster_Truck::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Monster_Truck");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_Monster_Truck2 */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Truck2"),
		CMonster_Truck2::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Monster_Truck2");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_Monster_OilTruck */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_OilTruck"),
		CMonster_OilTruck::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Monster_OilTruck");
		return E_FAIL;
	}
	//====================ITEM=====================//
	/* For.Prototype_GameObject_Item */

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item"),
		CItem::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Item");
		return E_FAIL;
	}
	//======================UI==================//
	/* For.Prototype_GameObject_UI */
	/*if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Money"),
		CUI_Money::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_UI_Money");
		return E_FAIL;
	}*/

	///* For.Prototype_GameObject_Effect */
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect"), CEffect::Create(m_pGraphic_Device))))
	//	return E_FAIL;

#pragma endregion

	RELEASE_INSTANCE(CGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForKoreaLevel()
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

#pragma region PROTOTYPE_COMPONENT
	pGameInstance->Set_CurrentLevelIndexForPrototype(LEVEL_KOREA);
	//==============================BasicOffer : Prototype Component Model=======================//
	Create_BasicOffer(LEVEL_KOREA);
	//==============================Directly Extracting : Prototype Component Model=======================//
	_matrix			TransformMatrix;
	//==============================Player : Prototype Component Model=======================//
	Create_PlayerPrototype(LEVEL_KOREA);
	//==============================Terrain : Prototype Component Model=======================//
	Create_TerrainPrototype(LEVEL_KOREA);
	//==============================Objects : Prototype Component Model=======================//
	Create_StructurePrototype(LEVEL_KOREA);
	//==============================Car, Train, Truck, Taxi : Prototype Component Model=======================//
	Create_MonsterPrototype(LEVEL_KOREA);
	//==============================Item : Prototype Component Model=======================//
	TransformMatrix = XMMatrixScaling(0.02f, 0.02f, 0.02f) * XMMatrixRotationY(XMConvertToRadians(0.0f));
	/* For.Prototype_Component_Model_Item */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_KOREA, TEXT("Prototype_Component_Model_Item"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Object/Coin/", "coin_optimised.fbx", TransformMatrix))))
	{
		MSGBOX("Failed to Prototype_Component_Model_Item");
		return E_FAIL;
	}
	//==============================UI : Prototype Component Model=======================//
	//==============================Shader : Prototype Component Model=======================//
	Create_Shader(LEVEL_KOREA);
#pragma endregion

#pragma  region PROTOTYPE_GAMEOBJECT
	/* 원형객체를 생성한다. */

	//===========================BasicOffer_Prototype_GameObject===========================//
	/* For.Prototype_GameObject_Player */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Player");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Terrain");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Monster */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster"),
		CMonster::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Monster");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Camera_Dynamic */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Dynamic"),
		CCamera_Dynamic::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Camera_Dynamic");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Fork */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Fork"),
		CFork::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Fork");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Sword */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sword"),
		CSword::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Sword");
		return E_FAIL;
	}
	//===========except basic prototype gameobjects================//
	/* For.Prototype_GameObject_Camera_Player */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Player"),
		CCamera_Player::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Camera_Player");
		return E_FAIL;
	}

	//================Directly Extracting Prototype Objects======================//
	//===================Terrain====================//
	//(임시로 오브젝트형식 빌려와서 사용)
	/* For.Prototype_GameObject_Grass */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Grass"),
		CGrass::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Grass");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Train */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Train"),
		CTrain::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Train");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Road */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Road"),
		CRoad::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Road");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Water */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Water"),
		CWater::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Water");
		return E_FAIL;
	}
	//=============================Objects=============================//
	/* For.Prototype_GameObject_Tree */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tree"),
		CTree::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Tree");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_TreeA */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TreeA"),
		CTreeA::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_TreeA");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_TreeB */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TreeB"),
		CTreeB::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_TreeB");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_TreeC */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TreeC"),
		CTreeC::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_TreeC");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_FineTreeA */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FineTreeA"),
		CFineTreeA::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_FineTreeA");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_FineTreeB */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FineTreeB"),
		CFineTreeB::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_FineTreeB");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_FineTreeC */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FineTreeC"),
		CFineTreeC::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_FineTreeC");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Buliding */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Buliding"),
		CBuliding::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Buliding");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Castle */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Castle"),
		CCastle::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Castle");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Shrine */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Shrine"),
		CShrine::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Shrine");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Sign */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sign"),
		CSign::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Sign");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Apartment */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Apartment"),
		CApartment::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Apartment");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_BBQ */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BBQ"),
		CBBQ::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_BBQ");
		return E_FAIL;
	}
	//=============================Player===============================//
	/* For.Prototype_GameObject_Psy */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Psy"),
		CPsy::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Psy");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Player_Chicken */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_Chicken"),
		CPlayer_Chicken::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Player_Chicken");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Player_Koala */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_Koala"),
		CPlayer_Koala::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Player_Koala");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Player_Korea_Drummer */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_Korea_Drummer"),
		CPlayer_Korea_Drummer::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Player_Korea_Drummer");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Player_Samurai */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_Samurai"),
		CPlayer_Samurai::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Player_Samurai");
		return E_FAIL;
	}

	//======================Car, Truck, Taxi, Train(Front, Middle, End)==================//
	//==========================Train============================//
	/* For.Prototype_GameObject_Monster_Train */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Train"),
		CMonster_Train::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Monster_Train");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Monster_TrainMiddle */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_TrainMiddle"),
		CMonster_TrainMiddle::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Monster_TrainMiddle");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Monster_TrainEnd */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_TrainEnd"),
		CMonster_TrainEnd::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Monster_TrainEnd");
		return E_FAIL;
	}
	//============================korea_train======================//
	/* For.Prototype_GameObject_Monster_Train */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_KTrain_Front"),
		CKTrain_Front::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Monster_KTrain_Front");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Monster_TrainMiddle */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_KTrain_Middle"),
		CKTrain_Middle::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Monster_KTrain_Middle");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Monster_TrainEnd */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_KTrain_End"),
		CKTrain_End::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Monster_KTrain_End");
		return E_FAIL;
	}
	//===========================Car============================//
	/* For.Prototype_GameObject_Monster_Car */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Car"),
		CMonster_Car::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Monster_Car");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_KoreaCar001 */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_KoreaCar001"),
		CKoreaCar001::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_KoreaCar001");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_KoreaCar002 */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_KoreaCar002"),
		CKoreaCar002::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_KoreaCar002");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_KoreaCar003 */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_KoreaCar003"),
		CKoreaCar003::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_KoreaCar003");
		return E_FAIL;
	}
	/* For.Prototype_GameObject_Muccle */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Muccle"),
		CMuccle::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Muccle");
		return E_FAIL;
	}
	//=====================Taxi========================//
	/* For.Prototype_GameObject_Monster_Taxi */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Taxi"),
		CMonster_Taxi::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Monster_Taxi");
		return E_FAIL;
	}
	//======================Truck========================//
	/* For.Prototype_GameObject_Monster_Truck */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Truck"),
		CMonster_Truck::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Monster_Truck");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_Monster_Truck2 */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Truck2"),
		CMonster_Truck2::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Monster_Truck2");
		return E_FAIL;
	}

	/* For.Prototype_GameObject_Monster_OilTruck */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_OilTruck"),
		CMonster_OilTruck::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Monster_OilTruck");
		return E_FAIL;
	}
	//====================ITEM=====================//
	/* For.Prototype_GameObject_Item */

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Item"),
		CItem::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_GameObject_Item");
		return E_FAIL;
	}
	//======================UI==================//
	/* For.Prototype_GameObject_UI */
	/*if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Money"),
	CUI_Money::Create(m_pDevice, m_pDeviceContext))))
	{
	MSGBOX("Failed to Prototype_GameObject_UI_Money");
	return E_FAIL;
	}*/

	///* For.Prototype_GameObject_Effect */
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect"), CEffect::Create(m_pGraphic_Device))))
	//	return E_FAIL;

#pragma endregion

	RELEASE_INSTANCE(CGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForJapanLevel()
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);



	RELEASE_INSTANCE(CGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForBoss()
{
	for (_uint i = 0; i < 9999999999; ++i)
		int a = 10;

	m_isFinished = true;

	return S_OK;
}

void CLoader::Create_BasicOffer(LEVEL CurLevel)
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);
	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/MyHeight.bmp")))))
	{
		MSGBOX("Failed to Prototype_Component_VIBuffer_Terrain");
	}

	/* For.Prototype_Component_VIBuffer_Cube */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pDeviceContext))))
	{
		MSGBOX("Failed to Prototype_Component_VIBuffer_Cube");
	}

	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/CrossRoad/Texture/Map.png"), 1))))
	{
		MSGBOX("Failed to Prototype_Component_Texture_Terrain");
	}

	/* For.Prototype_Component_Texture_Filter */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Filter"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Terrain/Filter.bmp"), 1))))
	{
		MSGBOX("Failed to Prototype_Component_Texture_Filter");
	}

	//==============================BasicOfferCollider : Prototype Component Model=======================//

	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pDeviceContext, CCollider::TYPE_AABB))))
	{
		MSGBOX("Failed to Prototype_Component_Collider_AABB");
	}

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pDeviceContext, CCollider::TYPE_OBB))))
	{
		MSGBOX("Failed to Prototype_Component_Collider_OBB");
	}

	/* For.Prototype_Component_Collider_SPHERE */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"),
		CCollider::Create(m_pDevice, m_pDeviceContext, CCollider::TYPE_SPHERE))))
	{
		MSGBOX("Failed to Prototype_Component_Collider_SPHERE");
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CLoader::Create_TerrainPrototype(LEVEL CurLevel)
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			TransformMatrix;

	TransformMatrix = XMMatrixScaling(0.02f, 0.01f, 0.02f);

	/* For.Prototype_Component_Model_Grass */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Grass"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Terrain/Grass/", "strip_grass_1_optimised.fbx", TransformMatrix))))
	{
		MSGBOX("Failed to Prototype_Component_Model_Grass");
	}

	/* For.Prototype_Component_Model_Train */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Train"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Terrain/Train/", "strip_train_optimised.fbx", TransformMatrix))))
	{
		MSGBOX("Failed to Prototype_Component_Model_Train");
	}

	/* For.Prototype_Component_Model_Road */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Road"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Terrain/Road/", "strip_road_1_optimised.fbx", TransformMatrix))))
	{
		MSGBOX("Failed to Prototype_Component_Model_Road");
	}

	/* For.Prototype_Component_Model_Water */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Water"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Terrain/Water/", "strip_water_1_optimised.fbx", TransformMatrix))))
	{
		MSGBOX("Failed to Prototype_Component_Model_Water");
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CLoader::Create_PlayerPrototype(LEVEL CurLevel)
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			TransformMatrix;

	TransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));

	TransformMatrix = XMMatrixScaling(1.0f, 1.0f, 1.0f) * XMMatrixRotationY(XMConvertToRadians(180.0f));

	/* For.Prototype_Component_Model_Psy */
	if (FAILED(pGameInstance->Add_Prototype(CurLevel, TEXT("Prototype_Component_Model_Psy"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Player/Psy/", "PsyDancers.fbx", TransformMatrix))))
	{
		MSGBOX("Failed to Prototype_Component_Model_Psy");
	}
	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	/* For.Prototype_Component_Model_Player_Chicken */
	if (FAILED(pGameInstance->Add_Prototype(CurLevel, TEXT("Prototype_Component_Model_Player_Chicken"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Player/Chicken/", "Chicken.fbx", TransformMatrix))))
	{
		MSGBOX("Failed to Prototype_Component_Model_Player_Chicken");
	}
	/* For.Prototype_Component_Model_Player_Koala */
	if (FAILED(pGameInstance->Add_Prototype(CurLevel, TEXT("Prototype_Component_Model_Player_Koala"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Player/Koala/", "koala_optimised.fbx", TransformMatrix))))
	{
		MSGBOX("Failed to Prototype_Component_Model_Player_Koala");
	}
	/* For.Prototype_Component_Model_Player_Korea_Drummer */
	if (FAILED(pGameInstance->Add_Prototype(CurLevel, TEXT("Prototype_Component_Model_Player_Korea_Drummer"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Player/KoreaDrummer/", "KoreanDrummer.fbx", TransformMatrix))))
	{
		MSGBOX("Failed to Prototype_Component_Model_Player_Korea_Drummer");
	}
	/* For.Prototype_Component_Model_Player_Samurai */
	if (FAILED(pGameInstance->Add_Prototype(CurLevel, TEXT("Prototype_Component_Model_Player_Samurai"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Player/Samurai/", "japan_samurai_idle_1_optimised.fbx", TransformMatrix))))
	{
		MSGBOX("Failed to Prototype_Component_Model_Player_Samurai");
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CLoader::Create_MonsterPrototype(LEVEL CurLevel)
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			TransformMatrix;

	TransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(0.0f));

	/* For.Prototype_Component_Model_Monster_Train */
	if (FAILED(pGameInstance->Add_Prototype(CurLevel, TEXT("Prototype_Component_Model_Monster_Train"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Monster/TrainCar/", "train_front_optimised.fbx", TransformMatrix))))
	{
		MSGBOX("Failed to Prototype_Component_Model_Monster_Train");
	}
	/* For.Prototype_Component_Model_KTrain_Front */
	if (FAILED(pGameInstance->Add_Prototype(CurLevel, TEXT("Prototype_Component_Model_KTrain_Front"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Monster/TrainCar/Korea/", "train_front_korea_optimised.fbx", TransformMatrix))))
	{
		MSGBOX("Failed to Prototype_Component_Model_KTrain_Front");
	}

	TransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	/* For.Prototype_Component_Model_Monster_TrainMiddle */
	if (FAILED(pGameInstance->Add_Prototype(CurLevel, TEXT("Prototype_Component_Model_Monster_TrainMiddle"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Monster/TrainCar/", "train_middle_optimised.fbx", TransformMatrix))))
	{
		MSGBOX("Failed to Prototype_Component_Model_Monster_TrainMiddle");
	}
	/* For.Prototype_Component_Model_KTrain_Front */
	if (FAILED(pGameInstance->Add_Prototype(CurLevel, TEXT("Prototype_Component_Model_KTrain_Middle"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Monster/TrainCar/Korea/", "train_middle_korea_optimised.fbx", TransformMatrix))))
	{
		MSGBOX("Failed to Prototype_Component_Model_KTrain_Middle");
	}
	/* For.Prototype_Component_Model_Monster_TrainEnd */
	if (FAILED(pGameInstance->Add_Prototype(CurLevel, TEXT("Prototype_Component_Model_Monster_TrainEnd"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Monster/TrainCar/", "train_back_optimised.fbx", TransformMatrix))))
	{
		MSGBOX("Failed to Prototype_Component_Model_Monster_TrainEnd");
	}
	/* For.Prototype_Component_Model_KTrain_Front */
	if (FAILED(pGameInstance->Add_Prototype(CurLevel, TEXT("Prototype_Component_Model_KTrain_End"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Monster/TrainCar/Korea/", "train_back_korea_optimised.fbx", TransformMatrix))))
	{
		MSGBOX("Failed to Prototype_Component_Model_KTrain_End");
	}

	TransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(270.0f));

	//======Cars=======//
	/* For.Prototype_Component_Model_Monster_Car */
	//Green
	if (FAILED(pGameInstance->Add_Prototype(CurLevel, TEXT("Prototype_Component_Model_Monster_Car"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Monster/Car/", "mini_car_1_optimised.fbx", TransformMatrix))))
	{
		MSGBOX("Failed to Prototype_Component_Model_Monster_Car");
	}

	TransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(270.0f));

	/* For.Prototype_Component_Model_CKoreaCar001 */
	//koreabox
	if (FAILED(pGameInstance->Add_Prototype(CurLevel, TEXT("Prototype_Component_Model_CKoreaCar001"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Monster/Car/", "box_truck_korea_optimised.fbx", TransformMatrix))))
	{
		MSGBOX("Failed to Prototype_Component_Model_CKoreaCar001");
	}

	/* For.Prototype_Component_Model_CKoreaCar002 */
	//jpn katamari sportscar
	if (FAILED(pGameInstance->Add_Prototype(CurLevel, TEXT("Prototype_Component_Model_CKoreaCar002"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Monster/Car/", "katamari_sportscar_optimised.fbx", TransformMatrix))))
	{
		MSGBOX("Failed to Prototype_Component_Model_CKoreaCar002");
	}

	TransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(90.0f));

	/* For.Prototype_Component_Model_CKoreaCar003 */
	//jpn gray
	if (FAILED(pGameInstance->Add_Prototype(CurLevel, TEXT("Prototype_Component_Model_CKoreaCar003"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Monster/Car/", "japannightcity_car_1_optimised.fbx", TransformMatrix))))
	{
		MSGBOX("Failed to Prototype_Component_Model_CKoreaCar003");
	}

	//if (FAILED(pGameInstance->Add_Prototype(CurLevel, TEXT("Prototype_Component_Model_CJapanCar001"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Monster/Car/", "box_truck_no_boxes_korea_optimised.fbx", TransformMatrix))))
	//{
	//	MSGBOX("Failed to Prototype_Component_Model_CJapanCar001");
	//}

	//if (FAILED(pGameInstance->Add_Prototype(CurLevel, TEXT("Prototype_Component_Model_CJapanCar002"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Monster/Car/", "flower_truck_korea_optimised.fbx", TransformMatrix))))
	//{
	//	MSGBOX("Failed to Prototype_Component_Model_CJapanCar002");
	//}

	//if (FAILED(pGameInstance->Add_Prototype(CurLevel, TEXT("Prototype_Component_Model_CJapanCar003"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Monster/Car/", "japannightcity_car_1_optimised.fbx", TransformMatrix))))
	//{
	//	MSGBOX("Failed to Prototype_Component_Model_CJapanCar003");
	//}

	/* For.Prototype_Component_Model_Muccle */
	if (FAILED(pGameInstance->Add_Prototype(CurLevel, TEXT("Prototype_Component_Model_CMuccle"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Monster/Car/", "muscle_optimised.fbx", TransformMatrix))))
	{
		MSGBOX("Failed to Prototype_Component_Model_CMuccle");
	}

	/* For.Prototype_Component_Model_Monster_Taxi */
	if (FAILED(pGameInstance->Add_Prototype(CurLevel, TEXT("Prototype_Component_Model_Monster_Taxi"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Monster/Car/", "mini_car_1_optimised.fbx", TransformMatrix))))
	{
		MSGBOX("Failed to Prototype_Component_Model_Monster_Taxi");
	}

	TransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(270.0f));
	/* For.Prototype_Component_Model_Monster_Truck */
	if (FAILED(pGameInstance->Add_Prototype(CurLevel, TEXT("Prototype_Component_Model_Monster_Truck"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Monster/Truck/", "truck_optimised.fbx", TransformMatrix))))
	{
		MSGBOX("Failed to Prototype_Component_Model_Monster_Truck");
	}

	/* For.Prototype_Component_Model_Monster_Truck2 */
	if (FAILED(pGameInstance->Add_Prototype(CurLevel, TEXT("Prototype_Component_Model_Monster_Truck2"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Monster/Truck/", "truck_blue_optimised.fbx", TransformMatrix))))
	{
		MSGBOX("Failed to Prototype_Component_Model_Monster_Truck2");
	}

	/* For.Prototype_Component_Model_Monster_OilTruck */
	if (FAILED(pGameInstance->Add_Prototype(CurLevel, TEXT("Prototype_Component_Model_Monster_OilTruck"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Monster/Truck/", "oil_truck_optimised.fbx", TransformMatrix))))
	{
		MSGBOX("Failed to Prototype_Component_Model_Monster_OilTruck");
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CLoader::Create_StructurePrototype(LEVEL CurLevel)
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			TransformMatrix;

	TransformMatrix = XMMatrixScaling(0.025f, 0.025f, 0.025f);

	/* For.Prototype_Component_Model_Tree */
	if (FAILED(pGameInstance->Add_Prototype(CurLevel, TEXT("Prototype_Component_Model_Tree"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Object/Tree/", "tree_1_optimised.fbx", TransformMatrix))))
	{
		MSGBOX("Failed to Prototype_Component_Model_Tree");
	}
	/* For.Prototype_Component_Model_TreeA */
	if (FAILED(pGameInstance->Add_Prototype(CurLevel, TEXT("Prototype_Component_Model_TreeA"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Object/Tree/", "tree_2_optimised.fbx", TransformMatrix))))
	{
		MSGBOX("Failed to Prototype_Component_Model_TreeA");
	}
	/* For.Prototype_Component_Model_TreeB */
	if (FAILED(pGameInstance->Add_Prototype(CurLevel, TEXT("Prototype_Component_Model_TreeB"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Object/Tree/", "tree_3_optimised.fbx", TransformMatrix))))
	{
		MSGBOX("Failed to Prototype_Component_Model_TreeB");
	}
	/* For.Prototype_Component_Model_TreeC */
	if (FAILED(pGameInstance->Add_Prototype(CurLevel, TEXT("Prototype_Component_Model_TreeC"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Object/Tree/", "tree_4_optimised.fbx", TransformMatrix))))
	{
		MSGBOX("Failed to Prototype_Component_Model_TreeC");
	}
	/* For.Prototype_Component_Model_FineTreeA */
	if (FAILED(pGameInstance->Add_Prototype(CurLevel, TEXT("Prototype_Component_Model_FineTreeA"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Object/Tree/", "tree_pine_1_optimised.fbx", TransformMatrix))))
	{
		MSGBOX("Failed to Prototype_Component_Model_FineTreeA");
	}
	/* For.Prototype_Component_Model_FineTreeB */
	if (FAILED(pGameInstance->Add_Prototype(CurLevel, TEXT("Prototype_Component_Model_FineTreeB"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Object/Tree/", "tree_pine_2_optimised.fbx", TransformMatrix))))
	{
		MSGBOX("Failed to Prototype_Component_Model_FineTreeB");
	}
	/* For.Prototype_Component_Model_FineTreeC */
	if (FAILED(pGameInstance->Add_Prototype(CurLevel, TEXT("Prototype_Component_Model_FineTreeC"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Object/Tree/", "tree_pine_3_optimised.fbx", TransformMatrix))))
	{
		MSGBOX("Failed to Prototype_Component_Model_FineTreeC");
	}
	///* For.Prototype_Component_Model_Buliding */ //undo
	//if (FAILED(pGameInstance->Add_Prototype(CurLevel, TEXT("Prototype_Component_Model_Buliding"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Object/Structure/Japan/", "tree_1_optimised.fbx", TransformMatrix))))
	//{
	//	MSGBOX("Failed to Prototype_Component_Model_Buliding");
	//}
	///* For.Prototype_Component_Model_Castle */ //undo
	//if (FAILED(pGameInstance->Add_Prototype(CurLevel, TEXT("Prototype_Component_Model_Castle"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Object/Structure/Japan/", "tree_1_optimised.fbx", TransformMatrix))))
	//{
	//	MSGBOX("Failed to Prototype_Component_Model_Castle");
	//}
	///* For.Prototype_Component_Model_Shrine */ //undo
	//if (FAILED(pGameInstance->Add_Prototype(CurLevel, TEXT("Prototype_Component_Model_Shrine"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Object/Structure/Japan/", "tree_1_optimised.fbx", TransformMatrix))))
	//{
	//	MSGBOX("Failed to Prototype_Component_Model_Shrine");
	//}
	///* For.Prototype_Component_Model_Sign */ //undo
	//if (FAILED(pGameInstance->Add_Prototype(CurLevel, TEXT("Prototype_Component_Model_Sign"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Object/Structure/Japan/", "tree_1_optimised.fbx", TransformMatrix))))
	//{
	//	MSGBOX("Failed to Prototype_Component_Model_Sign");
	//}
	/* For.Prototype_Component_Model_Apartment */
	if (FAILED(pGameInstance->Add_Prototype(CurLevel, TEXT("Prototype_Component_Model_Apartment"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Object/Structure/korea/Apartment/", "korean_city_1_optimised.fbx", TransformMatrix))))
	{
		MSGBOX("Failed to Prototype_Component_Model_Apartment");
	}
	/* For.Prototype_Component_Model_BBQ */
	if (FAILED(pGameInstance->Add_Prototype(CurLevel, TEXT("Prototype_Component_Model_BBQ"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/CrossRoad/Mesh/Object/Structure/korea/BBQ/", "KoreanBBQ.fbx", TransformMatrix))))
	{
		MSGBOX("Failed to Prototype_Component_Model_BBQ");
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CLoader::Create_Shader(LEVEL CurLevel)
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			TransformMatrix;

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	/* For.Prototype_Component_Shader_Cube */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_Cube"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxCubeTex.hlsl"),
			VTXCUBETEX_DECLARATION::Elements, VTXCUBETEX_DECLARATION::iNumElements))))
	{
		MSGBOX("Failed to Prototype_Component_Shader_Cube");
	}

	/* For.Prototype_Component_Shader_VtxNorTex */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"),
			VTXNORTEX_DECLARATION::Elements, VTXNORTEX_DECLARATION::iNumElements))))
	{
		MSGBOX("Failed to Prototype_Component_Shader_VtxNorTex");
	}

	/* For.Prototype_Component_Shader_NonAnimModel*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_NonAnimModel"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_NonAnimModel.hlsl"),
			VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements))))
	{
		MSGBOX("Failed to Prototype_Component_Shader_NonAnimModel");
	}

	/* For.Prototype_Component_Shader_AnimModel*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_AnimModel"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_AnimModel.hlsl"),
			VTXANIMMODEL_DECLARATION::Elements, VTXANIMMODEL_DECLARATION::iNumElements))))
	{
		MSGBOX("Failed to Prototype_Component_Shader_AnimModel");
	}

	RELEASE_INSTANCE(CGameInstance);
}

CLoader * CLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, LEVEL eLevel)
{
	CLoader*	pInstance = new CLoader(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(eLevel)))
	{
		MSGBOX("Failed to Creating CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);
	DeleteObject(m_hThread);
	DeleteCriticalSection(&m_CS);

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
