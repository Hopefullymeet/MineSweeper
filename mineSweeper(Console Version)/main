#include <stdio.h>
#include <stdlib.h>

//常量定义 最大行数24行，最大列数30列
#define maxRows 25
#define maxColumns 31

//定义全局变量
int level, N, M, C, cnt = 0, gameOver = 0, markNum, winMark = 0;  //变量依次为难度等级、行数、列数、雷数、操作次数计数器、游戏是否结束、剩余的标记旗数量、是否胜利的标记
char board[maxRows][maxColumns], showBoard[maxRows][maxColumns],memoryBoard[maxRows][maxColumns];  //变量依次为读入的原始棋盘和展示棋盘

void readBoard();  //读入棋盘
void formShowBoard();  //创建初始显示棋盘
void display(char displayBoard[maxRows][maxColumns]);  //展示棋盘的函数
void operation();  //操作函数
void expand(int x, int y);  //展开棋盘 变量依次为 用户所输入的棋盘、所要展示的棋盘、操作对象的坐标
int checkWin();  //检查是否获胜
int countMines(int x, int y);  //检查并返回周围九个格子的剩余雷数

int main()
{
    scanf("%d", &level);  //读入难度等级
    getchar();  //消除缓冲区的换行符
    if(level == 1) N = 9, M = 9, C = 10;
    else if(level == 2) N = 16 , M = 16, C = 40;
    else if(level == 3) N = 16, M = 30, C = 99;
    else if(level == 4)
    {
        scanf("%d%d%d", &N, &M, &C);
        getchar();
    }
    markNum = C;
    readBoard(),formShowBoard();  //初始化
    while(gameOver != 1)
    {
        operation();
        winMark = checkWin();
        if(winMark == 1)
        {
            printf("Game over, you win\n");
            gameOver = 1;
        }
        else if(winMark != 1 && gameOver != 1) printf("Game in progress\n");  //游戏未结束且未获胜时打印的内容
        else if(winMark != 1 && gameOver == 1) printf("Hit mine, you lose\n");  //游戏结束但未获得胜利时打印的内容
        printf("%d %d\n", cnt, markNum);  //展示操作次数与剩余标记旗数量
        if(gameOver != 1) 
        {
            display(showBoard);
        }

        //踩到雷后打印的棋盘
        else if(winMark != 1)
        {
            for(int i = 0; i < N; i++) for(int j = 0; j < M; j++)
            {
                if(board[i][j] == '*') showBoard[i][j] = board[i][j];
            }
            display(showBoard);
        }

        //取胜后打印完整原始棋盘
        else
        {
            for(int i = 0; i < N; i++) for(int j = 0; j < M; j++)
            {
                if(board[i][j] == '*') showBoard[i][j] = board[i][j];
            }
            display(showBoard);
        }
    }

    return 0;
}

void readBoard()
{
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < M; j++)
        {
            scanf("%c",&board[i][j]);
        }
        getchar(); //消除缓冲区的换行符
    }
}

void formShowBoard()
{
    for(int i = 0; i < N; i++) for(int j = 0; j < M; j++) showBoard[i][j] = '.';  //将展示板块所有的格子都填上“.”
}

void display(char displayBoard[maxRows][maxColumns])
{
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < M; j++)
           printf("%c", displayBoard[i][j]);
        printf("\n");
    }
}

int countMines(int x, int y)
{
    int count = 0;
    for(int i = x - 1; i <= x + 1; i++) for(int j = y - 1; j <= y + 1; j++)
    {
        if(i >= 0 && i < N && j >= 0 && j < M && board[i][j] == '*')
          count++;
    }
    return count;
}

//翻格子函数expand，传入参数的x，y最小为0
void expand(int x, int y)
{
    if(showBoard[x][y] != '.' && showBoard[x][y] != '?') return;  //若单击的格子已经打开

    if(board[x][y] == '*')  //若单击后为炸弹
    {
        gameOver = 1;  //标记游戏已经结束
        return;
    }

    showBoard[x][y] = countMines(x, y) + '0';  //打开该格子

    if(showBoard[x][y] == '0')  //若当前格子的数字为0，则展开周围9*9的格子
    {
        for(int i = x - 1; i <= x + 1; i++) for(int j = y - 1; j <= y + 1; j++)
        {
            if(i >= 0 && i < N && j >= 0 && j < M)
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
    for(int i = 0; i < N; i++) for(int j = 0; j < M; j++)
    {
        if(showBoard[i][j] == '!' || showBoard[i][j] == '?' || showBoard[i][j] == '.') t++;  //计算所有未被翻开的格子数量
    }
    if(t == C) return 1;  //若所有未被翻开的格子数量等于总雷数，则获胜
    else return 0;
}

void operation()
{
    cnt++;  //一旦调用operate函数，则操作次数计数器加一
    int operate,x,y;  //变量分别为操作类型和操作对象坐标
    scanf("%d%d%d", &operate, &x, &y);
    x -= 1, y -= 1;
  if(x >= 0 && x < N && y >= 0 && y < M)  //判断输入坐标是否合法
  {
    if(operate == 1)  //操作类型为左键单击
    {
        expand(x, y);
    }
    else if(operate == 2)  //操作类型为标记地雷，则“！”
    {
        if(showBoard[x][y] == '?' || showBoard[x][y] == '.')
        {
           showBoard[x][y] = '!';
           markNum--;  //减少现有的标记旗数
        }
    }
    else if(operate == 3)  //操作类型为标记疑问，即“？”
    {
        if(showBoard[x][y] == '.') showBoard[x][y] = '?';
        if(showBoard[x][y] == '!')
        {
            showBoard[x][y] = '?';
            markNum++;
        }
    }
    else if(operate == 4)  //操作类型为取消标记地雷
    {
        if(showBoard[x][y] == '!')
        {
            showBoard[x][y] = '.';
            markNum++;  //取消标记后恢复标记旗数
        }
        else if(showBoard[x][y] = '?')
        {
            showBoard[x][y] = '.';
        }
    }
    
    //操作类型为双击，且保证双击的格子已经被翻开
    else if(operate == 9 && (showBoard[x][y] != '!' && showBoard[x][y] != '?' && showBoard[x][y] != '.'))
    {
        int t = 0;  //计算双击格子周围九格内的标记旗数量
        for(int i = x - 1; i <= x + 1; i++) for(int j = y - 1; j <= y + 1 ; j++)
        {
            if(showBoard[i][j] == '!') t++;
        }
        if(showBoard[x][y] == t + '0')
        {
            //将原有的展示板储存在记忆棋盘内
            for(int i = 0; i <= N; i++) for(int j = 0; j <= M; j++)
            {
                if(i >= 0 && i < N && j >= 0 && j < M)
                  memoryBoard[i][j] = showBoard[i][j];
            }

            for(int i = x - 1; i <= x + 1; i++) for(int j = y - 1; j <= y + 1; j++)
            {
                if(i >= 0 && i < N && j >= 0 && j < M)
                  expand(i, j);
            }

            //双击踩雷后将记忆棋盘写入展开后的展示板
            if(checkWin() != 1 && gameOver == 1)
            {
                for(int i = 0; i < N; i++) for(int j = 0; j < M; j++)
                {
                    showBoard[i][j] = memoryBoard[i][j];
                }
            }
        }
    }
  }
}
