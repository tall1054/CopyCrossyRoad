#pragma once

#include "Client_Defines.h"
#include "Base.h"

/* �����带 �����Ѵ�. */
/* �������� �������Լ��� �����Ѵ�. (�� �������� �ε��ؾ��� �ڿ����� �����Ѵ�.) */

BEGIN(Client)

class CLoader final : public CBase
{
private:
	explicit CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CLoader() = default;

public:
	CRITICAL_SECTION Get_CS() {
		return m_CS;
	}

	LEVEL Get_LevelID() const {
		return m_eLevel;
	}

	_bool isFinished() const {
		return m_isFinished;
	}
public:
	HRESULT NativeConstruct(LEVEL eNextLevel);
	HRESULT Loading_ForGamePlay();
	HRESULT	Loading_ForKoreaLevel();
	HRESULT	Loading_ForJapanLevel();
	HRESULT Loading_ForBoss();

private:
	void	Create_BasicOffer(LEVEL	CurLevel);
	void	Create_TerrainPrototype(LEVEL CurLevel);
	void	Create_PlayerPrototype(LEVEL CurLevel);
	void	Create_MonsterPrototype(LEVEL CurLevel);
	void	Create_StructurePrototype(LEVEL CurLevel);
	void	Create_Shader(LEVEL CurLevel);

private:
	ID3D11Device*							m_pDevice = nullptr;
	ID3D11DeviceContext*					m_pDeviceContext = nullptr;
	LEVEL									m_eLevel = LEVEL_END;
	HANDLE									m_hThread;
	_bool									m_isFinished = false;
	CRITICAL_SECTION						m_CS;

public:
	static CLoader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, LEVEL eNextLevel);
	virtual void Free() override;
};

END