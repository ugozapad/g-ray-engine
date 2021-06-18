#ifndef MODEL_H
#define MODEL_H

#include "render/vertexbuffer.h"
#include "render/indexbuffer.h"
#include "render/material.h"
#include "render/aabb.h"

struct Vertex
{
	glm::vec3 m_position;
	glm::vec3 m_normal;
	glm::vec2 m_texCoord;
};

class ModelBase;

class SubMesh
{
	friend class ModelBase;
public:
	SubMesh();
	SubMesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indecies, const glm::mat4& position, const char* materialname);
	void Load(std::vector<Vertex>& vertices, std::vector<uint32_t>& indecies, const glm::mat4& position, const char* materialname);

	~SubMesh();

	void Destroy();

	void Render(const RenderContext& ctx);

	IVertexBuffer* GetVertexBuffer() { return m_vertexBuffer; }
	IIndexBuffer* GetIndexBuffer() { return m_indexBuffer; }

	uint32_t GetVerticesCount() { return m_verticesCount; }
	uint32_t GetIndeciesCount() { return m_indeciesCount; }

private:
	std::shared_ptr<Material> m_material;

	IVertexBuffer* m_vertexBuffer;
	IIndexBuffer* m_indexBuffer;

	glm::mat4 m_transform;

	uint32_t m_verticesCount;
	uint32_t m_indeciesCount;
};

class ModelBase
{
public:
	virtual void Load(const std::string& filename);
	virtual void Destroy();

	virtual void RenderObjects(const RenderContext& ctx);

	const std::string& GetFileName() { return m_filename; }

	AABB* GetBoundedBox() { return &m_aabb; }

private:
	std::vector<SubMesh*> m_subMeshes;
	std::string m_filename;

	AABB m_aabb;
};

#endif // !MODEL_H
