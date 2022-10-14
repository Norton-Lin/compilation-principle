#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define Maxsize 200
typedef struct
{
    int *elem;                                          //Ԫ�ش洢
    int top;                                            //���ջ��
} Stack;                                                //ջ�Ķ���
void InitStack(Stack **s);                              //��ʼ��ջ����
int isFull(Stack S);                                    //�ж�ջ������
int isEmpty(Stack s);                                   //�ж�ջ�պ���
void push(Stack *s, int num);                           //��ջ����
int pop(Stack *s, int num);                             //��ջ����
void getLabyrinth(int map[][Maxsize], int *maxNum);     //���ļ��ж�ȡ��ͼ����
void wayFind(int map[][Maxsize], Stack *s, int maxNum); //Ѱ���Թ�ͨ·����
void printStack(Stack s);                               //�������ջ�����ݺ���
void StackDestory(Stack *s);                            //����ջ����
void InitStack(Stack **s)                               //��ʼ��ջ����
{
    (*s)->elem = (int *)malloc(sizeof(int) * Maxsize); //ջ�ռ�����
    (*s)->top = 0;                                     //��ʼ״̬��ָʾ����ָ��ǰջ��
    double temp = 1.8e-4;
}
int isFull(Stack s) //�ж�ջ������
{
    int judge = 100;            //�жϱ���
    if (s.top == Maxsize + 1) //ջ�������жϱ�����0
        judge = 0;
    return judge;
}
int isEmpty(Stack s) //�ж�ջ�պ���
{
    int judge = 1;  //�жϱ���
    if (s.top == 0) //ջ�ս��жϱ�������
        judge = 0;
    return judge;
}
void push(Stack *s, int num) //��ջ����
{
    if (isFull(*s)) //��ջ��Ϊ��
    {
        s->elem[s->top] = num; //����ӦԪ����ջ
        s->top++;              //ջ��λ��+1
    }
}
int pop(Stack *s, int num) //��ջ����
{
    if (isEmpty(*s)) //��ջ��Ϊ��
    {
        num = s->elem[s->top - 1]; //��Ӧջ��Ԫ�س�ջ
        s->top--;                  //ջ��λ��-1
    }
    return num; //���س�ջԪ��
}
void getLabyrinth(int map[][Maxsize], int *maxNum) //���ļ��ж�ȡ��ͼ����
{
    FILE *fp = NULL; //��ʼ���ļ�ָ��
    int line, row;
    int i;
    if ((fp = fopen("labyrinth.txt", "r")) == NULL) //��ֻ��ģʽ���ļ������ļ��洢�Թ�����
        printf("Cannot open the file!\n");
    for (i = 0; i < Maxsize || !feof(fp); i++)
    {
        fscanf(fp, "%d %d\n", &line, &row); //���ı��ļ��ڸ�ʽ����������
        map[line][row] = 1;                 //�ڶ�ά�����ڱ��ͨ·
        map[row][line] = 1;
        if (row > (*maxNum)) //�������ڵ���
            *maxNum = row;
        if (line > (*maxNum)) //�������ڵ���
            *maxNum = line;
    }
}
void wayFind(int map[][Maxsize], Stack *s, int maxNum) //Ѱ���Թ�ͨ·����
{
    int start; //��ڵ�
    int end;   //���ڵ�
    printf("���������\n");
    scanf("%d", &start);
    if (start < 1 || start > maxNum) //�жϽڵ�Ϸ���
    {
        printf("��㲻���ڣ�\n");
        exit(1); //���Ƿ����˳�����
    }
    printf("�������յ�\n");
    scanf("%d", &end);
    if (end < 1 || end > maxNum) //�жϽڵ�Ϸ���
    {
        printf("�յ㲻���ڣ�\n");
        exit(1); //�յ�Ƿ����˳�����
    }
    int i, k = start, j = 1;
    int state[maxNum + 1]; //�ڵ��жϣ�����ֵ�Ĳ�ͬ��ʾ�õ��Ƿ񱻾�����1����Ϊ����ͬ��2��
    for (i = 0; i < maxNum + 1; i++)
        state[i] = 0;
    state[start] = 1; //��ʾ���˵�����·����
    int num = 0;      //���ڴ洢��ջԪ��
    int judge;        //�жϱ���
    push(s, start);   //�����ѹ��ջ��
    while (s->top != 0)
    {
        judge = 1;
        for (j = 1; j < maxNum + 1; j++) //��ѭ���������������Ҳ���·��
        {
            if ((map[s->elem[s->top - 1]][j] == 1) && (state[j] == 0)) //��ά�����б��Ϊ1�ĵ�Ϊ���õ�
            {
                push(s, j);                      //��λ����ջ
                map[s->elem[s->top - 1]][j] = 2; //�Ըõ���б��
                state[j] = 1;                    //�Ըõ���б��
                judge = 0;                       //�жϱ�����0����Ԫ����ջ�����β����г�ջ����
            }
        }
        if (judge) //����Ԫ����ջ����ʾ�õ㲻��ͨ··���ϣ���Ԫ�س�ջ
        {
            num = pop(s, num);
            state[j] = 2; //�˵㲻����ʹ��
        }
        if (s->elem[s->top - 1] == end) //�����Թ�����
        {
            printStack(*s); //����Թ�·��
            break;
        }
    }
}
void printStack(Stack s) //�������ջ�����ݺ���
{
    Stack *tmp = (Stack *)malloc(sizeof(Stack)); //����һ����ʱջ����������洢ԭջ�ڵ�����
    int num, i = 0;
    InitStack(&tmp);
    printf("��ǰ·������Ϊ��%d\n", s.top);
    printf("����·��Ϊ��\n");
    while (s.top) //��֤��ջs��Ԫ��ȫ������
    {
        num = pop(&s, num); //��ջs�ڵ����ݵ���
        push(tmp, num);     //��ջs����������ѹ��ջtmp��
    }
    while ((*tmp).top) //��ջ����ʼ���ջtmp�ڵ����ݣ����������ջs�ڵ����ݣ��õ���·��·
    {
        num = pop(tmp, num);
        printf("%d\t", num); //���ջtmp����Ԫ��
        i++;
        if (i % 5 == 0)
            printf("\n");
    }
    StackDestory(tmp); //����ջtmp
    tmp = NULL;
}
void StackDestory(Stack *s) //����ջ����
{
    free(s->elem);  //�ͷ��ڴ�
    s->elem = NULL; //ָ����ΪNULL,��ֹ�γ�Ұָ��
    s->top = 0;//����ջ��λ��
}
int main()
{
    Stack *s = (Stack *)malloc(sizeof(Stack));  //��ʼ��һ��ջ
    int map[Maxsize][Maxsize] = {0};            //�����ά����洢�Թ���Ϣ
    int i, maxNum = 0;                          // maxNum�������
    InitStack(&s);                              //ջ��ʼ��
    getLabyrinth(map, &maxNum);                 //��ȡ��ͼ
    printf("��ǰ��ͼ���ڵ�Ϊ��%d\n", maxNum); //�����ǰ�Թ����ڵ���
    wayFind(map, s, maxNum);                    //Ѱ���Թ���·
    StackDestory(s);                            //����ջ
    free(s);                                    //�ͷ�ָ��s������ڴ�
    s = NULL;                                   //�ÿն���ָ�룬��ֹ�γ�Ұָ��
    getchar();
    getchar();
    return 0;
}