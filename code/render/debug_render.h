#ifndef DEBUG_RENDER_H
#define DEBUG_RENDER_H

class IVertexBuffer;

class DebugRender
{
public:
	static DebugRender* Instance();

public:
	void Init();
	void Shutdown();

	void DrawSphere(const glm::vec3& position);

private:
	struct DebugMesh
	{
		IVertexBuffer* vb;
		size_t vxcount;
		std::shared_ptr<Material> material;

		void create(void* buffer, size_t size);
		void draw(const glm::vec3& position);
	};

	DebugMesh m_cube;
	DebugMesh m_sphere;
};

#endif // !DEBUG_RENDER_H
