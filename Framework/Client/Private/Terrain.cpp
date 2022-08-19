#include "stdafx.h"
#include "..\Public\Terrain.h"
#include "GameInstance.h"


CTerrain::CTerrain(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{

}

CTerrain::CTerrain(const CTerrain & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTerrain::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to CTerrain NativeConstruct_Prototype");
		return E_FAIL;
	}
	
	return S_OK;
}

HRESULT CTerrain::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct()))
	{
		MSGBOX("Failed to CTerrain NativeConstruct");
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
	{
		MSGBOX("Failed to CTerrain SetUp_Components");
		return E_FAIL;
	}

	if (FAILED(Create_FilterTexture()))
	{
		MSGBOX("Failed to CTerrain Create_FilterTexture");
		return E_FAIL;
	}

	return S_OK;
}

_int CTerrain::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	return _int();
}

_int CTerrain::LateTick(_double TimeDelta)
{
	if (0 > __super::LateTick(TimeDelta))
		return -1;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

HRESULT CTerrain::Render()
{
	if (nullptr == m_pVIBufferCom)
	{
		MSGBOX("Failed to CTerrain VIBuffer");
		return E_FAIL;
	}

	if (FAILED(SetUp_ConstantTable()))
	{
		MSGBOX("Failed to CTerrain SetUp_ConstantTable");
		return E_FAIL;
	}

	if (FAILED(m_pVIBufferCom->Render(m_pShaderCom, 0)))
	{
		MSGBOX("Failed to CTerrain VIBuffer Render");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CTerrain::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), 
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX("Failed to CTerrain Prototype_Component_Renderer");
		return E_FAIL;
	}

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"), 
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
	{
		MSGBOX("Failed to CTerrain Prototype_Component_Shader_VtxNorTex");
		return E_FAIL;
	}

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"), 
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX("Failed to CTerrain Prototype_Component_VIBuffer_Terrain");
		return E_FAIL;
	}

	/* For.Com_Texture_Diffuse */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"), 
		TEXT("Com_Texture_Diffuse"), (CComponent**)&m_pTextureCom[TYPE_DIFFUSE])))
	{
		MSGBOX("Failed to CTerrain Prototype_Component_Texture_Terrain");
		return E_FAIL;
	}

	/* For.Com_Texture_Filter */
	//if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Filter"), 
	//	TEXT("Com_Texture_Filter"), (CComponent**)&m_pTextureCom[TYPE_FILTER])))
	//{
	//	MSGBOX("Failed to CTerrain Prototype_Component_Texture_Filter");
	//	return E_FAIL;
	//}

	/* For.Com_Texture_Brush */
	/*if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Brush"), 
		TEXT("Com_Texture_Brush"), (CComponent**)&m_pTextureCom[TYPE_BRUSH])))
	{
		MSGBOX("Failed to CTerrain Prototype_Component_Texture_Brush");
		return E_FAIL;
	}*/

	return S_OK;
}

HRESULT CTerrain::SetUp_ConstantTable()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransform->Bind_OnShader(m_pShaderCom, "g_WorldMatrix")))
	{
		MSGBOX("Failed to CTerrain Transform Bind_OnShader(World)");
		return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
	{
		MSGBOX("Failed to CTerrain Shader Set_RawValue(View)");
		return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
	{
		MSGBOX("Failed to CTerrain Shader Set_RawValue(Proj)");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom[TYPE_DIFFUSE]->SetUp_OnShader(m_pShaderCom, "g_SourDiffuseTexture", 0)))
	{
		MSGBOX("Failed to CTerrain Texture(Diffuse) SetUp_OnShader(SourDiffuse)");
		return E_FAIL;
	}

	if (FAILED(m_pTextureCom[TYPE_DIFFUSE]->SetUp_OnShader(m_pShaderCom, "g_DestDiffuseTexture", 0)))
	{
		MSGBOX("Failed to CTerrain Texture(Diffuse) SetUp_OnShader(DestDiffuse)");
		return E_FAIL;
	}

	//if (FAILED(m_pTextureCom[TYPE_BRUSH]->SetUp_OnShader(m_pShaderCom, "g_BrushTexture", 0)))
	//{
	//	MSGBOX("Failed to CTerrain Texture(Brush) SetUp_OnShader(Brush)");
	//	return E_FAIL;
	//}

	//if (FAILED(m_pTextureCom[TYPE_FILTER]->SetUp_OnShader(m_pShaderCom, "g_FilterTexture", 0)))
	//return E_FAIL;

	////텍스쳐 적용
	//m_pShaderCom->Set_Texture("g_FilterTexture", m_pSRV);

	const LIGHTDESC* pLightDesc = pGameInstance->Get_LightDesc(0);
	if (nullptr == pLightDesc)
	{
		MSGBOX("Failed to CTerrain LIGHTDESC");
		return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightPos", &pLightDesc->vPosition, sizeof(_float4))))
	{
		MSGBOX("Failed to CTerrain Shader Set_RawValue(Light Position)");
		return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
	{
		MSGBOX("Failed to CTerrain Shader Set_RawValue(Light Direction)");
		return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
	{
		MSGBOX("Failed to CTerrain Shader Set_RawValue(Light Diffuse)");
		return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
	{
		MSGBOX("Failed to CTerrain Shader Set_RawValue(Light Ambient)");
		return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
	{
		MSGBOX("Failed to CTerrain Shader Set_RawValue(Light Specular)");
		return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Set_RawValue("g_vCamPosition", &pGameInstance->Get_CamPosition(), sizeof(_float4))))
	{
		MSGBOX("Failed to CTerrain Shader Set_RawValue(CamPosition)");
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CTerrain::Create_FilterTexture()
{
	ID3D11Texture2D*			pTexture;

	D3D11_TEXTURE2D_DESC		TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = 129;
	TextureDesc.Height = 129;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.Usage = D3D11_USAGE_DYNAMIC;
	TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA			SubResourceData;
	ZeroMemory(&SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	_ulong*		pPixel = new _ulong[129 * 129];
	ZeroMemory(pPixel, sizeof(_ulong) * 129 * 129);

	for (_uint i = 0; i < 129; ++i)
	{
		for (_uint j = 0; j < 129; ++j)
		{
			_uint iIndex = i * 129 + j;

			if (j < 65)
				pPixel[iIndex] = D3D11COLOR_ARGB(255, 255, 255, 255);
			else
				pPixel[iIndex] = D3D11COLOR_ARGB(255, 0, 0, 0);
		}
	}

	pPixel[0] = D3D11COLOR_ARGB(255, 255, 255, 255);

	SubResourceData.pSysMem = pPixel;
	SubResourceData.SysMemPitch = sizeof(_ulong) * 129;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, &SubResourceData, &pTexture)))
	{
		MSGBOX("Failed to CTerrain CreateTexture2D");
		return E_FAIL;
	}

	if (FAILED(m_pDevice->CreateShaderResourceView(pTexture, nullptr, &m_pSRV)))
	{
		MSGBOX("Failed to CTerrain CreateShaderResourceView");
		return E_FAIL;
	}

	if (FAILED(SaveWICTextureToFile(m_pDeviceContext, pTexture, GUID_ContainerFormatPng, TEXT("../Bin/Test.png"), &GUID_WICPixelFormat32bppBGRA)))
	{
		MSGBOX("Failed to CTerrain SaveWICTextureToFile");
		return E_FAIL;
	}

	Safe_Release(pTexture);

	Safe_Delete_Array(pPixel);

	return S_OK;
}



CTerrain * CTerrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CTerrain*	pInstance = new CTerrain(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CTerrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CTerrain::Clone(void * pArg)
{
	CTerrain*	pInstance = new CTerrain(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CTerrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTerrain::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);

	for (auto& pTexture : m_pTextureCom)
		Safe_Release(pTexture);

	Safe_Release(m_pSRV);
	Safe_Release(m_pShaderCom);
}
