#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <cstring>
using namespace std;

const STACK_INIT_SIZE=100;
const STACKINCREMETN=10;

typedef struct {
	char*  top;
	char*  base;
	int stacksize;
} sqstack;

typedef struct qnode {
	char data;
	struct qnode * next;
} qnode, * queueptr;

typedef struct {
	queueptr front;
	queueptr rear;
} linkqueue;

void initstack(sqstack & s) {
	s.base = (char* ) malloc(STACK_INIT_SIZE * sizeof(char));
	s.top = s.base;
	s.stacksize = STACK_INIT_SIZE;
}

void push(sqstack & s, char c) {
	if (s.top - s.base >= s.stacksize) {
		s.base = (char* ) realloc(s.base, (s.stacksize + STACKINCREMETN) * sizeof(char));
		s.top = s.base + s.stacksize;
		s.stacksize += STACKINCREMETN;
	}
	* (s.top++) = c;
}

void pop(sqstack & s, char & c) {
	c = * (--s.top);
}

int stackempty(sqstack & s) {
	if (s.top == s.base) {
		return 1;
	} else {
		return 0;
	}
}

void initqueue(linkqueue & q) {
	q.front = q.rear = (queueptr) malloc(sizeof(qnode));
	q.front->next = NULL;
}

void enqueue(linkqueue & q, char c) {
	qnode * p;
	p = (queueptr) malloc(sizeof(qnode));
	p->data = c;
	p->next = NULL;
	q.rear->next = p;
	q.rear = p;
}

void dequeue(linkqueue & q, char & c) {
	qnode * p;
	p = (queueptr) malloc(sizeof(qnode));
	p = q.front->next;
	c = p->data;
	q.front->next = p->next;
	if (q.rear == p) {
		q.rear = q.front;
	}
	free(p);
}
 
int queueempty(linkqueue q) {
	if (q.front == q.rear) {
		return 1;
	} else  {
		return 0;
	}
}

void main() {
	int i;
	char str[500], c, temp, code[5000];
	sqstack s;
	linkqueue q;
	initstack(s);
	initqueue(q);
	while (1) {
		printf("������ħ�����ԣ�\n");
		gets(str);
		int l = strlen(str);
		for (i = l; i >= 0; i--) {
			push(s,str[i]);
		}
		printf("ħ�����ԵĽ���Ϊ��\n");
		i = 0;
		memset(code, '\0', sizeof(code));
		while (! stackempty(s)) {
		    pop(s, c);
			if (c == 'A') {
				strcat(code, "sae");
				i += 3;
				printf("��һֻ��");
			} else if (c == 'B') {
				strcat(code, "tsaedsae");
				i += 8;
				printf("����һֻ�����һֻ��");
			} else if (c == '(') {
				pop(s, c);
				temp = c;
				enqueue(q, temp);
				while (1) {
					pop(s, c);
					if(c == ')') {
						break;
					}
					enqueue(q, c);
					enqueue(q, temp);
				}
				while(! queueempty(q)) {
					dequeue(q, c);
					push(s, c);
				}
			} else {
				code[i++] = c;
				if (c == 't')
				    printf("��");
				else if (c == 'd')
					printf("��");
				else if (c == 's')
					printf("��");
				else if (c == 'a')
					printf("һֻ");
				else if (c == 'e')
					printf("��");
				else if (c == 'z')
					printf("׷");
				else if (c == 'g')
					printf("��");
				else if (c == 'x')
					printf("��");
				else if (c == 'n')
					printf("��");
				else if (c == 'h')
					printf("��");
			}
		}
		printf("\n");
		for (i = 0; code[i] != '\0'; ++i) {
			printf("%c", code[i]);
		}
		printf("\n");
	}
}
