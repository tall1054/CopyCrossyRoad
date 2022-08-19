#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{
public:
	CVIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs);
	virtual ~CVIBuffer_Terrain() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pHeightMap);
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	_vector Get_Height(_fvector vTargetPos);
private:
	_uint			m_iNumVerticesX = 0;
	_uint			m_iNumVerticesZ = 0;

public:
	static CVIBuffer_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pHeightMap);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END