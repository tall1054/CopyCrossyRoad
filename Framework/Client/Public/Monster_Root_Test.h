#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)
class CMonster_Root_Test abstract : public CGameObject
{
protected:
	explicit CMonster_Root_Test(ID3D11Device* pDevice, ID3D11DeviceContext* pdeviceContext);
	explicit CMonster_Root_Test(const CMonster_Root_Test& rhs);
	virtual ~CMonster_Root_Test() = default;

public:
	virtual HRESULT		NativeConstruct_Prototype() override;
	virtual HRESULT		NativeConstruct(void* pArg) override;
	virtual _int		Tick(_double TimeDelta)override;
	virtual _int		LateTick(_double TimeDelta)override;
};

