#include "..\Public\Picking.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CPicking)

CPicking::CPicking()
{
}

HRESULT CPicking::NativeConstruct(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, HWND hWnd)
{
	m_hWnd = hWnd;
	m_pDevice = pDevice;
	m_pDeviceContext = pDeviceContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);

	return S_OK;
}

HRESULT CPicking::Transform_ToWorldSpace()
{
	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	POINT		tMousePos;
	GetCursorPos(&tMousePos);
	ScreenToClient(m_hWnd, &tMousePos);

	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));

	m_pDeviceContext->RSSetViewports(1, &ViewPortDesc);

	_float4		vMousePos;
	ZeroMemory(&vMousePos, sizeof(_float4));

	vMousePos.x = (_float)tMousePos.x / (ViewPortDesc.Width * 0.5f) - 1.f;
	vMousePos.y = (_float)tMousePos.y / (ViewPortDesc.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;
	vMousePos.w = 1.f;

	_matrix		ProjMatrixInverse;
	pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4);
	XMMatrixInverse(nullptr, ProjMatrixInverse);
	XMVector4Transform(XMLoadFloat4(&vMousePos), ProjMatrixInverse);
	//memcpy(&m_vRayDir, &(vMousePos - _float4(0.f, 0.f, 0.f, 1.f)), sizeof(_float3));
	m_vRayPos = _float3(0.f, 0.f, 0.f);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CPicking::Transform_ToLocalSpace(const _matrix & WorldMatrixInverse)
{
	XMVector3TransformNormal(m_vRayDirInLocal, WorldMatrixInverse);
	XMVector3TransformCoord(m_vRayPosInLocal, WorldMatrixInverse);
	XMVector3Normalize(m_vRayDirInLocal);

	return S_OK;
}

_bool CPicking::isPicking(_float3 * pLocalPoints, _float3 * pOut)
{
	/*_float	fU, fV, fDist;*/

	/*if (TRUE == xminterse(&pLocalPoints[0], &pLocalPoints[1], &pLocalPoints[2],
		&m_vRayPosInLocal, &m_vRayDirInLocal, &fU, &fV, &fDist))
	{
		*pOut = m_vRayPosInLocal + (m_vRayDirInLocal * fDist);
		return true;
	}
	else
	{
		return false;
	}*/
	return true;
}

void CPicking::Free()
{
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
