#pragma once

#include "Base.h"
#include "Client_Defines.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "GameInstance.h"

BEGIN(Client)

class CImgUi_Manager final : public CBase
{
	DECLARE_SINGLETON(CImgUi_Manager)

private:
	CImgUi_Manager();
	virtual~CImgUi_Manager() = default;

public:
	void	ImgUi_SetUp(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	void	ImgUi_Tick();
	void	ImgUi_Render();

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;
	CGameInstance*			m_pGameInstance = nullptr;

public:
	virtual void Free() override;
};

END