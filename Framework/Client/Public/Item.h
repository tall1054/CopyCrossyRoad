#pragma once

#include "Client_Defines.h"
#include "LandObject.h"
#include "StateComponent.h"

BEGIN(Engine)
class CTexture;
class CCollider;
class CShader;
class CRenderer;
class CModel;
END

BEGIN(Client)
class CStateComponent;
class CPsy;

class CItem final : public CLandObject
{
public:
	typedef struct tagItemDesc
	{
		_float4		vItemPos;
		_float		fItemValue;
	}ITEMDESC;

private:
	CItem(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CItem(const CItem& rhs);
	virtual ~CItem() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

public:
	ITEM				Get_ItemType()			{ return m_eItem; }
	const _float&		Get_ItemValue() const	{ return m_tItemDesc.fItemValue; }
	HRESULT				SetUpItem(_float fItemValue);

private:
	CShader*			m_pShaderCom = nullptr;//
	CRenderer*			m_pRendererCom = nullptr;//
	CModel*				m_pModelCom = nullptr;//
	CTexture*			m_pTextureCom = nullptr;
	CCollider*			m_pAABBCom = nullptr;//
	CStateComponent*	m_pStateCom = nullptr;

	ITEM				m_eItem = ITEM_END;
	ITEMDESC			m_tItemDesc;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();

public:
	static CItem* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END