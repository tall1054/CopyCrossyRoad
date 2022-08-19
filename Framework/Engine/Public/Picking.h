#pragma once

#include "Base.h"

BEGIN(Engine)

class CPicking final : public CBase
{
	DECLARE_SINGLETON(CPicking)
private:
	CPicking();
	virtual ~CPicking() = default;

public:
	HRESULT NativeConstruct(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, HWND hWnd);

public:
	HRESULT Transform_ToWorldSpace();
	HRESULT Transform_ToLocalSpace(const _matrix& WorldMatrixInverse);
	_bool	isPicking(_float3* pLocalPoints, _float3* pOut);

private:
	ID3D11Device*				m_pDevice = nullptr;
	ID3D11DeviceContext*		m_pDeviceContext = nullptr;
	HWND						m_hWnd;

	_vector						m_vRayDir;
	_float3						m_vRayPos;

	_vector						m_vRayDirInLocal;
	_vector						m_vRayPosInLocal;

public:
	virtual void Free() override;
};

END