#pragma once

#include "Component.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;

class ENGINE_DLL CCollider final : public CComponent
{
public:
	enum TYPE { TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPE_END };
	enum BOUNDING { BOUNDING_ORIGINAL, BOUNDING_TRANSFORM, BOUNDING_END };

public:
	typedef struct tagOBBDesc
	{
		_float3		vCenterAxis[3];
		_float3		vAlignAxis[3];
	}OBBDESC;

public:
	typedef struct tagColliderDesc
	{
		_float3 vScale; /* 배수 */
		_float4 vRotation;
		_float4 vTranslation;
	} COLLIDERDESC;
private:
	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(TYPE eType);
	virtual HRESULT NativeConstruct(void* pArg);

	void Update_Transform(_fmatrix Transform);


public:
	_bool Collision_AABB(CCollider* pTargetCollider);
	_bool Collision_OBB(CCollider* pTargetCollider);


	OBBDESC Compute_OBBDesc();

#ifdef _DEBUG
public:
	virtual HRESULT Render();
#endif // _DEBUG

private:
	BoundingBox*				m_pAABB[BOUNDING_END] = { nullptr };
	BoundingOrientedBox*		m_pOBB[BOUNDING_END] = { nullptr };
	BoundingSphere*				m_pSphere[BOUNDING_END] = { nullptr };
	TYPE						m_eType = TYPE_END;
	COLLIDERDESC				m_ColliderDesc;

	_bool						m_isCollision = false;

#ifdef _DEBUG
private:
	/* 정점들을 그릴수 있는 셰이더를 제공한다. */
	BasicEffect*									m_pBasicEffect = nullptr;

	ID3D11InputLayout*								m_pInputLayout = nullptr;

	/* 기본적인 도형을 그려나갈수 있는 정점 버퍼, 인덱스 버퍼를 제공하낟. */
	PrimitiveBatch<DirectX::VertexPositionColor>*	m_pBatch = nullptr;
#endif // _DEBUG

private:
	_matrix Remove_Rotation(_fmatrix TransformMatrix);




public:
	static CCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, TYPE eType);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END