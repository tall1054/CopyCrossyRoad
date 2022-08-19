#pragma once

#include "UI.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CVIBuffer_Rect;
class CTransform;
class CTexture;
END

BEGIN(Client)
class CUI_Loading : public CUI
{
private:
	explicit CUI_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pDEviceContext);
	explicit CUI_Loading(const CUI_Loading& rhs);
	virtual ~CUI_Loading() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int Late_Tick(_double TimeDelta);
	virtual HRESULT Render();

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;

private:
	_float4x4			m_ProjMatrix;
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float m_fFrame = 0.f;

public:
	static CUI_Loading* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDEviceContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END