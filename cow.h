#pragma once
#include "enemy.h"
class cow : public enemy
{
private:
	int cowHitCount;
	int cowHitWaitCount;
	int searchCount;
	int cowDashRange;
	bool tryAttack;

	bool isattacking;


private:

public:
	HRESULT init();
	void update();
	void render(HDC hdc) override;

	void cowAnimation();
	void cowMove();
	void cowAttack();
	void cowLookDirection();
	



	
};

