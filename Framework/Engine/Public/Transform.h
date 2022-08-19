#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

public:
	typedef struct tagTransformDesc
	{
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

	_matrix	Get_WorldMatrixInverse() const {
		return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix));
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
	HRESULT Go_Straight(_double TimeDelta);
	HRESULT Go_Right(_double TimeDelta);
	HRESULT Go_Left(_double TimeDelta);
	HRESULT Go_Backward(_double TimeDelta);
	HRESULT Go_Up(_double TimeDelta);
	HRESULT Go_Down(_double TimeDelta);

	HRESULT Turn(_fvector vAxis, _double TimeDelta);
	HRESULT Rotation(_fvector vAxis, _float fRadian);

	HRESULT Scaled(_fvector vScale);
	HRESULT Scaling(_double TimeDelta);
	HRESULT Chase(_fvector vTargetPos, _double TimeDelta);
	HRESULT LookAt(_fvector vTargetPos, _double TimeDelta);

private:
	TRANSFORMDESC							m_TransformDesc;
	_float4x4								m_WorldMatrix;

public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END