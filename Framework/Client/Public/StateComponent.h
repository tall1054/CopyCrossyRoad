#pragma once

#include "Client_Defines.h"
#include "Component.h"

BEGIN(Client)

class CStateComponent final : public CComponent
{
	typedef struct tagState
	{
		_float		m_fCurrent_Coin;
		_float		m_fMax_Coin;
	}STATE;

public:
	explicit CStateComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CStateComponent(const CStateComponent& pArg);
	virtual ~CStateComponent() = default;

public:
	virtual HRESULT	NativeConstruct_Prototype() override;
	virtual HRESULT	NativeConstruct(void* pArg) override;

public:
	void	Set_MaxCoin(_float fNewMaxCoin);
	void	Set_CurrentCoin(_float fNewCurCoin);
	void	Add_CurrentCoin(_float fAddCurrentCoin);

public:
	_float	Get_MaxCoin();
	_float	Get_CurrentCoin();
	_float	Get_CoinRatio();

public:
	_float	Add_Coin(_float fAddedCoin);

private:
	STATE		m_tState;

public:
	static CStateComponent*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent*	Clone(void* pArg) override;
	virtual void Free() override;
};

END