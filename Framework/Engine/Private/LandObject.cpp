#include "..\Public\LandObject.h"
#include "GameInstance.h"

CLandObject::CLandObject(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{

}

CLandObject::CLandObject(const CLandObject & rhs)
	: CGameObject(rhs)
{

}

HRESULT CLandObject::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CLandObject::NativeConstruct(const CTransform::TRANSFORMDESC * pTransformDesc)
{
	if (FAILED(__super::NativeConstruct(pTransformDesc)))
		return E_FAIL;

	return S_OK;
}

_int CLandObject::Tick(_double TimeDelta)
{
	return _int();
}

_int CLandObject::LateTick(_double TimeDelta)
{
	return _int();
}

HRESULT CLandObject::Render()
{
	return S_OK;
}

void CLandObject::SetUp_OnTerrain(_uint iLevelID, const _tchar* pTerrainLayerTag, _uint iIndex, const _tchar* pBufferComTag)
{
	CObject_Manager*		pObject_Manager = GET_INSTANCE(CObject_Manager);

	CTransform*		pTerrainTransform = (CTransform*)pObject_Manager->Get_Component(iLevelID, pTerrainLayerTag, CGameObject::m_pTransformTag, iIndex);
	if (nullptr == pTerrainTransform)
		return;

	CVIBuffer_Terrain*		pVIBuffer = (CVIBuffer_Terrain*)pObject_Manager->Get_Component(iLevelID, pTerrainLayerTag, pBufferComTag, iIndex);
	if (nullptr == pVIBuffer)
		return;

	_vector		vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);
	_matrix		WorldMatrix = pTerrainTransform->Get_WorldMatrix();
	_matrix		WorldMatrixInverse = XMMatrixInverse(nullptr, WorldMatrix);

	vPosition = pVIBuffer->Get_Height(XMVector3TransformCoord(vPosition, WorldMatrixInverse));
	vPosition = XMVector3TransformCoord(vPosition, WorldMatrix);

	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);

	RELEASE_INSTANCE(CObject_Manager);
}

void CLandObject::SetUp_OnObject(_uint iLevelID, const _tchar * pBackGroundObjectLayerTag, _uint iIndex, const _tchar* pBufferComTag)
{
	//CObject_Manager*		pObject_Manager = GET_INSTANCE(CObject_Manager);

	//CTransform*		pBackGroundObjectTransform = (CTransform*)pObject_Manager->Get_Component(iLevelID, pBackGroundObjectLayerTag, CGameObject::m_pTransformTag, iIndex);
	//if (nullptr == pBackGroundObjectTransform)
	//	return;

	//CModel*		pModel = (CModel*)pObject_Manager->Get_Component(iLevelID, pBackGroundObjectLayerTag, pBufferComTag, iIndex);
	//if (nullptr == pModel)
	//	return;

	//_vector		vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);
	//_matrix		WorldMatrix = pBackGroundObjectTransform->Get_WorldMatrix();
	//_matrix		WorldMatrixInverse = XMMatrixInverse(nullptr, WorldMatrix);

	//vPosition = pModel->Get_TransformMatrix
}

void CLandObject::Free()
{
	__super::Free();
}
