#include "Frustum.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CFrustum)

CFrustum::CFrustum()
{
}

HRESULT CFrustum::Initialize()
{
	m_vPoint[0] = _float3(-1.f, 1.f, 0.f);
	m_vPoint[1] = _float3(1.f, 1.f, 0.f);
	m_vPoint[2] = _float3(1.f, -1.f, 0.f);
	m_vPoint[3] = _float3(-1.f, -1.f, 0.f);

	m_vPoint[4] = _float3(-1.f, 1.f, 1.f);
	m_vPoint[5] = _float3(1.f, 1.f, 1.f);
	m_vPoint[6] = _float3(1.f, -1.f, 1.f);
	m_vPoint[7] = _float3(-1.f, -1.f, 1.f);

	return S_OK;
}

void CFrustum::Update()
{
	CPipeLine*		pPipeLine = GET_INSTANCE(CPipeLine);

	/* 2���� ���������̽� ���� ���ǵ� ����ü�� ������ ���� ����� �ű��. */
	/*  * ��������� �� * ������� �� */
	_matrix		ProjMatrixInv, ViewMatrixInv;

	ProjMatrixInv = pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);
	ProjMatrixInv = XMMatrixInverse(nullptr, ProjMatrixInv);

	ViewMatrixInv = pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);
	ViewMatrixInv = XMMatrixInverse(nullptr, ViewMatrixInv);

	/* �佺���̽��� ��ȯ�Ѵ�. */
	for (_uint i = 0; i < 8; ++i)
	{
		_vector			vPoint;

		vPoint = XMVector3TransformCoord(XMLoadFloat3(&m_vPoint[i]), ProjMatrixInv);
		vPoint = XMVector3TransformCoord(vPoint, ViewMatrixInv);

		XMStoreFloat3(&m_vPointInWorld[i], vPoint);
	}

	Compute_Plane(m_vPointInWorld, m_PlaneInWorld);

	RELEASE_INSTANCE(CPipeLine);
}

HRESULT CFrustum::Transform_ToLocalSpace(CTransform * pTransform)
{
	for (_uint i = 0; i < 8; ++i)
	{
		_vector			vPoint;

		vPoint = XMVector3TransformCoord(XMLoadFloat3(&m_vPointInWorld[i]), pTransform->Get_WorldMatrixInverse());

		XMStoreFloat3(&m_vPointInLocal[i], vPoint);
	}

	Compute_Plane(m_vPointInLocal, m_PlaneInLocal);

	return S_OK;
}

_bool CFrustum::isIn_WorldSpace(_fvector vPoint, _float fRange)
{
	/*a b c d
	x y z 1
	ax + by + cz + d*/

	for (_uint i = 0; i < 6; ++i)
	{
		/* ���������� �ظ� ���Ѵ�. */
		if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_PlaneInWorld[i]), vPoint)) > fRange)
			return false;
	}
	return true;
}

_bool CFrustum::isIn_LocalSpace(_fvector vPoint, _float fRange)
{
	for (_uint i = 0; i < 6; ++i)
	{
		/* ���������� �ظ� ���Ѵ�. */
		if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_PlaneInLocal[i]), vPoint)) > fRange)
			return false;
	}
	return true;
}

void CFrustum::Compute_Plane(const _float3 * pPoints, _float4 * pPlanes)
{
	/* +x */
	XMStoreFloat4(&pPlanes[0], XMPlaneFromPoints(XMLoadFloat3(&pPoints[1]), XMLoadFloat3(&pPoints[5]), XMLoadFloat3(&pPoints[6])));
	/* -x */
	XMStoreFloat4(&pPlanes[1], XMPlaneFromPoints(XMLoadFloat3(&pPoints[4]), XMLoadFloat3(&pPoints[0]), XMLoadFloat3(&pPoints[3])));
	/* +y */
	XMStoreFloat4(&pPlanes[2], XMPlaneFromPoints(XMLoadFloat3(&pPoints[4]), XMLoadFloat3(&pPoints[5]), XMLoadFloat3(&pPoints[1])));
	/* -y */
	XMStoreFloat4(&pPlanes[3], XMPlaneFromPoints(XMLoadFloat3(&pPoints[6]), XMLoadFloat3(&pPoints[7]), XMLoadFloat3(&pPoints[3])));
	/* +z */
	XMStoreFloat4(&pPlanes[4], XMPlaneFromPoints(XMLoadFloat3(&pPoints[5]), XMLoadFloat3(&pPoints[4]), XMLoadFloat3(&pPoints[7])));
	/* -z */
	XMStoreFloat4(&pPlanes[5], XMPlaneFromPoints(XMLoadFloat3(&pPoints[0]), XMLoadFloat3(&pPoints[1]), XMLoadFloat3(&pPoints[2])));
}

void CFrustum::Free()
{
}
