#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
//UI_Money�� �÷��̾ ���޽� �԰ԵǴ� ���������� ������ ǥ���ϴ� ��ġ
//���� 100�� �̻��� �Ǹ� �̱� �����ϰ� ������!!!

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI_Money final : public CGameObject
{
private:
	CUI_Money(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CUI_Money(const CUI_Money& rhs);
	virtual ~CUI_Money() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int	Tick(_double TimeDelta);
	virtual _int	LateTick(_double TimeDelta);
	virtual HRESULT	Render();

private:
	HRESULT	SetUp_Components();

	CTexture*				m_pTextureCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CShader*				m_pShaderCom = nullptr;

	_float4x4				m_ProjMatrix;
	_float					m_fX, m_fY, m_fSizeX, m_fSizeY;

public:
	static CUI_Money*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free() override;

};

END