#pragma once

#include "Graphic_Device.h"
#include "Input_Device.h"
#include "Timer_Manager.h"
#include "Component_Manager.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "PipeLine.h"
#include "Light_Manager.h"
#include "Frustum.h"
#include "Font_Manager.h"
#include "SoundMgr.h"

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	HRESULT Initialize_Engine(HINSTANCE hInstance, _uint iNumLevels, const CGraphic_Device::GRAPHICDESC& GraphicDesc, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut);
	_int Tick_Engine(_double TimeDelta);
	HRESULT Clear_LevelResource(_uint iLevelIndex);
	HRESULT	Clear_LevelResource_For_ReStart(_uint iLevelIndex);

public: /* For.Graphic_Device */
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT Clear_DepthStencil_View();
	HRESULT Present();

public: /* For.Input_Device */
	_byte Get_DIKeyState(_ubyte eKeyID);
	_long Get_DIMouseMoveState(CInput_Device::MOUSEMOVESTATE eMouseMoveState);
	_byte Get_DIMouseButtonState(CInput_Device::MOUSEBUTTONSTATE eMouseButtonState);
	_bool Key_Pressing(int _Key);
	_bool Key_Down(int _Key);
	_bool Key_Up(int _Key);

public: /* For.Timer_Manager */
	_double	Get_TimeDelta(const _tchar* pTimerTag);
	HRESULT Add_Timer(const _tchar* pTimerTag);

public: /* For.Level_Manager */
	HRESULT OpenLevel(_uint iLevelIndex, class CLevel* pNextLevel);
	HRESULT Render_Level();

	//=========new
	const _uint& Get_CurrentLevelIndex();

public: /* For.Component_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, class CComponent* pPrototype);
	CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg = nullptr);

public: /* For.Object_Manager */
	class CComponent*	Get_Component(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex = 0);
	HRESULT				Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	HRESULT				Add_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, void* pArg = nullptr);
	//==============New============//
	CGameObject*		Get_Front(_uint iLevelIndex, const _tchar* pLayerTag);
	CGameObject*		Get_Back(_uint iLevelIndex, const _tchar* playerTag);
	CGameObject*		Get_ListOfGameObject(_uint iLevelIndex, const _tchar* pLayerTag, int iterNum);
	CLayer*				Get_ObjectLayer(_uint iLevelIndex, const _tchar* pLayerTag);
	HRESULT				Destory_Front(_uint iLevelIndex, const _tchar* pLayerTag);
	HRESULT				Destory_Last(_uint iLevelIndex, const _tchar* pLayerTag);
	HRESULT				Clear_LevelObject(_uint iLevelIndex);
	HRESULT				Release_LayerObject(_uint iLevelIndex, const _tchar* pLayerTag);

	//==The New==========================//
	CGameObject*		Get_Clone(const _tchar * pPrototypeTag, void* pArg = nullptr);

public: /* For.PipeLine */
	HRESULT Set_Transform(CPipeLine::TRANSFORMSTATETYPE eStateType, _fmatrix TransformMatrix);
	_matrix Get_TransformMatrix(CPipeLine::TRANSFORMSTATETYPE eStateType);
	_float4x4 Get_TransformFloat4x4(CPipeLine::TRANSFORMSTATETYPE eStateType);
	_float4x4 Get_TransformFloat4x4_TP(CPipeLine::TRANSFORMSTATETYPE eStateType);
	_vector Get_CamPosition();

public: /* For.Light_Manager */
	const LIGHTDESC* Get_LightDesc(_uint iIndex) const;
	HRESULT Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const LIGHTDESC& LightDesc);

public: /* For.Frustum */
	_bool isInWorldSpace(_fvector vPoint, _float fRange = 0.f);

public:	/* For.Sound_Manager */
	int	 VolumeUp(CSoundMgr::CHANNELID eID, _float volume);
	int	 VolumeDown(CSoundMgr::CHANNELID eID, _float volume);
	int  BGMVolumeUp(_float volume);
	int  BGMVolumeDown(_float volume);
	int  Pause(CSoundMgr::CHANNELID eID);
	void Play_Sound(TCHAR * pSoundKey, CSoundMgr::CHANNELID eID, _float volume);
	void Play_Sound_Loop(TCHAR * pSoundKey, CSoundMgr::CHANNELID eID, _float volume);
	void PlayBGM(TCHAR * pSoundKey);
	void StopSound(CSoundMgr::CHANNELID eID);
	void StopAll();

public:
	const _uint& Get_CurrentLevelIndexForPrototype();
	void	Set_CurrentLevelIndexForPrototype(_uint iNextLevel);

public:	/* For.Font_Manager */
	HRESULT Add_Font(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pFontTag, const _tchar* pFontFilePath);
	HRESULT Render_Font(ID3D11DeviceContext* pDeviceContext, const _tchar* pFontTag, const _tchar* pText, _float2 vPosition, _fvector vColor);

private:
	_uint			m_iCurLevel;

private:
	CGraphic_Device*					m_pGraphic_Device = nullptr;
	CTimer_Manager*						m_pTimer_Manager = nullptr;
	CLevel_Manager*						m_pLevel_Manager = nullptr;
	CObject_Manager*					m_pObject_Manager = nullptr;
	CComponent_Manager*					m_pComponent_Manager = nullptr;
	CInput_Device*						m_pInput_Device = nullptr;
	CPipeLine*							m_pPipeLine = nullptr;
	CLight_Manager*						m_pLight_Manager = nullptr;
	CFrustum*							m_pFrustum = nullptr;
	CFont_Manager*						m_pFont_Manager = nullptr;
	CSoundMgr*							m_pSound_Manager = nullptr;

public:
	static void Release_Engine();
	virtual void Free() override;
};

END