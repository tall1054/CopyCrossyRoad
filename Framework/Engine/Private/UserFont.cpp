#include "..\Public\UserFont.h"
#include "PipeLine.h"

CUserFont::CUserFont(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

HRESULT CUserFont::NativeConstruct(const _tchar * pFontFilePath)
{
	m_pBatch = new SpriteBatch(m_pDeviceContext);
	if (nullptr == m_pBatch)
		return E_FAIL;

	m_pFont = new SpriteFont(m_pDevice, pFontFilePath);
	if (nullptr == m_pFont)
		return E_FAIL;

	return S_OK;
}

HRESULT CUserFont::Render(const _tchar* pText, _float2 vPosition, _fvector vColor)
{
	CPipeLine*		pPipeLine = GET_INSTANCE(CPipeLine);

	_matrix			ViewMatrix = pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);
	_matrix			ProjMatrix = pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);

	m_pBatch->Begin();

	m_pFont->DrawString(m_pBatch, pText, XMVectorSet(vPosition.x, vPosition.y, 0.f, 1.f), vColor, 0.f, XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(1.f, 1.f, 1.f, 0.f));

	m_pBatch->End();

	RELEASE_INSTANCE(CPipeLine);

	return S_OK;
}

CUserFont * CUserFont::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar * pFontFilePath)
{
	CUserFont*	pInstance = new CUserFont(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pFontFilePath)))
	{
		MSGBOX("Failed to Created CFont");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUserFont::Free()
{

	Safe_Delete(m_pBatch);
	Safe_Delete(m_pFont);

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
