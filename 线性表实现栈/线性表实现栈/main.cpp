#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
	int data;
	Node* next;
} Node;
typedef Node* Nodep;

int Pop(Nodep head);
Nodep Push(Nodep head, int data);
Nodep Create(int data);

int main(void) {
	Nodep head = NULL;
	head = Push(head, 3);
	for (int i = 0; i < 10; i++) {
		Push(head, i);
	}
	for (int i = 0; i < 12; i++) {
		printf("%d ", Pop(head));
	}
	return 0;
}

Nodep Create(int data) {
	Nodep temp = (Nodep)malloc(sizeof(Node));
	temp->data = data;
	temp->next = NULL;
	return temp;
}

int Pop(const Nodep head) {
	static Nodep temp = NULL, last = NULL;
	int data;
	temp = head;
	if (temp == NULL) {
		printf("链表为空，无法弹出元素！\n");
		return -1;
	}
	last = head->next;
	if (last == NULL) {
		printf("链表为空，无法弹出元素！\n");
		return -1;
	}
	while (last->next != NULL) {
		temp = last;
		last = last->next;
	}
	data = last->data;
	temp->next = NULL;
	free(last);
	return data;
}

Nodep Push(const Nodep head, const int data) {
	static Nodep temp = NULL;
	temp = head;
	if (temp == NULL) {
		temp = Create(data);
		return temp;
	}
	while (temp->next != NULL) {
		temp = temp->next;
	}
	temp->next = Create(data);
	return temp;
}