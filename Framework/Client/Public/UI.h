#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Player.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CShader;
END

BEGIN(Client)

class CUI abstract : public CGameObject
{

protected:
	explicit	CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit	CUI(const CUI& rhs);
	virtual		~CUI() = default;

public:
	virtual HRESULT			NativeConstruct_Prototype() override;
	virtual HRESULT			NativeConstruct(void* pArg);
	virtual _int			Tick(_double TimeDelta);
	virtual _int			Late_Tick(_double TimeDelta);
	virtual HRESULT			Render() override;

protected:
	_float4x4				m_ProjMatrix;
	_float					m_fX, m_fY, m_fSizeX, m_fSizeY;

	CTexture*				m_pTextureCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CShader*				m_pShaderCom = nullptr;

public:
	virtual CGameObject*		Clone(void* pArg)PURE;
	virtual void				Free();

};

END
