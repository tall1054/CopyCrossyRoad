#include "stdafx.h"
#include "..\Public\ImgUi_Manager.h"

IMPLEMENT_SINGLETON(CImgUi_Manager)

CImgUi_Manager::CImgUi_Manager()
{
}

void CImgUi_Manager::ImgUi_SetUp(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	m_pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(m_pGameInstance);

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(pDevice, pDeviceContext);
}

void CImgUi_Manager::ImgUi_Tick()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void CImgUi_Manager::ImgUi_Render()
{
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void CImgUi_Manager::Free()
{
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
