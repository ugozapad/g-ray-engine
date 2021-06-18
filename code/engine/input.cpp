#include "stdafx.h"
#include "input.h"

Input Input::ms_Singleton;

Input* Input::Instance()
{
	return &ms_Singleton;
}

void Input::KeyAction(uint32_t keyid, bool state)
{
	if (keyid >= m_kMaxmimumKeys) return;
	m_Keys[keyid] = state;
}

void Input::MousePosAction(float x, float y)
{
	m_MousePos.x = x;
	m_MousePos.y = y;
}

bool Input::GetKey(uint32_t keyid)
{
	if (keyid >= m_kMaxmimumKeys) return false;
	return m_Keys[keyid];
}

void Input::LockMouse()
{

}

void Input::UnlockMouse()
{

}
