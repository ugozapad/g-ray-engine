#ifndef TESTENTITY_H
#define TESTENTITY_H

#include "engine/entity.h"

class StaticObject : public Entity
{
	typedef Entity inherited;
public:
	StaticObject();
	~StaticObject();

	void Update(float dt) override;
	void Render() override;

	void LoadVisual(const std::string& name);

	void Load(Serializer* serializer) override;
	void Save(Serializer* serializer) override;

};

#endif // !TESTENTITY_H
