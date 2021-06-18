#ifndef LIGHT_H
#define LIGHT_H

#include "engine/entity.h"

class Light : public Entity
{
public:
	enum LightType
	{
		LT_SPOT,
		LT_AMBIENT,
		LT_PROJ
	};

private:
	typedef Entity inherited;

public:
	Light();
	~Light();

	void Update(float dt);

	// !!! STUB FUNCTION !!!
	void Render() override;
	// !!! STUB FUNCTION !!!

	void Load(Serializer* serializer);
	void Save(Serializer* serializer);
};

#endif