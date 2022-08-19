#include "..\public\Input_Device.h"

IMPLEMENT_SINGLETON(CInput_Device)

CInput_Device::CInput_Device()
{
}

_bool CInput_Device::Key_Pressing(int _Key)
{
	if (GetAsyncKeyState(_Key) & 0x8000)
		return true;

	return false;
}

_bool CInput_Device::Key_Down(int _Key)
{
	if (m_byKeyState[_Key] && !(GetAsyncKeyState(_Key) & 0x8000))
	{
		m_byKeyState[_Key] = !m_byKeyState[_Key];
		return true;
	}

	if (!m_byKeyState[_Key] && (GetAsyncKeyState(_Key) & 0x8000))
		m_byKeyState[_Key] = !m_byKeyState[_Key];

	return false;
}

_bool CInput_Device::Key_Up(int _Key)
{
	if (m_byKeyState[_Key] && !(GetAsyncKeyState(_Key) & 0x8000))
	{
		m_byKeyState[_Key] = !m_byKeyState[_Key];
		return true;
	}

	if (!m_byKeyState[_Key] && (GetAsyncKeyState(_Key) & 0x8000))
		m_byKeyState[_Key] = !m_byKeyState[_Key];

	return false;
}

HRESULT CInput_Device::Ready_Input_Device(HINSTANCE hInst, HWND hWnd)
{
	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, nullptr)))
	{
		MSGBOX("Failed to DirectInput8Create");
		return E_FAIL;
	}
	
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, nullptr)))
	{
		MSGBOX("Failed to CreateDevice_Keyboard");
		return E_FAIL;
	}

	if (FAILED(m_pKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		MSGBOX("Failed to SetDataFormat_Keyboard");
		return E_FAIL;
	}

	if (FAILED(m_pKeyboard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
	{
		MSGBOX("Failed to SetCooperativeLevel_Keyboard");
		return E_FAIL;
	}

	if (FAILED(m_pKeyboard->Acquire()))
	{
		MSGBOX("Failed to Acquire_Keyboard");
		return E_FAIL;
	}

	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
	{
		MSGBOX("Failed to CreateDevice_Mouse");
		return E_FAIL;
	}

	if (FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse)))
	{
		MSGBOX("Failed to SetDataFormat_Mouse");
		return E_FAIL;
	}

	if (FAILED(m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
	{
		MSGBOX("Failed to SetCooperativeLevel_Mouse");
		return E_FAIL;
	}

	if (FAILED(m_pMouse->Acquire()))
	{
		MSGBOX("Failed to Acquire_Mouse");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CInput_Device::SetUp_InputDeviceState()
{
	if (nullptr == m_pKeyboard ||
		nullptr == m_pMouse)
	{
		MSGBOX("Failed to Keyboard, Mouse");
		return E_FAIL;
	}

	m_pKeyboard->GetDeviceState(256, m_byKeyState);

	m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState);

	return S_OK;
}

void CInput_Device::Free()
{
	Safe_Release(m_pMouse);
	Safe_Release(m_pKeyboard);
	Safe_Release(m_pInput);
}
