#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CNavigation final : public CComponent
{
private:
	CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;

private:
	class CVIBuffer_Triangle*		m_pVIBuffer = nullptr;
	class CShader*					m_pShader = nullptr;
public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pNaviDataFilePath);
	virtual CComponent*	Clone(void* pArg) override;
	virtual void Free() override;
};

END