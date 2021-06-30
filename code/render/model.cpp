#include "stdafx.h"
#include "model.h"

#include "filesystem/filesystem.h"

#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "shader.h"
#include "material.h"
#include "device.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

SubMesh* ProccessSubMesh(aiMesh* mesh, aiNode* node, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indecies;

	// for speed-up loading
	vertices.reserve(1024);
	indecies.reserve(1024);

	for (uint32_t i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.m_position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.m_normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

		if (mesh->mTextureCoords[0])
			vertex.m_texCoord = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		else
			vertex.m_texCoord = glm::vec2(0.0f, 0.0f);

		if (mesh->mTangents)
			vertex.m_tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);

		if (mesh->mBitangents)
			vertex.m_bitangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);

		vertices.push_back(vertex);
	}

	for (uint32_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (uint32_t j = 0; j < face.mNumIndices; j++)
			indecies.push_back(face.mIndices[j]);
	}


	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	char buffer[256];
	sprintf(buffer, "%s/materials/%s.material", IFileSystem::Instance()->GetDefaultPath().c_str(), material->GetName().C_Str());

	if (!IFileSystem::Instance()->Exist(buffer))
	{
		aiString diffusePath;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &diffusePath);

		aiString normalPath;
		material->GetTexture(aiTextureType_NORMALS, 0, &normalPath);

		Material::CreateMaterialFromImport(material->GetName().C_Str(), diffusePath.C_Str(), normalPath.C_Str());
	}

	aiMatrix4x4 nodePosition = node->mTransformation;
	//glm::mat4 transform = glm::make_mat4(&nodePosition[0][0]);
	glm::mat4 transform = glm::mat4(1.0f);

	/*glm::mat4 transform = glm::mat4(
		nodePosition.a1, nodePosition.a2, nodePosition.a3, nodePosition.a4,
		nodePosition.b1, nodePosition.b2, nodePosition.b3, nodePosition.b4,
		nodePosition.c1, nodePosition.c2, nodePosition.c3, nodePosition.c4,
		nodePosition.d1, nodePosition.d2, nodePosition.d3, nodePosition.d4);*/

	return New(SubMesh, vertices, indecies, transform, material->GetName().C_Str());
}

void ProccessNode(std::vector<SubMesh*>& submeshes, aiNode* node, const aiScene* scene)
{
	for (uint32_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		SubMesh* submesh = ProccessSubMesh(mesh, node, scene);
		submeshes.push_back(submesh);
	}

	for (uint32_t i = 0; i < node->mNumChildren; i++)
	{
		ProccessNode(submeshes, node->mChildren[i], scene);
	}
}

void ModelBase::Load(const std::string& filename)
{
	m_filename = filename;
	Msg("loading %s", IFileSystem::Instance()->GetFileName(filename).c_str());

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		Error("Failed to load model. %s", importer.GetErrorString());

	ASSERT(scene);
	ProccessNode(m_subMeshes, scene->mRootNode, scene);
}

void ModelBase::Destroy()
{
	for (int i = 0; i < m_subMeshes.size(); i++)
	{
		m_subMeshes[i]->Destroy();
		mem_delete(m_subMeshes[i]);
	}

	m_subMeshes.clear();
}

void ModelBase::RenderObjects(const RenderContext& ctx)
{
	for (int i = 0; i < m_subMeshes.size(); i++)
		m_subMeshes[i]->Render(ctx);
}

SubMesh::SubMesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indecies, const glm::mat4& position, const char* materialname) :
	m_vertexBuffer(0),
	m_indexBuffer(0)
{
	Load(vertices, indecies, position, materialname);
}

SubMesh::SubMesh() :
	m_vertexBuffer(0),
	m_indexBuffer(0)
{

}

void SubMesh::Load(std::vector<Vertex>& vertices, std::vector<uint32_t>& indecies, const glm::mat4& position, const char* materialname)
{
	m_transform = position;
	m_vertexBuffer = g_renderDevice->CreateVertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex), BufferAccess::Static);
	m_indexBuffer = g_renderDevice->CreateIndexBuffer(indecies.data(), indecies.size() * sizeof(uint32_t), BufferAccess::Static);
	m_verticesCount = vertices.size();
	m_indeciesCount = indecies.size();

	m_material = g_resourceManager->LoadMaterial(materialname);
}

SubMesh::~SubMesh()
{
//	Destroy();
}

void SubMesh::Destroy()
{
	if (m_indexBuffer)
		mem_delete(m_indexBuffer);

	if (m_vertexBuffer)
		mem_delete(m_vertexBuffer);
}

#include "render.h"

void SubMesh::Render(const RenderContext& ctx)
{
	glm::mat4 mvp = glm::mat4(1.0f);
	mvp = ctx.proj * ctx.view * ctx.model/* * m_Transform*/;

//	g_render->RenderSubMesh(ctx, this, m_material->GetShader().get(), m_material.get());

	//g_render->RenderMesh(ctx, m_vertexBuffer, m_indexBuffer, m_indeciesCount, m_material->GetShader().get(),
	//	m_material.get());

	g_render->RenderMesh(ctx, m_vertexBuffer, m_verticesCount, m_material->GetShader().get(),
		m_material.get());
}
