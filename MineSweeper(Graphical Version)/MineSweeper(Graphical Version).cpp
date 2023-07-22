#define _CRT_SECURE_NO_WARNINGS 1

//常量定义 最大行数24行，最大列数30列
#define maxRows 25
#define maxColumns 31
#define picSize 35

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <graphics.h>
#include <conio.h>
#include <Windows.h>

//定义全局变量
int level, N, M, C, cnt = 0, gameOver = 0, markNum, winMark = 0, x1 = 0, y1 = 0, playAgain = 1, firstStepMine = 0;  //变量依次为难度等级、行数、列数、雷数、操作次数计数器、游戏是否结束、剩余的标记旗数量、是否胜利的标记,横坐标，纵坐标，是否再玩一把的标记，第一步踩雷的标记
char board[maxRows][maxColumns], showBoard[maxRows][maxColumns], memoryBoard[maxRows][maxColumns];  //变量依次为读入的原始棋盘和展示棋盘
clock_t startTime = 0, nowTime = 0;
IMAGE pic[13];
IMAGE bkPic[10];

void game();  //游戏函数
void readBoard();  //读入棋盘
void formShowBoard();  //创建初始显示棋盘
void display();  //展示棋盘的函数
void operation();  //操作函数
void expand(int x, int y);  //展开棋盘 变量依次为 用户所输入的棋盘、所要展示的棋盘、操作对象的坐标
int checkWin();  //检查是否获胜
int countMines(int x, int y);  //检查并返回周围九个格子的剩余雷数
void timeCounter();  //秒表函数
int mouseClick();  //鼠标操作函数（返回操作类型）
void showFlagNum();  //展示剩余标记旗数的函数
void textSmooth();  //字体抗锯齿函数（使字体更光滑）
void iniWelcomePage();  //初始化欢迎界面函数

int main()
{
    while (playAgain)
    {
        iniWelcomePage();
        game();
        int result = MessageBox(GetHWnd(), _T("你想要再玩一局吗？"), _T("提示"), MB_YESNO | MB_ICONINFORMATION);
        if (result == IDYES) playAgain = 1;
        else playAgain = 0;
    }

    closegraph();

    return 0;
}

void game()
{
    if (firstStepMine != 1)
    {
        while (1)
        {
            if (MouseHit())
            {
                MOUSEMSG msg = GetMouseMsg();
                if (msg.uMsg == WM_LBUTTONDOWN)
                {
                    if (msg.x >= 430 && msg.x <= 505 && msg.y >= 120 && msg.y <= 170)       //鼠标点击初级的情况
                    {
                        level = 1;
                        break;
                    }
                    else if (msg.x >= 430 && msg.x <= 505 && msg.y >= 220 && msg.y <= 270)      //鼠标点击中级的情况
                    {
                        level = 2;
                        break;
                    }
                    else if (msg.x >= 430 && msg.x <= 505 && msg.y >= 320 && msg.y <= 370)      //鼠标点击高级的情况
                    {
                        level = 3;
                        break;
                    }
                    if (msg.x >= 410 && msg.x <= 534 && msg.y >= 420 && msg.y <= 480)       //鼠标点击自定义的情况
                    {
                        level = 4;
                        break;
                    }
                }
            }
        }

        if (level == 1) N = 9, M = 9, C = 10;
        else if (level == 2) N = 16, M = 16, C = 40;
        else if (level == 3) N = 16, M = 30, C = 99;
        else if (level == 4)
        {
            char cN[3], cM[3], cC[4];
            InputBox(cN, 3, "请输入行数: ", "自定义雷区输入", NULL, 0, 0, FALSE);
            InputBox(cM, 3, "请输入列数: ", "自定义雷区输入", NULL, 0, 0, FALSE);
            InputBox(cC, 4, "请输入雷数: ", "自定义雷区输入", NULL, 0, 0, FALSE);
            N = atoi(cN);
            M = atoi(cM);
            C = atoi(cC);
        }
        cleardevice();
    }
    if (firstStepMine == 1)
    {
        firstStepMine = 0;
    }

    markNum = C;
    gameOver = 0, winMark = 0, cnt = 0;

    initgraph(M * picSize, N * picSize + 30);
    putimage(0, 0, &bkPic[0]);
    readBoard(), formShowBoard();  //初始化
    display();  //展示棋盘

    //初始时间展示
    int zero = 0;
    char cZeroTime[20];
    sprintf_s(cZeroTime, "时间: %03d S", zero);
    settextcolor(RED);
    settextstyle(25, 0, _T("微软雅黑"));
    outtextxy(0, 0, cZeroTime);

    while (gameOver != 1)
    {
        operation();
        winMark = checkWin();
        if (winMark == 1)
        {
            printf("Game over, you win\n");
            gameOver = 1;
        }
        else if (winMark != 1 && gameOver != 1) printf("Game in progress\n");  //游戏未结束且未获胜时打印的内容
        else if (winMark != 1 && gameOver == 1) printf("Hit mine, you lose\n");  //游戏结束但未获得胜利时打印的内容
        printf("%d %d\n", cnt, markNum);  //展示操作次数与剩余标记旗数量
        if (gameOver != 1)
        {
            display();
        }

        //踩到雷后打印的棋盘
        else if (winMark != 1)
        {
            if (cnt == 1)
            {
                firstStepMine = 1;
                game();
            }
            else 
            {
                for (int i = 0; i < N; i++) for (int j = 0; j < M; j++)
                {
                    if (board[i][j] == '*') showBoard[i][j] = board[i][j];
                }
                display();
                MessageBox(GetHWnd(), _T("踩到雷了，你输了!"), _T("游戏结束"), MB_OK | MB_ICONINFORMATION);
            }
        }

        //取胜后打印完整原始棋盘
        else
        {
            for (int i = 0; i < N; i++) for (int j = 0; j < M; j++)
            {
                if (board[i][j] == '*') showBoard[i][j] = board[i][j];
            }
            display();
            MessageBox(GetHWnd(), _T("找到所有雷，你赢了!"), _T("游戏结束"), MB_OK | MB_ICONINFORMATION);
        }
    }

    return;
}

void iniWelcomePage()
{
    //初始化图形界面
    initgraph(920, 657);

    loadimage(&bkPic[0], "./background.png");
    loadimage(&bkPic[1], "./junior.png");
    loadimage(&bkPic[2], "./mid.png");
    loadimage(&bkPic[3], "./senior.png");
    loadimage(&bkPic[4], "./self.png");

    putimage(0, 0, &bkPic[0]);
    putimage(430, 120, &bkPic[1]);
    putimage(430, 220, &bkPic[2]);
    putimage(430, 320, &bkPic[3]);
    putimage(410, 420, &bkPic[4]);

    textSmooth();

    char ctitle[10];
    sprintf(ctitle, "扫雷");
    settextcolor(RGB(250, 249, 222));
    settextstyle(80, 0, _T("宋体"));
    outtextxy(390, 0, ctitle);
}

void textSmooth()
{
    LOGFONT f;
    gettextstyle(&f);						
    f.lfHeight = 48;						
    _tcscpy(f.lfFaceName, _T("微软雅黑"));
    f.lfQuality = ANTIALIASED_QUALITY;		 
    settextstyle(&f);
}

void showFlagNum()
{
    char cFlagNum[20];
    sprintf(cFlagNum, "剩余雷数: %03d 个", markNum);
    settextcolor(RED);
    settextstyle(25, 0, _T("微软雅黑"));
    outtextxy(110, 0, cFlagNum);
}

void timeCounter()
{
        int elapsedTime = 0;        //时间差
        nowTime = clock();
        elapsedTime = int(nowTime - startTime) / CLOCKS_PER_SEC;
        char cTime[20];
        sprintf(cTime, "时间: %03d S", elapsedTime);
        settextcolor(RED);
        settextstyle(25, 0, _T("微软雅黑"));
        outtextxy(0, 0, cTime);
}

int mouseClick()
{
    int x = 0, y = 0, op = 0;
    while (1){
        if (cnt > 1)
        {
            timeCounter();
        }
        if (MouseHit()) {   // 有鼠标消息
            MOUSEMSG msg = GetMouseMsg();
            if (msg.uMsg == WM_LBUTTONDOWN)
            {
                x = (msg.y - 25) / picSize;
                y = msg.x / picSize;
                op = 1;
            }
            if (msg.uMsg == WM_RBUTTONDOWN)
            {
                x = (msg.y - 25) / picSize;
                y = msg.x / picSize;
                op = 2;
            }
            if (GetAsyncKeyState(VK_LBUTTON) & GetAsyncKeyState(VK_RBUTTON) & 0x8000)
            {
                x = (msg.y - 25) / picSize;
                y = msg.x / picSize;
                op = 9;
            }
            x1 = x;
            y1 = y;
            if (msg.uMsg == WM_MOUSEMOVE)
            {
                return 0;
            }
            break;
        }
    }
    return op;  //返回操作类型
}

void readBoard()
{
    int counter = 0;
    srand((unsigned)time(NULL));
    for (int i = 0; i < N; i++) for (int j = 0; j < M; j++)
    {
        board[i][j] = '.';
    }
    while (counter < C)
    {
        int x = rand() % N;
        int y = rand() % M;
        if (board[x][y] == '.')
        {
            board[x][y] = '*';
            counter++;
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++)
        {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void formShowBoard()
{
    for (int i = 0; i < N; i++) for (int j = 0; j < M; j++) showBoard[i][j] = '.';  //将展示板块所有的格子都填上“.”
}

void display()
{
    loadimage(&pic[0], "./originBlock.jpg");  //pic[0]代表原始格子
    loadimage(&pic[1], "./1.jpg");
    loadimage(&pic[2], "./2.jpg");
    loadimage(&pic[3], "./3.jpg");
    loadimage(&pic[4], "./4.jpg");
    loadimage(&pic[5], "./5.jpg");
    loadimage(&pic[6], "./6.jpg");
    loadimage(&pic[7], "./7.jpg");
    loadimage(&pic[8], "./8.jpg");
    loadimage(&pic[9], "./mine.jpg");  //pic[9]代表雷
    loadimage(&pic[10], "./flag.jpg");  //pic[10]代表旗子标记
    loadimage(&pic[11], "./questionMark.png");  //pic[11]表示问号标记
    loadimage(&pic[12], "./noneZone.png");  //pic[12]表示已经被翻开的格子
    for (int i = 0; i < N; i++) for (int j = 0; j < M; j++)
    {
        int dx, dy;
        dx = picSize * j;
        dy = picSize * i + 25;
        if (showBoard[i][j] == '.') putimage(dx, dy, &pic[0]);
        if (showBoard[i][j] == '0') putimage(dx, dy, &pic[12]);
        if (showBoard[i][j] == '1') putimage(dx, dy, &pic[1]);
        if (showBoard[i][j] == '2') putimage(dx, dy, &pic[2]);
        if (showBoard[i][j] == '3') putimage(dx, dy, &pic[3]);
        if (showBoard[i][j] == '4') putimage(dx, dy, &pic[4]);
        if (showBoard[i][j] == '5') putimage(dx, dy, &pic[5]);
        if (showBoard[i][j] == '6') putimage(dx, dy, &pic[6]);
        if (showBoard[i][j] == '7') putimage(dx, dy, &pic[7]);
        if (showBoard[i][j] == '8') putimage(dx, dy, &pic[8]);
        if (showBoard[i][j] == '*') putimage(dx, dy, &pic[9]);
        if (showBoard[i][j] == '!') putimage(dx, dy, &pic[10]);
        if (showBoard[i][j] == '?') putimage(dx, dy, &pic[11]);
    }
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
            printf("%c", showBoard[i][j]);
        printf("\n");
    }
}

int countMines(int x, int y)
{
    int count = 0;
    for (int i = x - 1; i <= x + 1; i++) for (int j = y - 1; j <= y + 1; j++)
    {
        if (i >= 0 && i < N && j >= 0 && j < M && board[i][j] == '*')
            count++;
    }
    return count;
}

//翻格子函数expand，传入参数的x，y最小为0
void expand(int x, int y)
{
    if (showBoard[x][y] != '.' && showBoard[x][y] != '?') return;  //若单击的格子已经打开

    if (board[x][y] == '*')  //若单击后为炸弹
    {
        gameOver = 1;  //标记游戏已经结束
        return;
    }

    showBoard[x][y] = countMines(x, y) + '0';  //打开该格子

    if (showBoard[x][y] == '0')  //若当前格子的数字为0，则展开周围9*9的格子
    {
        for (int i = x - 1; i <= x + 1; i++) for (int j = y - 1; j <= y + 1; j++)
        {
            if (i >= 0 && i < N && j >= 0 && j < M)
            {
                expand(i, j);
            }
        }
    }
}

//检查是否获胜函数
int checkWin()
{
    int t = 0;
    for (int i = 0; i < N; i++) for (int j = 0; j < M; j++)
    {
        if (showBoard[i][j] == '!' || showBoard[i][j] == '?' || showBoard[i][j] == '.') t++;  //计算所有未被翻开的格子数量
    }
    if (t == C) return 1;  //若所有未被翻开的格子数量等于总雷数，则获胜
    else return 0;
}

void operation()
{
    showFlagNum();
    cnt++;  //一旦调用operate函数，则操作次数计数器加一
    int operate = 0, x, y;  //变量分别为操作类型和操作对象坐标
    //scanf("%d%d%d", &operate, &x1, &y1);
    while (operate == 0)
    {
        operate = mouseClick();
    }

    if (cnt == 1) //第一次点击时开始计时
    {
        startTime = clock();
    }

    x = x1, y = y1;
    printf("x = %d, y = %d, operate = %d\n", x, y, operate);
    if (x >= 0 && x < N && y >= 0 && y < M)  //判断输入坐标是否合法
    {
        if (operate == 1)  //操作类型为左键单击
        {
            expand(x, y);
        }
        else if (operate == 2 && (showBoard[x][y] == '.' || showBoard[x][y] == '!' || showBoard[x][y] == '?'))  //操作类型为标记地雷，则“！”
        {
            if (showBoard[x][y] == '.')
            {
                showBoard[x][y] = '!';
                markNum--;  //减少现有的标记旗数
            }
            else if (showBoard[x][y] == '!')
            {
                showBoard[x][y] = '?';
                markNum++;
            }
            else if (showBoard[x][y] == '?')
            {
                showBoard[x][y] = '.';
            }
        }

        //操作类型为双击，且保证双击的格子已经被翻开
        else if (operate == 9 && (showBoard[x][y] != '!' && showBoard[x][y] != '?' && showBoard[x][y] != '.'))
        {
            int t = 0;  //计算双击格子周围九格内的标记旗数量
            for (int i = x - 1; i <= x + 1; i++) for (int j = y - 1; j <= y + 1; j++)
            {
                if (showBoard[i][j] == '!') t++;
            }
            if (showBoard[x][y] == t + '0')
            {
                //将原有的展示板储存在记忆棋盘内
                for (int i = 0; i <= N; i++) for (int j = 0; j <= M; j++)
                {
                    if (i >= 0 && i < N && j >= 0 && j < M)
                        memoryBoard[i][j] = showBoard[i][j];
                }

                for (int i = x - 1; i <= x + 1; i++) for (int j = y - 1; j <= y + 1; j++)
                {
                    if (i >= 0 && i < N && j >= 0 && j < M)
                        expand(i, j);
                }

                //双击踩雷后将记忆棋盘写入展开后的展示板
                if (checkWin() != 1 && gameOver == 1)
                {
                    for (int i = 0; i < N; i++) for (int j = 0; j < M; j++)
                    {
                        showBoard[i][j] = memoryBoard[i][j];
                    }
                }
            }
        }
    }
}
