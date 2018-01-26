#ifndef __ACTION_H__
#define __ACTION_H__

#include <string>

class Character;

class Action
{
public:
	virtual void Start() = 0;
	virtual void Update(float step) = 0;
	virtual void End() = 0;

	void SetOwnerCharacter(Character* ownerCharacter) { mOwnerCharacter = ownerCharacter; }

protected:
	Character* mOwnerCharacter;
};

class ActionSetImage : public Action
{
public:
	ActionSetImage(const char* image) { mImage = image; }

	virtual void Start();
	virtual void Update(float step) { }
	virtual void End() { }

private:
	std::string mImage;
};

class ActionMoveToTarget : public Action
{
public:
	ActionMoveToTarget(float speed) { mSpeed = speed; }

	virtual void Start() { }
	virtual void Update(float step);
	virtual void End() { }

private:
	float mSpeed;
};

class ActionAttack : public Action
{
public:
	ActionAttack(float damage) { mDamage = damage; }

	virtual void Start();
	virtual void Update(float step) { }
	virtual void End() { }

private:
	float mDamage;
};

class ActionDie : public Action
{
public:
	ActionDie() { }

	virtual void Start();
	virtual void Update(float step) { }
	virtual void End() { }
};

#endif