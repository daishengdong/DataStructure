#include <iostream>
#include <algorithm>
using	namespace	std;

const int MAXN = 80000;

int hash[MAXN];
int A[MAXN >> 1];
int B[MAXN >> 1];
int H[MAXN >> 1];

struct node {
	int left;
	int right;
	int mid;
	int hight;
	node* lchild;
	node* rchild;

	node(int left, int right) : left(left), right(right), mid(left + ((right - left) >> 1)), hight(0), lchild(NULL), rchild(NULL) {}
};

class SegTree {
public:
	SegTree(int left, int right) : _left(left), _right(right) {
		_root = new node(left, right);
	}

	void build();
	void insert(int left, int right, int hight);
	int cal();

	void print() {
		_print_aux(_root);
	}
	void _print_aux(node* curNode) {
		if (curNode == NULL) return;

		cout << "[" << curNode->left << "," << curNode->right << "]  " << endl;
		if (curNode->lchild != NULL) _print_aux(curNode->lchild);
		if (curNode->rchild != NULL) _print_aux(curNode->rchild);
	}

private:
	void _build_aux(node* curNode, int left, int right);
	void _insert_aux(node* curNode, int left, int right, int hight);
	int _cal(node* curNode, int hight);

	int _left, _right;
	node* _root;
};

void SegTree::build() {
	_build_aux(_root, _left, _right);
}

void SegTree::_build_aux(node* curNode, int left, int right) {
	if (left + 1 == right) return;
	node* left_child = new node(left, curNode->mid);
	node* right_child = new node(curNode->mid, right);
	curNode->lchild = left_child;
	curNode->rchild = right_child;
	_build_aux(left_child, left, curNode->mid);
	_build_aux(right_child, curNode->mid, right);
}

void SegTree::insert(int left, int right, int hight) {
	_insert_aux(_root, left, right, hight);
}

void SegTree::_insert_aux(node* curNode, int left, int right, int hight) {
	if (curNode->left == left && curNode->right == right) {
		if (curNode->hight < hight) curNode->hight = hight;
		return;
	}
	if (right <= curNode->mid) {
		_insert_aux(curNode->lchild, left, right, hight);
	} else if (left >= curNode->mid) {
		_insert_aux(curNode->rchild, left, right, hight);
	} else {
		_insert_aux(curNode->lchild, left, curNode->mid, hight);
		_insert_aux(curNode->rchild, curNode->mid, right, hight);
	}
}

int SegTree::cal() {
	return _cal(_root, _root->hight);
}

int SegTree::_cal(node* curNode, int hight) {
	if (hight > curNode->hight) curNode->hight = hight;
	if (curNode->left + 1 == curNode->right) return curNode->hight * (hash[curNode->right] - hash[curNode->left]);
	return _cal(curNode->lchild, curNode->hight) + _cal(curNode->rchild, curNode->hight);
}

inline int getn(int p, int n) {
	for (int i = 0; i < 2 * n; ++i)
		if (hash[i] == p) return i;
}

int
main(void) {
	int n;
	cin >> n;
	SegTree st(0, 2 * n);
	st.build();
	for (int i = 0, j = 0; i < n; ++i, ++j) {
		cin >> A[i] >> B[i] >> H[i];
		hash[j] = A[i];
		hash[++j] = B[i];
	}

	sort(hash, hash + 2 * n);

	for (i = 0; i < n; ++i)
		st.insert(getn(A[i], n), getn(B[i], n), H[i]);
	cout << st.cal() << endl;

	return 0;
}