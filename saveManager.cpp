#include "stdafx.h"
#include "saveManager.h"

void saveManager::save()
{
	HANDLE file;
	DWORD write;

	file = CreateFile(slotName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(file, _tiles, sizeof(tile) * arraySize(), &write, NULL);
	CloseHandle(file);

}

void saveManager::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile(slotName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, _tiles, sizeof(tile)* arraySize(), &read, NULL);
	CloseHandle(file);

}

void saveManager::exchangeToArray(vector<tile> _vTiles)
{
	for (int i = 0; i < _vTiles.size(); i++)
	{
		_tiles[i] = _vTiles[i];
	}
}

int saveManager::arraySize()
{
	int i = 0;
	for (; i < 8000; i++) {
		if (_tiles[i].objHp) {
			break;
		}

	}
	return i;
}
