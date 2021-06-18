#ifndef R_STATS_H
#define R_STATS_H

class RenderStats
{
public:
	int m_verticesCount;
	int m_drawCalls;
	float m_framerate;

	void Clean();

	void Draw();
};

extern RenderStats g_renderStats;

#endif // !R_STATS_H
