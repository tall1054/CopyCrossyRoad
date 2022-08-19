#include "..\Public\Collider.h"
#include "DebugDraw.h"
#include "PipeLine.h"

CCollider::CCollider(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{

}

CCollider::CCollider(const CCollider & rhs)
	: CComponent(rhs)
	, m_pBasicEffect(rhs.m_pBasicEffect)
	, m_pBatch(rhs.m_pBatch)
	, m_eType(rhs.m_eType)
	, m_pInputLayout(rhs.m_pInputLayout)
	, m_isCollision(rhs.m_isCollision)
{
	for (_uint i = 0; i < BOUNDING_END; ++i)
	{
		if (nullptr != rhs.m_pAABB[BOUNDING_ORIGINAL])
			m_pAABB[i] = new BoundingBox(*rhs.m_pAABB[BOUNDING_ORIGINAL]);

		if (nullptr != rhs.m_pOBB[BOUNDING_ORIGINAL])
			m_pOBB[i] = new BoundingOrientedBox(*rhs.m_pOBB[BOUNDING_ORIGINAL]);

		if (nullptr != rhs.m_pSphere[BOUNDING_ORIGINAL])
			m_pSphere[i] = new BoundingSphere(*rhs.m_pSphere[BOUNDING_ORIGINAL]);
	}

	Safe_AddRef(m_pInputLayout);

}

HRESULT CCollider::NativeConstruct_Prototype(TYPE eType)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	m_eType = eType;

	/* �߽����� ����. ��� ������� 1��. */

	switch (m_eType)
	{
	case TYPE_AABB:
		m_pAABB[BOUNDING_ORIGINAL] = new BoundingBox(_float3(0.f, 0.f, 0.f), _float3(0.5f, 0.5f, 0.5f));
		break;
	case TYPE_OBB:
		m_pOBB[BOUNDING_ORIGINAL] = new BoundingOrientedBox(_float3(0.f, 0.f, 0.f), _float3(0.5f, 0.5f, 0.5f), _float4(0.f, 0.f, 0.f, 1.f));
		break;
	case TYPE_SPHERE:
		m_pSphere[BOUNDING_ORIGINAL] = new BoundingSphere(_float3(0.f, 0.f, 0.f), 0.5f);
		break;
	}

#ifdef _DEBUG
	m_pBasicEffect = new BasicEffect(m_pDevice);
	m_pBasicEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCode = nullptr;
	size_t		iShaderByteCodeLength = 0;

	m_pBasicEffect->GetVertexShaderBytecode(&pShaderByteCode, &iShaderByteCodeLength);

	if (FAILED(m_pDevice->CreateInputLayout(DirectX::VertexPositionColor::InputElements, DirectX::VertexPositionColor::InputElementCount,
		pShaderByteCode, iShaderByteCodeLength, &m_pInputLayout)))
		return E_FAIL;

	m_pBatch = new PrimitiveBatch<DirectX::VertexPositionColor>(m_pDeviceContext);

#endif // _DEBUG



	return S_OK;
}
HRESULT CCollider::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	_matrix		TransformMatrix;

	if (nullptr != pArg)
	{
		memcpy(&m_ColliderDesc, pArg, sizeof(COLLIDERDESC));
		TransformMatrix = XMMatrixAffineTransformation(XMLoadFloat3(&m_ColliderDesc.vScale), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMLoadFloat4(&m_ColliderDesc.vRotation), XMLoadFloat4(&m_ColliderDesc.vTranslation));
	}
	else
		TransformMatrix = XMMatrixIdentity();



	if (nullptr != m_pAABB[BOUNDING_ORIGINAL])
		m_pAABB[BOUNDING_ORIGINAL]->Transform(*m_pAABB[BOUNDING_ORIGINAL], TransformMatrix);
	if (nullptr != m_pOBB[BOUNDING_ORIGINAL])
		m_pOBB[BOUNDING_ORIGINAL]->Transform(*m_pOBB[BOUNDING_ORIGINAL], TransformMatrix);
	if (nullptr != m_pSphere[BOUNDING_ORIGINAL])
		m_pSphere[BOUNDING_ORIGINAL]->Transform(*m_pSphere[BOUNDING_ORIGINAL], TransformMatrix);

	return S_OK;
}
void CCollider::Update_Transform(_fmatrix TransformMatrix)
{
	if (nullptr != m_pAABB[BOUNDING_ORIGINAL])
		m_pAABB[BOUNDING_ORIGINAL]->Transform(*m_pAABB[BOUNDING_TRANSFORM], Remove_Rotation(TransformMatrix));

	if (nullptr != m_pOBB[BOUNDING_ORIGINAL])
		m_pOBB[BOUNDING_ORIGINAL]->Transform(*m_pOBB[BOUNDING_TRANSFORM], TransformMatrix);

	if (nullptr != m_pSphere[BOUNDING_ORIGINAL])
		m_pSphere[BOUNDING_ORIGINAL]->Transform(*m_pSphere[BOUNDING_TRANSFORM], TransformMatrix);

}

_bool CCollider::Collision_AABB(CCollider * pTargetCollider)
{
	if (nullptr == m_pAABB[BOUNDING_TRANSFORM] ||
		nullptr == pTargetCollider->m_pAABB[BOUNDING_TRANSFORM])
		return false;

	_float3		vSourMin, vSourMax;
	_float3		vDestMin, vDestMax;

	vSourMin = _float3(m_pAABB[BOUNDING_TRANSFORM]->Center.x - m_pAABB[BOUNDING_TRANSFORM]->Extents.x,
		m_pAABB[BOUNDING_TRANSFORM]->Center.y - m_pAABB[BOUNDING_TRANSFORM]->Extents.y,
		m_pAABB[BOUNDING_TRANSFORM]->Center.z - m_pAABB[BOUNDING_TRANSFORM]->Extents.z);

	vSourMax = _float3(m_pAABB[BOUNDING_TRANSFORM]->Center.x + m_pAABB[BOUNDING_TRANSFORM]->Extents.x,
		m_pAABB[BOUNDING_TRANSFORM]->Center.y + m_pAABB[BOUNDING_TRANSFORM]->Extents.y,
		m_pAABB[BOUNDING_TRANSFORM]->Center.z + m_pAABB[BOUNDING_TRANSFORM]->Extents.z);

	vDestMin = _float3(pTargetCollider->m_pAABB[BOUNDING_TRANSFORM]->Center.x - pTargetCollider->m_pAABB[BOUNDING_TRANSFORM]->Extents.x,
		pTargetCollider->m_pAABB[BOUNDING_TRANSFORM]->Center.y - pTargetCollider->m_pAABB[BOUNDING_TRANSFORM]->Extents.y,
		pTargetCollider->m_pAABB[BOUNDING_TRANSFORM]->Center.z - pTargetCollider->m_pAABB[BOUNDING_TRANSFORM]->Extents.z);

	vDestMax = _float3(pTargetCollider->m_pAABB[BOUNDING_TRANSFORM]->Center.x + pTargetCollider->m_pAABB[BOUNDING_TRANSFORM]->Extents.x,
		pTargetCollider->m_pAABB[BOUNDING_TRANSFORM]->Center.y + pTargetCollider->m_pAABB[BOUNDING_TRANSFORM]->Extents.y,
		pTargetCollider->m_pAABB[BOUNDING_TRANSFORM]->Center.z + pTargetCollider->m_pAABB[BOUNDING_TRANSFORM]->Extents.z);

	m_isCollision = true;

	if (max(vSourMin.x, vDestMin.x) > min(vSourMax.x, vDestMax.x))
	{
		m_isCollision = false;
	}

	if (max(vSourMin.y, vDestMin.y) > min(vSourMax.y, vDestMax.y))
	{
		m_isCollision = false;
	}

	if (max(vSourMin.z, vDestMin.z) > min(vSourMax.z, vDestMax.z))
	{
		m_isCollision = false;
	}

	return m_isCollision;


}

_bool CCollider::Collision_OBB(CCollider * pTargetCollider)
{
	if (nullptr == m_pOBB[BOUNDING_TRANSFORM])
		return false;

	OBBDESC			OBBDesc[2];

	OBBDesc[0] = Compute_OBBDesc();
	OBBDesc[1] = pTargetCollider->Compute_OBBDesc();

	m_isCollision = true;

	_float		fDistance[3];

	for (_uint i = 0; i < 2; ++i)
	{
		for (_uint j = 0; j < 3; ++j)
		{
			fDistance[0] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterAxis[0]), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterAxis[1]), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterAxis[2]), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j]))));

			fDistance[1] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterAxis[0]), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterAxis[1]), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterAxis[2]), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j]))));

			fDistance[2] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&pTargetCollider->m_pOBB[BOUNDING_TRANSFORM]->Center) -
				XMLoadFloat3(&m_pOBB[BOUNDING_TRANSFORM]->Center), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j]))));

			if (fDistance[0] + fDistance[1] < fDistance[2])
			{
				m_isCollision = false;
				break;
			}
		}
		if (false == m_isCollision)
			break;
	}

	//m_isCollision = false;

	//if (nullptr != pTargetCollider->m_pAABB[BOUNDING_TRANSFORM])
	//{
	//	if (true == m_pOBB[BOUNDING_TRANSFORM]->Intersects(*pTargetCollider->m_pAABB[BOUNDING_TRANSFORM]))
	//		m_isCollision = true;
	//}
	//else
	//{
	//	if (true == m_pOBB[BOUNDING_TRANSFORM]->Intersects(*pTargetCollider->m_pOBB[BOUNDING_TRANSFORM]))
	//		m_isCollision = true;

	//}

	return m_isCollision;
}

CCollider::OBBDESC CCollider::Compute_OBBDesc()
{
	OBBDESC		OBBDesc;
	ZeroMemory(&OBBDesc, sizeof(OBBDESC));

	_float3		vPoint[8];
	m_pOBB[BOUNDING_TRANSFORM]->GetCorners(vPoint);

	XMStoreFloat3(&OBBDesc.vAlignAxis[0], XMVector3Normalize(XMLoadFloat3(&vPoint[2]) - XMLoadFloat3(&vPoint[3])));
	XMStoreFloat3(&OBBDesc.vAlignAxis[1], XMVector3Normalize(XMLoadFloat3(&vPoint[2]) - XMLoadFloat3(&vPoint[1])));
	XMStoreFloat3(&OBBDesc.vAlignAxis[2], XMVector3Normalize(XMLoadFloat3(&vPoint[2]) - XMLoadFloat3(&vPoint[6])));

	XMStoreFloat3(&OBBDesc.vCenterAxis[0], (XMLoadFloat3(&vPoint[2]) + XMLoadFloat3(&vPoint[5])) * 0.5f - XMLoadFloat3(&m_pOBB[BOUNDING_TRANSFORM]->Center));
	XMStoreFloat3(&OBBDesc.vCenterAxis[1], (XMLoadFloat3(&vPoint[2]) + XMLoadFloat3(&vPoint[7])) * 0.5f - XMLoadFloat3(&m_pOBB[BOUNDING_TRANSFORM]->Center));
	XMStoreFloat3(&OBBDesc.vCenterAxis[2], (XMLoadFloat3(&vPoint[2]) + XMLoadFloat3(&vPoint[0])) * 0.5f - XMLoadFloat3(&m_pOBB[BOUNDING_TRANSFORM]->Center));

	return OBBDesc;
}

#ifdef _DEBUG

HRESULT CCollider::Render()
{
	m_pDeviceContext->IASetInputLayout(m_pInputLayout);

	m_pBasicEffect->SetWorld(XMMatrixIdentity());

	CPipeLine*		pPipeLine = GET_INSTANCE(CPipeLine);

	m_pBasicEffect->SetView(XMLoadFloat4x4(&pPipeLine->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW)));
	m_pBasicEffect->SetProjection(XMLoadFloat4x4(&pPipeLine->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ)));

	RELEASE_INSTANCE(CPipeLine);

	m_pBasicEffect->Apply(m_pDeviceContext);


	_vector			vColliderColor = m_isCollision == true ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f);

	m_pBatch->Begin();

	if (nullptr != m_pAABB[BOUNDING_TRANSFORM])
		DX::Draw(m_pBatch, *m_pAABB[BOUNDING_TRANSFORM], vColliderColor);
	if (nullptr != m_pOBB[BOUNDING_TRANSFORM])
		DX::Draw(m_pBatch, *m_pOBB[BOUNDING_TRANSFORM], vColliderColor);
	if (nullptr != m_pSphere[BOUNDING_TRANSFORM])
		DX::Draw(m_pBatch, *m_pSphere[BOUNDING_TRANSFORM], vColliderColor);

	m_pBatch->End();

	return S_OK;
}
#endif // _DEBUG

_matrix CCollider::Remove_Rotation(_fmatrix TransformMatrix)
{
	_matrix		Matrix = TransformMatrix;

	Matrix.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(TransformMatrix.r[0]));
	Matrix.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(TransformMatrix.r[1]));
	Matrix.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVectorGetX(XMVector3Length(TransformMatrix.r[2]));

	return Matrix;
}

CCollider * CCollider::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, TYPE eType)
{
	CCollider*	pInstance = new CCollider(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(eType)))
	{
		MSGBOX("Failed to Creating CCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCollider::Clone(void * pArg)
{
	CCollider*	pInstance = new CCollider(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider::Free()
{
	__super::Free();

	for (_uint i = 0; i < BOUNDING_END; ++i)
	{
		Safe_Delete(m_pAABB[i]);
		Safe_Delete(m_pOBB[i]);
		Safe_Delete(m_pSphere[i]);
	}

	Safe_Release(m_pInputLayout);

#ifdef _DEBUG
	if (false == m_isCloned)
	{
		Safe_Delete(m_pBasicEffect);
		Safe_Delete(m_pBatch);
	}
#endif // _DEBUG
}
