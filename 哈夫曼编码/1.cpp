#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <queue>
#include <map>
using	namespace	std;

// ȫ�ֳ���
const char LEFT = '0';
const char RIGHT = '1';
const char NONE = '2';
const char ROOT = 'r';
map< char, int > weightMap;

// �����ݽṹ
typedef struct treeNode {
	char chr;
	char tag;
	int weight;
	struct treeNode * parent;
	struct treeNode * lChild, * rChild;
} TreeNode, * ptrTreeNode;

// �������ڵ���������ݽṹ
typedef struct listNode {
	ptrTreeNode root;
	struct listNode * next;
} ListNode, * ptrListNode;

// ����
typedef struct list {
	int countOfRoot;
	ptrListNode head;
} ListLink, * ptrListLink;

// ������������
typedef class hfmCode {
public:
	void Initialization();
	void Encoding();	// ����
	void Decoding();	// ����
	void TreePrint();	// ����δ�ӡ��������

private:
	void buildTree();	// ����
	ptrTreeNode getPtr(ptrTreeNode root, char ch);
	char * getCoding(ptrTreeNode node);

	ptrListLink listOfRoot;	// �������ڵ������ͷ
	ptrTreeNode SUMRoot;	// ������õ��Ĺ��������ĸ��ڵ�
} * ptrHfmCode;

// ��ʼ������Ҫ�Ǵӹ�ϣ���ж�ȡ�ַ�����������������
void hfmCode::Initialization() {
	listOfRoot = new ListLink;
	listOfRoot->countOfRoot = 0;
	listOfRoot->head = new ListNode;
	listOfRoot->head->root = NULL;
	listOfRoot->head->next = NULL;

	ptrListNode curNode = listOfRoot->head;
	map< char, int >::iterator it = weightMap.begin();
	for (; it != weightMap.end(); ++it) {
		ptrTreeNode newNode = new TreeNode;
		newNode->chr = it->first;
		newNode->weight = it->second;
		newNode->tag = NONE;
		newNode->lChild = NULL;
		newNode->rChild = NULL;
		newNode->parent = NULL;
		curNode->next = new ListNode;
		curNode->next->root = newNode;
		curNode->next->next = NULL;
		curNode = curNode->next;
		++listOfRoot->countOfRoot;
	}

	buildTree();
}

void hfmCode::TreePrint() {
	if (!SUMRoot) {
		return;
	}
	queue < ptrTreeNode > q;

	cout << SUMRoot->chr << endl;
	q.push(SUMRoot->lChild);
	q.push(SUMRoot->rChild);

	while (!q.empty()) {
		int size = q.size();
		for (int i = 0; i < size; ++i) {
			ptrTreeNode root = q.front();
			q.pop();
			if (root) {
				if (root->chr == ' ') {
					cout << "^" << "   ";
				} else {
					cout << root->chr << "   ";
				}
				q.push(root->lChild);
				q.push(root->rChild);
			} else {
				cout << "n   ";
			}
		}
		cout << endl;
	}
}

void hfmCode::buildTree() {
	while (listOfRoot->countOfRoot > 1) {
		int min1, min2, min;
		ptrTreeNode ptrMin1 = new TreeNode;
		ptrTreeNode ptrMin2 = new TreeNode;
		ptrListNode ptrPreMin = new ListNode;

		for (int i = 1; i <= 2; ++i) {
			ptrListNode curRoot = listOfRoot->head;
			min = curRoot->next->root->weight;
			while (curRoot->next) {
				int weight = curRoot->next->root->weight;
				if (weight <= min) {
					min = weight;
					if (1 == i) {
						min1 = min;
						ptrMin1 = curRoot->next->root;
					} else {
						min2 = min;
						ptrMin2 = curRoot->next->root;
					}
					ptrPreMin = curRoot;
				}
				curRoot = curRoot->next;
			}

			ptrPreMin->next = ptrPreMin->next->next;
			--listOfRoot->countOfRoot;
		}

		// �ҵ���СȨֵ�������ڵ��ȡ����
		ptrTreeNode newRoot = new TreeNode;
		newRoot->chr = ROOT;
		newRoot->tag = NONE;
		newRoot->parent = NULL;
		newRoot->lChild = ptrMin1;
		newRoot->rChild = ptrMin2;
		newRoot->weight = min1 + min2;
		ptrMin1->parent = newRoot;
		ptrMin1->tag = LEFT;
		ptrMin2->parent = newRoot;
		ptrMin2->tag = RIGHT;

		ptrListNode newListNode = new ListNode;
		newListNode->root = newRoot;
		
		// �µõ��Ľڵ���ͷ���������
		newListNode->next = listOfRoot->head->next;
		listOfRoot->head->next = newListNode;
		++listOfRoot->countOfRoot;
	}

	// ���ʣ�µĽڵ�������յĸ��ڵ�
	SUMRoot = listOfRoot->head->next->root;
}

void hfmCode::Encoding() {
	string fileNameIn = "ToBeTran.txt";
	string fileNameOut = "CodeFile.txt";
	ifstream fin(fileNameIn.c_str());
	ofstream fout(fileNameOut.c_str());
	char chr;
	while (fin.get(chr)) {
		ptrTreeNode ptr = getPtr(SUMRoot, chr);
		cout << chr << "  ";
		char * coding = getCoding(ptr);
		cout << coding << endl;
		fout << coding;
	}
	fout.flush();
	fout.close();
	fin.close();
}

void hfmCode::Decoding() {
	string fileNameIn = "CodeFile.txt";
	string fileNameOut = "TextFile.txt";
	ifstream fin(fileNameIn.c_str());
	ofstream fout(fileNameOut.c_str());
	char tag;
	ptrTreeNode curNode = SUMRoot;
	while (fin.get(tag)) {
		if (LEFT == tag) {
			curNode = curNode->lChild;
		} else if (RIGHT == tag) {
			curNode = curNode->rChild;
		}
		if (curNode->chr != ROOT) {
			cout << curNode->chr << endl;
			fout << curNode->chr;
			curNode = SUMRoot;
		}
	}
	fout.flush();
	fout.close();
	fin.close();
}

ptrTreeNode hfmCode::getPtr(ptrTreeNode root, char ch) {
	ptrTreeNode tempNode = NULL;
	if (!root) {
		return NULL;
	}

	if (root->chr == ch) {
		return root;
	}

	if (root->lChild) {
		tempNode = getPtr(root->lChild, ch);
		if (tempNode) {
			return tempNode;
		}
	}

	if (root->rChild) {
		tempNode = getPtr(root->rChild, ch);
		if (tempNode) {
			return tempNode;
		}
	}

	return NULL;
}

char * hfmCode::getCoding(ptrTreeNode node) {
	stack< char > coding;
	while (node->parent) {
		coding.push(node->tag);
		node = node->parent;
	}

	int i = 0;
	char * codeString = new char[coding.size() + 1];
	while (!coding.empty()) {
		codeString[i] = coding.top();
		coding.pop();
		++i;
	}
	codeString[i] = '\0';
	return codeString;
}

void initMap() {
	weightMap.insert(pair< char, int > (' ', 186));
	weightMap.insert(pair< char, int > ('A', 64));
	weightMap.insert(pair< char, int > ('B', 13));
	weightMap.insert(pair< char, int > ('C', 22));
	weightMap.insert(pair< char, int > ('D', 32));
	weightMap.insert(pair< char, int > ('E', 103));
	weightMap.insert(pair< char, int > ('F', 21));
	weightMap.insert(pair< char, int > ('G', 15));
	weightMap.insert(pair< char, int > ('H', 47));
	weightMap.insert(pair< char, int > ('I', 57));
	weightMap.insert(pair< char, int > ('J', 1));
	weightMap.insert(pair< char, int > ('K', 5));
	weightMap.insert(pair< char, int > ('L', 32));
	weightMap.insert(pair< char, int > ('M', 20));
	weightMap.insert(pair< char, int > ('N', 57));
	weightMap.insert(pair< char, int > ('O', 63));
	weightMap.insert(pair< char, int > ('P', 15));
	weightMap.insert(pair< char, int > ('Q', 1));
	weightMap.insert(pair< char, int > ('R', 48));
	weightMap.insert(pair< char, int > ('S', 51));
	weightMap.insert(pair< char, int > ('T', 80));
	weightMap.insert(pair< char, int > ('U', 23));
	weightMap.insert(pair< char, int > ('V', 8));
	weightMap.insert(pair< char, int > ('W', 18));
	weightMap.insert(pair< char, int > ('X', 1));
	weightMap.insert(pair< char, int > ('Y', 16));
	weightMap.insert(pair< char, int > ('Z', 1));
}

int main() {
	initMap();

	ptrHfmCode insHfmCode = new hfmCode;
	insHfmCode->Initialization();
	insHfmCode->TreePrint();
	insHfmCode->Encoding();
	insHfmCode->Decoding();
	
	system("pause");
	return 0;
}
