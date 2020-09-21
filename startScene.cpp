#include "stdafx.h"
#include "startScene.h"


HRESULT startScene::init()
{
	// 마우스 숨기기
	ShowCursor(false);

	// 이미지 등록 (추후 로딩 구현시 옮겨질 것들)
	IMAGEMANAGER->addImage("Logo", "Images/이미지/img_Logo.bmp", 300, 180, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("BtnCredit", "Images/이미지/GUI/img_UI_BtnCredit.bmp", 213, 104, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("BtnExit", "Images/이미지/GUI/img_UI_BtnExit.bmp", 213, 104, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("BtnSetting", "Images/이미지/GUI/img_UI_BtnSetting.bmp", 213, 104, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("GameSlot", "Images/이미지/GUI/img_UI_GameSlotBoundary.bmp", 227, 227, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("GameSlotInner", "Images/이미지/GUI/img_UI_GameSlotInner.bmp", 227, 227, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("TitleBG", "Images/이미지/img_Background.bmp", 2000, 1300, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("TitleCursor", "Images/이미지/GUI/커서.bmp", 15, 15, true, RGB(255, 0, 255));


	// 커서 반응형 이미지 설정
	for (int i = 0; i < BUTTON_MAX / 2; i++) {
		RECT slotRC = RectMakeCenter(WINSIZEX / 2 - 400 + i * 400, 360, 227, 227);
		RECT btnRC = RectMakeCenter(WINSIZEX / 2 - 300 + i * 300, WINSIZEY - 120, 213, 104);

		_button[i + BUTTON_MAX / 2].init(slotRC, 18, 4, "GameSlot", "GameSlotInner", 100);
		if (i == 0) _button[i].init(btnRC, 12, 3, "BtnSetting");
		else if (i == 1) _button[i].init(btnRC, 12, 3, "BtnCredit");
		else if (i == 2) _button[i].init(btnRC, 12, 3, "BtnExit");
	}

	// 타겟팅 박스 설정
	_targetingBox = new targetingBox;
	_targetingBox->init();

	return S_OK;
}

void startScene::release()
{
	// 타겟팅 박스 제거
	SAFE_DELETE(_targetingBox);
}

void startScene::update()
{
	// 마우스 호버 체크
	CheckCursorOnButton();
	
	// 마우스 클릭 체크
	CheckButtonClick();
}

void startScene::render()
{

	// 파란 배경 렌더
	IMAGEMANAGER->findImage("TitleBG")->render(getMemDC(), -300, -300);

	// 로고 렌더
	IMAGEMANAGER->findImage("Logo")->render(getMemDC(), WINSIZEX / 2 - 150, 0);


	// 게임슬롯 렌더
	for (int i = 0; i < BUTTON_MAX; i++) {
		_button[i].render(getMemDC());
	}

	// 타겟팅 박스 렌더
	_targetingBox->render(getMemDC());

	// 커서 렌더
	IMAGEMANAGER->findImage("TitleCursor")->render(getMemDC(), _ptMouse.x, _ptMouse.y);


}

void startScene::CheckCursorOnButton()
{
	bool _cursor = false;
	for (int i = 0; i < BUTTON_MAX; i++)
	{
		_button[i].update();

		// 버튼 위에 커서
		if (PtInRect(&_button[i].GetRect(), _ptMouse)) {
			_targetingBox->SetTarget(_button[i].GetRect());
			_button[i].CursorIn();
			_cursor = true;
		}
		else
			_button[i].CursorOut();
	}
	if (!_cursor)
		_targetingBox->CursorOut();

	_targetingBox->update();

}




void startScene::CheckButtonClick()
{
	// 버튼 클릭
	if (INPUT->GetKeyDown(VK_LBUTTON))
	{
		for (int i = 0; i < 3; i++)
		{
			if (PtInRect(&_button[i].GetRect(), _ptMouse))
			{
				switch (i) {
				case BTN::SETTING:	SCENEMANAGER->loadScene("맵툴화면"); break;	// 환경설정 창
				case BTN::CREDIT:	SCENEMANAGER->loadScene("맵툴화면"); break;	// 크레딧 창
				case BTN::EXIT:		SCENEMANAGER->loadScene("맵툴화면"); break;	// 게임 종료
				case BTN::SLOT1:	SCENEMANAGER->loadScene("맵툴화면"); break;	// 게임 슬롯 1
				case BTN::SLOT2:	SCENEMANAGER->loadScene("맵툴화면"); break;	// 게임 슬롯 2
				case BTN::SLOT3:	SCENEMANAGER->loadScene("맵툴화면"); break;	// 게임 슬롯 3
				}
			}
		}
	}
}
