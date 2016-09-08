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
	// ��ʼ�����
	virtual void SetHWND(HWND hWnd);
	// ���ƺ���
	virtual void Draw(int nX, int nY);
	// ���ƺ���
	virtual void DrawItem(int nX, int nY, int nType);
	// ��������
	virtual void Change(int col, int row, int nType);
	// ��������
	virtual void Load(const char *pName);
	virtual void Load(int nGate);
	// ��������
	virtual void Save(const char *pName);

	// ����
	virtual void TurnUp();
	// ����
	virtual void TurnDown();
	// ����
	virtual void TurnLeft();
	// ����
	virtual void TurnRight();
	// ������״̬
	virtual void CheckEnd();
	
private:
	HWND m_hWnd;
	// ����DC
	HDC m_hDC;
	// ����DC
	HDC m_hBufDC;
	// ����λͼ
	HBITMAP m_hBufBmp;
	// ͼƬDC
	HDC m_hImgDC;
	// ͼƬλͼ
	HBITMAP m_hImgBmp;

protected:
	char m_MapData[MAP_COL*MAP_ROW];
	char m_SrcData[MAP_COL*MAP_ROW];

	// ��ǰС�˵�λ��
	int m_nManIndex;
	// ��ǰ�ܿ�
	int m_nGate;
	// ��ǰ�ߵĲ���
	int m_nFoot;
	// ����·��
	std::string m_strDataDir;
};

#define g_pGame ((Game*)GameEnv::GetGameModule(eGMIT_Game))

#endif // __Game_H__