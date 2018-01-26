#ifndef __STATEMACHINE_H__
#define __STATEMACHINE_H__

#include <vector>
#include <string>

class TiXmlElement;
class State;
class Character;
class Condition;
class Action;

class StateMachine
{
public:
	StateMachine() { mCurrentState = nullptr;  mHasBeenInitialized = false; }

	bool HasBeenInitialized() const { return mHasBeenInitialized; }

	bool ReadStateMachineFile(const char* filename);
	void InitOwnerCharacterReference(Character* ownerCharacter);

	void OnStart();
	void Update(float step);

private:
	bool mHasBeenInitialized;

	std::vector<State*> mStates;
	State* mCurrentState;
	std::string mInitialStateID;

	void ReadAction(TiXmlElement* actionElem, Action*& action);
	void ReadTransitions(TiXmlElement* transitionsElem, State*& state);
	void ReadCondition(TiXmlElement* conditionElem, Condition*& condition);
};

#endif