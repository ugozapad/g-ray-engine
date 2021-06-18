#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include <typeinfo>

class Entity;

template <typename T>
T* CreateObjectPfn()
{
	return new T();
}

class ObjectFactory
{
	typedef void* (*PfnCreateObject)();
public:
	ObjectFactory();

	void RegisterObject(const char* name, PfnCreateObject pfnCreateObject);

	template <typename T>
	void RegisterObject()
	{
		RegisterObject(typeid(T).name(), (PfnCreateObject)CreateObjectPfn<T>);
	}

	template <typename T>
	void RegisterObject(const char* name)
	{
		RegisterObject(name, (PfnCreateObject)CreateObjectPfn<T>);
	}

	Entity* CreateObject(const char* classname);

private:
	std::vector<std::pair<const char*, PfnCreateObject>> m_Factory;
};

ObjectFactory* GetObjectFactory();

#endif // !OBJECTFACTORY_H
