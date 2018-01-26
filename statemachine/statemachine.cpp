#include "stdafx.h"
#include "statemachine.h"
#include <tinyxml.h>
#include "state.h"
#include "action.h"
#include "transition.h"
#include "condition.h"

bool StateMachine::ReadStateMachineFile(const char* filename)
{
	TiXmlDocument doc(filename);
	if (!doc.LoadFile())
	{
		fprintf(stderr, "Couldn't read state machine from %s", filename);
		return false;
	}

	TiXmlHandle hDoc(&doc);

	TiXmlElement* pElem;
	pElem = hDoc.FirstChildElement().Element();
	if (!pElem)
	{
		fprintf(stderr, "Invalid format for %s", filename);
		return false;
	}

	TiXmlHandle hRoot(pElem);
	TiXmlHandle hStates = hRoot.FirstChildElement("states");

	mInitialStateID = hStates.Element()->Attribute("initial-state");

	TiXmlElement* stateElem = hStates.FirstChild().Element();

	for (stateElem; stateElem; stateElem = stateElem->NextSiblingElement())
	{
		State* state = new State();

		state->SetID(stateElem->Attribute("id"));

		float waitTime = 0.0f;
		stateElem->Attribute("wait-time", &waitTime);
		state->SetWaitTime(waitTime);

		TiXmlElement* stateSubElem = stateElem->FirstChildElement();

		for (stateSubElem; stateSubElem; stateSubElem = stateSubElem->NextSiblingElement())
		{
			const char* subElemName = stateSubElem->Value();

			if (!strcmp(subElemName, "enter-action") || 
				!strcmp(subElemName, "exit-action") ||
				!strcmp(subElemName, "state-action"))
			{
				Action* action = nullptr;

				ReadAction(stateSubElem, action);

				if (!strcmp(subElemName, "enter-action"))
				{
					state->SetEnterAction(action);
				}
				else if (!strcmp(subElemName, "exit-action"))
				{
					state->SetExitAction(action);
				}
				else if (!strcmp(subElemName, "state-action"))
				{
					state->SetStateAction(action);
				}
			}
			else if (!strcmp(subElemName, "transitions"))
			{
				ReadTransitions(stateSubElem, state);
			}
		}

		mStates.push_back(state);
	}

	return true;
}

void StateMachine::ReadAction(TiXmlElement* actionElem, Action*& action)
{
	const char* actionElemName = actionElem->Value();
	const char* actionType = actionElem->Attribute("type");

	if (!strcmp(actionType, "set-image"))
	{
		action = new ActionSetImage(actionElem->Attribute("image"));
	}
	else if (!strcmp(actionType, "move-to-target"))
	{
		float speed = 0.0f;
		actionElem->Attribute("speed", &speed);
		action = new ActionMoveToTarget(speed);
	}
	else if (!strcmp(actionType, "attack"))
	{
		float damage = 0.0f;
		actionElem->Attribute("damage", &damage);
		action = new ActionAttack(damage);
	}
	else if (!strcmp(actionType, "die"))
	{
		action = new ActionDie();
	}
}

void StateMachine::ReadTransitions(TiXmlElement* transitionsElem, State*& state)
{
	std::vector<Transition*> transitions;

	TiXmlElement* transitionElem = transitionsElem->FirstChildElement();

	for (transitionElem; transitionElem; transitionElem = transitionElem->NextSiblingElement())
	{
		Transition* transition = new Transition();

		transition->SetTargetStateID(transitionElem->Attribute("target-state"));

		TiXmlElement* transitionSubElem = transitionElem->FirstChildElement();

		if (transitionSubElem != nullptr)
		{
			for (transitionSubElem; transitionSubElem; transitionSubElem = transitionSubElem->NextSiblingElement())
			{
				const char* subElemName = transitionSubElem->Value();

				// Trigger action
				if (!strcmp(subElemName, "trigger-action"))
				{
					Action* action = nullptr;

					ReadAction(transitionSubElem, action);

					transition->SetTriggerAction(action);
				}
				// Conditions
				else if (!strcmp(subElemName, "condition"))
				{
					Condition* condition = nullptr;

					ReadCondition(transitionSubElem, condition);

					transition->SetCondition(condition);
				}
			}
		}

		transitions.push_back(transition);
	}

	state->SetTransitions(transitions);
}

void StateMachine::ReadCondition(TiXmlElement* conditionElem, Condition*& condition)
{
	if (conditionElem != nullptr)
	{
		const char* conditionType = conditionElem->Attribute("type");

		if (!strcmp(conditionType, "inverse"))
		{
			TiXmlElement* subConditionElem = conditionElem->FirstChildElement();
			Condition* subCondition = nullptr;

			ReadCondition(subConditionElem, subCondition);

			condition = new ConditionInverse(subCondition);
		}
		else if (!strcmp(conditionType, "and"))
		{
			TiXmlElement* subConditionElem = conditionElem->FirstChildElement();
			std::vector<Condition*> subConditions;

			for (subConditionElem; subConditionElem; subConditionElem = subConditionElem->NextSiblingElement())
			{
				Condition* subCondition = nullptr;

				ReadCondition(subConditionElem, subCondition);

				subConditions.push_back(subCondition);
			}

			condition = new ConditionAnd(subConditions);
		}
		else if (!strcmp(conditionType, "check-for-target"))
		{
			condition = new ConditionCheckForTarget();
		}
		else if (!strcmp(conditionType, "check-distance"))
		{
			float threshold = 0.0f;
			conditionElem->Attribute("threshold", &threshold);
			condition = new ConditionCheckDistance(threshold);
		}
		else if (!strcmp(conditionType, "receive-damage"))
		{
			condition = new ConditionReceiveDamage();
		}
		else if (!strcmp(conditionType, "check-health"))
		{
			float value = 0.0f;
			conditionElem->Attribute("value", &value);
			condition = new ConditionCheckHealth(value);
		}
	}
}

void StateMachine::InitOwnerCharacterReference(Character* ownerCharacter)
{
	int numberOfStates = mStates.size();

	for (int i = 0; i < numberOfStates; ++i)
	{
		if (mStates[i] != nullptr)
		{
			mStates[i]->SetOwnerCharacter(ownerCharacter);
		}
	}
}

void StateMachine::OnStart()
{
	int numberOfStates = mStates.size();

	for (int i = 0; i < numberOfStates; ++i)
	{
		if (mStates[i] != nullptr)
		{
			// Get initial state
			if (!strcmp(mStates[i]->GetID(), mInitialStateID.c_str()))
			{
				mCurrentState = mStates[i];
			}

			// Set target state for each transition
			std::vector<Transition*> transitions = mStates[i]->GetTransitions();
			int numberOfTransitions = transitions.size();

			for (int j = 0; j < numberOfTransitions; ++j)
			{
				const char* targetStateID = transitions[j]->GetTargetStateID();

				for (int k = 0; k < numberOfStates; ++k)
				{
					if (!strcmp(mStates[k]->GetID(), targetStateID))
					{
						transitions[j]->SetTargetState(mStates[k]);
					}
				}
			}
		}
	}

	if (mCurrentState != nullptr)
	{
		mCurrentState->OnEnter();
	}

	mHasBeenInitialized = true;
}

void StateMachine::Update(float step)
{
	if (mCurrentState != nullptr)
	{
		mCurrentState->Update(step);

		if (mCurrentState->UpdateWaitTime(step) <= 0.0f)
		{
			std::vector<Transition*> transitions = mCurrentState->GetTransitions();
			int numberOfTransitions = transitions.size();

			for (int i = 0; i < numberOfTransitions; ++i)
			{
				if (transitions[i]->CanTrigger(step) && transitions[i]->GetTargetState() != nullptr)
				{
					mCurrentState->OnExit();
					State* nextState = transitions[i]->Trigger();
					nextState->OnEnter();
					mCurrentState = nextState;

					return;
				}
			}
		}
	}
}