#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CCamera_Player final : public CCamera
{
	explicit CCamera_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CCamera_Player(const CCamera_Player& rhs);
	virtual ~CCamera_Player() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

private:
	CTransform*		m_pPlayerTransform = nullptr;

public:
	static CCamera_Player* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END