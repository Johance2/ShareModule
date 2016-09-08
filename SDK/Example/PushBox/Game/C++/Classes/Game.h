#ifndef __Game_H__
#define __Game_H__

#include "../Interface/IGame.h"
#include <windows.h>

class Game : public IGame
{
public:
    Game();
    ~Game();
	
	virtual void Init(ModuleAttribute *pAttribute);
	virtual void Shut();
	virtual void Update(float fTime);
	
public:
	// 初始化句柄
	virtual void SetHWND(HWND hWnd);
	// 绘制函数
	virtual void Draw(int nX, int nY);
	// 绘制函数
	virtual void DrawItem(int nX, int nY, int nType);
	// 设置数据
	virtual void Change(int col, int row, int nType);
	// 加载数据
	virtual void Load(const char *pName);
	virtual void Load(int nGate);
	// 保存数据
	virtual void Save(const char *pName);

	// 向上
	virtual void TurnUp();
	// 向下
	virtual void TurnDown();
	// 向左
	virtual void TurnLeft();
	// 向右
	virtual void TurnRight();
	// 检测完成状态
	virtual void CheckEnd();
	
private:
	HWND m_hWnd;
	// 窗口DC
	HDC m_hDC;
	// 缓存DC
	HDC m_hBufDC;
	// 缓存位图
	HBITMAP m_hBufBmp;
	// 图片DC
	HDC m_hImgDC;
	// 图片位图
	HBITMAP m_hImgBmp;

protected:
	char m_MapData[MAP_COL*MAP_ROW];
	char m_SrcData[MAP_COL*MAP_ROW];

	// 当前小人的位置
	int m_nManIndex;
	// 当前管卡
	int m_nGate;
	// 当前走的步数
	int m_nFoot;
	// 数据路径
	std::string m_strDataDir;
};

#define g_pGame ((Game*)GameEnv::GetGameModule(eGMIT_Game))

#endif // __Game_H__