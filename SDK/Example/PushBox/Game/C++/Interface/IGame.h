#ifndef __IGame_H__
#define __IGame_H__

#include "IKernel.h"

#define GameModuleID 131

#define MAP_COL (8)
#define MAP_ROW (8)
#define TILE_WIDTH (50)
#define TILE_HEIGHT (50)
#define MAP_WIDTH (MAP_COL*TILE_WIDTH)
#define MAP_HEIGHT (MAP_ROW*TILE_HEIGHT)
#define MAP_INDEX(X,Y) ((X)+(Y)*MAP_COL)

enum TileType
{
	TT_NONE,
	TT_BOX,
	TT_TARGET,
	TT_WALL,
	TT_MAN,
};

class IGame : public IModule
{
public:
	virtual void Init(ModuleAttribute *pAttribute) = 0;
	virtual void Shut() = 0;
	virtual void Update(float fTime) = 0;
	virtual UCHAR GetModuleID() {return GameModuleID;};

public:
	virtual void SetHWND(HWND hWnd) = 0;
	// ���ƺ���
	virtual void Draw(int nX, int nY) = 0;
	// ���ƺ���
	virtual void DrawItem(int nX, int nY, int nType) = 0;
	// ��������
	virtual void Change(int col, int row, int nType) = 0;
	// ��������
	virtual void Load(const char *pName) = 0;
	virtual void Load(int nGate) = 0;
	// ��������
	virtual void Save(const char *pName) = 0;

	// ����
	virtual void TurnUp() = 0;
	// ����
	virtual void TurnDown() = 0;
	// ����
	virtual void TurnLeft() = 0;
	// ����
	virtual void TurnRight() = 0;
	// ������״̬
	virtual void CheckEnd() = 0;
};

extern IGame *g_pGame;

#endif // __IGame_H__