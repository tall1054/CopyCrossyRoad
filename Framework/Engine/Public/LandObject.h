#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CLandObject abstract : public CGameObject
{
protected:
	CLandObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CLandObject(const CLandObject& rhs);
	virtual ~CLandObject() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(const CTransform::TRANSFORMDESC* pTransformDesc = nullptr);
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

public:
	void SetUp_OnTerrain(_uint iLevelID, const _tchar* pTerrainLayerTag, _uint iIndex, const _tchar* pBufferComTag);
	void SetUp_OnObject(_uint iLevelID, const _tchar* pBackGroundObjectLayer, _uint iIndex, const _tchar* pBufferComTag);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END