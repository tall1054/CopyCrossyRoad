#pragma once

#include "Base.h"

BEGIN(Engine)

class CFont_Manager final : public CBase
{
	DECLARE_SINGLETON(CFont_Manager)
public:
	CFont_Manager();
	virtual ~CFont_Manager() = default;

public:
	HRESULT	Add_Font(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pFontTag, const _tchar* pFontFilePath);
	HRESULT Render_Font(ID3D11DeviceContext* pDeviceContext, const _tchar* pFontTag, const _tchar* pText, _float2 vPosition, _fvector vColor);

private:
	map<const _tchar*, class CUserFont*>			m_Fonts;
	typedef map<const _tchar*, class CUserFont*>	FONTS;

private:
	class CUserFont*	Find_UserFont(const _tchar* pFontTag);

public:
	virtual void Free() override;
};

END