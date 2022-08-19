#pragma once
#include "UI.h"
#include "GameInstance.h"
#include "Psy.h"
#include "Player_Samurai.h"
#include "Player_Chicken.h"
#include "Player_Koala.h"
#include "Player_Korea_Drummer.h"

//������ ��ġ�ϸ� �̵��ϰ� �׿����� ������ �ö󰡴� ������ ���� UI������
//������ �ε���2��(�簢��)�� �������� ���� 1���� �����ϴ� UI�� �����.

class CUI_Touch : public CUI
{
private:
	explicit CUI_Touch(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI_Touch(const CUI_Touch& rhs);
	virtual ~CUI_Touch() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int	Tick(_double TimeDelta);
	virtual _int	LateTick(_double TimeDelta);
	virtual HRESULT	Render();

private:
	CShader*		m_pShaderCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CVIBuffer_Rect*	m_pVIBufferCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;

private:
	_float4x4		m_ProjMatrix;
	_float			m_fX, m_fY, m_fSizeX, m_fSizeY;
	_uint			m_iCount;

private:
	HRESULT	SetUp_Components();

public:
	static CUI_Touch*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free() override;

};

