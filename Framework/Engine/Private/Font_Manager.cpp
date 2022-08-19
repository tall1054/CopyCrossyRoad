#include "..\Public\Font_Manager.h"
#include "UserFont.h"

IMPLEMENT_SINGLETON(CFont_Manager)

CFont_Manager::CFont_Manager()
{
}

HRESULT CFont_Manager::Add_Font(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar * pFontTag, const _tchar * pFontFilePath)
{
	if (nullptr != Find_UserFont(pFontTag))
		return E_FAIL;

	CUserFont*	pUserFont = CUserFont::Create(pDevice, pDeviceContext, pFontFilePath);
	if (nullptr == pUserFont)
		return E_FAIL;

	m_Fonts.emplace(pFontTag, pUserFont);

	return S_OK;
}

HRESULT CFont_Manager::Render_Font(ID3D11DeviceContext * pDeviceContext, const _tchar * pFontTag, const _tchar * pText, _float2 vPosition, _fvector vColor)
{
	CUserFont*		pFont = Find_UserFont(pFontTag);
	if (nullptr == pFont)
		return E_FAIL;

	pDeviceContext->GSSetShader(nullptr, nullptr, 0);

	return pFont->Render(pText, vPosition, vColor);
}

CUserFont * CFont_Manager::Find_UserFont(const _tchar * pFontTag)
{
	auto	iter = find_if(m_Fonts.begin(), m_Fonts.end(), CTagFinder(pFontTag));

	if (iter == m_Fonts.end())
		return nullptr;

	return iter->second;
}

void CFont_Manager::Free()
{
	for (auto& Pair : m_Fonts)
		Safe_Release(Pair.second);

	m_Fonts.clear();
}
