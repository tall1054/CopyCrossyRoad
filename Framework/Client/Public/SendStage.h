#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CTransform;
class CCollider;
class CVIBuffer_Cube;
END

BEGIN(Client)

class CSendStage final : public CGameObject
{
public:
	typedef struct tagCSendStage
	{
		_float3		vAxis;
		_float		fAngle;
		_float4		vPosition;
		_float3		vScale;
	}SendStage;

private:
	CSendStage(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CSendStage(const CSendStage& rhs);
	virtual ~CSendStage() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int Late_Tick(_double TimeDelta);
	virtual HRESULT Render() override;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();
	HRESULT Send_StageKorea();

private:
	CShader*				m_pTextureCom = nullptr;
	CVIBuffer_Cube*			m_pVIBufferCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CCollider*				m_pAABBCom = nullptr;
	CCollider*				m_pOBBCom = nullptr;
	CCollider*				m_pSphereCom = nullptr;
	SendStage				m_tSendStage;

public:
	static CSendStage* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END