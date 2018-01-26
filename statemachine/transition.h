#ifndef __TRANSITION_H__
#define __TRANSITION_H__

#include <string>

class State;
class Action;
class Condition;
class Character;

class Transition
{
public:
	Transition() { mCondition = nullptr; mTargetState = nullptr; mTriggerAction = nullptr; }

	bool CanTrigger(float step) const;
	State* Trigger();

	Condition* GetCondition() const { return mCondition; }
	void SetCondition(Condition* condition) { mCondition = condition; }

	State* GetTargetState() const { return mTargetState; }
	void SetTargetState(State* targetState) { mTargetState = targetState; }

	Action* GetTriggerAction() const { return mTriggerAction; }
	void SetTriggerAction(Action* triggerAction) { mTriggerAction = triggerAction; }

	const char* GetTargetStateID() const { return mTargetStateID.c_str(); }
	void SetTargetStateID(const char* targetStateID) { mTargetStateID = targetStateID; }

	void SetOwnerCharacter(Character* ownerCharacter);

private:
	Condition* mCondition;
	State* mTargetState;
	Action* mTriggerAction;

	std::string mTargetStateID;
};

#endif