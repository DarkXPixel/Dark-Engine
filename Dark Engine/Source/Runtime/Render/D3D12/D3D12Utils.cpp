#include "D3D12Utils.h"
#include "D3D12Render.h"
#include "D3D12PSO.h"
#include "D3D12Texture.h"
#include <Engine/public/DEngine.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <DTK12/DDSTextureLoader.h>
#include <DTK12/ResourceUploadBatch.h>


std::unordered_map<UINT, TUniquePtr<D3D12PipelineShaderRootSignature>> D3DUtil::Pipelines;
std::unordered_map<FString, TUniquePtr<D3D12Mesh>> D3DUtil::m_meshes;
std::unordered_map<FString, TUniquePtr<D3D12Texture>> D3DUtil::m_textures;
std::vector<CD3DX12_STATIC_SAMPLER_DESC> D3DUtil::m_samplers(6);

void D3DUtil::Init()
{
	LoadTexture(D3D_DEFAULT_TEXTURE);

}

void D3DUtil::Shutdown()
{
	Pipelines.clear();
	m_meshes.clear();
}

UINT D3DUtil::CreatePipeline(eShaderType type)
{
	auto render = static_cast<D3D12Renderer*>(GEngine.GetRenderer());

	switch (type)
	{
	case Default:
	{
		std::vector<CD3DX12_ROOT_PARAMETER1> parametrs(4);
		std::vector<D3D12_ROOT_PARAMETER1> pParametrs(parametrs.size());
		CD3DX12_DESCRIPTOR_RANGE1 srvRange;
		srvRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 2, 0);

		CD3DX12_DESCRIPTOR_RANGE1 descriptorRanges[] = {srvRange };

		parametrs[0].InitAsConstantBufferView(0, 0);
		parametrs[1].InitAsDescriptorTable(_countof(descriptorRanges), descriptorRanges);
		parametrs[2].InitAsConstantBufferView(2, 0);
		parametrs[3].InitAsConstantBufferView(3, 0);


		for (size_t i = 0; i < parametrs.size(); i++) { pParametrs[i] = parametrs[i]; }

		D3D12PipelineShaderRootSignature* PSO = new D3D12PipelineShaderRootSignature(render->m_device.Get(),
			"shaders/VertexShader.hlsl", "shaders/PixelShader.hlsl", pParametrs);

		Pipelines.emplace(PSO->GetID(), PSO);
	}
	break;
	case Light:
		break;
	default:
		break;
	}

	return 0;
}

void D3DUtil::InitStaticSamples()
{
	const CD3DX12_STATIC_SAMPLER_DESC pointWrap(
		0,
		D3D12_FILTER_MIN_MAG_MIP_POINT);

	const CD3DX12_STATIC_SAMPLER_DESC pointClamp(
		1,
		D3D12_FILTER_MIN_MAG_MIP_POINT,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP, 
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP);

	const CD3DX12_STATIC_SAMPLER_DESC linearWrap(
		2,
		D3D12_FILTER_MIN_MAG_MIP_LINEAR);			 

	const CD3DX12_STATIC_SAMPLER_DESC linearClamp(
		3,	
		D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP);
	const CD3DX12_STATIC_SAMPLER_DESC anisotropicWrap(
		4,
		D3D12_FILTER_ANISOTROPIC);

	const CD3DX12_STATIC_SAMPLER_DESC anisotropicClamp(
		5,
		D3D12_FILTER_ANISOTROPIC,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP);


	m_samplers = { pointWrap, pointClamp, linearWrap, linearClamp, anisotropicWrap, anisotropicClamp };


}



ID3D12Device8* D3DUtil::GetDevice()
{
	return static_cast<D3D12Renderer*>(DEngine::GetEngine()->GetRenderer())->m_device.Get();
}
ID3D12CommandQueue* D3DUtil::GetCommandQueue()
{
	return static_cast<D3D12Renderer*>(DEngine::GetEngine()->GetRenderer())->m_commandQueue.Get();
}

//std::vector<D3D12Mesh*> D3DUtil::LoadMeshes(FString path, bool bCombineMeshes)
//{
//	return std::vector<D3D12Mesh*>();
//}


D3D12Texture* D3DUtil::LoadTexture(FString path)
{
	if (m_textures.find(path) != m_textures.end())
	{
		return m_textures.find(path)->second.get();
	}


 

	auto texture = new D3D12Texture();
	
	m_textures.emplace(path, TUniquePtr<D3D12Texture>(texture));
	

	HRESULT hr = S_OK;
	auto m_device = GetDevice();
	ResourceUploadBatch upload(m_device);
	upload.Begin();


	CreateDDSTextureFromFile(m_device, upload,
		strw(path).c_str(), &texture->m_textureBuffer);


	upload.End(GetCommandQueue()).wait();

	

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;

	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = texture->m_textureBuffer->GetDesc().Format;

	switch (texture->m_textureBuffer->GetDesc().Dimension)
	{	
	case D3D12_RESOURCE_DIMENSION_TEXTURE2D:
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = texture->m_textureBuffer->GetDesc().MipLevels;
		srvDesc.Texture2D.ResourceMinLODClamp = 0.f;
		srvDesc.Texture2D.PlaneSlice = 0;
		break;
	default:
		break;
	}


	texture->m_srvDesc = srvDesc;
	return texture;
}


D3D12Mesh* D3DUtil::LoadMesh(FString path)
{
	if (m_meshes.find(path) != m_meshes.end())
	{
		return m_meshes.find(path)->second.get();
	}

	

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);
	if (!scene) return nullptr;

	INT64 CountVertices = 0;

	for (size_t i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* Mesh = scene->mMeshes[i];
		CountVertices += Mesh->mNumVertices;
	}


	std::vector<Vertex> vertices(CountVertices);
	std::vector<WORD> indices;

	int64_t Counter = 0;
	int64_t LastCountVertices = 0;

	for (size_t i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* Mesh = scene->mMeshes[i];
		
	
		
		for (size_t j = 0; j < Mesh->mNumVertices; j++)
		{
			Vertex vert;
			{
				XMFLOAT3 position;
				position.x = Mesh->mVertices[j].x;
				position.y = Mesh->mVertices[j].y;
				position.z = Mesh->mVertices[j].z;



				vert.position = position;
			}
			if (Mesh->HasNormals())
			{
				XMFLOAT3 normal;

				normal.x = Mesh->mNormals[j].x;
				normal.y = Mesh->mNormals[j].y;
				normal.z = Mesh->mNormals[j].z;
				vert.normal = normal;
			}

			{
				if (Mesh->HasTextureCoords(0))
				{
					XMFLOAT2 textureCoordinate;

					textureCoordinate.x = Mesh->mTextureCoords[0][j].x;
					textureCoordinate.y = Mesh->mTextureCoords[0][j].y;
					vert.textureCoordinate = textureCoordinate;
				}
				else
				{
					vert.textureCoordinate = XMFLOAT2(0.f, 0.f);
				}
			}



			if (Mesh->HasTangentsAndBitangents())
			{
				XMFLOAT3 tangent, bitangent;

				tangent.x = Mesh->mTangents[j].x;
				tangent.y = Mesh->mTangents[j].y;
				tangent.z = Mesh->mTangents[j].z;

				bitangent.x = Mesh->mBitangents[j].x;
				bitangent.y = Mesh->mBitangents[j].y;
				bitangent.z = Mesh->mBitangents[j].z;

				vert.tangent = tangent;
				vert.bitangent = bitangent;
			}

			vert.color = XMFLOAT4(0.f, 0.f, 0.f, 1.f);
			vertices[Counter] = vert;
			Counter++;
		}


		for (size_t j = 0; j < Mesh->mNumFaces; j++)
		{
			aiFace face = Mesh->mFaces[j];

			for (size_t k = 0; k < face.mNumIndices; k++)
			{
				indices.push_back(face.mIndices[k] + LastCountVertices);
			}
		}

		LastCountVertices += Mesh->mNumVertices;

	}


	for (size_t i = 0; i < vertices.size(); i += 3)
	{
		if (i + 2 >= vertices.size())
		{
			break;
		}
		XMFLOAT3 v0 = vertices[i + 0].position;
		XMFLOAT3 v1 = vertices[i + 1].position;
		XMFLOAT3 v2 = vertices[i + 2].position;



		XMFLOAT2 uv0 = vertices[i + 0].textureCoordinate;
		XMFLOAT2 uv1 = vertices[i + 1].textureCoordinate;
		XMFLOAT2 uv2 = vertices[i + 2].textureCoordinate;

		XMFLOAT3 deltaPos1 = v1 - v0;
		XMFLOAT3 deltaPos2 = v2 - v0;

		XMFLOAT2 deltaUV1 = uv1 - uv0;
		XMFLOAT2 deltaUV2 = uv2 - uv0;

		float r = 1.f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		
		XMFLOAT3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
		XMFLOAT3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

		vertices[i + 0].tangent = tangent;
		vertices[i + 1].tangent = tangent;
		vertices[i + 2].tangent = tangent;

		vertices[i + 0].bitangent = bitangent;
		vertices[i + 1].bitangent = bitangent;
		vertices[i + 2].bitangent = bitangent;
		

	}



	m_meshes.emplace(path, std::make_unique<D3D12Mesh>(vertices, indices));
	return m_meshes.find(path)->second.get();

}

void D3DUtil::DeleteMesh(D3D12Mesh* mesh)
{
	auto it = m_meshes.find(mesh->PathToMesh);

	if (it == m_meshes.end())
		return;
	it->second.reset();


}

