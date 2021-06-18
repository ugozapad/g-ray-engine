#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include "gfxcommon.h"

class IVertexBuffer : public IGPUBuffer
{
public:
	virtual ~IVertexBuffer() {}

	virtual void Bind() = 0;
};

#endif // VERTEX_BUFFER_H
