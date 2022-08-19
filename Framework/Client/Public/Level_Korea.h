#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Korea final : public CLevel
{
public:
	explicit CLevel_Korea(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CLevel_Korea() = default;
public:
	virtual HRESULT NativeConstruct();
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Lights();

private:
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Monster(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Item(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Effect(const _tchar* pLayerTag);
	HRESULT	Ready_Layer_Structure(const _tchar* pLayerTag);
	HRESULT	Ready_Layer_UI(const _tchar* pLayerTag);

public:
	static CLevel_Korea* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual void Free() override;
};

END