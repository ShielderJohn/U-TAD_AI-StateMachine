#include "stdafx.h"
#include "transition.h"
#include "state.h"
#include "action.h"
#include "condition.h"

bool Transition::CanTrigger(float step) const
{
	if (mCondition != nullptr)
	{
		return mCondition->Check();
	}

	return true;
}

State* Transition::Trigger()
{
	if (mTriggerAction != nullptr)
	{
		mTriggerAction->Start();
	}

	return mTargetState;
}

void Transition::SetOwnerCharacter(Character* ownerCharacter)
{
	if (mCondition != nullptr)
	{
		mCondition->SetOwnerCharacter(ownerCharacter);
	}

	if (mTriggerAction != nullptr)
	{
		mTriggerAction->SetOwnerCharacter(ownerCharacter);
	}
}