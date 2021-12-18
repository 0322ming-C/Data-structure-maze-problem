#include<stdio.h>
#include<stdlib.h>

#define MAZE_SIZE 16
typedef struct PosType
{
	int i;				//行坐标
	int j;				//列坐标
}PosType;

typedef struct
{
	int ord;			//通道块在路径上的“序号”
	PosType seat;		//通道在迷宫中的“坐标位置”
	int di;				//从此通道块走向下一通道块“方向”
}SElemType;				//栈的元素类型

typedef struct LineStack
{
	SElemType Pos;		//路径信息
	struct LineStack* next;
}LineStack;

enum { None, EAST, SOUTH, WEST, NORTH };//枚举类型对应方向
typedef enum { FALSE, TRUE }Bool;		//布尔类型

char maze[MAZE_SIZE][MAZE_SIZE];//二维数组存储地图

void InitMaze();//初始化地图
int MazePath(char maze[MAZE_SIZE][MAZE_SIZE], PosType start, PosType end);//寻找路径
void FootPrint(PosType curpos);//留下通过足迹
int Pass(PosType curpos);//判断当前位置可通行
PosType NextPos(PosType curpos, int Direct);//转到下一位置
void MakePrint(PosType curpos);//留下不可通行的标记
void PrintMaze(char maze[MAZE_SIZE][MAZE_SIZE]);//打印地图

void Push(LineStack* head, SElemType c)			//入栈
{
	LineStack* Node = (LineStack*)malloc(sizeof(LineStack));
	Node->Pos = c;
	Node->next = head->next;
	head->next = Node;
}

void Pop(LineStack* head,SElemType* e)						//出栈
{
	LineStack* p = head->next;
	*e = p->Pos;
	head->next = p->next;
	free(p);
}

int StackEmpty(LineStack* s)					//判断栈是否为空，为空返回TURE
{
	if (s!=NULL&&s->next == NULL)
	{
		return TRUE;
	}
	else
		return FALSE;
}

void InitMaze()									//初始化地图
{
	FILE* fp;
  fp=fopen（"C：\\maze.txt"，"r+"）;//可自行更换路径
	if (!fp)
	{
		printf("打开失败。");
	}
	for (int i = 0; i < MAZE_SIZE; i++)
	{
		for (int j = 0; j < MAZE_SIZE; j++)
		{
			if (fp)
			{
				fscanf(fp, "%c", &maze[i][j]);	//读取文件内容
				if(maze[i][j]=='\n')			//抵消文件中的回车符
				{
					j--;
				}
			}
		}
	}
	if (fp)
	{
		fclose(fp);
	}
}

int MazePath(char maze[MAZE_SIZE][MAZE_SIZE], PosType start, PosType end)//寻找路径
{
	LineStack *S;
	S = (LineStack*)malloc(sizeof(LineStack));
	S->next = NULL;
	SElemType e;
	PosType curpos;
	int curstep;
	curpos = start;							//设定“当前位置”为入口位置
	curstep = 1;							//探索第一步
	do
	{
		if (Pass(curpos))					//当前位置可以通过，即是未曾走到过的通讯块
		{
			FootPrint(curpos);				//留下足迹
			e = { curstep, curpos, EAST };
			Push(S, e);						//计入路径
			if (curpos.i==end.i&& curpos.j == end.j)				//到达终点
			{
				printf(" 找到路径。");
				return TRUE;
			}
			curpos = NextPos(curpos, EAST);	//下一个位置是当前的东邻
			curstep++;						//探索下一步
		}
		else
		{
			if (!StackEmpty(S))
			{
				Pop(S,&e);
				while (e.di==NORTH&&!StackEmpty(S))
				{
					MakePrint(e.seat);
					Pop(S,&e);					//留下不能通过的标记，并退回一步
				}
				if (e.di<NORTH)
				{
					e.di++;					//改变方向
					Push(S, e);				//换下一个探索方向
					curpos = NextPos(e.seat,e.di);//设定当前位置是该新方向上的相邻块
				}
			}
		}
	} while (!StackEmpty(S));
	if (StackEmpty(S))						//判断无解
	{
		printf("\n无解。");
	}
	return FALSE;
}

void FootPrint(PosType curpos)//留下通过足迹
{
	maze[curpos.i][curpos.j] = '1';
}

int Pass(PosType curpos)//判断当前位置可通行
{
	if (maze[curpos.i][curpos.j]=='*'|| maze[curpos.i][curpos.j] == '1')//遇到障碍或不可通行标记返回FALSE
	{
		return FALSE;
	}
	else if(maze[curpos.i][curpos.j] == '0')//可通行返回TURE
	{
		return TRUE;
	}
}

PosType NextPos(PosType curpos,int Direct)//转向下一位置
{
	switch (Direct)
	{
	case EAST:
		curpos.i++;
		return curpos;break;
	case SOUTH:
		curpos.j++;
		return curpos; break;
	case WEST:
		curpos.i--;
		return curpos; break;
	case NORTH:
		curpos.j--;
		return curpos; break;
	}
}

void MakePrint(PosType curpos)//留下不可通过标记
{
	maze[curpos.i][curpos.j] = '!';
}

void PrintMaze(char maze[MAZE_SIZE][MAZE_SIZE])//打印地图
{
	for (int i = 0; i < MAZE_SIZE; i++)
	{
		for (int j = 0; j < MAZE_SIZE; j++)
		{
			printf("%c ", maze[i][j]);
		}
		printf("\n");
	}
}

int Check(int i, int j)//检查当前位置是否为障碍
{
	if (maze[i][j] == '*' || maze[i][j] == '1')
		return FALSE;
	else
		return TRUE;
}

void Input(PosType* location)//输入坐标
{
	scanf("%d%*c%d", &(*location).i, &(*location).j);
	if (!(Check((*location).i, (*location).j)))
	{
		printf(" 该位置不可通行，请重新输入坐标：");
		Input(location);
	}
}

int main()
{
	InitMaze();//初始化地图
	printf("初始地图\n");
	PrintMaze(maze);
	PosType start,end;//输入起点终点
	printf("\n 请输入起点坐标（如：0,0）：");
	Input(&start);//输入起点坐标
	printf("\n 请输入终点坐标（如：15,15）：");
	Input(&end);//输入终点坐标
	MazePath(maze, start, end);//寻找路径
	printf("\n");
	PrintMaze(maze);
}
