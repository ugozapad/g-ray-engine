#ifndef GAME_H
#define GAME_H

class IGame
{
public:
	virtual void Init() = 0;
	virtual void Shutdown() = 0;

	virtual void Update(float dt) = 0;
	virtual void Render() = 0;
};

IGame* GetGame();

#endif // !GAME_H
