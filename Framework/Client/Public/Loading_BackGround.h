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

class CLoading_BackGround final : public CGameObject
{
private:
	CLoading_BackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CLoading_BackGround(const CLoading_BackGround& rhs);
	virtual ~CLoading_BackGround() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

private:
	CShader*			m_pShaderCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

private:
	_float4x4			m_ProjMatrix;
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;

private:
	HRESULT SetUp_Components();

public:
	static CLoading_BackGround* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END