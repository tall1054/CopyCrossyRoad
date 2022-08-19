#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CModel;
class CShader;
class CTexture;
class CRenderer;
END

BEGIN(Client)

class CInteraction_Object abstract : public CGameObject
{
protected:
	explicit CInteraction_Object(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CInteraction_Object(const CInteraction_Object& pArg);
	virtual ~CInteraction_Object() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() PURE;
	virtual HRESULT NativeConstruct(void* pArg) PURE;
	virtual _int	Tick(_double TimeDelta) PURE;
	virtual _int	LateTick(_double timeDelta) PURE;
	virtual HRESULT	Render() PURE;

protected:
	_float			Distance_Player();

public:
	virtual CGameObject*	Clone(void* pArg)PURE;
	virtual void Free() override;
};

END