#include "Game.h"
#include <assert.h>
#include "ILog.h"

Game::Game()
{
	m_hWnd = NULL;
	memset(m_MapData, 0, sizeof(m_MapData));
}

Game::~Game()
{
}

void Game::Init(ModuleAttribute *pAttribute)
{	
	m_strDataDir = (*pAttribute)["DataDir"];
}

void Game::SetHWND(HWND hWnd)
{	
	m_hWnd = hWnd;

	// 窗口DC
	m_hDC = GetDC(m_hWnd);
	// 缓存DC
	m_hBufDC = CreateCompatibleDC(m_hDC);
	// 缓存位图
	m_hBufBmp = CreateCompatibleBitmap(m_hDC, MAP_WIDTH, MAP_HEIGHT);
	SelectObject(m_hBufDC, m_hBufBmp);
	// 图片DC
	m_hImgDC = CreateCompatibleDC(m_hDC);;
	// 图片位图
	m_hImgBmp = (HBITMAP)LoadImageA(NULL, (m_strDataDir + "/tile.bmp").c_str(), IMAGE_BITMAP, 250, 50, LR_LOADFROMFILE);
	SelectObject(m_hImgDC, m_hImgBmp);
	InvalidateRect(m_hWnd, NULL, FALSE);
}

void Game::Shut()
{
	if(m_hWnd == NULL)
		return ;

	DeleteDC(m_hBufDC);
	DeleteDC(m_hImgDC);
	DeleteObject(m_hBufBmp);
	DeleteObject(m_hImgBmp);
}

void Game::Update(float fTime)
{
}


void Game::Change(int col, int row, int nType)
{
	m_MapData[MAP_INDEX(col, row)] = nType;
	InvalidateRect(m_hWnd, NULL, FALSE);
}

// 绘制函数
void Game::DrawItem(int nX, int nY, int nType)
{
	BitBlt(m_hDC, nX, nY, TILE_WIDTH, TILE_HEIGHT, m_hImgDC, TILE_WIDTH*nType, 0, SRCCOPY);
}

// 绘制函数
void Game::Draw(int nX, int nY)
{
	//BitBlt(m_hDC, 0, 0, 250, 50, m_hImgDC, 0, 0, SRCCOPY);

	for(int col = 0; col < MAP_COL; col++)
	{		
		for(int row = 0; row < MAP_ROW; row++)
		{
			BitBlt(m_hBufDC, col*TILE_WIDTH, row*TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, m_hImgDC, TILE_WIDTH*m_MapData[MAP_INDEX(col, row)], 0, SRCCOPY);
		}
	}

	BitBlt(m_hDC, nX, nY, MAP_WIDTH, MAP_HEIGHT,  m_hBufDC, 0, 0, SRCCOPY);
}

// 加载数据
void Game::Load(const char *pName)
{
	std::string mapFile = m_strDataDir + "/" + pName;
	FILE *fp = fopen(mapFile.c_str(), "rb");
	if(fp == NULL)
	{
		LOGFMTE("Load Map %s Failure!", mapFile.c_str());
		return ;
	}

	fread(m_MapData, sizeof(m_MapData), 1, fp);
	fclose(fp);

	for(int i = 0; i < MAP_ROW*MAP_COL; i++)
	{
		if(m_MapData[i] == TT_MAN)
		{
			m_nManIndex = i;
			break;
		}
	}

	memcpy(m_SrcData, m_MapData, sizeof(m_MapData));
	m_nFoot = 0;
	InvalidateRect(m_hWnd, NULL, FALSE);
}

void Game::Load(int nGate)
{
	if(nGate == -1)
		nGate = m_nGate;

	m_nGate = nGate;

	char szName[200];
	sprintf(szName, "%d.map", m_nGate);
	Load(szName);
}

// 保存数据
void Game::Save(const char *pName)
{
	FILE *fp = fopen((m_strDataDir + "/" + pName).c_str(), "wb");
	if(fp == NULL)
	{
		return ;
	}

	fwrite(m_MapData, sizeof(m_MapData), 1, fp);
	fclose(fp);

	MessageBoxA(m_hWnd, "保存成功", "提示", MB_OK);
}

// 向上
void Game::TurnUp()
{
	int nRow = m_nManIndex / MAP_COL;
	int nCol = m_nManIndex % MAP_COL;

	if(nRow == 0)
		return ;

	int nToPos = MAP_INDEX(nCol, nRow-1);

	if(m_MapData[nToPos] == TT_WALL)
		return ;

	if(m_MapData[nToPos] == TT_BOX)
	{
		if(nRow == 1)
			return ;

		int nNexPos = MAP_INDEX(nCol, nRow-2);
		if(m_MapData[nNexPos] == TT_WALL || m_MapData[nNexPos] == TT_BOX)
			return;

		m_MapData[nNexPos] = TT_BOX;
	}

	m_MapData[nToPos] = TT_MAN;

	if(m_SrcData[m_nManIndex] == TT_TARGET)
	{
		m_MapData[m_nManIndex] = TT_TARGET;
	}
	else
	{
		m_MapData[m_nManIndex] = TT_NONE;
	}

	m_nManIndex = nToPos;
	
	InvalidateRect(m_hWnd, NULL, FALSE);
	m_nFoot++;
	CheckEnd();
}

// 向下
void Game::TurnDown()
{
	int nRow = m_nManIndex / MAP_COL;
	int nCol = m_nManIndex % MAP_COL;
	
	if(nRow == MAP_ROW-1)
		return ;

	int nToPos = MAP_INDEX(nCol, nRow+1);

	if(m_MapData[nToPos] == TT_WALL)
		return ;

	if(m_MapData[nToPos] == TT_BOX)
	{
		if(nRow == MAP_ROW-2)
			return ;

		int nNexPos = MAP_INDEX(nCol, nRow+2);
		if(m_MapData[nNexPos] == TT_WALL || m_MapData[nNexPos] == TT_BOX)
			return;
		m_MapData[nNexPos] = TT_BOX;
	}

	m_MapData[nToPos] = TT_MAN;

	if(m_SrcData[m_nManIndex] == TT_TARGET)
	{
		m_MapData[m_nManIndex] = TT_TARGET;
	}
	else
	{
		m_MapData[m_nManIndex] = TT_NONE;
	}

	m_nManIndex = nToPos;

	InvalidateRect(m_hWnd, NULL, FALSE);
	m_nFoot++;
	CheckEnd();
}

// 向左
void Game::TurnLeft()
{
	int nRow = m_nManIndex / MAP_COL;
	int nCol = m_nManIndex % MAP_COL;

	if(nCol == 0)
		return ;

	int nToPos = MAP_INDEX(nCol-1, nRow);

	if(m_MapData[nToPos] == TT_WALL)
		return ;

	if(m_MapData[nToPos] == TT_BOX)
	{
		if(nCol == 1)
			return ;

		int nNexPos = MAP_INDEX(nCol-2, nRow);
		if(m_MapData[nNexPos] == TT_WALL || m_MapData[nNexPos] == TT_BOX)
			return;
		m_MapData[nNexPos] = TT_BOX;
	}

	m_MapData[nToPos] = TT_MAN;

	if(m_SrcData[m_nManIndex] == TT_TARGET)
	{
		m_MapData[m_nManIndex] = TT_TARGET;
	}
	else
	{
		m_MapData[m_nManIndex] = TT_NONE;
	}

	m_nManIndex = nToPos;
	
	InvalidateRect(m_hWnd, NULL, FALSE);
	m_nFoot++;
	CheckEnd();
}

// 向右
void Game::TurnRight()
{
	int nRow = m_nManIndex / MAP_COL;
	int nCol = m_nManIndex % MAP_COL;
	
	if(nCol == MAP_COL-1)
		return ;

	int nToPos = MAP_INDEX(nCol+1, nRow);

	if(m_MapData[nToPos] == TT_WALL)
		return ;

	if(m_MapData[nToPos] == TT_BOX)
	{
		if(nCol == MAP_COL-2)
			return ;

		int nNexPos = MAP_INDEX(nCol+2, nRow);
		if(m_MapData[nNexPos] == TT_WALL || m_MapData[nNexPos] == TT_BOX)
			return;
		m_MapData[nNexPos] = TT_BOX;
	}

	m_MapData[nToPos] = TT_MAN;

	if(m_SrcData[m_nManIndex] == TT_TARGET)
	{
		m_MapData[m_nManIndex] = TT_TARGET;
	}
	else
	{
		m_MapData[m_nManIndex] = TT_NONE;
	}

	m_nManIndex = nToPos;

	InvalidateRect(m_hWnd, NULL, FALSE);
	m_nFoot++;
	CheckEnd();
}

void Game::CheckEnd()
{
	bool bIsEnd = true;
	for(int i = 0; i < MAP_ROW*MAP_COL; i++)
	{
		if(m_MapData[i] == TT_TARGET)
		{
			bIsEnd = false;
			break;
		}
	}

	if(m_SrcData[m_nManIndex] == TT_TARGET)
	{
		bIsEnd = false;
	}

	if(bIsEnd)
	{
		char szName[200];
		sprintf(szName, "以%d步数完成, 是否进入下一关", m_nFoot);
		if(MessageBoxA(m_hWnd, szName, "恭喜", MB_YESNO) == IDYES)
		{
			Load(m_nGate+1);
		}
		else
		{
			Load(m_nGate);
		}
	}
}