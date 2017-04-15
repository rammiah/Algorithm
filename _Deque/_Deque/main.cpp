#include <iostream>
#define SIZE 20
using namespace std;

class Deque {
private:
	int deq[SIZE];
	int start;
	int tail;
public:
	Deque() {
		start = tail = 0;
	}
	void Push_back(int data) {
		if (!IsFull()) {
			deq[(tail + SIZE) % SIZE] = data;
			++tail;
			return;
		}
		cout << "队列已满，无法插入！" << endl;
	}

	int Pop_front() {
		if (IsEmpty()) {
			cout << "空队列，无法输出元素！";
			return -1;
		}
		int data = deq[(start + SIZE) % SIZE];
		++start;
		return data;
	}

	int Size() {
		return (tail - start + SIZE) % SIZE;
	}

	bool IsEmpty() {
		return Size() == 0;
	}

	bool IsFull() {
		return Size() == SIZE - 1;
	}
};

int main()
{
	Deque* deq = new Deque();
	for (int i = 0; i < 21; i++) {
		deq->Push_back(i);
		cout << "Size is " << deq->Size() << endl;
	}

	for (int i = 0; i < 21; i++) {
		cout << deq->Pop_front() << " ";
		if ((i + 1) % 5 == 0) cout << endl;
	}
	delete deq;
	return 0;
}
