#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include "gfxcommon.h"

class IIndexBuffer : public IGPUBuffer
{
public:
	virtual ~IIndexBuffer() {}

	virtual void Bind() = 0;
};

#endif // INDEX_BUFFER_H
