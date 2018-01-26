#ifndef __STATE_H__
#define __STATE_H__

#include <vector>
#include <string>

class Action;
class Transition;
class Character;

class State
{
public:
	State() { mEnterAction = nullptr; mExitAction = nullptr; mStateAction = nullptr; mInitialWaitTime = 0.0f; mCurrentWaitTime = 0.0f; }

	void OnEnter();
	void Update(float step);
	void OnExit();

	Action* GetEnterAction() const { return mEnterAction; }
	void SetEnterAction(Action* enterAction) { mEnterAction = enterAction; }

	Action* GetExitAction() const { return mExitAction; }
	void SetExitAction(Action* exitAction) { mExitAction = exitAction; }

	Action* GetStateAction() const { return mStateAction; }
	void SetStateAction(Action* stateAction) { mStateAction = stateAction; }

	std::vector<Transition*> GetTransitions() const { return mTransitions; }
	void SetTransitions(std::vector<Transition*> transitions) { mTransitions = transitions; }

	const char* GetID() const { return mID.c_str(); }
	void SetID(const char* ID) { mID = ID; }

	float GetInitialWaitTime() const { return mInitialWaitTime; }
	float GetCurrentWaitTime() const { return mCurrentWaitTime; }
	void SetWaitTime(float initialWaitTime) { mInitialWaitTime = initialWaitTime; mCurrentWaitTime = initialWaitTime; }
	float UpdateWaitTime(float step);

	void SetOwnerCharacter(Character* ownerCharacter);

protected:
	Action* mEnterAction;
	Action* mExitAction;
	Action* mStateAction;
	std::vector<Transition*> mTransitions;

	std::string mID;
	Character* mOwnerCharacter;

	float mInitialWaitTime;
	float mCurrentWaitTime;
};

#endif