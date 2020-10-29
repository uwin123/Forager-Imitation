#include "stdafx.h"
#include "muBoss.h"

HRESULT muBoss::init()
{
	_state5 = MIDLE;
	_canFire = false;
	_readyToCrash = false;
	fireCount = 0;
	crashCount = 0;
	crashPointCount = 0;

	return S_OK;
}

void muBoss::update()
{
	muuAnimation();
	muBossExplode();
	
}

void muBoss::render(HDC hdc)
{
	switch (_state5)
	{
	case MIDLE:
		IMAGEMANAGER->frameRender("muBoss", hdc, CAMERA->GetRelativeX(rc.left + 60),
			CAMERA->GetRelativeY(rc.top - 20), objFrameX, objFrameY, CAMERA->GetZoom());
		break;
	case CRY:
		IMAGEMANAGER->frameRender("muBossCry", hdc, CAMERA->GetRelativeX(rc.left + 60),
			CAMERA->GetRelativeY(rc.top - 20), objFrameX, objFrameY, CAMERA->GetZoom());
		break;
	case MUSHOOT:
		IMAGEMANAGER->frameRender("muBossCry", hdc, CAMERA->GetRelativeX(rc.left + 60),
			CAMERA->GetRelativeY(rc.top - 20), objFrameX, objFrameY, CAMERA->GetZoom());
		break;
	}
}

void muBoss::muuAnimation()
{
	switch (_state5)
	{
	case MIDLE:
		objFrameX = _index;
		objFrameY = 0;
		if (_count++ % 10 == 0)
		{
			if (_index++ > 9)
				_index = 0;
		}
		break;
	case CRY:
		objFrameX = _index;
		objFrameY = 0;
		if (_count++ % 15 == 0)
		{
			if (_index++ > 6)
			{
				_index = 0;
				_state5 = MIDLE;
			}
		}
		break;
	case MUSHOOT:
		objFrameX = _index;
		objFrameY = 0;
		if (_count++ % 15 == 0)
		{
			if (_index++ > 6)
			{
				_index = 0;
				_state5 = MIDLE;
			}
		}
		break;
	}
}

void muBoss::hurt(int damage, bool onlyEffect)
{
	SOUNDMANAGER->play("무맞을때소리");
	_index = 0;
	_count = 0;
	_state5 = CRY;

	vector<string> t_vStr;
	int t_random = RANDOM->range(0, 3);
	if (t_random == 0)
		t_vStr.push_back("그, 그래도 난 널 믿어...!");
	else if (t_random == 1)
		t_vStr.push_back("갑자기 왜 그러는 거야...!");
	else
		t_vStr.push_back("내, 내가 뭘 잘못한 거니...?!");

	DIALOGUE->ShowDialogue(t_vStr, &rc, 10);
	if (onlyEffect)
		return;

	unit::hurt(damage);
}

void muBoss::dead()
{
	UNITMANAGER->AddTreasure("treasureBox", "slot_Bow", { GetCenterX(), GetCenterY() });
}

void muBoss::muFire()
{
	//if (fireCount++ % 10 == 0)
	//{
	//	_canFire = true;
	//	UNITMANAGER->GetProjectileMG()->CreateProjectile("muMissile", x,y, 10, shootToTarget(), 5, 50, true, true);
	//}
	
}

void muBoss::muBossExplode()
{
	if (currentHp <= maxHp * 0.7f)
	{
		if (crashPointCount++ == 180)
		{
			int t_anchorRange = 225;
			int t_range = 75;
			int t_random = RANDOM->range(-t_range, t_range);
			EFFECTMANAGER->ShowEffectAlpha("damageBoundary", { (long)x - t_anchorRange + t_random, (long)y + t_random }, 0, 255, 3, 20, true, false);

			t_random = RANDOM->range(-t_range, t_range);
			EFFECTMANAGER->ShowEffectAlpha("damageBoundary", { (long)x + t_anchorRange + t_random, (long)y + t_random }, 0, 255, 3, 20, true, false);

			t_random = RANDOM->range(-t_range, t_range);
			EFFECTMANAGER->ShowEffectAlpha("damageBoundary", { (long)x - t_anchorRange + t_random, (long)y - t_anchorRange + t_random }, 0, 255, 3, 20, true, false);

			t_random = RANDOM->range(-t_range, t_range);
			EFFECTMANAGER->ShowEffectAlpha("damageBoundary", { (long)x + t_anchorRange + t_random, (long)y - t_anchorRange + t_random }, 0, 255, 3, 20, true, false);

			t_random = RANDOM->range(-t_range, t_range);
			EFFECTMANAGER->ShowEffectAlpha("damageBoundary", { (long)x - t_anchorRange + t_random, (long)y + t_anchorRange + t_random }, 0, 255, 3, 20, true, false);

			t_random = RANDOM->range(-t_range, t_range);
			EFFECTMANAGER->ShowEffectAlpha("damageBoundary", { (long)x + t_anchorRange + t_random, (long)y + t_anchorRange + t_random }, 0, 255, 3, 20, true, false);

			t_random = RANDOM->range(-t_range, t_range);
			EFFECTMANAGER->ShowEffectAlpha("damageBoundary", { (long)x + t_random, (long)y + t_anchorRange + t_random }, 0, 255, 3, 20, true, false);

			t_random = RANDOM->range(-t_range, t_range);
			EFFECTMANAGER->ShowEffectAlpha("damageBoundary", { (long)x + t_random, (long)y - t_anchorRange + t_random }, 0, 255, 3, 20, true, false);
			crashPointCount = 0;

		
		}

	}

	
}
	
