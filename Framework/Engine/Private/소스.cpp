#pragma once

#include "Component.h"
#include "Shader.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	//============상태===================//
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

public:
	//===========변환을 위한 상수들을 모은 구조체=============//
	typedef struct tagTransformDesc
	{
		//==========속도, 회전, 크기=============//
		_float		fSpeedPerSec = 0.f;
		_float		fRotationPerSec = 0.f;
		_float		fScalingPerSec = 0.f;

		tagTransformDesc() = default;
		tagTransformDesc(_float _fSpeedPerSec, _float _fRotationPerSec = 0.f, _float _fScalingPerSec = 0.f)
			: fSpeedPerSec(_fSpeedPerSec), fRotationPerSec(_fRotationPerSec), fScalingPerSec(_fScalingPerSec)
		{
		}
	}TRANSFORMDESC;

private:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:
	_vector Get_Scale(STATE eState) const;
	_vector Get_State(STATE eState) const;

	_matrix Get_WorldMatrix() const {
		return XMLoadFloat4x4(&m_WorldMatrix);
	}

	_float4x4 Get_WorldFloat4x4() const {
		return m_WorldMatrix;
	}

public:
	void Set_State(STATE eState, _fvector vState);
	void Set_TransformDesc(const TRANSFORMDESC& TransformDesc);

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	HRESULT Bind_OnShader(class CShader* pShader, const char* pValueName);

public:
	//============이동==============//
	HRESULT Go_Straight(_double TimeDelta);
	HRESULT Go_Right(_double TimeDelta);
	HRESULT Go_Left(_double TimeDelta);
	HRESULT Go_Backward(_double TimeDelta);

	//=============회전==============//
	HRESULT Turn(_fvector vAxis, _double TimeDelta);
	HRESULT Rotation(_fvector vAxis, _float fRadian);

	//=============크기==============//
	HRESULT Scaled(_fvector vScale);
	HRESULT Scaling(_double TimeDelta);

	//=============추적 및 바라보기==================//
	HRESULT Chase(_fvector vTargetPos, _double TimeDelta);
	HRESULT LookAt(_fvector vTargetPos, _double TimeDelta);

private:
	TRANSFORMDESC				m_TransformDesc;
	_float4x4					m_WorldMatrix;

public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
}
END

//================클래스 내용====================//

CTransform::CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{
}

CTransform::CTransform(const CTransform& rhs)
	: CComponent(rhs)
	, m_WorldMatrix(rhs.m_WorldMatrix)
{
}

_vector CTransform::Get_Scale(STATE eState) const
{
	return XMVector3Length(XMLoadFloat3((_float3*)&m_WorldMatrix.m[eState][0]));
}

_vector CTransform::Get_State(STATE eState) const
{
	return XMLoadFloat4((_float4*)&m_WorldMatrix.m[eState][0]);
}

void CTransform::Set_State(STATE eState, _fvector vState)
{
	XMStoreFloat4((_float4*)&m_WorldMatrix.m[eState][0], vState);
}

void CTransform::Set_TransformDesc(const TRANSFORMDESC& TransformDesc)
{
	m_TransformDesc = TransformDesc;
}

HRESULT CTransform::NativeConstruct_Prototype()
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	return S_OK;
}

HRESULT CTransform::NativeConstruct(void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_TransformDesc, pArg, sizeof(TRANSFORMDESC));
	}

	return S_OK;
}

HRESULT CTransform::Bind_OnShader(CShader* pShader, const char* pValueName)
{
	_float4x4		WorldMatrix;

	XMStoreFloat4x4(&WorldMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMatrix)));

	return pShader->Set_RawValue(pValueName, &WorldMatrix, sizeof(_float4x4));
}

HRESULT CTransform::Go_Straight(_double TimeDelta)
{
	//==============포지션 얻고, 룩을 얻어낸다===========================//
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	//==============
	vPosition += XMVector3Normalize(vLook) * m_TransformDesc.fSpeedPerSec * (_float)TimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_Right(_double TimeDelta)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition += XMVector3Normalize(vRight) * m_TransformDesc.fSpeedPerSec * (_float)TimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_Backward(_double TimeDelta)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	vPosition -= XMVector3Normalize(vLook) * m_TransformDesc.fSpeedPerSec * (_float)TimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_Left(_double TimeDelta)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition -= XMVector3Normalize(vRight) * m_TransformDesc.fSpeedPerSec * (_float)TimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Turn(_fvector vAxis, _double TimeDelta)
{
	//라이트와 룩과 업을 정의해주는 구간이다.
	//XMVECTOR를 러턴해주는 값으로 정의되어 있으며 
	//float4(x, y, z, w)를 얻어낸 월드행렬에 4타입벡터로 변환시켜준다. 
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_vector		vUp = Get_State(CTransform::STATE_UP);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	//공전행렬을 구하는 식
	//축을 기준으로 공전 행렬을 구하고 기준축과 공전 시간을 인자로 받아 사용한다.
	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, m_TransformDesc.fRotationPerSec * (_float)TimeDelta);

	vRight = XMVector4Transform(vRight, RotationMatrix);
	vUp = XMVector4Transform(vUp, RotationMatrix);
	vLook = XMVector4Transform(vLook, RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	return S_OK;
}

HRESULT CTransform::Rotation(_fvector vAxis, _float fRadian)
{
	//부동 소수점을 이용함 벡터를 right, up, look에 해당하는 벡터의 길이를 x, y, z에 저장
	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(Get_State(CTransform::STATE_RIGHT)));
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(Get_State(CTransform::STATE_UP)));
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVectorGetX(XMVector3Length(Get_State(CTransform::STATE_LOOK)));

	//축과 회전각도를 통해, 회전을 구현한다.
	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

	vRight = XMVector4Transform(vRight, RotationMatrix);
	vUp = XMVector4Transform(vUp, RotationMatrix);
	vLook = XMVector4Transform(vLook, RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	return S_OK;
}

HRESULT CTransform::Scaled(_fvector vScale)
{
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_vector		vUp = Get_State(CTransform::STATE_UP);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	vRight = XMVector3Normalize(vRight) * XMVectorGetX(vScale);
	vUp = XMVector3Normalize(vUp) * XMVectorGetY(vScale);
	vLook = XMVector3Normalize(vLook) * XMVectorGetZ(vScale);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	return S_OK;
}

HRESULT CTransform::Scaling(_double TimeDelta)
{

	return S_OK;
}

HRESULT CTransform::Chase(_fvector vTargetPos, _double TimeDelta)
{
	return S_OK;
}

HRESULT CTransform::LookAt(_fvector vTargetPos, _double TimeDelta)
{
	_vector		vLook = vTargetPos - Get_State(CTransform::STATE_LOOK);
	vLook = XMVector3Normalize(vLook) * XMVectorGetZ(XMVector3Length(Get_State(CTransform::STATE_LOOK)));

	_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	/*Get_State(CTransform::STATE_RIGHT)*/
	vRight = XMVector3Normalize(vRight) * XMVectorGetX(XMVector3Length(Get_State(CTransform::STATE_RIGHT)));
	
	_vector		vUp = XMVector3Cross(vLook, vRight);
	vUp = XMVector3Normalize(vUp) * XMVectorGetY(XMVector3Length(Get_State(CTransform::STATE::STATE_UP)));

	Set_State(CTransform::STATE_LOOK, vLook);
	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);

	return S_OK;
}

CTransform* CTransform::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CTransform*		pInstance = new CTransform(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CTransform::Clone(void* pArg)
{
	CTransform* pInstance = new CTransform(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{
	__super::Free();
}

