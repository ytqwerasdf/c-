#include <stdlib.h>
#include <string.h>
#include <graphics.h>
#include <windows.h>
#include <conio.h>
#include <graphics.h>
#include <windows.h>
#include <stdio.h>
#define _CRT_SECURE_NO_WARNINGS
#pragma warning (disable: 4996)
#pragma comment(lib,"winmm.lib")

void initgamePicture();   //加载游戏图片
void SetPlayer();       //显示角色信息
void initPlayer();      //初始化游戏角色
void SetMap();         //加载游戏地图
char* intToString(int Number); //把整数转化成字符串
void Flour(int Number);//显示楼层
void playGame();          //开始游戏
void NewGame();        //新游戏
void SetItem();       //主菜单
void Change(int h, int temp);//根据楼层计算玩家的初始位置
void CaLv();                //计算等级
void Messages();           //游戏说明
void Music();
int Combat(int x);
int VS(int playHP, int playATT, int playDEF, int monHP, int monATT, int monDEF);

int playery, playerx, playerh;
char str[20] = "";
char str1[10] = "";
//地图1
int map[10][13][13];
IMAGE Wall, Ground, Green_Slime, Red_Slime, Blue_Cry, Red_Cry, Blue_Key, Yellow_Key,
Red_Med, Blue_Med, upLadder, downLadder, Small_Skull, Big_Skull, Small_Bat, Small_Wizard,
Blue_door, Yellow_door, Player, Message, Item, a, champion,monpic;
HWND hwnd;
struct gamerole
{
	char name[20] = "骑士";  //玩家名字
	int HP;	    //血量
	int MP;
	int DEF;	//防御
	int ATT;    //攻击
	int Lv;     //等级
	int Exp;    //经验
	int Num_Blue_Key; //蓝钥匙数量
	int Num_Yellow_Key;
}player;

struct monster    //怪物属性
{
	int HP;	    //血量
	int ATT;    //攻击
	int DEF;	//防御
	int Exp;    //经验
};
struct monster Green_Slime_Pro = { 50,16,7,50 };    //绿史莱姆属性    
struct monster Small_Bat_Pro = { 37, 18, 8, 100 };         //小蝙蝠属性
struct monster Small_Skull_Pro = { 50, 15, 9, 200 };   //小骷髅属性
struct monster Red_Slime_Pro = { 50,16 , 10, 300 };  //红史莱姆属性
struct monster Small_Wizard_Pro = { 100, 30, 12, 400 };//小巫师属性
struct monster Big_Skull_Pro = { 200,23 , 14, 500 };     //大骷髅属性


int main()
{
	Music();
	hwnd = initgraph(60 * 14, 60 * 13);
	initgamePicture();
	SetItem();
	while (1) {
		SetMap();
		SetPlayer();
		playGame();
	}

	return 0;
}

/*
*新的游戏
*/
void NewGame()
{
	initPlayer(); //初始化游戏角色信息
	FILE* fp;
	fp = fopen("map.txt", "r");//读取保存在map.txt中的地图
	for (int k = 0; k < 10; k++)
		for (int i = 0; i < 13; i++)
			for (int j = 0; j < 13; j++)
				fscanf(fp, "%d", &map[k][i][j]);
	fclose(fp);
}
/*
*继续游戏
*/
void KeepGame()
{
	FILE* fp; //读取保存在keepmap.txt中的地图
	fp = fopen("keepmap.txt", "r");
	for (int k = 0; k < 10; k++)
		for (int i = 0; i < 13; i++)
			for (int j = 0; j < 13; j++)
				fscanf(fp, "%d", &map[k][i][j]);

	fscanf(fp, "%d", &playerh);
	fscanf(fp, "%d", &playery);
	fscanf(fp, "%d", &playerx);

	fscanf(fp, "%s", &player.name);
	fscanf(fp, "%d", &player.HP);
	fscanf(fp, "%d", &player.MP);
	fscanf(fp, "%d", &player.DEF);
	fscanf(fp, "%d", &player.ATT);
	fscanf(fp, "%d", &player.Lv);
	fscanf(fp, "%d", &player.Exp);
	fscanf(fp, "%d", &player.Num_Blue_Key);
	fscanf(fp, "%d", &player.Num_Yellow_Key);
	fclose(fp);
}
/*
*保存游戏
*/
void SaveGame()
{
	FILE* fp;
	fp = fopen("keepmap.txt", "w");
	for (int k = 0; k < 10; k++) {
		for (int i = 0; i < 13; i++) {
			for (int j = 0; j < 13; j++)
				fprintf(fp, "%d ", map[k][i][j]);
			fprintf(fp, "\n");
		}
		fprintf(fp, "\n");
	}
	fprintf(fp, "%d\n", playerh);
	fprintf(fp, "%d\n", playery);
	fprintf(fp, "%d\n", playerx);
	fprintf(fp, "%s\n", player.name);
	fprintf(fp, "%d\n", player.HP);
	fprintf(fp, "%d\n", player.MP);
	fprintf(fp, "%d\n", player.DEF);
	fprintf(fp, "%d\n", player.ATT);
	fprintf(fp, "%d\n", player.Lv);
	fprintf(fp, "%d\n", player.Exp);
	fprintf(fp, "%d\n", player.Num_Blue_Key);
	fprintf(fp, "%d\n", player.Num_Yellow_Key);

	fclose(fp);
}

/*游戏开始的四个选项：
*n新的游戏
*j继续游戏
*c游戏说明
*e退出游戏
*/
void SetItem()
{
	putimage(0, 0, &Item);
	while (1) {
		char ch = _getch();
		switch (ch) {
		case 'N':                //新的游戏
		case 'n':NewGame(); return;


		case 'J':					//继续游戏
		case 'j':KeepGame(); return;


		case 'C':               //游戏说明
		case 'c': Messages();
			break;

		case 'E':     //退出游戏
		case 'e':exit(0); return;

		}
	}


}

//根据不同楼层显示角色的初始位置
void Change(int h, int temp)
{
	switch (h)
	{
	case 0:
		if (temp == 0) {
			playery = 1;
			playerx = 2;
			map[playerh][playery][playerx] = 99;
		}
		break;
	case 1:
		if (temp == 1) {
			playery = 11;
			playerx = 10;
			map[playerh][playery][playerx] = 99;
		}
		else {
			playery = 2;
			playerx = 1;
			map[playerh][playery][playerx] = 99;
		}
		break;
	case 2:
		if (temp == 1) {
			playery = 7;
			playerx = 2;
			map[playerh][playery][playerx] = 99;
		}
		else
		{
			playery = 8;
			playerx = 10;
			map[playerh][playery][playerx] = 99;
		}
		break;
	case 3:
		if (temp == 1) {
			playery = 11;
			playerx = 2;
			map[playerh][playery][playerx] = 99;
		}
		else
		{
			playery = 4;
			playerx = 11;
			map[playerh][playery][playerx] = 99;
		}
		break;
	case 4:
		if (temp == 1) {
			playery = 11;
			playerx = 5;

			map[playerh][playery][playerx] = 99;
		}
		else
		{
			playery = 1;
			playerx = 2;
			map[playerh][playery][playerx] = 99;
		}
		break;
	case 5:
		if (temp == 1) {
			playery = 6;
			playerx = 7;
			map[playerh][playery][playerx] = 99;
		}
		else
		{
			playery = 4;
			playerx = 7;
			map[playerh][playery][playerx] = 99;
		}
		break;
	case 6:
		if (temp == 1) {
			playery = 10;
			playerx = 6;
			map[playerh][playery][playerx] = 99;
		}
		else
		{
			playery = 2;
			playerx = 6;
			map[playerh][playery][playerx] = 99;
		}
		break;
	case 7:
		if (temp == 1) {
			playery = 3;
			playerx = 8;
			map[playerh][playery][playerx] = 99;
		}
		else
		{
			playery = 3;
			playerx = 4;
			map[playerh][playery][playerx] = 99;
		}
		break;
	case 8:
		if (temp == 1) {
			playery = 2;
			playerx = 11;
			map[playerh][playery][playerx] = 99;
		}
		else
		{
			playery = 10;
			playerx = 11;
			map[playerh][playery][playerx] = 99;
		}
		break;
	case 9:
		if (temp == 1) {
			playery = 10;
			playerx = 6;
			map[playerh][playery][playerx] = 99;
		}

		break;
	}
}
/*
*显示角色信息
*/
void SetPlayer()
{
	putimage(60 * 13, 0, &Message);
	outtextxy(60 * 13 + 12, 100, player.name);
	Flour(playerh + 1);          //算层数
	outtextxy(60 * 13 + 12, 0, str1); //打印层数
	outtextxy(60 * 13 + 12, 180, intToString(player.Lv));
	outtextxy(60 * 13 + 12, 235, intToString(player.Exp));
	outtextxy(60 * 13 + 12, 362, intToString(player.HP));
	outtextxy(60 * 13 + 12, 420, intToString(player.MP));
	outtextxy(60 * 13 + 12, 517, intToString(player.ATT));
	outtextxy(60 * 13 + 12, 567, intToString(player.DEF));
	outtextxy(60 * 13 + 12, 689, intToString(player.Num_Yellow_Key));
	outtextxy(60 * 13 + 12, 759, intToString(player.Num_Blue_Key));
}


void Music()
{
	PlaySound(TEXT("2.wav"), 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
}
/*
*   加载游戏图片
*/
void initgamePicture()
{
	loadimage(&Wall, "墙.jpg", 60, 60);
	loadimage(&Ground, "地板.jpg", 60, 60);
	loadimage(&Green_Slime, "绿史莱姆.jpg", 60, 60);
	loadimage(&Red_Slime, "红史莱姆.jpg", 60, 60);

	loadimage(&Blue_Cry, "蓝水晶.jpg", 60, 60);
	loadimage(&Red_Cry, "红水晶.jpg", 60, 60);

	loadimage(&Blue_Key, "蓝钥匙.jpg", 60, 60);
	loadimage(&Yellow_Key, "黄钥匙.jpg", 60, 60);

	loadimage(&Red_Med, "小红药水.jpg", 60, 60);
	loadimage(&Blue_Med, "小蓝药水.jpg", 60, 60);
	loadimage(&champion, "图鉴.jpg", 60, 60);

	loadimage(&upLadder, "上梯子.jpg", 60, 60);
	loadimage(&downLadder, "下梯子.png", 60, 60);
	loadimage(&Small_Bat, "小蝙蝠.jpg", 60, 60);
	loadimage(&Small_Wizard, "小巫师.jpg", 60, 60);
	loadimage(&Small_Skull, "骷髅兵.jpg", 60, 60);
	loadimage(&Big_Skull, "大骷髅兵.jpg", 60, 60);

	loadimage(&Blue_door, "蓝门.jpg", 60, 60);
	loadimage(&Yellow_door, "黄门.jpg", 60, 60);
	loadimage(&Player, "人.jpg", 60, 60);
	loadimage(&Message, "info.jpg");
	loadimage(&a, "游戏说明.png", 840, 780);
	loadimage(&monpic, "怪物图鉴.png", 840, 780);
	//loadimage(&a, "魔塔.jpeg", 840, 780);
	loadimage(&Item, "菜单.jpg", 840, 780);
}

/*
*初始化游戏角色
*/
void initPlayer()
{
	player.Lv = 1;
	player.ATT = 10;
	player.DEF = 10;
	player.Num_Blue_Key = 0;
	player.Num_Yellow_Key = 0;
	player.HP = 800;
	player.MP = 250;
	player.Exp = 0;
	playery = 11;
	playerx = 6;
	playerh = 0;
}

//整数转换为字符
void Flour(int Number)
{
	switch (Number)
	{
	case 1:
		strcpy(str1, "第一层");
		break;
	case 2:
		strcpy(str1, "第二层");
		break;
	case 3:
		strcpy(str1, "第三层");
		break;
	case 4:
		strcpy(str1, "第四层");
		break;
	case 5:
		strcpy(str1, "第五层");
		break;
	case 6:
		strcpy(str1, "第六层");
		break;
	case 7:
		strcpy(str1, "第七层");
		break;
	case 8:
		strcpy(str1, "第八层");
		break;
	case 9:
		strcpy(str1, "第九层");
		break;
	case 10:
		strcpy(str1, "第十层");
		break;
	}

}
char* intToString(int Number)
{
	int len = 0;

	if (Number == 0) {
		str[0] = '0';
		len++;
	}
	while (Number)
	{
		str[len++] = Number % 10 + '0';
		Number /= 10;
	}
	for (int i = 0; i < len / 2; i++) {
		char t = str[i];
		str[i] = str[len - i - 1];
		str[len - i - 1] = t;
	}
	str[len] = '\0';
	return str;
}

/*
*加载游戏地图
*
*/
void SetMap()
{
	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			switch (map[playerh][i][j])
			{

			case 0:
				putimage(j * 60, i * 60, &Wall);          //墙
				break;
			case 1:
				putimage(j * 60, i * 60, &Ground);        //地板
				break;
			case 2:
				putimage(j * 60, i * 60, &Blue_door);     //蓝门
				break;
			case 3:
				putimage(j * 60, i * 60, &Yellow_door);    //黄门
				break;
			case 4:
				putimage(j * 60, i * 60, &Blue_Cry);         //蓝水晶
				break;
			case 5:
				putimage(j * 60, i * 60, &Red_Cry);          //红水晶
				break;
			case 6:
				putimage(j * 60, i * 60, &Blue_Key);        //蓝钥匙
				break;
			case 7:
				putimage(j * 60, i * 60, &Yellow_Key);     //黄钥匙
				break;
			case 8:
				putimage(j * 60, i * 60, &Red_Med);          //红药水
				break;
			case 9:
				putimage(j * 60, i * 60, &Blue_Med);         //蓝药水
				break;
			case 10:
				putimage(j * 60, i * 60, &Small_Bat);        //小蝙蝠
				break;
			case 11:
				putimage(j * 60, i * 60, &Small_Wizard);     //小巫师
				break;
			case 12:
				putimage(j * 60, i * 60, &Small_Skull);      //小骷髅
				break;
			case 13:
				putimage(j * 60, i * 60, &Big_Skull);        //大骷髅
				break;
			case 14:
				putimage(j * 60, i * 60, &Green_Slime);      //绿史莱姆
				break;
			case 15:
				putimage(j * 60, i * 60, &Red_Slime);        //红史莱姆
				break;
			case 97:
				putimage(j * 60, i * 60, &downLadder);         //下楼梯子
				break;
			case 98:
				putimage(j * 60, i * 60, &upLadder);         //上楼梯子
				break;
			case 99:
				putimage(j * 60, i * 60, &Player);          //玩家
				break;
			case 100:
				putimage(j * 60, i * 60, &champion);
				break;
			}

		}
	}
}

int Combat(int x)
{
	int ID;
	switch (x) {
	case 10:
		ID = MessageBox(NULL, "小蝙蝠", "是否攻击？", MB_YESNO);
		if (ID == IDYES)
		{
			if (VS(player.HP, player.ATT, player.DEF, Small_Bat_Pro.HP, Small_Bat_Pro.ATT, Small_Bat_Pro.DEF)) {
				player.Exp += Small_Bat_Pro.Exp;
				CaLv();
				return 1;
			}
		}
		break;
	case 11:
		ID = MessageBox(NULL, "遇到小巫师", "是否攻击？", MB_YESNO);
		if (ID == IDYES)
		{
			if (VS(player.HP, player.ATT, player.DEF, Small_Wizard_Pro.HP, Small_Wizard_Pro.ATT, Small_Wizard_Pro.DEF)) {
				player.Exp += Small_Wizard_Pro.Exp;
				CaLv();
				return 1;
			}
		}
		break;
	case 12:
		ID = MessageBox(NULL, "遇到小骷髅", "是否攻击？", MB_YESNO);
		if (ID == IDYES)
		{
			if (VS(player.HP, player.ATT, player.DEF, Small_Skull_Pro.HP, Small_Skull_Pro.ATT, Small_Skull_Pro.DEF)) {
				player.Exp += Small_Skull_Pro.Exp;
				CaLv();
				return 1;
			}
		}
		break;
	case 13:
		ID = MessageBox(NULL, "遇到大骷髅", "是否攻击？", MB_YESNO);
		if (ID == IDYES)
		{
			if (VS(player.HP, player.ATT, player.DEF, Big_Skull_Pro.HP, Big_Skull_Pro.ATT, Big_Skull_Pro.DEF)) {
				player.Exp += Big_Skull_Pro.Exp;
				CaLv();
				return 1;
			}
		}
		break;
	case 14:
		ID = MessageBox(NULL, "遇到绿史莱姆", "是否攻击？", MB_YESNO);
		if (ID == IDYES)
		{
			if (VS(player.HP, player.ATT, player.DEF, Green_Slime_Pro.HP, Green_Slime_Pro.ATT, Green_Slime_Pro.DEF)) {
				player.Exp += Green_Slime_Pro.Exp;
				CaLv();
				return 1;
			}
		}
		break;
	case 15:
		ID = MessageBox(NULL, "遇到红史莱姆", "是否攻击？", MB_YESNO);
		if (ID == IDYES)
		{
			if (VS(player.HP, player.ATT, player.DEF, Red_Slime_Pro.HP, Red_Slime_Pro.ATT, Red_Slime_Pro.DEF)) {
				player.Exp += Red_Slime_Pro.Exp;
				CaLv();
				return 1;
			}
		}
		break;

	}
	return 0;
}
//计算等级
void CaLv()
{
	if (player.Exp >= player.Lv * 1000) {
		player.Exp -= player.Lv * 1000;
		player.Lv++;
		player.ATT += 1;
		player.DEF += 1;
		player.HP += 500;
	}

}

//游戏说明
void Messages()
{
	putimage(0, 0, &a);
	_getch();
	SetItem();
}
void monsterpic()
{
	SaveGame();
	putimage(0, 0, &monpic);
	_getch();
	KeepGame();
	SetMap();
	SetPlayer();
	playGame();
}

//计算战斗是否胜利
int VS(int playHP, int playATT, int playDEF, int monHP, int monATT, int monDEF)
{
	int loss = 0;
	int count = 0;
	if (playATT <= monDEF) {
		MessageBox(NULL, "", "打不过", MB_YESNO);
		return 0;
	}
	else {
		if (monATT <= playDEF) {
			loss = 0;
			return 1;
		}
		else {
			monHP -= (playATT - monDEF);
			while (monHP > 0) {
				monHP -= (playATT - monDEF);
				count++;
			}
			loss = count * (monATT - playDEF);
			if (loss >= player.HP) {
				MessageBox(NULL, "", "打不过", MB_YESNO);
				return 0;
			}
			else {
				player.HP -= loss;
				return 1;
			}
		}
	}

}



void playGame()
{
	while (1)
	{
		int ID;
		char ch = _getch();
		switch (ch) {
		case 'q':
		case 'Q':
			ID = MessageBox(NULL, "保存游戏", "是否保存游戏？", MB_YESNO);
			if (ID == IDYES)
			{
				SaveGame();
				SetItem();
			}
			break;
		case 'r':
		case 'R':
			monsterpic();
			break;
		case 'w':
		case 'W':
		case 72:
			if (map[playerh][playery - 1][playerx] == 1) {         //下一步是地板
				map[playerh][playery - 1][playerx] = 99;
				map[playerh][playery][playerx] = 1;
				playery--;
			}
			else if (map[playerh][playery - 1][playerx] == 6) {      //下一步是蓝钥匙
				player.Num_Blue_Key++;
				map[playerh][playery - 1][playerx] = 99;
				map[playerh][playery][playerx] = 1;
				playery--;
			}
			else if (map[playerh][playery - 1][playerx] == 7) {     //下一步是黄钥匙
				player.Num_Yellow_Key++;
				map[playerh][playery - 1][playerx] = 99;
				map[playerh][playery][playerx] = 1;
				playery--;
			}
			//下一步是怪物
			else if (map[playerh][playery - 1][playerx] == 10 || map[playerh][playery - 1][playerx] == 11 ||
				map[playerh][playery - 1][playerx] == 12 || map[playerh][playery - 1][playerx] == 13 ||
				map[playerh][playery - 1][playerx] == 14 || map[playerh][playery - 1][playerx] == 15)
			{
				int x = Combat(map[playerh][playery - 1][playerx]);
				if (x == 1) {
					map[playerh][playery - 1][playerx] = 99;
					map[playerh][playery][playerx] = 1;
					playery--;
				}
			}
			//红蓝药水
			else if (map[playerh][playery - 1][playerx] == 8 || map[playerh][playery - 1][playerx] == 9) {
				if (map[playerh][playery - 1][playerx] == 8)
					player.HP += 500;   //红色药水加500hp
				else
					player.HP += 200;  //蓝色药水加200hp
				map[playerh][playery - 1][playerx] = 99;
				map[playerh][playery][playerx] = 1;
				playery--;
			}
			//2:蓝门3:黄门
			else if (map[playerh][playery - 1][playerx] == 2 || map[playerh][playery - 1][playerx] == 3) {
				if (map[playerh][playery - 1][playerx] == 2 && player.Num_Blue_Key) {
					player.Num_Blue_Key--;
					map[playerh][playery - 1][playerx] = 99;
					map[playerh][playery][playerx] = 1;
					playery--;
				}
				if (map[playerh][playery - 1][playerx] == 3 && player.Num_Yellow_Key) {
					player.Num_Yellow_Key--;
					map[playerh][playery - 1][playerx] = 99;
					map[playerh][playery][playerx] = 1;
					playery--;
				}
			}
			//红蓝水晶
			//红水晶+1攻击
			//蓝水晶+1防御
			else if (map[playerh][playery - 1][playerx] == 4 || map[playerh][playery - 1][playerx] == 5) {
				if (map[playerh][playery - 1][playerx] == 4)
					player.DEF += 1;
				else if (map[playerh][playery - 1][playerx] == 5)
					player.ATT += 1;
				map[playerh][playery - 1][playerx] = 99;
				map[playerh][playery][playerx] = 1;
				playery--;
			}
			//下一步是获胜奖杯
			else if (map[playerh][playery - 1][playerx] == 100) {
				ID = MessageBox(NULL, "是否退出游戏", "you win ！！", MB_YESNO);
				if (ID == IDYES)
				{
					exit(0);
				}
				map[playerh][playery - 1][playerx] = 99;
				map[playerh][playery][playerx] = 1;
				playery--;
			}
			//下一步是向上的楼梯
			else if (map[playerh][playery - 1][playerx] == 98) {
				map[playerh][playery][playerx] = 1;
				playerh++;
				Change(playerh, 1);
			}

			//下一步是向下的楼梯
			else if (map[playerh][playery - 1][playerx] == 97) {
				map[playerh][playery][playerx] = 1;
				playerh--;
				Change(playerh, 0);
			}
			break;
		case 's':
		case 'S':
		case 80:
			if (map[playerh][playery + 1][playerx] == 1) {         //下一步是地板
				map[playerh][playery + 1][playerx] = 99;
				map[playerh][playery][playerx] = 1;
				playery++;
			}
			else if (map[playerh][playery + 1][playerx] == 6) {      //下一步是蓝钥匙
				player.Num_Blue_Key++;
				map[playerh][playery + 1][playerx] = 99;
				map[playerh][playery][playerx] = 1;
				playery++;
			}
			else if (map[playerh][playery + 1][playerx] == 7) {     //下一步是黄钥匙
				player.Num_Yellow_Key++;
				map[playerh][playery + 1][playerx] = 99;
				map[playerh][playery][playerx] = 1;
				playery++;
			}
			//下一步是怪物
			else if (map[playerh][playery + 1][playerx] == 10 || map[playerh][playery + 1][playerx] == 11 ||
				map[playerh][playery + 1][playerx] == 12 || map[playerh][playery + 1][playerx] == 13 ||
				map[playerh][playery + 1][playerx] == 14 || map[playerh][playery + 1][playerx] == 15)
			{
				int x = Combat(map[playerh][playery + 1][playerx]);
				if (x == 1) {
					map[playerh][playery + 1][playerx] = 99;
					map[playerh][playery][playerx] = 1;
					playery++;
				}
			}
			//红蓝药水
			else if (map[playerh][playery + 1][playerx] == 8 || map[playerh][playery + 1][playerx] == 9) {
				if (map[playerh][playery + 1][playerx] == 8)
					player.HP += 200;
				else
					player.HP += 500;
				map[playerh][playery + 1][playerx] = 99;
				map[playerh][playery][playerx] = 1;
				playery++;
			}
			//红蓝门
			else if (map[playerh][playery + 1][playerx] == 2 || map[playerh][playery + 1][playerx] == 3) {
				if (map[playerh][playery + 1][playerx] == 2 && player.Num_Blue_Key) {
					player.Num_Blue_Key++;
					map[playerh][playery + 1][playerx] = 99;
					map[playerh][playery][playerx] = 1;
					playery++;
				}
				if (map[playerh][playery + 1][playerx] == 3 && player.Num_Yellow_Key) {
					player.Num_Yellow_Key++;
					map[playerh][playery + 1][playerx] = 99;
					map[playerh][playery][playerx] = 1;
					playery++;
				}
			}
			//红蓝水晶
			//红水晶+1攻击
			//蓝水晶+1防御
			else if (map[playerh][playery + 1][playerx] == 4 || map[playerh][playery + 1][playerx] == 5) {
				if (map[playerh][playery + 1][playerx] == 4)
					player.DEF += 1;
				else if (map[playerh][playery + 1][playerx] == 5)
					player.ATT += 1;
				map[playerh][playery + 1][playerx] = 99;
				map[playerh][playery][playerx] = 1;
				playery++;
			}
			//下一步是获胜奖杯
			else if (map[playerh][playery + 1][playerx] == 100) {
				ID = MessageBox(NULL, "是否退出游戏", "you win ！！", MB_YESNO);
				if (ID == IDYES)
				{
					exit(0);
				}
				map[playerh][playery + 1][playerx] = 99;
				map[playerh][playery][playerx] = 1;
				playery++;
			}
			//下一步是向上楼梯
			else if (map[playerh][playery + 1][playerx] == 98) {
				map[playerh][playery][playerx] = 1;
				playerh++;
				Change(playerh, 1);
			}
			//下一步是向下楼梯
			else if (map[playerh][playery + 1][playerx] == 97) {
				map[playerh][playery][playerx] = 1;
				playerh--;
				Change(playerh, 0);
			}
			break;
		case 'a':
		case 75:
			if (map[playerh][playery][playerx - 1] == 1) {         //下一步是地板
				map[playerh][playery][playerx - 1] = 99;
				map[playerh][playery][playerx] = 1;
				playerx--;
			}
			else if (map[playerh][playery][playerx - 1] == 6) {      //下一步是蓝钥匙
				player.Num_Blue_Key++;
				map[playerh][playery][playerx - 1] = 99;
				map[playerh][playery][playerx] = 1;
				playerx--;
			}
			else if (map[playerh][playery][playerx - 1] == 7) {     //下一步是黄钥匙
				player.Num_Yellow_Key++;
				map[playerh][playery][playerx - 1] = 99;
				map[playerh][playery][playerx] = 1;
				playerx--;
			}
			//下一步是怪物
			else if (map[playerh][playery][playerx - 1] == 10 || map[playerh][playery][playerx - 1] == 11 ||
				map[playerh][playery][playerx - 1] == 12 || map[playerh][playery][playerx - 1] == 13 ||
				map[playerh][playery][playerx - 1] == 14 || map[playerh][playery][playerx - 1] == 15)
			{
				int x = Combat(map[playerh][playery][playerx - 1]);
				if (x == 1) {
					map[playerh][playery][playerx - 1] = 99;
					map[playerh][playery][playerx] = 1;
					playerx--;
				}
			}
			//红蓝药水
			else if (map[playerh][playery][playerx - 1] == 8 || map[playerh][playery][playerx - 1] == 9) {
				if (map[playerh][playery][playerx - 1] == 8)
					player.HP += 200;
				else
					player.HP += 500;
				map[playerh][playery][playerx - 1] = 99;
				map[playerh][playery][playerx] = 1;
				playerx--;
			}
			//红蓝门
			else if (map[playerh][playery][playerx - 1] == 2 || map[playerh][playery][playerx - 1] == 3) {
				if (map[playerh][playery][playerx - 1] == 2 && player.Num_Blue_Key) {
					player.Num_Blue_Key--;
					map[playerh][playery][playerx - 1] = 99;
					map[playerh][playery][playerx] = 1;
					playerx--;
				}
				if (map[playerh][playery][playerx - 1] == 3 && player.Num_Yellow_Key) {
					player.Num_Yellow_Key--;
					map[playerh][playery][playerx - 1] = 99;
					map[playerh][playery][playerx] = 1;
					playerx--;
				}
			}
			//红蓝水晶
			//红水晶+1攻击
			//蓝水晶+1防御
			else if (map[playerh][playery][playerx - 1] == 4 || map[playerh][playery][playerx - 1] == 5) {
				if (map[playerh][playery][playerx - 1] == 4)
					player.DEF += 1;
				else if (map[playerh][playery][playerx - 1] == 5)
					player.ATT += 1;
				map[playerh][playery][playerx - 1] = 99;
				map[playerh][playery][playerx] = 1;
				playerx--;
			}
			//下一步是奖杯
			else if (map[playerh][playery][playerx - 1] == 100) {
				ID = MessageBox(NULL, "是否退出游戏", "you win ！！", MB_YESNO);
				if (ID == IDYES)
				{
					exit(0);
				}
				map[playerh][playery][playerx - 1] = 99;
				map[playerh][playery][playerx] = 1;
				playerx--;
			}
			//下一步是向上楼梯
			else if (map[playerh][playery][playerx - 1] == 98) {
				map[playerh][playery][playerx] = 1;
				playerh++;
				Change(playerh, 1);
			}
			//下一步是向下楼梯
			else if (map[playerh][playery][playerx - 1] == 97) {
				map[playerh][playery][playerx] = 1;
				playerh--;
				Change(playerh, 0);
			}
			break;
		case 'd':
		case 'D':
		case 77:
			if (map[playerh][playery][playerx + 1] == 1) {         //下一步是地板
				map[playerh][playery][playerx + 1] = 99;
				map[playerh][playery][playerx] = 1;
				playerx++;
			}
			else if (map[playerh][playery][playerx + 1] == 6) {      //下一步是蓝钥匙
				player.Num_Blue_Key++;
				map[playerh][playery][playerx + 1] = 99;
				map[playerh][playery][playerx] = 1;
				playerx++;
			}
			else if (map[playerh][playery][playerx + 1] == 7) {     //下一步是黄钥匙
				player.Num_Yellow_Key++;
				map[playerh][playery][playerx + 1] = 99;
				map[playerh][playery][playerx] = 1;
				playerx++;
			}
			//下一步是怪物
			else if (map[playerh][playery][playerx + 1] == 10 || map[playerh][playery][playerx + 1] == 11 ||
				map[playerh][playery][playerx + 1] == 12 || map[playerh][playery][playerx + 1] == 13 ||
				map[playerh][playery][playerx + 1] == 14 || map[playerh][playery][playerx + 1] == 15)
			{
				int x = Combat(map[playerh][playery][playerx + 1]);
				if (x == 1) {
					map[playerh][playery][playerx + 1] = 99;
					map[playerh][playery][playerx] = 1;
					playerx++;
				}
			}
			//红蓝药水
			else if (map[playerh][playery][playerx + 1] == 8 || map[playerh][playery][playerx + 1] == 9) {
				if (map[playerh][playery][playerx + 1] == 8)
					player.HP += 200;
				else
					player.HP += 500;
				map[playerh][playery][playerx + 1] = 99;
				map[playerh][playery][playerx] = 1;
				playerx++;
			}
			//红蓝门
			else if (map[playerh][playery][playerx + 1] == 2 || map[playerh][playery][playerx + 1] == 3) {
				if (map[playerh][playery][playerx + 1] == 2 && player.Num_Blue_Key) {
					player.Num_Blue_Key--;
					map[playerh][playery][playerx + 1] = 99;
					map[playerh][playery][playerx] = 1;
					playerx++;
				}
				if (map[playerh][playery][playerx + 1] == 3 && player.Num_Yellow_Key) {
					player.Num_Yellow_Key--;
					map[playerh][playery][playerx + 1] = 99;
					map[playerh][playery][playerx] = 1;
					playerx++;
				}
			}
			//红蓝水晶
			//红水晶+1攻击
			//蓝水晶+1防御
			else if (map[playerh][playery][playerx + 1] == 4 || map[playerh][playery][playerx + 1] == 5) {
				if (map[playerh][playery][playerx + 1] == 4)
					player.DEF += 1;
				else if (map[playerh][playery][playerx + 1] == 5)
					player.ATT += 1;
				map[playerh][playery][playerx + 1] = 99;
				map[playerh][playery][playerx] = 1;
				playerx++;
			}
			//下一步是奖杯
			else if (map[playerh][playery][playerx + 1] == 100) {
				ID = MessageBox(NULL, "you win ！！\n        you are genius!!!\n     oh!it is you\n   why are you so smart??\n  oh,my ga!,", "是否退出游戏", MB_YESNO);
				if (ID == IDYES)
				{
					exit(0);
				}
				map[playerh][playery][playerx + 1] = 99;
				map[playerh][playery][playerx] = 1;
				playerx++;
			}
			//下一步是向上楼梯
			else if (map[playerh][playery][playerx + 1] == 98) {
				map[playerh][playery][playerx] = 1;
				playerh++;
				Change(playerh, 1);
			}
			//下一步是向下楼梯
			else if (map[playerh][playery][playerx + 1] == 97) {
				map[playerh][playery][playerx] = 1;
				playerh--;
				Change(playerh, 0);
			}
			break;
		}
		SetMap();  //重新显示地图
		SetPlayer(); //重新显示角色信息
	}
}
