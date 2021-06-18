#ifndef EDITORSYSTEM_H
#define EDITORSYSTEM_H

#include "editor/editorwindow.h"

class EditorSystem
{
public:
	static EditorSystem* Instance();

public:
	void Render();

	const bool GetGameRunning() { return m_isGameRunning; }

private:
	EditorWindow m_mainWindow;
	PropertyWindow m_propertyWindow;

	bool m_isGameRunning;
};

#endif // !EDITORSYSTEM_H
