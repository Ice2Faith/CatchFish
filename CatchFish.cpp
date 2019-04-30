#include<graphics.h>
#include<stdio.h>
#include<conio.h>
#include<math.h>
#include<time.h>
#include<windows.h>
#define NUM 16
const double PI = 3.1415926;
typedef struct
{
	int red;
	int green;
	int blue;
}RGBC;
typedef struct
{
    int life;
    int live;
    int x;
    int y;
    int wid;
    int hei;
	int gold;
	RGBC color;
}Fish;
typedef struct
{
    int harm;
    int live;
    double direc;
    int x;
    int y;
    int r;
	RGBC color;
}Bullet;
typedef struct
{
    int x;
    int y;
	int r;
	int len;
    double direc;
    int power;
	RGBC color;
}Cannon;

Fish fishgroup[NUM];
Bullet bulletgroup[NUM];
Cannon cannon;

IMAGE bgimage;

void InitGame();
void DrawFace();
void MoveCannon();
void MoveBullet();
void MoveFish();
void CreateBullet();
void CreateFish();
void CheckCrash();
void GuideFace();
void ExitGame();

int ScreenX,ScreenY;
int WindowX,WindowY;
int GameWindowX, GameWindowY;
int FontWid,FontHei;

int Score, Level, Jewel, Gold, Coin;
DWORD tb0,tf0, tb1,tf1;
int main()
{
	InitGame();
	while (true)
	{
		DrawFace();
		MoveCannon();
		if (WindowY<320)
		{
			tb1 = GetTickCount();
			if (tb1 - tb0 > 0)
			{
				MoveBullet();
				tb0 = tb1;
			}
		}
		else
			MoveBullet();
		
		if (WindowX<480)
		{
			tf1 = GetTickCount();
			if (tf1 - tf0 > 1)
			{
				MoveFish();
				tf0 = tf1;
			}
		}
		else
			MoveFish();
		
		CreateBullet();
		CreateFish();
		CheckCrash();
		ExitGame();
	}
    return 0;
}
void GuideFace()
{
	int GFontW, GFontH;
	GFontW = GameWindowX * 5 / 100;
	GFontH = GameWindowY * 5 / 100;
	setfillcolor(RGB(27, 220, 162));
	solidrectangle(0, 0, GameWindowX, GameWindowY);
	DrawFace();
	settextstyle(GFontW, GFontH, "黑体");
	settextcolor(RGB(245,245,100));
	outtextxy(GameWindowX*7/20, GameWindowY*1/10, "捕鱼达人");
	outtextxy(GameWindowX * 2 / 10, GameWindowY * 2 / 10, "鼠标移动控制发射方向");
	outtextxy(GameWindowX * 2 / 10, GameWindowY * 3 / 10, "左键、空格发射炮弹");
	outtextxy(GameWindowX * 2 / 10, GameWindowY * 4 / 10, "ESC可以任意时候退出游戏");
	outtextxy(GameWindowX * 5/20, GameWindowY * 6 / 10, "点击任意键开始游戏");
	ExitGame();
	_getch();
}
void ExitGame()
{
	if (_kbhit())
	if (_getch() == '*')
		exit(0);
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
		exit(0);
}
void InitGame()
{
	ScreenX = GetSystemMetrics(SM_CXSCREEN) ;
	ScreenY = GetSystemMetrics(SM_CYSCREEN) ;
	GameWindowX = ScreenX * 2 / 3;
	GameWindowY = ScreenY * 2 / 3;
	WindowX = GameWindowX * 9 / 10;
	WindowY = GameWindowY;
	initgraph(GameWindowX, GameWindowY);
	srand((unsigned)time(NULL));
	setbkmode(TRANSPARENT);
	FontWid = WindowX * 2 / 100;
	FontHei = WindowY * 2 / 100;
	cannon.direc = PI*3/2;
	cannon.power = 20;
	cannon.x = WindowX / 2;
	cannon.y = WindowY * 95 / 100;
	cannon.len = WindowY * 10 / 100;
	cannon.r = WindowY * 5 / 100;
	cannon.color.red = 60;
	cannon.color.green = 60;
	cannon.color.blue = 60;
	Score = 0;
	Level = 1;
	Jewel = 0;
	Gold = 0;
	Coin = 0;
	tb0 = GetTickCount();
	tb1 = tb0;
	tf0 = tb0;
	tf1 = tb0;
	loadimage(&bgimage, "bgimage.jpg", GameWindowX, GameWindowY);
	for (int i = 0; i < NUM; i++)
	{
		if (rand() % 100<33)
			fishgroup[i].live = 0;
		else
			fishgroup[i].live = 1;
		fishgroup[i].hei = rand() % (WindowY * 8 / 100) + (WindowY * 2 / 100);
		fishgroup[i].wid = rand() % (WindowX * 8 / 100) + (WindowY * 2 / 100);
		fishgroup[i].x = rand() % WindowX;
		fishgroup[i].y = rand() % WindowY;
		fishgroup[i].life = rand()%80+20;
		fishgroup[i].gold = fishgroup[i].hei*fishgroup[i].wid * 1 / 100 + 1;
		fishgroup[i].color.red = rand() % 256;
		fishgroup[i].color.green = rand() % 256;
		fishgroup[i].color.blue = rand() % 256;

		bulletgroup[i].direc = cannon.direc;
		bulletgroup[i].harm = cannon.power;
		bulletgroup[i].live = 0;
		bulletgroup[i].r = WindowY * 2 / 100;
		bulletgroup[i].x = cannon.x + cannon.len*cos(cannon.direc);
		bulletgroup[i].y = cannon.y + cannon.len*sin(cannon.direc);
		bulletgroup[i].color.red = rand() % 256;
		bulletgroup[i].color.green = rand() % 256;
		bulletgroup[i].color.blue = rand() % 256;

	}
	GuideFace();
}
void CheckCrash()
{
	for (int i = 0; i < NUM; i++)
	{
		if(fishgroup[i].live==1)
		{
			for (int j = 0; j < NUM; j++)
			{
				if(bulletgroup[j].live==1)
				{
					if(bulletgroup[j].x>fishgroup[i].x&&bulletgroup[j].x<fishgroup[i].x+fishgroup[i].wid&&\
					bulletgroup[j].y>fishgroup[i].y&&bulletgroup[j].y<fishgroup[i].y+fishgroup[i].hei)
					{
						bulletgroup[j].live=0;
						fishgroup[i].life-=bulletgroup[j].harm;
						if(fishgroup[i].life<=0)
						fishgroup[i].live=0;
						Score += 1;
						Coin += fishgroup[i].gold;
						if (Coin>100)
						{
							Gold++;
							Coin = Coin - 100;
						}
						if (Gold>100)
						{
							Jewel++;
							Gold = Gold - 100;
						}
						if (Score>100)
						{
							Level++;
							Score = Score - 100;
						}
							
					}
				}
			}
		}
	}
}
void CreateFish()
{
	int count=0;
	for (int i = 0; i < NUM; i++)
	{
		if(fishgroup[i].live==0)
		{
			count++;
			fishgroup[i].life = rand() % 80 + 20;
			fishgroup[i].live = 1;
			fishgroup[i].hei = rand() % (WindowY * 8 / 100) + (WindowY * 2 / 100);
			fishgroup[i].wid = rand() % (WindowX * 8 / 100) + (WindowY * 2 / 100);
			fishgroup[i].x =WindowX-2*fishgroup[i].wid ;
			fishgroup[i].y = rand()%WindowY;
			fishgroup[i].gold = fishgroup[i].hei*fishgroup[i].wid * 1 / 100 + 1;
			fishgroup[i].color.red = rand() % 256;
			fishgroup[i].color.green = rand() % 256;
			fishgroup[i].color.blue = rand() % 256;
		}
		if(count==3)
			break;
	}
}
void CreateBullet()
{
	int count=0;
	for (int i = 0; i < NUM; i++)
	{
		if(bulletgroup[i].live==0)
		{
			count++;
			bulletgroup[i].live=0;
			bulletgroup[i].harm=cannon.power;
			bulletgroup[i].direc=cannon.direc;
			bulletgroup[i].x = cannon.x + (cannon.len + WindowY * 2 / 100)*cos(cannon.direc);
			bulletgroup[i].y = cannon.y + (cannon.len + WindowY * 2 / 100)*sin(cannon.direc);
			bulletgroup[i].color.red = rand() % 256;
			bulletgroup[i].color.green = rand() % 256;
			bulletgroup[i].color.blue = rand() % 256;
		}
		if(count==3)
			break;
	}
}
void MoveFish()
{
		for (int i = 0; i < NUM; i++)
		{
			if (fishgroup[i].live == 1)
			{
				int tp = (WindowY * 2 / 1000);
				if (tp <= 0)
					tp = 1;
				fishgroup[i].y += (rand() % 3 - 1);
				fishgroup[i].x -= tp;
			}
			if (fishgroup[i].x + fishgroup[i].wid<0 || fishgroup[i].x - fishgroup[i].wid>WindowX\
				|| fishgroup[i].y + fishgroup[i].hei<0 || fishgroup[i].y - fishgroup[i].hei>WindowY)
				fishgroup[i].live = 0;
		}

}
void MoveBullet()
{
		for (int i = 0; i < NUM; i++)
		{
			if (bulletgroup[i].live == 1)
			{
				int tp = WindowY * 3 / 1000;
				if (tp <= 2)
					tp = 2;
				bulletgroup[i].y += (tp)*sin(bulletgroup[i].direc);
				
				bulletgroup[i].x += (tp)*cos(bulletgroup[i].direc);
			}
			if (bulletgroup[i].x + bulletgroup[i].r<0 || bulletgroup[i].x - bulletgroup[i].r>WindowX\
				|| bulletgroup[i].y + bulletgroup[i].r<0 || bulletgroup[i].y - bulletgroup[i].r>WindowY)
				bulletgroup[i].live = 0;
		}
	
}
void MoveCannon()
{
	MOUSEMSG msg;
	int shoot = 0;
	while (MouseHit())
	{
		msg = GetMouseMsg();
		cannon.direc = atan2(msg.y - cannon.y*1.0,msg.x - cannon.x*1.0);
		if (msg.uMsg == WM_LBUTTONDOWN)
			shoot = 1;
	}
	char ch = '\0';
	if (_kbhit())
		ch=_getch();
	if (ch == ' ')
		shoot = 1;
	if (shoot == 1)
	{
		for (int i = 0; i < NUM; i++)
		{
			if (bulletgroup[i].live == 0)
			{
				bulletgroup[i].x = cannon.x;
				bulletgroup[i].y = cannon.y;
				bulletgroup[i].r = WindowY * 2 / 100;
				bulletgroup[i].harm = cannon.power;
				bulletgroup[i].direc = cannon.direc;
				bulletgroup[i].live = 1;
				break;
			}
		}
	}
}
void DrawFace()
{
	BeginBatchDraw();
	setfillcolor(RGB(112,196,252));
	solidrectangle(0, 0, WindowX, WindowY);
	setfillcolor(RGB(103, 224, 197));
	solidrectangle(WindowX, 0, GameWindowX, GameWindowY);
	putimage(0, 0, &bgimage);
	int i=0;
	for (i = 0; i < NUM; i++)
	{
		if (fishgroup[i].live == 1)
		{
			setfillcolor(RGB(fishgroup[i].color.red, fishgroup[i].color.green, fishgroup[i].color.blue));
			solidellipse(fishgroup[i].x, fishgroup[i].y, fishgroup[i].x + fishgroup[i].wid, fishgroup[i].y + fishgroup[i].hei);
		}
	}
	for (i = 0; i < NUM; i++)
	{
		if (bulletgroup[i].live == 1)
		{
			setfillcolor(RGB(bulletgroup[i].color.red, bulletgroup[i].color.green, bulletgroup[i].color.green));
			solidcircle(bulletgroup[i].x, bulletgroup[i].y, bulletgroup[i].r);
		}
	}
	setfillcolor(RGB(cannon.color.red,cannon.color.green,cannon.color.blue));
	solidcircle(cannon.x,cannon.y,cannon.r);
	setlinecolor(RGB(cannon.color.red, cannon.color.green, cannon.color.blue));
	setlinestyle(PS_SOLID|PS_ENDCAP_ROUND,WindowY*2/100);
	line(cannon.x,cannon.y,cannon.x+cannon.len*cos(cannon.direc),cannon.y+cannon.len*sin(cannon.direc));
	settextstyle(FontWid, FontHei, "黑体");
	char temp[40];
	outtextxy(WindowX + (GameWindowX - WindowX)*0.3, WindowY * 1 / 12, "Score");
	sprintf_s(temp, "%d\0", Score);
	outtextxy(WindowX + (GameWindowX - WindowX)*0.3, WindowY * 2 / 12, temp);

	outtextxy(WindowX + (GameWindowX - WindowX)*0.3, WindowY * 3 / 12, "Level");
	sprintf_s(temp, "%d\0", Level);
	outtextxy(WindowX + (GameWindowX - WindowX)*0.3, WindowY * 4 / 12, temp);

	outtextxy(WindowX + (GameWindowX - WindowX)*0.3, WindowY * 5 / 12, "Jewel");
	sprintf_s(temp, "%d\0", Jewel);
	outtextxy(WindowX + (GameWindowX - WindowX)*0.3, WindowY * 6 / 12, temp);

	outtextxy(WindowX + (GameWindowX - WindowX)*0.3, WindowY * 7 / 12, "Gold");
	sprintf_s(temp, "%d\0", Gold);
	outtextxy(WindowX + (GameWindowX - WindowX)*0.3, WindowY * 8 / 12, temp);

	outtextxy(WindowX + (GameWindowX - WindowX)*0.3, WindowY * 9 / 12, "Coin");
	sprintf_s(temp, "%d\0", Coin);
	outtextxy(WindowX + (GameWindowX - WindowX)*0.3, WindowY * 10 / 12, temp);

	outtextxy(WindowX + (GameWindowX - WindowX)*0, WindowY * 11 / 12, "Ice2Faith");
	FlushBatchDraw();
	EndBatchDraw();
}
