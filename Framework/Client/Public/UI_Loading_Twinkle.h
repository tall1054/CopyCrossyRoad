#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI_Loading_Twinkle final : public CGameObject
{
public:
	//enum TYPE { TYPE_DEFAULT, TYPE_END };
	//enum STATE { STATE_NORMAL, STATE_TRANS, STATE_END };
private:
	CUI_Loading_Twinkle(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CUI_Loading_Twinkle(const CUI_Loading_Twinkle& rhs);
	virtual ~CUI_Loading_Twinkle() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int Late_Tick(_double TimeDelta);
	virtual HRESULT Render();

private:
	HRESULT SetUp_Components();

public:
	//void	Set_FrameCheck(_bool bFrameCheck) { m_bFrameCheck = bFrameCheck; }
	//STATE	Get_State() { return m_eState; }
	//_float	Get_Frame() { return m_fFrame; }
	//_bool	Get_FrameCheck() { return m_bFrameCheck; }

private:
	CTexture*		m_pTextureCom = nullptr;
	CVIBuffer_Rect*	m_pVIBufferCom = nullptr;
	CShader*		m_pShaderCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;

private:
	_float4x4		m_ProjMatrix;
	_float			m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float			m_fFrame = 0.f;
	//_float		m_fCurFrame = 0.f;
	//_int			m_iCount = 0;
	//TYPE			m_eType = TYPE_END;
	//STATE			m_eState = STATE_END;
	//_bool			m_bFrameCheck = false;
	//_bool			m_bClone = false;

private:
	//map<const TYPE, CTexture*>*			m_pTextureComps = nullptr;
	//typedef map<const TYPE, CTexture*>	TEXTURECOMPS;

public:
	static CUI_Loading_Twinkle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
