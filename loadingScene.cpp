#include "stdafx.h"
#include "loadingScene.h"

HRESULT loadingScene::init()
{
	//로딩클래스 초기화
	_loading = new loading;
	_loading->init();

	//로딩 이미지 및 사운드 초기화
	this->loadingImage();
	this->loadingSound();

	return S_OK;
}

void loadingScene::release()
{
	if(_loading)
		_loading->release();
	SAFE_DELETE(_loading);
}

void loadingScene::update()
{
	//로딩클래스 업데이트
	_loading->update();
}

void loadingScene::render()
{
	//로딩클래스 렌더
	_loading->render();

	//textOut(getMemDC(), 10, 10, "로딩화면");
}

void loadingScene::loadingImage()
{
	//로딩이 너무 빠르게 진행되서 천천히 돌아가도록 테스트용으로 만들기

	//IMAGEMANAGER->addImage("mountain", "Images/mountain.bmp", WINSIZEX, WINSIZEY);
	//IMAGEMANAGER->addImage("ball", "Images/ball.bmp", 60, 60);
	

}

void loadingScene::loadingSound()
{
}
