#pragma once

#include "Client_Defines.h"
#include "LandObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
END

BEGIN(Client)

class CFork final : public CLandObject
{
private:
	CFork(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CFork(const CFork& rhs);
	virtual ~CFork() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModelCom = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();

public:
	static CFork* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END