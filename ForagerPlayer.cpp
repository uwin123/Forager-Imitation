#include "stdafx.h"
#include "ForagerPlayer.h"
#include "ForagerStatManager.h"

HRESULT ForagerPlayer::init()
{
	x = WINSIZEX / 2;
	y = WINSIZEY / 2;
	_rcForager = RectMakeCenter(x, y, 30, 41);
	
	_rcHammer = RectMake((_rcForager.left + _rcForager.right) / 2, (_rcForager.top + _rcForager.bottom) / 2 - 28, 56, 56);

	//플레이어 가만히 있을 때 프레임 이미지 3*2
	IMAGEMANAGER->addFrameImage("playerStop", "Images/이미지/플레이어/player_idle_frame.bmp", 120,112, 3, 2, true, RGB(255, 0, 255));

	//플레이어 뛰다닐 때 프레임 이미지 4*2
	IMAGEMANAGER->addFrameImage("playerRUN", "Images/이미지/플레이어/player_run_frame.bmp", 160, 112, 4, 2, true, RGB(255, 0, 255));

	//플레이어 굴러다닐 때 프레임 이미지 12*1 2개 (init에서 아싸리 함수 호출 해서 GetKeyDown으로 한번 샤라락 굴림)
	IMAGEMANAGER->addFrameImage("playerRotate", "Images/이미지/플레이어/player_rotate_frame.bmp", 672, 56, 12, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("playerRotateLeft", "Images/이미지/플레이어/player_rotate_frame_left.bmp", 672, 56, 12, 1, true, RGB(255, 0, 255));

	//플레이어가 곡괭이'질'할 때 플레이어의 프레임 이미지 
	IMAGEMANAGER->addFrameImage("playerWork", "Images/이미지/플레이어/player_hammering_frame.bmp", 130, 100, 3, 2, true, RGB(255, 0, 255));

	//플레이어가 곡괭이'질'할 때 곡괭이의 프레임 이미지 3*1 
	IMAGEMANAGER->addFrameImage("playerHammering", "Images/이미지/아이템/곡괭이질하기.bmp",170,112, 3, 2, true, RGB(255, 0, 255));

	//곡괭이가 플레이어랑 같이 회전할 때 프레임 이미지 12*1 2개
	IMAGEMANAGER->addFrameImage("HammerImg", "Images/이미지/아이템/곡괭이right.bmp", 672, 56, 12, 1,true,RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("HammerImgLeft", "Images/이미지/아이템/곡괭이left.bmp", 672, 56, 12, 1,true,RGB(255, 0, 255));

	//플레이어가 그냥 이동할 때 나타나는 기본 곡괭이 이미지 
	IMAGEMANAGER->addImage("Hammer", "Images/이미지/아이템/곡괭이.bmp", 56, 56, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("HammerLeft", "Images/이미지/아이템/곡괭이왼쪽.bmp", 56, 56, true, RGB(255, 0, 255));

	//플레이어 체력(앞)게이지 이미지
	//IMAGEMANAGER->addImage("heart", "img_UI_Heart.bmp", );


	_foragerIdle = IMAGEMANAGER->findImage("playerStop");
	_foragerRun = IMAGEMANAGER->findImage("playerRUN");
	_foragerRotate = IMAGEMANAGER->findImage("playerRotate");
	_foragerHammering = IMAGEMANAGER->findImage("playerHammering");
	_playerHammering = IMAGEMANAGER->findImage("playerWork");
	_state = STATE::IDLE;

	_hammer = IMAGEMANAGER->findImage("Hammer");
	_hammerLeft = IMAGEMANAGER->findImage("Hammer");
	
	_footWalkCount = 0;
	_footWalkEffectInterval = 5;

	//플레이어 회전
	for(int i = 1 ; i < 12; i++)
	{
		image *left = IMAGEMANAGER->findImage("playerRotateLeft");
		image *right = IMAGEMANAGER->findImage("playerRotate");

		Rotate(left, left->getFrameWidth(), left->getFrameHeight(), i, true);
		Rotate(right, right->getFrameWidth(), right->getFrameHeight(), i, false);
	}

	//곡괭이 회전 
	for (int i = 1; i < 12; i++)
	{
		image *hammerLeft = IMAGEMANAGER->findImage("HammerImgLeft");
		image *hammerRight = IMAGEMANAGER->findImage("HammerImg");
	
		Rotate(hammerLeft, hammerLeft->getFrameWidth(), hammerLeft->getFrameHeight(), i, true);
		Rotate(hammerRight, hammerRight->getFrameWidth(), hammerRight->getFrameHeight(), i, false);
	}

	//프레임이미지 애니메이션 관련 변수 초기화 
	_count = 0;
	_index = 0;

	//회전관련 변수 초기화 
	_Acount = 0;
	_spinCount = 0;
	_spinSpeed = 0;

	//플레이어 상태 불값 초기화 
	_isLeft = false;
	_isUp = false;
	_isMoveHorizon = false;
	_isMoveVertical = false;
	_isMoveRotate = false;
	_isRun = false;
	_isHammering = false;
	_foragerHp = new ForagerStatManager;
	_foragerHp->init();

	return S_OK;
}

void ForagerPlayer::release()
{

}

void ForagerPlayer::update()
{
	animation();
	PlayerControll();
	playerMove();
	playerLookingDirection();
	
	_rcHammer = RectMake((_rcForager.left + _rcForager.right) / 2 , (_rcForager.top + _rcForager.bottom) / 2 - 28, 56, 56);
	CAMERA->targetFollow(_rcForager.left, _rcForager.top);
	
	_foragerHp->update();

}

void ForagerPlayer::render(HDC hdc)
{
	switch (_state)
	{
	case IDLE:
		IMAGEMANAGER->frameRender("playerStop", hdc, CAMERA->GetRelativeX(_rcForager.left), CAMERA->GetRelativeY(_rcForager.top));
		break;
	case RUN:
		IMAGEMANAGER->frameRender("playerRUN", hdc, CAMERA->GetRelativeX(_rcForager.left), CAMERA->GetRelativeY(_rcForager.top));
		break;
	case ROTATE: 
		if (_isLeft) 
		{
			IMAGEMANAGER->frameRender("playerRotateLeft", hdc, CAMERA->GetRelativeX(_rcForager.left), CAMERA->GetRelativeY(_rcForager.top));
			IMAGEMANAGER->frameRender("HammerImgLeft", hdc, CAMERA->GetRelativeX(_rcForager.left), CAMERA->GetRelativeY(_rcForager.top));
		}
		else 
		{
			IMAGEMANAGER->frameRender("playerRotate", hdc, CAMERA->GetRelativeX(_rcForager.left), CAMERA->GetRelativeY(_rcForager.top));
			IMAGEMANAGER->frameRender("HammerImg", hdc, CAMERA->GetRelativeX(_rcForager.left), CAMERA->GetRelativeY(_rcForager.top));
		}

		break;
	case HAMMERING :
		IMAGEMANAGER->frameRender("playerWork", hdc, CAMERA->GetRelativeX(_rcForager.left), CAMERA->GetRelativeY(_rcForager.top));
		IMAGEMANAGER->frameRender("playerHammering", hdc, CAMERA->GetRelativeX(_rcForager.left), CAMERA->GetRelativeY(_rcForager.top));
		break;
	}

	if (_state != ROTATE && _state != HAMMERING)
	{
		if (_isLeft)
			IMAGEMANAGER->render("Hammer", hdc, CAMERA->GetRelativeX(_rcHammer.left), CAMERA->GetRelativeY(_rcHammer.top));
		else
			IMAGEMANAGER->render("HammerLeft", hdc, CAMERA->GetRelativeX(_rcHammer.left - 40), CAMERA->GetRelativeY(_rcHammer.top));
	}

	_foragerHp->render();
}

void ForagerPlayer::animation()
{


	switch (_state)
	{
	case IDLE:
		if (_isLeft)
		{
			_foragerIdle->setFrameY(1);
			_foragerIdle->setFrameX(_index);
			if (_count++ % 5 == 0)
			{
				if (_index-- <= 0)
					_index = 2;
			}
		}
		else
		{
			_foragerIdle->setFrameY(0);
			_foragerIdle->setFrameX(_index);
			if (_count++ % 5 == 0)
			{
				if (_index++ > 3)
					_index = 0;
			}
		}
		break;

	case RUN:
		if (_isLeft)
		{
			_foragerRun->setFrameY(1);
			_foragerRun->setFrameX(_index);
			if (_count++ % 5 == 0)
			{
				if (_index-- <= 0)
					_index = 3;
			}
		}
		else
		{
			_foragerRun->setFrameY(0);
			_foragerRun->setFrameX(_index);
			if (_count++ % 5 == 0)
			{
				if (_index++ > 4)
					_index = 0;
			}
		}
		break;
	case ROTATE:
		if (_isLeft)
		{
			if (_count++ % 1 == 0)
			{
				if (_index++ > 11)
				{
					_index = 0;
					_state = STATE::IDLE;
					_isMoveRotate = false;
				}
				IMAGEMANAGER->findImage("playerRotateLeft")->setFrameX(_index);
				IMAGEMANAGER->findImage("HammerImgLeft")->setFrameX(_index);
			}
		}
		else
		{
			_foragerRotate->setFrameY(0);
			_foragerRotate->setFrameX(_index);
			if (_count++ % 1 == 0)
			{
				if (_index++ >11)
				{
					_index = 0;
					_state = STATE::IDLE;
					_isMoveRotate = false;
				}
				IMAGEMANAGER->findImage("HammerImg")->setFrameX(_index);
			}
		}
		break;
	case HAMMERING :
		if (_isLeft)
		{
			_foragerHammering->setFrameY(1);
			_playerHammering->setFrameY(1);
			_foragerHammering->setFrameX(_index);
			_playerHammering->setFrameX(_index);
			if (_count++ % 5 == 0)
			{
				if (_index-- <= 0)
					_index = 3;
			}
		}
		else
		{
			_foragerHammering->setFrameY(0);
			_playerHammering->setFrameY(0);
			_foragerHammering->setFrameX(_index);
			_playerHammering->setFrameX(_index);
			if (_count++ % 5 == 0)
			{
				if (_index++ > 3)
					_index = 0;
			}
		}
		break;
	}
}

void ForagerPlayer::PlayerControll()
{
	//_map->setPlayerPosTile();

	if (_state != STATE::ROTATE) {
		//가만히 있는 상태
		if (!INPUT->GetKey(VK_LEFT) || !INPUT->GetKey(VK_RIGHT))
		{
			_state = IDLE;
			_isMoveHorizon = false;
			_isMoveVertical = false;
		}
		//뛰어다니는 상태 (좌우 움직임)
		if (INPUT->GetKey('A') || INPUT->GetKey('D'))
		{
			_isMoveHorizon = true;
			_state = RUN;
			_isLeft = (INPUT->GetKey('A')) ? true : false;	//방향설정
		}
		//뛰어다니는 상태 (상하 움직임)
		if (INPUT->GetKey('W') || INPUT->GetKey('S'))
		{
			_isMoveVertical = true;
			_state = RUN;
			_isUp = (INPUT->GetKey('W')) ? true : false;	//방향 설정
		}

		// 움직일 떄만 굴러갈 수 있게
		if (_state != STATE::IDLE)
		{
			//굴러다니는 상태 
			if (INPUT->GetKeyDown(VK_SPACE))
			{
				_state = ROTATE;
				_isMoveRotate = true;
				IMAGEMANAGER->findImage("스테미나")->setWidth(5);
			}
		}
		// 망치질 하는 상태 
		if (INPUT->GetKey(VK_LBUTTON))
		{
			_state = HAMMERING;
		}
	}
}


void ForagerPlayer::playerMove()
{
	//플레이어 좌우 움직임 처리 
	if (_isMoveHorizon)
	{
		bool _cantMove = false;
		int applySpeed = (_isLeft) ? -3 : 3;

		//_cantMove = (_isLeft) ? _map->checkCanMove(-1) : _map->checkCanMove(1);

		if (!_cantMove) {

			OffsetRect(&_rcForager, applySpeed, 0);
			//플레이어가 움직이다가, 스페이스바 누르면 회전하면서 가속
			if (_state == STATE::ROTATE) {
				_spinSpeed = applySpeed * 2;
				OffsetRect(&_rcForager, _spinSpeed, 0);
			}
		}
		

	}

	//플레이어 상하 움직임 처리 
	if (_isMoveVertical)
	{
		bool _cantMove = false;
		int applySpeed = (_isUp) ? -3 : 3;

		//_cantMove = (_isUp) ? _map->checkCanMove(-MAPTILEX) : _map->checkCanMove(MAPTILEX);

		if (!_cantMove) {

			OffsetRect(&_rcForager, 0, applySpeed);

			//플레이어가 움직이다가, 스페이스바 누르면 회전하면서 가속
			if (_state == STATE::ROTATE)
			{
				_spinSpeed = applySpeed * 2;
				OffsetRect(&_rcForager, 0, _spinSpeed);
			}
		}
	}

	if (_state == ROTATE || _state == RUN) {
		// 발걸음 이펙트
		if (_footWalkCount++ >= _footWalkEffectInterval) {
			_footWalkCount = 0;
			POINT ptCenter = { _rcForager.left + (_rcForager.right - _rcForager.left) / 2 + RANDOM->range(-10, 0),
							 _rcForager.bottom - RANDOM->range(-1, -6) };
			float randomScale = RANDOM->range(0.01f, 0.03f);
			EFFECTMANAGER->ShowEffectAlphaSize("Walk1", ptCenter, 0, randomScale, 50, 150, true);
		}
	}
}


//플레이어가 보는 시선이 마우스 위치에 따라 변경 
void ForagerPlayer::playerLookingDirection()
{
	int forgaerCenter = (_rcForager.left + _rcForager.right) / 2;


	if (_state != STATE::ROTATE) {
		if (forgaerCenter < CAMERA->GetMouseRelativePos(_ptMouse).x)
			_isLeft = false;
		else
			_isLeft = true;
	}
}

void ForagerPlayer::RotateImage(image* img)
{
	if (_spinCount++ >= 12)
		_spinCount = 1;

	Rotate(img, img->getFrameWidth(), img->getFrameHeight(), _spinCount, _isLeft);
}

void ForagerPlayer::Rotate(image* img, int sizeX, int sizeY, int frameX, bool left)
{
	int x, y;
	int orig_x, orig_y;
	int pixel;

	double radian = 0;

	if (left)
		radian = (frameX * 30 * PI / 180.0);
	else
		radian = (frameX * -30 * PI / 180.0);

	double cc = cos(radian), ss = sin(-radian);
	double xcenter = (double)sizeX / 2.0, ycenter = (double)sizeY / 2.0; // (2)
	for (y = 0; y < sizeY; y++)
	{
		for (x = 0; x < sizeX; x++)
		{
			orig_x = (int)(xcenter + ((double)y - ycenter)*ss + ((double)x - xcenter)*cc);
			orig_y = (int)(ycenter + ((double)y - ycenter)*cc - ((double)x - xcenter)*ss);
			pixel = 0; // (3)

			if ((orig_y >= 0 && orig_y < sizeY) && (orig_x >= 0 && orig_x < sizeX)) // (4)
				SetPixel(img->getMemDC(), frameX * sizeX + x, y, GetPixel(img->getMemDC(), orig_x, orig_y));

		}
	}
}

