#ifndef INPUT_H
#define INPUT_H

class Input
{
private:
	static Input ms_Singleton;
public:
	static Input* Instance();
public:
	void KeyAction(uint32_t keyid, bool state);
	void MousePosAction(float x, float y);

	bool GetKey(uint32_t keyid);
	glm::vec2& GetMousePos() { return m_MousePos; }

	void LockMouse();
	void UnlockMouse();

	bool GetMouseLock() { return m_isMouseLocked; }
private:
	static const int m_kMaxmimumKeys = 460;
	bool m_Keys[m_kMaxmimumKeys];

	glm::vec2 m_MousePos;

	bool m_isMouseLocked = false;
};

#endif // !INPUT_H
