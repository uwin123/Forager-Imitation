#pragma once
#include "gameNode.h"
#include "tileNode.h"

class maptoolScene : public gameNode
{
private:
	tagTile _tiles[TILEX * TILEY]; //昔惟績鉢檎(図楕鉢檎)拭 紫遂鞠澗 展析 恥 400鯵
	tagSampleTile _sampleTile[SAMPLETILEX * SAMPLETILEY]; //児巴展析 恥 160鯵
	tagCurrentTile _currentTile; //薄仙展析

	RECT _rcSave;		//室戚崎
	RECT _rcLoad;		//稽球
	RECT _rcTerrain;	//走莫
	RECT _rcObject;		//神崎詮闘
	RECT _rcEraser;		//走酔鯵 (神崎詮闘幻 走錘陥, 走莫精 気嬢昇錘陥)

	bool test; // 訊 鋼慎戚 照 鞠劃.けけいしけいしけいしし//朕行

	CTRL _ctrlSelect;	//薄仙 嬢恐 珍闘継 雌殿昔走

public:
	HRESULT init();
	void release();
	void update();
	void render();

	//己祷室特
	void maptoolSetup();
	void setMap();
	void save();
	void load();

	//走莫, 神崎詮闘 識澱馬奄
	TERRAIN terrainSelect(int frameX, int frameY);
	OBJECT objectSelect(int frameX, int frameY);

};

