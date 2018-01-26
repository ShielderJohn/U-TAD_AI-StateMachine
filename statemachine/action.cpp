#include "stdafx.h"
#include "action.h"
#include "character.h"

// Set Image
void ActionSetImage::Start()
{
	if (mOwnerCharacter != nullptr)
	{
		int imageIndex = mOwnerCharacter->GetImageIndex(mImage.c_str());
		mOwnerCharacter->SetImage(imageIndex);
	}
}

// Move to target
void ActionMoveToTarget::Update(float step)
{
	if (mOwnerCharacter != nullptr)
	{
		Character* target = mOwnerCharacter->GetCurrentTarget();

		if (target != nullptr)
		{
			USVec2D direction = target->GetLoc() - mOwnerCharacter->GetLoc();
			float length = direction.Length();

			if (length > 0.0f)
			{
				direction.NormSafe();
				direction *= mSpeed;

				mOwnerCharacter->SetLoc(mOwnerCharacter->GetLoc() + direction * step);
			}
		}
	}
}

// Attack
void ActionAttack::Start()
{
	if (mOwnerCharacter != nullptr)
	{
		Character* target = mOwnerCharacter->GetCurrentTarget();

		if (target != nullptr)
		{
			target->ReceiveDamage(mDamage);

			if (target->IsDead())
			{
				mOwnerCharacter->DeleteCurrentTarget();
			}
		}
	}
}

// Die
void ActionDie::Start()
{
	if (mOwnerCharacter != nullptr)
	{
		mOwnerCharacter->Die();
	}
}