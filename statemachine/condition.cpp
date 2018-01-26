#include "stdafx.h"
#include "condition.h"
#include "character.h"

// Inverse condition
bool ConditionInverse::Check()
{
	if (mSubCondition != nullptr)
	{
		return !mSubCondition->Check();
	}

	return false;
}

void ConditionInverse::SetOwnerCharacter(Character* ownerCharacter)
{
	mOwnerCharacter = ownerCharacter;

	if (mSubCondition != nullptr)
	{
		mSubCondition->SetOwnerCharacter(ownerCharacter);
	}
}

// And condition
bool ConditionAnd::Check()
{
	bool returnValue = false;

	int numberOfSubconditions = mSubConditions.size();

	for (int i = 0; i < numberOfSubconditions; ++i)
	{
		if (mSubConditions[i] != nullptr)
		{
			if (i == 0)
			{
				returnValue = mSubConditions[i]->Check();
			}

			returnValue = returnValue && mSubConditions[i]->Check();
		}
	}

	return returnValue;
}

void ConditionAnd::SetOwnerCharacter(Character* ownerCharacter)
{
	mOwnerCharacter = ownerCharacter;

	int numberOfSubconditions = mSubConditions.size();

	for (int i = 0; i < numberOfSubconditions; ++i)
	{
		if (mSubConditions[i] != nullptr)
		{
			mSubConditions[i]->SetOwnerCharacter(ownerCharacter);
		}
	}
}

// Check for target
bool ConditionCheckForTarget::Check()
{
	if (mOwnerCharacter != nullptr && mOwnerCharacter->GetCurrentTarget() != nullptr)
	{
		return true;
	}

	return false;
}

// Check distance
bool ConditionCheckDistance::Check()
{
	if (mOwnerCharacter != nullptr)
	{
		Character* target = mOwnerCharacter->GetCurrentTarget();

		if (target != nullptr)
		{
			USVec2D characterLoc2D = USVec2D(mOwnerCharacter->GetLoc().mX, mOwnerCharacter->GetLoc().mY);
			USVec2D enemyLoc2D = USVec2D(target->GetLoc().mX, target->GetLoc().mY);

			float distance = characterLoc2D.Dist(enemyLoc2D);

			if (distance <= mThreshold)
			{
				return true;
			}
		}
	}

	return false;
}

// Receive damage
bool ConditionReceiveDamage::Check()
{
	if (mOwnerCharacter != nullptr)
	{
		return mOwnerCharacter->HasReceivedDamage();
	}

	return false;
}

// Check health
bool ConditionCheckHealth::Check()
{
	if (mOwnerCharacter != nullptr)
	{
		return mOwnerCharacter->GetHealth() <= mValue;
	}

	return false;
}