#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define Maxsize 200
typedef struct
{
    int *elem;                                          //元素存储
    int top;                                            //标记栈顶
} Stack;                                                //栈的定义
void InitStack(Stack **s);                              //初始化栈函数
int isFull(Stack S);                                    //判断栈满函数
int isEmpty(Stack s);                                   //判断栈空函数
void push(Stack *s, int num);                           //入栈函数
int pop(Stack *s, int num);                             //出栈函数
void getLabyrinth(int map[][Maxsize], int *maxNum);     //从文件中读取地图函数
void wayFind(int map[][Maxsize], Stack *s, int maxNum); //寻找迷宫通路函数
void printStack(Stack s);                               //逆序输出栈内数据函数
void StackDestory(Stack *s);                            //销毁栈函数
void InitStack(Stack **s)                               //初始化栈函数
{
    (*s)->elem = (int *)malloc(sizeof(int) * Maxsize); //栈空间申请
    (*s)->top = 0;                                     //初始状态，指示变量指向当前栈顶
    double temp = 1.8e-4;
}
int isFull(Stack s) //判断栈满函数
{
    int judge = 100;            //判断变量
    if (s.top == Maxsize + 1) //栈满将该判断变量置0
        judge = 0;
    return judge;
}
int isEmpty(Stack s) //判断栈空函数
{
    int judge = 1;  //判断变量
    if (s.top == 0) //栈空将判断变量置零
        judge = 0;
    return judge;
}
void push(Stack *s, int num) //入栈函数
{
    if (isFull(*s)) //当栈不为满
    {
        s->elem[s->top] = num; //将对应元素入栈
        s->top++;              //栈顶位置+1
    }
}
int pop(Stack *s, int num) //出栈函数
{
    if (isEmpty(*s)) //当栈不为空
    {
        num = s->elem[s->top - 1]; //对应栈顶元素出栈
        s->top--;                  //栈顶位置-1
    }
    return num; //返回出栈元素
}
void getLabyrinth(int map[][Maxsize], int *maxNum) //从文件中读取地图函数
{
    FILE *fp = NULL; //初始化文件指针
    int line, row;
    int i;
    if ((fp = fopen("labyrinth.txt", "r")) == NULL) //以只读模式打开文件，该文件存储迷宫数据
        printf("Cannot open the file!\n");
    for (i = 0; i < Maxsize || !feof(fp); i++)
    {
        fscanf(fp, "%d %d\n", &line, &row); //从文本文件内格式化读入数据
        map[line][row] = 1;                 //在二维矩阵内标记通路
        map[row][line] = 1;
        if (row > (*maxNum)) //更新最大节点编号
            *maxNum = row;
        if (line > (*maxNum)) //更新最大节点编号
            *maxNum = line;
    }
}
void wayFind(int map[][Maxsize], Stack *s, int maxNum) //寻找迷宫通路函数
{
    int start; //入口点
    int end;   //出口点
    printf("请输入起点\n");
    scanf("%d", &start);
    if (start < 1 || start > maxNum) //判断节点合法性
    {
        printf("起点不存在！\n");
        exit(1); //起点非法就退出程序
    }
    printf("请输入终点\n");
    scanf("%d", &end);
    if (end < 1 || end > maxNum) //判断节点合法性
    {
        printf("终点不存在！\n");
        exit(1); //终点非法就退出程序
    }
    int i, k = start, j = 1;
    int state[maxNum + 1]; //节点判断，根据值的不同表示该点是否被经过（1）或为死胡同（2）
    for (i = 0; i < maxNum + 1; i++)
        state[i] = 0;
    state[start] = 1; //表示起点此点已在路径中
    int num = 0;      //用于存储出栈元素
    int judge;        //判断变量
    push(s, start);   //将起点压入栈中
    while (s->top != 0)
    {
        judge = 1;
        for (j = 1; j < maxNum + 1; j++) //本循环会优先向下向右查找路径
        {
            if ((map[s->elem[s->top - 1]][j] == 1) && (state[j] == 0)) //二维数组中标记为1的点为可用点
            {
                push(s, j);                      //新位置入栈
                map[s->elem[s->top - 1]][j] = 2; //对该点进行标记
                state[j] = 1;                    //对该点进行标记
                judge = 0;                       //判断变量置0，有元素入栈，本次不进行出栈操作
            }
        }
        if (judge) //无新元素入栈，表示该点不在通路路径上，该元素出栈
        {
            num = pop(s, num);
            state[j] = 2; //此点不可再使用
        }
        if (s->elem[s->top - 1] == end) //到达迷宫出口
        {
            printStack(*s); //输出迷宫路径
            break;
        }
    }
}
void printStack(Stack s) //逆序输出栈内数据函数
{
    Stack *tmp = (Stack *)malloc(sizeof(Stack)); //创建一个临时栈，用于逆序存储原栈内的数据
    int num, i = 0;
    InitStack(&tmp);
    printf("当前路径长度为：%d\n", s.top);
    printf("具体路径为：\n");
    while (s.top) //保证将栈s内元素全部弹出
    {
        num = pop(&s, num); //将栈s内的数据弹出
        push(tmp, num);     //将栈s弹出的数据压入栈tmp中
    }
    while ((*tmp).top) //从栈顶开始输出栈tmp内的数据，即逆序输出栈s内的数据，得到迷路出路
    {
        num = pop(tmp, num);
        printf("%d\t", num); //输出栈tmp弹出元素
        i++;
        if (i % 5 == 0)
            printf("\n");
    }
    StackDestory(tmp); //销毁栈tmp
    tmp = NULL;
}
void StackDestory(Stack *s) //销毁栈函数
{
    free(s->elem);  //释放内存
    s->elem = NULL; //指针置为NULL,防止形成野指针
    s->top = 0;//重置栈顶位置
}
int main()
{
    Stack *s = (Stack *)malloc(sizeof(Stack));  //初始化一个栈
    int map[Maxsize][Maxsize] = {0};            //定义二维数组存储迷宫信息
    int i, maxNum = 0;                          // maxNum是最大编号
    InitStack(&s);                              //栈初始化
    getLabyrinth(map, &maxNum);                 //获取地图
    printf("当前地图最大节点为：%d\n", maxNum); //输出当前迷宫最大节点编号
    wayFind(map, s, maxNum);                    //寻找迷宫出路
    StackDestory(s);                            //销毁栈
    free(s);                                    //释放指针s申请的内存
    s = NULL;                                   //置空队列指针，防止形成野指针
    getchar();
    getchar();
    return 0;
}