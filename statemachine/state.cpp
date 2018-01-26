#include "stdafx.h"
#include "state.h"
#include "action.h"
#include "transition.h"

void State::OnEnter()
{
	if (mEnterAction != nullptr)
	{
		mEnterAction->Start();
	}

	if (mStateAction != nullptr)
	{
		mStateAction->Start();
	}
}

void State::Update(float step)
{
	if (mStateAction != nullptr)
	{
		mStateAction->Update(step);
	}
}

void State::OnExit()
{
	if (mStateAction != nullptr)
	{
		mStateAction->End();
	}

	if (mExitAction != nullptr)
	{
		mExitAction->Start();
	}
}

float State::UpdateWaitTime(float step)
{
	mCurrentWaitTime -= step;
	
	if (mCurrentWaitTime <= 0.0f)
	{
		float valueToReturn = mCurrentWaitTime;
		mCurrentWaitTime = mInitialWaitTime;

		return valueToReturn;
	}

	return mCurrentWaitTime;
}

void State::SetOwnerCharacter(Character* ownerCharacter)
{
	mOwnerCharacter = ownerCharacter;

	if (mEnterAction != nullptr)
	{
		mEnterAction->SetOwnerCharacter(ownerCharacter);
	}

	if (mExitAction != nullptr)
	{
		mExitAction->SetOwnerCharacter(ownerCharacter);
	}

	if (mStateAction != nullptr)
	{
		mStateAction->SetOwnerCharacter(ownerCharacter);
	}

	int numberOfTransitions = mTransitions.size();

	for (int i = 0; i < numberOfTransitions; ++i)
	{
		if (mTransitions[i] != nullptr)
		{
			mTransitions[i]->SetOwnerCharacter(ownerCharacter);
		}
	}
}