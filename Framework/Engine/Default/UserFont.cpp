#include "UserFont.h"

CUserFont::CUserFont(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
}

HRESULT CUserFont::NativeConstruct(const _tchar * pFontFIlePath)
{
	return E_NOTIMPL;
}

HRESULT CUserFont::Render(const _tchar * pText, _float2 vPosition, _fvector vColor)
{
	return E_NOTIMPL;
}

CUserFont * CUserFont::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar * pFontFilePath)
{
	return nullptr;
}

void CUserFont::Free()
{
}
