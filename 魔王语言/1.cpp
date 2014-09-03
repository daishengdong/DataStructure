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
		printf("请输入魔王语言：\n");
		gets(str);
		int l = strlen(str);
		for (i = l; i >= 0; i--) {
			push(s,str[i]);
		}
		printf("魔王语言的解释为：\n");
		i = 0;
		memset(code, '\0', sizeof(code));
		while (! stackempty(s)) {
		    pop(s, c);
			if (c == 'A') {
				strcat(code, "sae");
				i += 3;
				printf("上一只鹅");
			} else if (c == 'B') {
				strcat(code, "tsaedsae");
				i += 8;
				printf("天上一只鹅地上一只鹅");
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
				    printf("天");
				else if (c == 'd')
					printf("地");
				else if (c == 's')
					printf("上");
				else if (c == 'a')
					printf("一只");
				else if (c == 'e')
					printf("鹅");
				else if (c == 'z')
					printf("追");
				else if (c == 'g')
					printf("赶");
				else if (c == 'x')
					printf("下");
				else if (c == 'n')
					printf("蛋");
				else if (c == 'h')
					printf("恨");
			}
		}
		printf("\n");
		for (i = 0; code[i] != '\0'; ++i) {
			printf("%c", code[i]);
		}
		printf("\n");
	}
}
