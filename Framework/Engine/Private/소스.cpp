#pragma once

#include "Component.h"
#include "Shader.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	//============����===================//
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

public:
	//===========��ȯ�� ���� ������� ���� ����ü=============//
	typedef struct tagTransformDesc
	{
		//==========�ӵ�, ȸ��, ũ��=============//
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
	//============�̵�==============//
	HRESULT Go_Straight(_double TimeDelta);
	HRESULT Go_Right(_double TimeDelta);
	HRESULT Go_Left(_double TimeDelta);
	HRESULT Go_Backward(_double TimeDelta);

	//=============ȸ��==============//
	HRESULT Turn(_fvector vAxis, _double TimeDelta);
	HRESULT Rotation(_fvector vAxis, _float fRadian);

	//=============ũ��==============//
	HRESULT Scaled(_fvector vScale);
	HRESULT Scaling(_double TimeDelta);

	//=============���� �� �ٶ󺸱�==================//
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

//================Ŭ���� ����====================//

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
	//==============������ ���, ���� ����===========================//
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
	//����Ʈ�� ��� ���� �������ִ� �����̴�.
	//XMVECTOR�� �������ִ� ������ ���ǵǾ� ������ 
	//float4(x, y, z, w)�� �� ������Ŀ� 4Ÿ�Ժ��ͷ� ��ȯ�����ش�. 
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_vector		vUp = Get_State(CTransform::STATE_UP);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	//��������� ���ϴ� ��
	//���� �������� ���� ����� ���ϰ� ������� ���� �ð��� ���ڷ� �޾� ����Ѵ�.
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
	//�ε� �Ҽ����� �̿��� ���͸� right, up, look�� �ش��ϴ� ������ ���̸� x, y, z�� ����
	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(Get_State(CTransform::STATE_RIGHT)));
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(Get_State(CTransform::STATE_UP)));
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVectorGetX(XMVector3Length(Get_State(CTransform::STATE_LOOK)));

	//��� ȸ�������� ����, ȸ���� �����Ѵ�.
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

