
#include<stdio.h>
#include<graphics.h>//图形库
#include<conio.h>//
#include<time.h>//包含clock()函数

enum My
{
	WIDTH = 480,//窗口宽高
	HEIGHT = 700,

	BULLET_NUM = 15,//玩家子弹数量

	ENEMY_NUM=10 ,//敌机数量

	BIG,
	SMALL
};


struct Plane//飞机
{
	int x;
	int y;
	bool live;
	int width;
	int height;
	int hp;
	int type;//敌机类型 big small

}player, bullet[BULLET_NUM],enemy[ENEMY_NUM];//定义在;号之前



IMAGE bk;        //="./images/background.jpg"; 不存在这样的用法,不能直接用路径加载

IMAGE img_player[2];//保存玩家图片 player
IMAGE img_bullet[2];//保存子弹图片
IMAGE img_enemy[2][2];//两种敌机四张图片




void loadimg()//定义一个图像加载函数
{
	//加载图片
		//图片480*700 如何适应窗口高度?
	loadimage(&bk, "./images/background.png");   //可能会报错error2665"loadimage"两个重载中没有一个可转换所有参数类型
												//参数有误,字符集不对,Unicode和多字节字符集

	loadimage(&img_player[0], "./images/planeNormal_1.jpg");//飞机掩码图
	loadimage(&img_player[1], "./images/planeNormal_2.jpg");//飞机原图


	loadimage(&img_bullet[0], "./images/bullet2_b.jpg");//子弹掩码图
	loadimage(&img_bullet[1], "./images/bullet1_b.jpg");//子弹原图

	loadimage(&img_enemy[0][0], "./images/enemy_1.jpg");//敌机1掩码图
	loadimage(&img_enemy[0][1], "./images/enemy_2.jpg");//敌机1原图

	loadimage(&img_enemy[1][0], "./images/enemyPlane1.jpg");//敌机2掩码图
	loadimage(&img_enemy[1][1], "./images/enemyPlane2.jpg");//敌机2原图


}

void enemyHp(int i) 
{


	//这里简单介绍一下其工作原理。求余数运算符（％）可以获得整除之后的余数。
		//eg.	dice = rand() % 6 + 1;
		//		当使用通过 rand 函数返回的正整数除以6时，余数将是 0〜5 的数字。
	if (rand()%10==0)//产生0~9的随机数
		
	{
		enemy[i].type = BIG;
		enemy[i].hp = 3;
		enemy[i].width = 104;
		enemy[i].height = 148;
	}

	else
	{
		enemy[i].type = SMALL;
		enemy[i].hp = 1;
		enemy[i].width = 52;
		enemy[i].height = 39;
	}
}

void gameinit()
{
	player.x = WIDTH / 2;
	player.y = HEIGHT - 120;
	player.live = true;


	//初始化子弹
	for (int i = 0; i < BULLET_NUM; i++)//子弹不止一个，for循环来生成
	{
		bullet[i].x = 0;
		bullet[i].y = 0;
		bullet[i].live=false;
	}


	//初始化敌机
	for (INT i = 0; i < ENEMY_NUM; i++)
	{
		enemy[i].live=false;
		enemyHp(i);
	}
}



void gameDraw()
{
	loadimg();//也只进行一次

	putimage(0, 0, &bk);//(x轴,y轴)
	putimage(player.x, player.y, &img_player[0], NOTSRCERASE);//传入第三个参数NOTSRCERASE/SRCINVERT分别做
	putimage(player.x, player.y, &img_player[1], SRCINVERT);

	//绘制子弹
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bullet[i].live)
		{
			putimage(bullet[i].x, bullet[i].y, &img_bullet[0], NOTSRCERASE);//传入第三个参数NOTSRCERASE/SRCINVERT分别做
			putimage(bullet[i].x, bullet[i].y, &img_bullet[1], SRCINVERT);

		}

	}

	//绘制敌机
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if(enemy[i].live)
		{
			if (enemy[i].type == BIG)
			{
				putimage(enemy[i].x, enemy[i].y, &img_enemy[1][0], NOTSRCERASE);
				putimage(enemy[i].x, enemy[i].y, &img_enemy[1][1], SRCINVERT);
				//大飞机在第二组
			}
			else
			{
				putimage(enemy[i].x, enemy[i].y, &img_enemy[0][0], NOTSRCERASE);
				putimage(enemy[i].x, enemy[i].y, &img_enemy[0][1], SRCINVERT);
			}
		}
	}


}


void creat_bullet() {
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (!bullet[i].live)
		{
			bullet[i].x = player.x + 60;
			bullet[i].y = player.y;
			bullet[i].live = true;
			break;
		}

	}
}



void BulletMove(int speed) 
{
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bullet[i].live)
		{

			bullet[i].y -= speed;
			if (bullet[i].y < 0)
			{
				bullet[i].live = false;
			}


		}
	}
}

/// bx为子弹x坐标，by为子弹y坐标
/// 当bx>x&&by>y
/// 且bx<x+PWidth&&by<y时作碰撞
void playPlance()
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (!enemy[i].live)
		
		continue;
		for (int k = 0; k < BULLET_NUM; k++)
		{
			if (!bullet[k].live)
			continue;
			
			if (bullet[k].x > enemy[i].x && bullet[k].x < enemy[i].x + enemy[i].width
				&& bullet[k].y <= enemy[i].y + enemy[i].height)
			{
				bullet[i].live = false;
				enemy[i].hp--;
			}
			
		}

		if (enemy[i].hp<=0)
		{
			enemy[i].live = false;
		}




	}


}
bool Timer(int ms, int id)//注意c语言中是没有bool类型的
{
	static DWORD t[10];//静态变量是自动初始化为0的
	if (clock() - t[id] > ms)
	{

		t[id] = clock();
		return true;//为什么这一行要写在t1=t2后呢？
					//执行return 语句后，函数就返回了，并不会执行return后面的语句。
					//这样就没法让t1=t2了

	}


	return false;//注意bool类型不要值返回一种就不管了，可能会有默认值为不为0都是真
}


void playerMove(int speed)//角色移动需要获取键盘输入
{
#if 0 




	//检测是否有键盘按下，如果有，返回true，else false
	if (_kbhit())//键值检测放入这个函数，避免了阻塞
	{
		//两种方式
		//1._getch()函数,阻塞函数,和scanf一样,没有输入就会卡主程序,一直等待输入,注意这个函数不是c语言标准函数,第三方函数,mscv特有,需要头文件conio.h

		char key = _getch();
		switch (key)
		{

		case'W':
		case'w':
			player.y -= speed;
			break;

		case'A':
		case'a':
			player.x -= speed;
			break;

		case'S':
		case's':
			player.y += speed;
			break;

		case'D':
		case'd':
			player.x += speed;
			break;
		}

	}


#elif 1
	//介绍第2种方法，利用Windows函数获取键盘输入
	//GetAsyncKeyState()函数非阻塞异步函数会非常流畅,但注意字母只能检测大写
	if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W'))//即方向上↑键
	{
		if (player.y > 0)//边界处理,大于这个值才能动
		{
			player.y -= speed;
		}

	}


	if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState('S'))//即方向↓键
	{
		if (player.y < HEIGHT - 60)//边界处理,大于这个值才能动
		{
			player.y += speed;
		}
	}

	if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D'))//即方向→键
	{
		if (player.x < WIDTH - 58)//边界处理,大于这个值才能动
		{
			player.x += speed;
		}

	}

	if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A'))//即方向←键
	{
		if (player.x + 60 > 0)//边界处理,大于这个值才能动
		{
			player.x -= speed;
		}

	}


#endif // 0



	if (GetAsyncKeyState(VK_SPACE)&&Timer(100,1))//100ms
	{

		creat_bullet();//创建一个子弹
	
	}
	
	//GetTickCount返回从操作系统启动所经过（elapsed）的毫秒数，它的返回值是DWORD。
}

//产生敌机
void createEnemy()
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (!enemy[i].live)
		{
			enemy[i].live = true;
			
			enemy[i].x = rand() % (WIDTH - 60);
			enemy[i].y = 0;
			enemyHp(i);//注意重置Hp不至于陷入Hp<=0判断的死循环
			break;

		}
	}
}


void enemyMove(int speed)
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].live)
		{

			enemy[i].y += speed;
			if (enemy[i].y > HEIGHT)
			{
				enemy[i].live = false;

			}

		}
	}
}





int main()
{

	initgraph(WIDTH, HEIGHT,SHOWCONSOLE);
	gameinit();

	//利用easyx的双缓冲绘图,BeginBatchDraw();FlushBatchDraw();EndBatchDraw();函数解决可能的闪屏问题

	BeginBatchDraw();

	while (1)
	{

		gameDraw();
		FlushBatchDraw();//需要时刷新

		playerMove(5);//注意要传进参数speed的值
		//printf("Hello\n");//注意图形窗口输出不了printf,应该

		BulletMove(3);
		
		if (Timer(500,0)) 
		{
			createEnemy();
	
		}
		
		

		if (Timer(20, 2))
		{
			enemyMove(1);
		}


		playPlance();
	}
	EndBatchDraw();
	
	return 0;
}