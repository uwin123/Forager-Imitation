#pragma once
#include "gameNode.h"

struct tagForagerHp
{
	RECT ForagerHpRc;
	string imgName;
	bool _isHp;

};

struct tagStamina
{
	RECT staminaRc;
	string imgName;
	
};

class ForagerPlayer;
class ForagerStatManager :public gameNode
{
private:

	RECT _rcForagerHpBar;				//ü�¹� ��Ʈ
	float _x, _y;					//ü�¹� ��ġ��ǥ
	float _width;					//ü�¹� ���α���
	float _height;					//ü�¹� ���α���
	float _staminaImgSizeMax;		//�ִ� ���׹̳� 
	tagStamina* _foragerStamina;
	ForagerPlayer* _player;

	

public :

	HRESULT init();
	void release();
	void update();
	void render();

	vector<tagForagerHp*> _foragerHp;

	//�÷��̾� ���׹̳� ���� ī��Ʈ
	int playerStaminaCount;
	bool staminaLoss;
	


	//ü�¹� ��ġ �����ϱ�
	void setPos(float x, float y) { _x = x; _y = y; }
	//ü�¹� ������ �����ϱ�
	void setGauge(float maxHp, float currentHp);
};
