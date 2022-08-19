#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Test final : public CLevel
{
public:
	explicit CLevel_Test(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CLevel_Test() = default;

public:
	virtual HRESULT NativeConstruct(LEVEL eNextLevel);
	virtual _int	Tick(_double TimeDelta);
	virtual _int	LateTick(_double TimeDelta);
	virtual HRESULT	Render();

private:
	HRESULT	Ready_Layer_Camera(const _tchar* pLayerTag, LEVEL eNextLevel);
	HRESULT	Ready_Layer_BackGround(const _tchar* pLayerTag, LEVEL eNextLevel);
	HRESULT	Ready_Layer_Player(const _tchar* pLayerTag, LEVEL eNextLevel);
	HRESULT	Ready_Layer_Monster(const _tchar* pLayerTag, LEVEL eNextLevel);
	HRESULT Ready_Layer_UI(const _tchar* pLayerTag, LEVEL eNextLevel);

private:
	HRESULT Ready_Light();
	LEVEL	m_NextLevel = LEVEL_END;

public:
	static CLevel_Test* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, LEVEL eNextLevel);
	virtual void Free();
};

END