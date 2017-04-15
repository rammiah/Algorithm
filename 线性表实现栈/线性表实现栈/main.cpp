#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
	int data;
	Node* next;
} Node;
typedef Node* Nodep;

int Pop(Nodep head);
void Push(Nodep head, int data);
Nodep Create(int data);

int main(void) {
	Nodep head = Create(-1);
	Push(head, 3);
	for (int i = 0; i < 10; i++) {
		Push(head, i);
	}
	for (int i = 0; i < 6; i++) {
		printf("%d ", Pop(head));
	}
	printf("\n");
	for (int i = 0; i < 5; i++) {
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
	temp = last = head;
	while (last->next != NULL) {
		temp = last;
		last = last->next;
	}
	if (temp == head) {
		printf("\n空表无法弹出元素！");
		return -1;
	}
	data = last->data;
	free(last);
	temp->next = NULL;
	return data;
}

void Push(const Nodep head, const int data) {
	static Nodep temp = NULL;
	temp = head;
	while (temp->next != NULL) {
		temp = temp->next;
	}
	temp->next = Create(data);
}