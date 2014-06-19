#include <iostream>
using	namespace	std;

typedef struct node {
	int number;
	int code;
	node * next;
} Node, * linkList;

class Joseph {
public:
	void init();
	void run();

private:
	int n;
	int m;
	linkList firstNode;
};

void Joseph::init() {
	cout << "n:" << endl;
	cin >> n;
	cout << "m:" << endl;
	cin >> m;

	linkList lastNode;
	int curCode;
	firstNode = new Node;
	cin >> curCode;
	firstNode->number = 1;
	firstNode->code = curCode;
	firstNode->next = NULL;
	lastNode = firstNode;

	linkList newNode;
	for ( int i = 2; i <= n; ++i ) {
		cin >> curCode;
		newNode = new Node;
		newNode->number = i;
		newNode->code = curCode;
		newNode->next = NULL;
		lastNode->next = newNode;
		lastNode = newNode;
	}
	if ( n >= 2 ) {
		lastNode->next = firstNode;
	} else {
		firstNode->next = firstNode;
	}
}

void Joseph::run() {
	int count = 1;
	linkList curTempNode = firstNode;
	linkList preNode;
	while ( curTempNode->next != curTempNode ) {
		if ( count == m ) {
			count = 1;
			m = curTempNode->code;
			cout << curTempNode->number << " ";
			linkList nodeToFree = curTempNode;
			preNode->next = curTempNode->next;
			curTempNode = curTempNode->next;
			delete nodeToFree;
		} else {
			++count;
			preNode = curTempNode;
			curTempNode = preNode->next;
		}
	}
	cout << curTempNode->number << endl;
}

int main() {
	Joseph * insJoseph = new Joseph();
	while ( 1 ) {
		insJoseph->init();
		insJoseph->run();
	}

	return 0;
}
