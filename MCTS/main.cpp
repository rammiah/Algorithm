#include <iostream>
#include <vector>	// 存储子节点
#include <math.h>	// 计算UCB会用到
#include <set>
#include <ctime>
using namespace std;
// 便于识别
typedef int Player;
// 宏定义player
#define NOBODY 0
#define HUMAN 1
#define AI 6	// 计算胜负比较容易
const int n_in_row = 5;
struct Point {
	// 屏幕左上角为远点，横向为x，纵向为y
	int x;	// x坐标
	int y;	// y坐标
	// 构造函数
	Point(int x = 0, int y = 0, Player player = NOBODY):x(x), y(y) {}
	Point(const Point&point): x(point.x), y(point.y) {}
	// 重载输出运算符,==的话可能也要写一下啊
	friend ostream&operator<<(ostream&out, Point&point) {
		out << point.x << ", " << point.y;
		return out;
	}
};

class Board {
protected:
	int boardSize;
	// 注意需要自己写复制构造函数，不然父节点的children也会传下来
	vector<vector<Player>>board;		// 棋盘的点的一个二维vector
	

	Player check(int&sum) {
		if (sum / HUMAN == n_in_row) {
			return HUMAN;
		}
		else if (sum / AI == n_in_row) {
			return AI;
		}
		return NOBODY;
	}
	int left;
public:
	
	Board(int boardSize = 10) {
		this->boardSize = boardSize;
		left = boardSize * boardSize;
		this->board = vector<vector<Player>>(boardSize, vector<Player>(boardSize, NOBODY));
	}
	// 判断此点是否可走
	bool canMove(const Point&point) const {
		int x = point.x;
		int y = point.y;
		// 先判断位置是否合法
		if (x < 0 || y < 0 || x >= boardSize || y >= boardSize) return false;
		//  再判断此点的player即可
		if (board[y][x] == NOBODY) {
			return true;
		}
		else {
			return false;
		}
	}
	// 设置point位置的player
	void setPlayer(const Point &point, Player player) {
		int x = point.x;
		int y = point.y;
		board[y][x] = player;
		--left;
	}
	// 得到棋盘大小
	int getSize() const {
		return boardSize;
	}

	bool isFull() {
		return left <= 0;
	}

	// 判断是否有winner，多子连线也认为获胜
	Player hasWinner();
	void display();
	// 居然还要重载=/(ㄒoㄒ)/~~
	const Board&operator=(const Board&board) {
		this->board = board.board;
		this->left = board.left;
		return *this;
	}
};

class Human {
public:
	// 获取人类走点，失败继续递归获取
	Point getMove(const Board&board) {
		int x, y;
		cout << "Please enter your place(x y): ";
		cin >> x >> y;
		Point point(x, y, HUMAN);
		if (board.canMove(point)) {
			return point;
		}
		else {
			cout << "Sorry, illegal place." << endl;
			return this->getMove(board);
		}
	}
};

class AIBoard{
private:
	static int limitTimes;
	static int allTimes;
	Point point;	// 本次多的是那个点
	vector<Point>childrenMove;		// 可走点
	vector<AIBoard>children;		// 指向子节点的集合，一维vector
	int win;		// 此节点胜利次数
	int all;		// 此节点所有进行次数
	const static int width = 1;

	// 获取临近的点
	void getNeighbor(vector<Point>&points, const Point&center, set<int>&points_set) {
		Point point;
		int size = board.getSize();
		for (int row_off = -width; row_off <= width; ++row_off) {
			for (int col_off = -width; col_off <= width; ++col_off) {
				point.x = center.x + col_off;
				point.y = center.y + row_off;
				if (board.canMove(point)) {
					if (points_set.find(point.y * size + point.x) == points_set.end()) {
						points.push_back(point);
						points_set.insert(point.y * size + point.x);
					}
				}
			}
		}
	}
public:
	Board board;	// 棋盘
	// 由MCTS初始化的
	AIBoard(const Board&board):board(board), win(1), all(1){
		children = vector<AIBoard>();
		//childrenMove = getCanMove();
	}
	// 自己调用初始化的
	AIBoard(const Board&board, const Point&point, Player player) :board(board), win(1), all(1), point(point) {
		children = vector<AIBoard>();
		this->board.setPlayer(point, player);
		//childrenMove = getCanMove();
	}

	// 获取UCB值
	double getUCB(int parentAll) {
		return 1.0 * win / all + sqrt(2 * log(parentAll) / all);
	}

	// 获取胜率
	double getRate() {
		return 1.0 * win / all;
	}

	// 获取此棋盘多的那个点
	Point getPoint() {
		return point;
	}

	// 获取最好的点，这个只需要MCTS调用
	Point getBestMove() {
		int best = 0;
		for (int i = 0; i < children.size(); ++i) {
			if (children[i].getRate() > children[best].getRate()) {
				best = i;
			}
		}
		return children[best].getPoint();
	}

	// 获取周围棋子，感觉这应该是个自己的函数
	vector<Point> getCanMove() {
		Point point;
		set<int> points_set;
		vector<Point>points;
		for (int row = 0; row < board.getSize(); ++row) {
			for (int col = 0; col < board.getSize(); ++col) {
				point.x = col;
				point.y = row;
				// 在这里不能走说明有人，下面获取周围的位置
				if (!board.canMove(point)) {
					getNeighbor(points, point, points_set);	// 传参为引用，直接修改points，board是共有的
				}
			}
		}
		return points;
	}

	// 模拟开始，直到有一方胜利，函数返回胜利的player，感觉那个走的point也要返回，不如建一个子节点
	Player silmulation(Player player) {
		++allTimes;
		Board tempBoard(board);		// temp先保存现在的棋盘，等模拟完换回去
		Point point;
		vector<Point> canMove;
		srand(time(nullptr));
		while (board.hasWinner() == NOBODY && !board.isFull()) {
			canMove = getCanMove();
			// 随机挑选一个点
			unsigned int _rand = ((unsigned)rand()) % canMove.size();
			//cout << "index: " << _rand << " size: " << canMove.size() << endl;
			point = canMove[_rand];
			board.setPlayer(point, player);
			player = AI + HUMAN - player;
			//board.display();
		}
		player = board.hasWinner();
		board = tempBoard;
		if (player == AI) {
			++win;
		}
		++all;
		return player;
	}

	// 反向传播的不如直接写到getMove里面去，调用结束进行评估
	// 这个只负责模拟，不负责获取最后输出到棋盘的结果
	Player getMove(Player player) {
		// 模拟
		while (allTimes <= limitTimes) {
			int best = 0;
			for (int i = 0; i < children.size(); ++i) {
				if (children[i].getUCB(all) > children[best].getUCB(all)) {
					best = i;
				}
			}
			Player p = children[best].silmulation(AI);
			if (p == AI) {
				++win;
			}
			++all;
		}
		return player;
	}

	void setBoard(const Board&board) {
		this->board = board;
		childrenMove.clear();
		childrenMove = getCanMove();	// 此处出问题了
		// 把children也设置了
		children.clear();
		for (int i = 0; i < childrenMove.size(); ++i) {
			children.push_back(AIBoard(board, childrenMove[i], AI));
		}
	}

	// 输出一下子棋盘的情况
	void displayChildren() {
		for (int i = 0; i < children.size(); ++i) {
			cout << "Point: " << children[i].point << " rate: " << children[i].getRate() << endl;
		}
	}

	// 重置一下次数
	static void setTimes(int LimitTimes, int AllTimes) {
		limitTimes = LimitTimes;
		allTimes = AllTimes;
	}
};
// 类外初始化
int AIBoard::allTimes = 0;
int AIBoard::limitTimes = 0;

class MCTS {
private:
	// AI内部类，便于自己计算
	AIBoard myboard;
public:
	// 关于构造函数，我想传入一个Board，可以看做节点向下传的，传的就是内部类的那个board
	MCTS(const Board&board) : myboard(board) { }

	Point getMove(const Board&board) {
		// 设置其最多模拟10^5次
		AIBoard::setTimes(100, 0);
		this->myboard.setBoard(board);
		myboard.getMove(AI);
		myboard.displayChildren();
		return myboard.getBestMove();
	}
};

int main(void) {
	Board board(6);			// 初始化棋盘
	Player player = HUMAN;	// 设置第一步人先走
	Human human;			// 初始化玩家
	MCTS ai(board);			// 初始化AI
	Point move(0, 0);		// 每次落点的一个临时变量
	board.display();

	while (board.hasWinner() == NOBODY) {
		if (player == HUMAN) {
			move = human.getMove(board);
		}
		else {
			move = ai.getMove(board);
		}
		board.setPlayer(move, player);	// 设置位置
		// 输出走的位置
		cout << ((player == HUMAN) ? "Human: " : "AI:   ") << move << endl;
		board.display();
		// 切换玩家
		player = HUMAN + AI - player;
	}
	cout << endl;
	cout << "The winner is " << ((board.hasWinner() == HUMAN) ? "HUMAN" : "AI")  << endl;

	return 0;
}

// 判断是否有winner
Player Board::hasWinner() {
	int x = 0, y = 0;
	// 先判断横向
	int sum = 0;
	for (int row = 0; row < boardSize; ++row) {
		sum = 0;
		y = row;
		for (int i = 0; i < n_in_row; ++i) {
			sum += board[y][i];
		}
		if (check(sum) != NOBODY) return check(sum);
		for (int start = 0; start < boardSize - n_in_row; ++start) {
			sum = sum - board[y][start] + board[y][start + n_in_row];
			if (check(sum) != NOBODY) return check(sum);
		}
	}
	// 判断竖向
	for (int col = 0; col < boardSize; ++col) {
		sum = 0;
		x = col;
		for (int i = 0; i < n_in_row; ++i) {
			sum += board[i][x];
		}
		if (check(sum) != NOBODY) return check(sum);
		for (int start = 0; start < boardSize - n_in_row; ++start) {
			sum = sum - board[start][x] + board[start + n_in_row][x];
			if (check(sum) != NOBODY) return check(sum);
		}
	}

	// 下面判断斜向
	// 判断左上到右下方向
	// 被等号坑死了   /(ㄒoㄒ)/~~
	for (int row = 0; row <= boardSize - n_in_row; ++row) {
		sum = 0;
		for (int col = 0; col <= boardSize - n_in_row; ++col) {
			x = col;
			y = row;
			sum = 0;
			for (int i = 0; i < n_in_row; ++i) {
				sum += board[y][x];
				x++;
				y++;
			}
			if (sum / HUMAN == n_in_row) {
				return HUMAN;
			}
			else if (sum / AI == 5) {
				return AI;
			}
		}
	}
	// 判断左下到右上方向
	for (int col = n_in_row - 1; col < boardSize; ++col) {
		for (int row = 0; row <= boardSize - n_in_row; ++row) {
			x = col;
			y = row;
			sum = 0;
			for (int i = 0; i < n_in_row; ++i) {
				sum += board[y][x];
				x--;
				y++;
			}
			if (sum / HUMAN == n_in_row) {
				return HUMAN;
			}
			else if (sum / AI == n_in_row) {
				return AI;
			}
		}
	}
	return NOBODY;
}
// 输出棋盘到屏幕
void Board::display() {
	cout << "  ";
	for (int i = 0; i < boardSize; ++i) {
		cout << i << " ";
	}
	cout << endl;
	for (int row = 0; row < boardSize; ++row) {
		cout << row << " ";
		for (int col = 0; col < boardSize; ++col) {
			if (board[row][col] == HUMAN) {
				cout << "o ";
			}
			else if (board[row][col] == AI) {
				cout << "x ";
			}
			else {
				cout << "_ ";
			}
		}
		cout << endl;
	}
	cout << "Human: o" << endl;
	cout << "AI:    x" << endl;
	cout << endl;
}
