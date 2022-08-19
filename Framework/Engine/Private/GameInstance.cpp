#include "..\Public\GameInstance.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
	: m_pGraphic_Device(CGraphic_Device::GetInstance())
	, m_pInput_Device(CInput_Device::GetInstance())
	, m_pTimer_Manager(CTimer_Manager::GetInstance())
	, m_pComponent_Manager(CComponent_Manager::GetInstance())
	, m_pLevel_Manager(CLevel_Manager::GetInstance())
	, m_pObject_Manager(CObject_Manager::GetInstance())
	, m_pPipeLine(CPipeLine::GetInstance())
	, m_pLight_Manager(CLight_Manager::GetInstance())
	, m_pFrustum(CFrustum::GetInstance())
	, m_pFont_Manager(CFont_Manager::GetInstance())
	, m_pSound_Manager(CSoundMgr::GetInstance())
{
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pInput_Device);
	Safe_AddRef(m_pTimer_Manager);
	Safe_AddRef(m_pComponent_Manager);
	Safe_AddRef(m_pLevel_Manager);
	Safe_AddRef(m_pObject_Manager);
	Safe_AddRef(m_pPipeLine);
	Safe_AddRef(m_pLight_Manager);
	Safe_AddRef(m_pFrustum);
	Safe_AddRef(m_pFont_Manager);
	Safe_AddRef(m_pSound_Manager);
}

HRESULT CGameInstance::Initialize_Engine(HINSTANCE hInstance, _uint iNumLevels, const CGraphic_Device::GRAPHICDESC & GraphicDesc, ID3D11Device ** ppDeviceOut, ID3D11DeviceContext ** ppDeviceContextOut)
{
	if (nullptr == m_pGraphic_Device ||
		nullptr == m_pObject_Manager)
	{
		MSGBOX("Failed to Initialize_Engine");
		return E_FAIL;
	}
	
	if (FAILED(m_pGraphic_Device->Ready_Graphic_Device(GraphicDesc.hWnd, GraphicDesc.eWinMode, GraphicDesc.iWinCX, GraphicDesc.iWinCY, ppDeviceOut, ppDeviceContextOut)))
	{
		MSGBOX("Failed to Graphic_Device Ready_Graphic_Device");
		return E_FAIL;
	}

	if (FAILED(m_pInput_Device->Ready_Input_Device(hInstance, GraphicDesc.hWnd)))
	{
		MSGBOX("Failed to Input_Device Ready_Input_Device");
		return E_FAIL;
	}

	if (FAILED(m_pObject_Manager->Reserve_Container(iNumLevels)))
	{
		MSGBOX("Failed to Object_Manager Reserve_Container");
		return E_FAIL;
	}

	if (FAILED(m_pComponent_Manager->Reserve_Container(iNumLevels)))
	{
		MSGBOX("Failed to Component_Manager Reserve_Container");
		return E_FAIL;
	}

	if (FAILED(m_pFrustum->Initialize()))
	{
		MSGBOX("Failed to Frustum Initialize");
		return E_FAIL;
	}

	if (FAILED(m_pSound_Manager->NativeConstruct()))
	{
		MSGBOX("Failed to SoundMgr");
		return E_FAIL;
	}

	return S_OK;
}

_int CGameInstance::Tick_Engine(_double TimeDelta)
{
	if (nullptr == m_pLevel_Manager ||
		nullptr == m_pObject_Manager)
	{
		MSGBOX("Failed to Tick_Engine");
		return -1;
	}

	if (FAILED(m_pInput_Device->SetUp_InputDeviceState()))
	{
		MSGBOX("Failed to Input_Device SetUp_InputDeviceState");
		return -1;
	}

	if (0 > m_pObject_Manager->Tick(TimeDelta))
	{
		MSGBOX("Failed to Object_Manager Tick");
		return -1;
	}

	if (0 > m_pLevel_Manager->Tick(TimeDelta))
	{
		MSGBOX("Failed to Level_Manager Tick");
		return -1;
	}

	m_pPipeLine->Update();

	if (0 > m_pObject_Manager->LateTick(TimeDelta))
	{
		MSGBOX("Failed to Object_Manager LateTick");
		return -1;
	}

	if (0 > m_pLevel_Manager->LateTick(TimeDelta))
	{
		MSGBOX("Failed to Level_Manager LateTick");
		return -1;
	}

	return _int();
}

HRESULT CGameInstance::Clear_LevelResource(_uint iLevelIndex)
{
	if (0 > iLevelIndex)
	{
		MSGBOX("CGameInstance::Clear_LevelResource : Out of range");
		return E_FAIL;
	}

	if (nullptr == m_pObject_Manager ||
		nullptr == m_pComponent_Manager)
	{
		MSGBOX("Failed to Clear_LevelResource");
		return E_FAIL;
	}

	if (FAILED(m_pComponent_Manager->Clear_LevelObject(iLevelIndex)))
	{
		MSGBOX("Failed to Component_Manager Clear_LevelObject");
		return E_FAIL;
	}

	if (FAILED(m_pObject_Manager->Clear_LevelObject(iLevelIndex)))
	{
		MSGBOX("Failed to Object_Manager Clear_LevelObject");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CGameInstance::Clear_LevelResource_For_ReStart(_uint iLevelIndex)
{
	if (0 > iLevelIndex)
	{
		MSGBOX("CGameInstance::Clear_LevelResource : Out of range");
		return E_FAIL;
	}

	if (nullptr == m_pObject_Manager)
	{
		MSGBOX("CGameInstance::Clear_LevelResource : Not found Object_Manager");
		return E_FAIL;
	}

	if (nullptr == m_pComponent_Manager)
	{
		MSGBOX("CGameInstance::Clear_LevelResource : Not found Component_Manager");
		return E_FAIL;
	}

	if (FAILED(m_pObject_Manager->Clear_LevelObject(iLevelIndex)))
	{
		MSGBOX("CGameInstance::Clear_LevelResource : Failed to Clear_LevelObject(Obj_Manager)");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CGameInstance::Clear_BackBuffer_View(_float4 vClearColor)
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("Failed to Clear_BackBuffer_View");
		return E_FAIL;
	}

	return m_pGraphic_Device->Clear_BackBuffer_View(vClearColor);
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("Failed to Clear_DepthStencil_View");
		return E_FAIL;
	}

	return m_pGraphic_Device->Clear_DepthStencil_View();
}

HRESULT CGameInstance::Present()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSGBOX("Failed to Present");
		return E_FAIL;
	}

	return m_pGraphic_Device->Present();
}

_byte CGameInstance::Get_DIKeyState(_ubyte eKeyID)
{
	if (nullptr == m_pInput_Device)
	{
		MSGBOX("Failed to Get_DIKeyState");
		return 0;
	}

	return m_pInput_Device->Get_DIKeyState(eKeyID);
}

_long CGameInstance::Get_DIMouseMoveState(CInput_Device::MOUSEMOVESTATE eMouseMoveState)
{
	if (nullptr == m_pInput_Device)
	{
		MSGBOX("Failed to Get_DIMouseMoveState");
		return 0;
	}

	return m_pInput_Device->Get_DIMouseMoveState(eMouseMoveState);
}

_byte CGameInstance::Get_DIMouseButtonState(CInput_Device::MOUSEBUTTONSTATE eMouseButtonState)
{
	if (nullptr == m_pInput_Device)
	{
		MSGBOX("Failed to Get_DIMouseButtonState");
		return 0;
	}

	return m_pInput_Device->Get_DIMouseButtonState(eMouseButtonState);
}

_bool CGameInstance::Key_Pressing(int _Key)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Key_Pressing(_Key);
}

_bool CGameInstance::Key_Down(int _Key)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Key_Down(_Key);
}

_bool CGameInstance::Key_Up(int _Key)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Key_Up(_Key);
}

_double CGameInstance::Get_TimeDelta(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
	{
		MSGBOX("Failed to Get_TimeDelta");
		return 0.f;
	}

	return m_pTimer_Manager->Get_TimeDelta(pTimerTag);
}

HRESULT CGameInstance::Add_Timer(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
	{
		MSGBOX("Failed to Add_Timer");
		return E_FAIL;
	}

	return m_pTimer_Manager->Add_Timer(pTimerTag);
}

HRESULT CGameInstance::OpenLevel(_uint iLevelIndex, CLevel * pNextLevel)
{
	if (nullptr == m_pLevel_Manager)
	{
		MSGBOX("Failed to OpenLevel");
		return E_FAIL;
	}

	return m_pLevel_Manager->OpenLevel(iLevelIndex, pNextLevel);
}

HRESULT CGameInstance::Render_Level()
{
	if (nullptr == m_pLevel_Manager)
	{
		MSGBOX("Failed to Render_Level");
		return E_FAIL;
	}

	return m_pLevel_Manager->Render();
}

const _uint & CGameInstance::Get_CurrentLevelIndex()
{
	if (nullptr == m_pLevel_Manager)
		MSGBOX("CGameInstance::Get_CurrentLevelIndex : Not found Level Manager");

	return m_pLevel_Manager->Get_CurrentLevelIndex();
}

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _tchar * pPrototypeTag, CComponent * pPrototype)
{
	if (nullptr == m_pComponent_Manager)
	{
		MSGBOX("Failed to Add_Prototype");
		return E_FAIL;
	}

	return m_pComponent_Manager->Add_Prototype(iLevelIndex, pPrototypeTag, pPrototype);
}

CComponent * CGameInstance::Clone_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, void * pArg)
{
	if (nullptr == m_pComponent_Manager)
	{
		MSGBOX("Failed to Clone_Component");
		return nullptr;
	}

	return m_pComponent_Manager->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
}

CComponent * CGameInstance::Get_Component(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pComponentTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
	{
		MSGBOX("Failed to Get_Component");
		return nullptr;
	}

	return m_pObject_Manager->Get_Component(iLevelIndex, pLayerTag, pComponentTag, iIndex);
}

HRESULT CGameInstance::Add_Prototype(const _tchar * pPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == m_pObject_Manager)
	{
		MSGBOX("Failed to Add_Prototype");
		return E_FAIL;
	}

	return m_pObject_Manager->Add_Prototype(pPrototypeTag, pPrototype);
}

CGameObject * CGameInstance::Get_Clone(const _tchar * pPrototypeTag, void * pArg)
{
	if (nullptr == m_pObject_Manager)
	{
		//MSGBOX("CGameInstance::Get_GameObjectList : Not found Object Manager");
		return nullptr;
	}

	return m_pObject_Manager->Get_Clone(pPrototypeTag, pArg);
}

HRESULT CGameInstance::Add_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pPrototypeTag, void * pArg)
{
	if (nullptr == m_pObject_Manager)
	{
		MSGBOX("Failed to Add_GameObject");
		return E_FAIL;
	}

	return m_pObject_Manager->Add_GameObject(iLevelIndex, pLayerTag, pPrototypeTag, pArg);
}

CGameObject * CGameInstance::Get_Front(_uint iLevelIndex, const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_Front(iLevelIndex, pLayerTag);
}

CGameObject * CGameInstance::Get_Back(_uint iLevelIndex, const _tchar * playerTag)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_Back(iLevelIndex, playerTag);
}

CGameObject * CGameInstance::Get_ListOfGameObject(_uint iLevelIndex, const _tchar * pLayerTag, int iterNum)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_ListOfGameObject(iLevelIndex, pLayerTag, iterNum);
}

CLayer * CGameInstance::Get_ObjectLayer(_uint iLevelIndex, const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_ObjectLayer(iLevelIndex, pLayerTag);
}

HRESULT CGameInstance::Destory_Front(_uint iLevelIndex, const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Destory_Front(iLevelIndex, pLayerTag);
}

HRESULT CGameInstance::Destory_Last(_uint iLevelIndex, const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Destory_Last(iLevelIndex, pLayerTag);
}

HRESULT CGameInstance::Clear_LevelObject(_uint iLevelIndex)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Clear_LevelObject(iLevelIndex);
}

HRESULT CGameInstance::Release_LayerObject(_uint iLevelIndex, const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Release_LayerObject(iLevelIndex, pLayerTag);
}

HRESULT CGameInstance::Set_Transform(CPipeLine::TRANSFORMSTATETYPE eStateType, _fmatrix TransformMatrix)
{
	if (nullptr == m_pPipeLine)
	{
		MSGBOX("Failed to Set_Transform");
		return E_FAIL;
	}

	return m_pPipeLine->Set_Transform(eStateType, TransformMatrix);
}

_matrix CGameInstance::Get_TransformMatrix(CPipeLine::TRANSFORMSTATETYPE eStateType)
{
	if (nullptr == m_pPipeLine)
	{
		MSGBOX("Failed to Get_TransformMatrix");
		return XMMatrixIdentity();
	}

	return m_pPipeLine->Get_TransformMatrix(eStateType);
}

_float4x4 CGameInstance::Get_TransformFloat4x4(CPipeLine::TRANSFORMSTATETYPE eStateType)
{
	if (nullptr == m_pPipeLine)
	{
		MSGBOX("Failed to Get_TransformFloat4x4");
		return _float4x4();
	}

	return m_pPipeLine->Get_TransformFloat4x4(eStateType);
}

_float4x4 CGameInstance::Get_TransformFloat4x4_TP(CPipeLine::TRANSFORMSTATETYPE eStateType)
{
	if (nullptr == m_pPipeLine)
	{
		MSGBOX("Failed to Get_TransformFloat4x4_TP");
		return _float4x4();
	}

	return m_pPipeLine->Get_TransformFloat4x4_TP(eStateType);
}

_vector CGameInstance::Get_CamPosition()
{
	if (nullptr == m_pPipeLine)
	{
		MSGBOX("Failed to Get_CamPosition");
		return XMVectorZero();
	}

	return m_pPipeLine->Get_CamPosition();
}

const LIGHTDESC * CGameInstance::Get_LightDesc(_uint iIndex) const
{
	if (nullptr == m_pLight_Manager)
	{
		MSGBOX("Failed to Get_LightDesc");
		return nullptr;
	}

	return m_pLight_Manager->Get_LightDesc(iIndex);
}

HRESULT CGameInstance::Add_Light(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const LIGHTDESC & LightDesc)
{
	if (nullptr == m_pLight_Manager)
	{
		MSGBOX("Failed to Add_Light");
		return E_FAIL;
	}

	return m_pLight_Manager->Add_Light(pDevice, pDeviceContext, LightDesc);
}

_bool CGameInstance::isInWorldSpace(_fvector vPoint, _float fRange)
{
	if (nullptr == m_pFrustum)
		return false;

	return m_pFrustum->isIn_WorldSpace(vPoint, fRange);
}

int CGameInstance::VolumeUp(CSoundMgr::CHANNELID eID, _float volume)
{
	if (nullptr == m_pSound_Manager)
		return 0;

	return m_pSound_Manager->VolumeUp(eID, volume);
}

int CGameInstance::VolumeDown(CSoundMgr::CHANNELID eID, _float volume)
{
	if (nullptr == m_pSound_Manager)
		return 0;

	return m_pSound_Manager->VolumeDown(eID, volume);
}

int CGameInstance::BGMVolumeUp(_float volume)
{
	if (nullptr == m_pSound_Manager)
		return 0;

	return m_pSound_Manager->BGMVolumeUp(volume);
}

int CGameInstance::BGMVolumeDown(_float volume)
{
	if (nullptr == m_pSound_Manager)
		return 0;

	return m_pSound_Manager->BGMVolumeDown(volume);
}

int CGameInstance::Pause(CSoundMgr::CHANNELID eID)
{
	if (nullptr == m_pSound_Manager)
		return 0;

	return m_pSound_Manager->Pause(eID);
}

void CGameInstance::Play_Sound(TCHAR * pSoundKey, CSoundMgr::CHANNELID eID, _float volume)
{
	if (nullptr == m_pSound_Manager)
		return;

	return m_pSound_Manager->Play_Sound(pSoundKey, eID, volume);
}

void CGameInstance::Play_Sound_Loop(TCHAR * pSoundKey, CSoundMgr::CHANNELID eID, _float volume)
{
	if (nullptr == m_pSound_Manager)
		return;
	
	return m_pSound_Manager->Play_Sound_Loop(pSoundKey, eID, volume);
}

void CGameInstance::PlayBGM(TCHAR * pSoundKey)
{
	if (nullptr == m_pSound_Manager)
		return;

	return m_pSound_Manager->PlayBGM(pSoundKey);
}

void CGameInstance::StopSound(CSoundMgr::CHANNELID eID)
{
	if (nullptr == m_pSound_Manager)
		return;

	return m_pSound_Manager->StopSound(eID);
}

void CGameInstance::StopAll()
{
	if (nullptr == m_pSound_Manager)
		return;

	return m_pSound_Manager->StopAll();
}

const _uint & CGameInstance::Get_CurrentLevelIndexForPrototype()
{
	return m_iCurLevel;
}

void CGameInstance::Set_CurrentLevelIndexForPrototype(_uint iNextLevel)
{
	m_iCurLevel = iNextLevel;
}

HRESULT CGameInstance::Add_Font(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar * pFontTag, const _tchar * pFontFilePath)
{
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	return m_pFont_Manager->Add_Font(pDevice, pDeviceContext, pFontTag, pFontFilePath);
}

HRESULT CGameInstance::Render_Font(ID3D11DeviceContext * pDeviceContext, const _tchar * pFontTag, const _tchar * pText, _float2 vPosition, _fvector vColor)
{
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	return m_pFont_Manager->Render_Font(pDeviceContext, pFontTag, pText, vPosition, vColor);
}

void CGameInstance::Release_Engine()
{
	if (0 != CGameInstance::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CGameInstance");

	if (0 != CTimer_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CTimer_Manager");

	if (0 != CObject_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CObject_Manager");

	if (0 != CComponent_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CComponent_Manager");

	if (0 != CLevel_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CLevel_Manager");

	if (0 != CPipeLine::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CPipeLine");

	if (0 != CInput_Device::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CInput_Device");

	if (0 != CLight_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CLight_Manager");

	if (0 != CFrustum::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CFrustum");

	if (0 != CSoundMgr::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CSoundMgr");

	if (0 != CFont_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CFont_Manager");

	if (0 != CGraphic_Device::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CGraphic_Device");
}

void CGameInstance::Free()
{
	Safe_Release(m_pFont_Manager);
	Safe_Release(m_pSound_Manager);
	Safe_Release(m_pFrustum);
	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pInput_Device);
}
