#include "stdafx.h"
#include "saveManager.h"
#include "quick_slot.h"
#include "earth.h"
void saveManager::save()
{
	this->Item_transform();
	this->equip_transform();
	this->Tile_transform();
	this->Unit_transform();
	this->Player_transform();
	this->npc_transform();

	HANDLE file;
	DWORD write;

	file = CreateFile(My_Game_save_file_item, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(file, My_Item, sizeof(inventory_slot) * ITEMSIZE, &write, NULL);
	CloseHandle(file);

	file = CreateFile(My_Game_save_file_equip, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(file, My_equip, sizeof(inventory_slot) * EQUIPSIZE, &write, NULL);
	CloseHandle(file);

	file = CreateFile(My_Game_save_file_tile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(file, My_Tile, sizeof(tile) * TILEMAXSIZE, &write, NULL);
	CloseHandle(file);

	file = CreateFile(My_Game_save_file_unit, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(file, My_unit, sizeof(unit) * _Unit.size(), &write, NULL);
	CloseHandle(file);

	file = CreateFile(My_Game_save_file_npc, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(file, quest, sizeof(npc) * NPCMAXSIZE, &write, NULL);
	CloseHandle(file);

}

bool saveManager::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile(My_Game_save_file_item, GENERIC_READ, 0, NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, My_Item, sizeof(inventory_slot) *ITEMSIZE, &read, NULL);
	CloseHandle(file);
	if (file != INVALID_HANDLE_VALUE) { // 파일의 존재 여부 확인
		for (int i = 0; i < ITEMMANAGER->getvInventory_info().size(); i++) {
			ITEMMANAGER->getvInventory_info()[i]->count = My_Item[i].count;
			ITEMMANAGER->getvInventory_info()[i]->_item =  My_Item[i]._item;
			ITEMMANAGER->getvInventory_info()[i]->isCheck = My_Item[i].isCheck;
			ITEMMANAGER->getvInventory_info()[i]->x = My_Item[i].x;
			ITEMMANAGER->getvInventory_info()[i]->y = My_Item[i].y;
			ITEMMANAGER->getvInventory_info()[i]->_rc = My_Item[i]._rc;
		}
	}

	file = CreateFile(My_Game_save_file_equip, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, My_equip, sizeof(inventory_slot) *EQUIPSIZE, &read, NULL);
	CloseHandle(file);


	if (file != INVALID_HANDLE_VALUE) { // 파일의 존재 여부 확인
		for (int i = 0; i < ITEMMANAGER->get_equip_info().size(); i++) {
			ITEMMANAGER->get_equip_info()[i]->count = My_equip[i].count;
			ITEMMANAGER->get_equip_info()[i]->_item = My_equip[i]._item;
			ITEMMANAGER->get_equip_info()[i]->isCheck = My_equip[i].isCheck;
			ITEMMANAGER->get_equip_info()[i]->x = My_equip[i].x;
			ITEMMANAGER->get_equip_info()[i]->y = My_equip[i].y;
			ITEMMANAGER->get_equip_info()[i]->_rc = My_equip[i]._rc;
		}
		_quick_slot->quick_slot_update();
	}

	file = CreateFile(My_Game_save_file_tile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, My_Tile, sizeof(tile) *TILEMAXSIZE, &read, NULL);
	CloseHandle(file);


	if (file != INVALID_HANDLE_VALUE) {
		for (int i = 0; i < TILEMAXSIZE; i++) {
			(*_game_tile)[i].terrKey = My_Tile[i].terrKey ;
			(*_game_tile)[i].terrainFrameX = My_Tile[i].terrainFrameX ;
			(*_game_tile)[i].terrainFrameY = My_Tile[i].terrainFrameY;
			(*_game_tile)[i].hasUnit = My_Tile[i].hasUnit;
			(*_game_tile)[i].canPass = My_Tile[i].canPass;
			(*_game_tile)[i].layer = My_Tile[i].layer;
			(*_game_tile)[i].rc = My_Tile[i].rc;
			(*_game_tile)[i].tag = My_Tile[i].tag;
			(*_game_tile)[i].x = My_Tile[i].x;
			(*_game_tile)[i].y = My_Tile[i].y;
		}
	}
	file = CreateFile(My_Game_save_file_unit, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, My_unit, sizeof(unit) * UNITMAXSIZE, &read, NULL);
	CloseHandle(file);

	if (file != INVALID_HANDLE_VALUE) {

		for (int i = 0; i < UNITMAXSIZE ; i++) {
			tile* ptile = nullptr;
			vector<tile*> vTemp;
			int exp;
			int level;
			int stamina;
			int staminaBack;
			if(My_unit[i].tileIndex >= 0)
				ptile = _map->GetTileP(My_unit[i].tileIndex);

			switch (My_unit[i].tag) {
			//case TAG::ITEM:
			//	UNITMANAGER->AddProduction(My_unit[i].objKey, { My_unit[i].rc.left, My_unit[i].rc.top });
			//	break;
			case TAG::ENEMY:
				UNITMANAGER->AddUnits(My_unit[i].objKey, { My_unit[i].rc.left, My_unit[i].rc.top }, true);
				break;
			case TAG::PLAYER:
				exp = INIDATA->loadDataInteger(My_Game_save_file_player, "Player", "Exp");
				level = INIDATA->loadDataInteger(My_Game_save_file_player, "Player", "Level");
				stamina = INIDATA->loadDataInteger(My_Game_save_file_player, "Player", "Stamina");
				staminaBack = INIDATA->loadDataInteger(My_Game_save_file_player, "Player", "StaminaBack");
				ITEMMANAGER->setMoney(INIDATA->loadDataInteger(My_Game_save_file_player, "Player", "Money"));
				STATMANAGER->SetCurrentExp(exp);
				STATMANAGER->SetLevel(level);
				STATMANAGER->SetStamina(stamina);
				STATMANAGER->SetStaminaMax(staminaBack);
				break;
			case TAG::OBJECT:
				UNITMANAGER->AddResource(My_unit[i].objKey, ptile, My_unit[i].tileIndex);
				break;
			case TAG::NPC:
				
				break;
			case TAG::BUILDING:
				if (My_unit[i].objKey == "bridge" || My_unit[i].objKey == "fishtrap") {
					UNITMANAGER->AddBuilding(My_unit[i].objKey, ptile, My_unit[i].tileIndex);
				}
				else {
					vTemp.push_back(ptile);
					vTemp.push_back(_map->GetTileP(My_unit[i].tileIndex+1));
					vTemp.push_back(_map->GetTileP(My_unit[i].tileIndex+MAPTILEX));
					vTemp.push_back(_map->GetTileP(My_unit[i].tileIndex+MAPTILEX+1));
					UNITMANAGER->AddBuilding(My_unit[i].objKey, vTemp, My_unit[i].tileIndex);
				}
				
				break;
			}
		}
	}

	file = CreateFile(My_Game_save_file_npc, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, quest, sizeof(npc) * NPCMAXSIZE, &read, NULL);
	CloseHandle(file);

	if (file != INVALID_HANDLE_VALUE) {
		vector<npc*> _npclode = UNITMANAGER->GetNpc();
		for (int i = 0; i < NPCMAXSIZE;i++) {
			UNITMANAGER->GetNpc()[i]->set_isQuestReceive(quest[i].get_isQuestReceive());
			UNITMANAGER->GetNpc()[i]->set_isQuestComplete(quest[i].get_isQuestComplete());
		}
	}


	return true;
}

void saveManager::Item_transform()
{
	for (int i = 0; i < ITEMMANAGER->getvInventory_info().size(); i++) {
		My_Item[i].count =ITEMMANAGER->getvInventory_info()[i]->count;
		My_Item[i]._item =ITEMMANAGER->getvInventory_info()[i]->_item;
		My_Item[i].isCheck =ITEMMANAGER->getvInventory_info()[i]->isCheck;
		My_Item[i].x =ITEMMANAGER->getvInventory_info()[i]->x;
		My_Item[i].y =ITEMMANAGER->getvInventory_info()[i]->y;
		My_Item[i]._rc =ITEMMANAGER->getvInventory_info()[i]->_rc;
	}

}

void saveManager::equip_transform()
{
	for (int i = 0; i < ITEMMANAGER->get_equip_info().size(); i++) {
		 My_equip[i].count = ITEMMANAGER->get_equip_info()[i]->count;
		 My_equip[i]._item = ITEMMANAGER->get_equip_info()[i]->_item;
		 My_equip[i].isCheck = ITEMMANAGER->get_equip_info()[i]->isCheck;
		 My_equip[i].x = ITEMMANAGER->get_equip_info()[i]->x;
		 My_equip[i].y = ITEMMANAGER->get_equip_info()[i]->y;
		 My_equip[i]._rc = ITEMMANAGER->get_equip_info()[i]->_rc;
	}
}

void saveManager::Unit_transform()
{
	_Unit = UNITMANAGER->GetUnits();

	for (int i = 0; i < UNITMAXSIZE; i++) {
		My_unit[i].tileIndex = 1;
		if (i < _Unit.size()) {
			My_unit[i].objKey = _Unit[i]->objKey;
			My_unit[i].tag = _Unit[i]->tag;
			My_unit[i].rc = _Unit[i]->rc;
			My_unit[i].tileIndex = _Unit[i]->tileIndex;
		}
		else {
			My_unit[i].tag = TAG::NONE;
		}
	}
}

void saveManager::Player_transform()
{
	int exp = STATMANAGER->GetCurrentExp();
	int level = STATMANAGER->GetLevel();
	int stamina = STATMANAGER->GetStamina();
	int staminaBack = STATMANAGER->GetStaminaMax();
	int money = ITEMMANAGER->getMoney();
	string sExp = to_string(exp);
	string sLevel = to_string(level);
	string sStamina = to_string(stamina);
	string sStaminaBack = to_string(staminaBack);
	string smoney = to_string(money);
	
	INIDATA->addData("Player", "Exp", sExp.c_str());
	INIDATA->addData("Player", "Level", sLevel.c_str());
	INIDATA->addData("Player", "Stamina", sStamina.c_str());
	INIDATA->addData("Player", "StaminaBack", sStaminaBack.c_str());
	INIDATA->addData("Player", "Money", smoney.c_str());
	INIDATA->saveINI(My_Game_save_file_player);
}

void saveManager::npc_transform()
{
	vector<npc*> *_npc = UNITMANAGER->PGetNpc();
	for (int i = 0; i < (*_npc).size(); i++) {
		quest[i].set_isQuestReceive((*_npc)[i]->get_isQuestReceive());
		
		quest[i].set_isQuestComplete((*_npc)[i]->get_isQuestComplete());
	
	}
}

void saveManager::Tile_transform()
{

	for (int i = 0; i <(*_game_tile).size(); i++) {
		My_Tile[i].terrKey = (*_game_tile)[i].terrKey;
		My_Tile[i].terrainFrameX =(*_game_tile)[i].terrainFrameX;
		My_Tile[i].terrainFrameY =(*_game_tile)[i].terrainFrameY;
		My_Tile[i].hasUnit =(*_game_tile)[i].hasUnit;
		My_Tile[i].canPass =(*_game_tile)[i].canPass;
		My_Tile[i].layer =(*_game_tile)[i].layer;
		My_Tile[i].rc =(*_game_tile)[i].rc;
		My_Tile[i].tag =(*_game_tile)[i].tag;
		My_Tile[i].x =(*_game_tile)[i].x;
		My_Tile[i].y =(*_game_tile)[i].y;
	}
}

