#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;
// 边的结构体
class EData
{
    public:
        char start; // 边的起点
        char end;   // 边的终点
        int weight; // 边的权重
    public:
        EData(){}
        EData(char s, char e, int w):start(s),end(e),weight(w){}
};
class MatrixUDG {
    #define MAX    100
    #define INF    (~(0x1<<31))        // 无穷大(即0X7FFFFFFF)
	private:
        char mVexs[MAX];    // 顶点集合
        int mVexNum;             // 顶点数
        int mEdgNum;             // 边数
        int mMatrix[MAX][MAX];   // 邻接矩阵
    public:
        // 创建图(自己输入数据)
		MatrixUDG();
        // 创建图(用已提供的矩阵)
        //MatrixUDG(char vexs[], int vlen, char edges[][2], int elen);
        MatrixUDG(char vexs[], int vlen, int matrix[][9]);
		~MatrixUDG();
        // 深度优先搜索遍历图
        void DFS();
        // 广度优先搜索（类似于树的层次遍历）
        void BFS();
        // Floyd最短路径
        void floyd(int path[][MAX], int dist[][MAX]);
        // 打印矩阵队列图
        void print();
	private:
        // 读取一个输入字符
        char readChar();
        // 返回ch在mMatrix矩阵中的位置
        int getPosition(char ch);
        // 返回顶点v的第一个邻接顶点的索引，失败则返回-1
        int firstVertex(int v);
        // 返回顶点v相对于w的下一个邻接顶点的索引，失败则返回-1
        int nextVertex(int v, int w);
        // 深度优先搜索遍历图的递归实现
        void DFS(int i, int *visited);
        // 获取图中的边
        EData* getEdges();
        // 获取i的终点
        int getEnd(int vends[], int i);
};
/* 
 * 创建图(自己输入数据)
 */
MatrixUDG::MatrixUDG()
{
    char c1, c2;
    int i, j, weight, p1, p2;
    // 输入"顶点数"和"边数"
    cout << "input vertex number: ";
    cin >> mVexNum;
    cout << "input edge number: ";
    cin >> mEdgNum;
    if ( mVexNum < 1 || mEdgNum < 1 || (mEdgNum > (mVexNum * (mVexNum-1))))
    {
        cout << "input error: invalid parameters!" << endl;
        return ;
    }
    // 初始化"顶点"
    for (i = 0; i < mVexNum; i++)
    {
        cout << "vertex(" << i << "): ";
        mVexs[i] = readChar();
    }
    // 1. 初始化"边"的权值
    for (i = 0; i < mVexNum; i++)
    {
        for (j = 0; j < mVexNum; j++)
        {
            if (i==j)
                mMatrix[i][j] = 0;
            else
                mMatrix[i][j] = INF;
        }
    }
    // 2. 初始化"边"的权值: 根据用户的输入进行初始化
    for (i = 0; i < mEdgNum; i++)
    {
        // 读取边的起始顶点，结束顶点，权值
        cout << "edge(" << i << "): ";
        c1 = readChar();
        c2 = readChar();
        cin >> weight;
        p1 = getPosition(c1);
        p2 = getPosition(c2);
        if (p1==-1 || p2==-1)
        {
            cout << "input error: invalid edge!" << endl;
            return ;
        }
        mMatrix[p1][p2] = weight;
        mMatrix[p2][p1] = weight;
    }
}
/*
 * 创建图(用已提供的矩阵)
 *
 * 参数说明：
 *     vexs  -- 顶点数组
 *     vlen  -- 顶点数组的长度
 *     matrix-- 矩阵(数据)
 */
MatrixUDG::MatrixUDG(char vexs[], int vlen, int matrix[][9])
{
    int i, j;
    // 初始化"顶点数"和"边数"
    mVexNum = vlen;
    // 初始化"顶点"
    for (i = 0; i < mVexNum; i++)
        mVexs[i] = vexs[i];
    // 初始化"边"
    for (i = 0; i < mVexNum; i++)
        for (j = 0; j < mVexNum; j++)
            mMatrix[i][j] = matrix[i][j];
    // 统计边的数目
    for (i = 0; i < mVexNum; i++)
        for (j = 0; j < mVexNum; j++)
            if (i!=j && mMatrix[i][j]!=INF)
                mEdgNum++;
    mEdgNum /= 2;
}
/* 
 * 析构函数
 */
MatrixUDG::~MatrixUDG() 
{
}
/*
 * 返回ch在mMatrix矩阵中的位置
 */
int MatrixUDG::getPosition(char ch)
{
    int i;
    for(i=0; i<mVexNum; i++)
        if(mVexs[i]==ch)
            return i;
    return -1;
}
/*
 * 读取一个输入字符
 */
char MatrixUDG::readChar()
{
    char ch;
    do {
        cin >> ch;
    } while(!((ch>='a'&&ch<='z') || (ch>='A'&&ch<='Z')));
    return ch;
}
/*
 * 返回顶点v的第一个邻接顶点的索引，失败则返回-1
 */
int MatrixUDG::firstVertex(int v)
{
    int i;
    if (v<0 || v>(mVexNum-1))
        return -1;
    for (i = 0; i < mVexNum; i++)
        if (mMatrix[v][i]!=0 && mMatrix[v][i]!=INF)
            return i;
    return -1;
}
/*
 * 返回顶点v相对于w的下一个邻接顶点的索引，失败则返回-1
 */
int MatrixUDG::nextVertex(int v, int w)
{
    int i;
    if (v<0 || v>(mVexNum-1) || w<0 || w>(mVexNum-1))
        return -1;
    for (i = w + 1; i < mVexNum; i++)
        if (mMatrix[v][i]!=0 && mMatrix[v][i]!=INF)
            return i;
    return -1;
}
/*
 * 深度优先搜索遍历图的递归实现
 */
void MatrixUDG::DFS(int i, int *visited)
{
    int w;
    visited[i] = 1;
    cout << mVexs[i] << " ";
    // 遍历该顶点的所有邻接顶点。若是没有访问过，那么继续往下走
    for (w = firstVertex(i); w >= 0; w = nextVertex(i, w))
    {
        if (!visited[w])
            DFS(w, visited);
    }
}
/*
 * 深度优先搜索遍历图
 */
void MatrixUDG::DFS()
{
    int i;
    int visited[MAX];       // 顶点访问标记
    // 初始化所有顶点都没有被访问
    for (i = 0; i < mVexNum; i++)
        visited[i] = 0;
    cout << "DFS: ";
    for (i = 0; i < mVexNum; i++)
    {
        //printf("\n== LOOP(%d)\n", i);
        if (!visited[i])
            DFS(i, visited);
    }
    cout << endl;
}
/*
 * 广度优先搜索（类似于树的层次遍历）
 */
void MatrixUDG::BFS()
{
    int head = 0;
    int rear = 0;
    int queue[MAX];     // 辅组队列
    int visited[MAX];   // 顶点访问标记
    int i, j, k;
    for (i = 0; i < mVexNum; i++)
        visited[i] = 0;
    cout << "BFS: ";
    for (i = 0; i < mVexNum; i++)
    {
        if (!visited[i])
        {
            visited[i] = 1;
            cout << mVexs[i] << " ";
            queue[rear++] = i;  // 入队列
        }
        while (head != rear) 
        {
            j = queue[head++];  // 出队列
            for (k = firstVertex(j); k >= 0; k = nextVertex(j, k)) //k是为访问的邻接顶点
            {
                if (!visited[k])
                {
                    visited[k] = 1;
                    cout << mVexs[k] << " ";
                    queue[rear++] = k;
                }
            }
        }
    }
    cout << endl;
}
/*
 * 打印矩阵队列图
 */
void MatrixUDG::print()
{
    int i,j;
    cout << "Martix Graph:" << endl;
    for (i = 0; i < mVexNum; i++)
    {
        for (j = 0; j < mVexNum; j++)
            cout << setw(10) << mMatrix[i][j] << " ";
        cout << endl;
    }
}
/*
 * floyd最短路径。
 * 即，统计图中各个顶点间的最短路径。
 *
 * 参数说明：
 *     path -- 路径。path[i][j]=k表示，"顶点i"到"顶点j"的最短路径会经过顶点k。
 *     dist -- 长度数组。即，dist[i][j]=sum表示，"顶点i"到"顶点j"的最短路径的长度是sum。
 */
void MatrixUDG::floyd(int path[][MAX], int dist[][MAX])
{
    int i,j,k;
    int tmp;
    // 初始化
    for (i = 0; i < mVexNum; i++)
    {
        for (j = 0; j < mVexNum; j++)
        {
            dist[i][j] = mMatrix[i][j];    // "顶点i"到"顶点j"的路径长度为"i到j的权值"。
            path[i][j] = j;                // "顶点i"到"顶点j"的最短路径是经过顶点j。
        }
    }
    // 计算最短路径
    for (k = 0; k < mVexNum; k++)
    {
        for (i = 0; i < mVexNum; i++)
        {
            for (j = 0; j < mVexNum; j++)
            {
                // 如果经过下标为k顶点路径比原两点间路径更短，则更新dist[i][j]和path[i][j]
                tmp = (dist[i][k]==INF || dist[k][j]==INF) ? INF : (dist[i][k] + dist[k][j]);
                if (dist[i][j] > tmp)
                {
                    // "i到j最短路径"对应的值设，为更小的一个(即经过k)
                    dist[i][j] = tmp;
                    // "i到j最短路径"对应的路径，经过k
                    path[i][j] = path[i][k];
                }
            }
        }
    }
    // 打印floyd最短路径的结果
    cout << "floyd: " << endl;
        for (j = 0; j < mVexNum; j++)
            cout << setw(2) << dist[i][j] << "  ";
        cout << endl;
    }
}
int main()
{
    int prev[MAX] = {0};
    int dist[MAX] = {0};
    int path[MAX][MAX] = {0};    // 用于保存floyd路径
    int floy[MAX][MAX] = {0};    // 用于保存floyd长度
    char vexs[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};
    int matrix[][9] = {
             /*A*//*B*//*C*//*D*//*E*//*F*//*G*/
      /*A*/ {   0,  12, INF, INF, INF,  16,  14},
      /*B*/ {  12,   0,  10, INF, INF,   7, INF},
      /*C*/ { INF,  10,   0,   3,   5,   6, INF},
      /*D*/ { INF, INF,   3,   0,   4, INF, INF},
      /*E*/ { INF, INF,   5,   4,   0,   2,   8},
      /*F*/ {  16,   7,   6, INF,   2,   0,   9},
      /*G*/ {  14, INF, INF, INF,   8,   9,   0}};
    int vlen = sizeof(vexs)/sizeof(vexs[0]);
    MatrixUDG* pG;
    // 采用已有的"图"
    pG = new MatrixUDG(vexs, vlen, matrix);
    // floyd算法获取各个顶点之间的最短距离
    pG->floyd(path, floy);
    return 0;
}
