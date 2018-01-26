#ifndef __CONDITION_H__
#define __CONDITION_H__

class Character;

class Condition
{
public:
	Condition() { mOwnerCharacter = nullptr; }

	virtual bool Check() = 0;

	virtual void SetOwnerCharacter(Character* ownerCharacter) { mOwnerCharacter = ownerCharacter; }

protected:
	Character* mOwnerCharacter;
};

class ConditionInverse : public Condition
{
public:
	ConditionInverse(Condition* subCondition) : Condition() { mSubCondition = subCondition; }

	virtual void SetOwnerCharacter(Character* ownerCharacter);

	virtual bool Check();

private:
	Condition* mSubCondition;
};

class ConditionAnd : public Condition
{
public:
	ConditionAnd(std::vector<Condition*> subConditions) : Condition() { mSubConditions = subConditions; }

	virtual void SetOwnerCharacter(Character* ownerCharacter);

	virtual bool Check();

private:
	std::vector<Condition*> mSubConditions;
};

class ConditionCheckForTarget : public Condition
{
public:
	ConditionCheckForTarget() : Condition() { }

	virtual bool Check();
};

class ConditionCheckDistance : public Condition
{
public:
	ConditionCheckDistance(float threshold) : Condition() { mThreshold = threshold; }

	virtual bool Check();

private:
	float mThreshold;
};

class ConditionReceiveDamage : public Condition
{
public:
	ConditionReceiveDamage() : Condition() { }

	virtual bool Check();
};

class ConditionCheckHealth : public Condition
{
public:
	ConditionCheckHealth(float value) : Condition() { mValue = value; }

	virtual bool Check();

private:
	float mValue;
};

#endif