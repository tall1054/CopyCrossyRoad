#include "..\Public\Navigation.h"
#include "Cell.h"
#include "Shader.h"
#include "PipeLine.h"
#include "Transform.h"
#include "VIBuffer_Triangle.h"

CNavigation::CNavigation(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{
}

CNavigation::CNavigation(const CNavigation & rhs)
	:CComponent(rhs)
{
}

CNavigation * CNavigation::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar * pNaviDataFilePath)
{
	return nullptr;
}

CComponent * CNavigation::Clone(void * pArg)
{
	return nullptr;
}

void CNavigation::Free()
{
}
