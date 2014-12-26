#include <iostream>
#include <queue>
using	namespace	std;

#define __debug__

const int t = 3;

struct BTNode {
	int             n;          // key number of current node
	int				key[2 * t + 1];	// 0th key is not used
	struct BTNode   *c[2 * t + 1];  // childs of current node
    bool            leaf;       // is current node a leaf?
};

struct result {
	BTNode *nd;
	int i;

	result(BTNode *nd, int i) : nd(nd), i(i) {}
};

class BTree {
public:
    BTree();
    void print();
	result *search(int k);
	void insert(int k);
	void del(int k);

private:
    void _create();
	result *_search_aux(BTNode *x, int k);
	void _split_child(BTNode *x, int i, BTNode *y);
	void _insert_nonfull(BTNode *x, int k);
    void _del_aux(BTNode *x, int k);
    void _merge_child(BTNode *x, int i, BTNode *y);

	BTNode *_root;
};

struct Info {
	BTNode *node;
	int layer;

	Info(BTNode *pn, int pl) :
	node(pn), layer(pl) {}
};

void BTree::print() {
    queue<Info*> q;
    q.push(new Info(_root, 0));
    while (!q.empty()) {
        Info *info = q.front();
		q.pop();
		cout << info->layer << ": ";
		for (int i = 1; i <= info->node->n; ++i)
			cout << (char)info->node->key[i] << " ";
		cout << endl;

		if (!info->node->leaf)
			for (i = 1; i <= info->node->n + 1; ++i)
				q.push(new Info(info->node->c[i], info->layer + 1));
    }
}

BTree::BTree() {
    _create();

#ifdef __debug__
	_root->leaf = false;
	_root->key[1] = 'G';
	_root->key[2] = 'M';
	_root->key[3] = 'P';
	_root->key[4] = 'X';
	_root->n = 4;

	BTNode *node1 = new BTNode;;
	node1->key[1] = 'A';
	node1->key[2] = 'C';
	node1->key[3] = 'D';
	node1->key[4] = 'E';
	node1->n = 4;
    node1->leaf = true;

	BTNode *node2 = new BTNode;;
	node2->key[1] = 'J';
	node2->key[2] = 'K';
	node2->n = 2;
    node2->leaf = true;

	BTNode *node3 = new BTNode;;
	node3->key[1] = 'N';
	node3->key[2] = 'O';
	node3->n = 2;
    node3->leaf = true;

	BTNode *node4 = new BTNode;;
	node4->key[1] = 'R';
	node4->key[2] = 'S';
	node4->key[3] = 'T';
	node4->key[4] = 'U';
	node4->key[5] = 'V';
	node4->n = 5;
    node4->leaf = true;

	BTNode *node5 = new BTNode;;
	node5->key[1] = 'Y';
	node5->key[2] = 'Z';
	node5->n = 2;
    node5->leaf = true;

    _root->c[1] = node1;
    _root->c[2] = node2;
    _root->c[3] = node3;
    _root->c[4] = node4;
    _root->c[5] = node5;
#endif	
}

void BTree::_create() {
    _root = new BTNode;
    _root->leaf = true;
    _root->n = 0;
}

result *BTree::search(int k) {
    return _search_aux(_root, k);
}

result *BTree::_search_aux(BTNode *x, int k) {
    int i = 1;
    while (i <= x->n && k > x->key[i])
        ++i;

    if (i <= x->n && k == x->key[i])
        return new result(x, i);

    if (x->leaf) return NULL;
    else return _search_aux(x->c[i], k);
}

/*
 * split the sub node of x'c[i]
 */
void BTree::_split_child(BTNode *x, int i, BTNode *y) {
	BTNode *z = new BTNode;
    z->leaf = y->leaf;
    z->n = t - 1;
    for (int j = 1; j <= t - 1; ++j)
        z->key[j] = y->key[j + t];
    if (!y->leaf)
        for (int j = 1; j <= t; ++j)
            z->c[j] = y->c[j + t];
    y->n = t - 1;
    for (j = x->n + 1; j >= i + 1; --j)
         x->c[j + 1] = x->c[j];
    x->c[i + 1] = z;
    for (j = x->n; j >= i; --j)
        x->key[j + 1] = x->key[j];
    x->key[i] = y->key[t];
    x->n += 1;
}

void BTree::insert(int k) {
    BTNode *r = _root;
    if (r->n == 2 * t - 1) {
        BTNode *s = new BTNode;
        _root = s;
        s->leaf = false;
        s->n = 0;
        s->c[1] = r;
        _split_child(s, 1, r);
        _insert_nonfull(s, k);
    } else _insert_nonfull(r, k);
}

void BTree::_insert_nonfull(BTNode *x, int k) {
    int i = x->n;
    if (x->leaf) {
        while (i >= 1 && k < x->key[i]) {
            x->key[i + 1] = x->key[i];
            --i;
        }
        x->key[i + 1] = k;
        x->n += 1;
    } else {
        while (i >= 1 && k < x->key[i])
            --i;
        i += 1;
        if (x->c[i]->n == 2 * t - 1) {
            _split_child(x, i, x->c[i]);
            if (k > x->key[i]) i += 1;
        }
        _insert_nonfull(x->c[i], k);
    }
}

void BTree::del(int k) {
    _del_aux(_root, k);
}

/*
 * merge the two sub trees besides x'key[i]
 * y point to the new tree
 */
void BTree::_merge_child(BTNode *x, int i, BTNode *y) {
    // merge
    BTNode *z = x->c[i + 1];
    int k = x->key[i];
    y->key[y->n + 1] = k;
    for (int j = 1; j <= z->n; ++j)
        y->key[y->n + 1 + j] = z->key[j];
    if (!y->leaf)
        for (int j = 1; j <= z->n + 1; ++j)
            y->c[y->n + 1 + j] = z->c[j];
    
    // delete key[i] and c[i + 1] in x
    for (j = i; j < x->n; ++j)
        x->key[j] = x->key[j + 1];
    if (!x->leaf)
        for (int j = i + 1; j < x->n + 1; ++j)
            x->c[j] = x->c[j + 1];
    y->n += z->n + 1;
	x->n -= 1;

	if (x == _root && x->n == 0) {
		BTNode *r = _root;
		_root = y;
		delete r;
	}

	delete z;
}

void BTree::_del_aux(BTNode *x, int k) {
    int i = 1;
    while (i <= x->n && k > x->key[i])
        ++i;

    bool found = false;
    if (i <= x->n && k == x->key[i])
        found = true;

    // 1) found and x is a leaf
    if (found && x->leaf) {
        for(;i < x->n; ++i)
            x->key[i] = x->key[i + 1];
        x->n -= 1;
        return;
    }
    // 2) found and x is not a leaf
    if (found && !x->leaf) {
        // 2a)
        BTNode *y = x->c[i];
        BTNode *z = x->c[i + 1];
        if (y->n >= t) {
            while (!y->leaf)
                y = y->c[y->n + 1];
            int k_ = y->key[y->n];
            x->key[i] = k_;
            _del_aux(x->c[i], k_);
        } else if (z->n >= t) {
            // 2b)
            while (!z->leaf)
                z = z->c[1];
            int k_ = z->key[1];
            x->key[i] = k_;
            _del_aux(x->c[i + 1], k_);
        } else {
            // 2c)
            _merge_child(x, i, y);
            _del_aux(y, k);
        }
    }
    // 3) not found and x is not a leaf
    if (!found && !x->leaf) {
        BTNode *y = x->c[i];
        BTNode * b = NULL;  // brother
        if (y->n == t - 1) {
            // 3a)
            if (i > 1 && x->c[i - 1]->n >= t) { // left brother
                b = x->c[i - 1];
                // step 1: x's key[i - 1] down to y
                for (int j = y->n; j >= 1; --j)
                    y->key[j + 1] = y->key[j];
                if (!y->leaf)
                    for (int j = y->n + 1; j >= 1; --j)
                        y->c[j + 1] = y->c[j];
                y->key[1] = x->key[i - 1];
                // if y is not a leaf, copy b's last c to y->c[1]
                if (!y->leaf)
                    y->c[1] = b->c[b->n + 1];
                y->n += 1;

                // step 2: change x's key[i - 1] as last key in b
                x->key[i - 1] = b->key[b->n];

                // step 3: remove last key and c in b
                b->n -= 1;
				_del_aux(y, k);
            } else if (i < x->n + 1 && x->c[i + 1]->n >= t) {   // right brother
                b = x->c[i + 1];
                // step 1: x's key[i] down to y
                y->key[y->n + 1] = x->key[i];
                // if y is not a leaf, copy b's first c to y's last c
                if (!y->leaf)
                    y->c[y->n + 2] = b->c[1];
                y->n += 1;

                // step 2: change x's key[i] as first key in b
                x->key[i] = b->key[1];

                // step 3: remove first key and c in b
                for (int j = 1; j < b->n; ++j)
                    b->key[j] = b->key[j + 1];
                if (!b->leaf)
                    for (int j = 1; j < b->n + 1; ++j)
                        b->c[j + 1] = b->c[j];
                b->n -= 1;
				_del_aux(y, k);
            } 
            // 3b)
            else if (i > 1 && x->c[i - 1]->n == t - 1) { // left brother
                b = x->c[i - 1];
                _merge_child(x, i - 1, b);
                _del_aux(b, k);
            } else if (i < x->n + 1 && x->c[i + 1]->n == t - 1) {   // right brother
                b = x->c[i + 1];
                _merge_child(x, i, y);
                _del_aux(y, k);
            }
        }
        _del_aux(y, k);
    }
}

int
main(void) {
	BTree bt;

	cout << "tree builded" << endl;
	bt.print();
	cout << "**********************************" << endl << endl;

	bt.insert('B');
	cout << "B inserted" << endl;
	bt.print();
	cout << "**********************************" << endl << endl;

	bt.insert('Q');
	cout << "Q inserted" << endl;
	bt.print();
	cout << "**********************************" << endl << endl;

	bt.insert('L');
	cout << "L inserted" << endl;
	bt.print();
	cout << "**********************************" << endl << endl;

	bt.insert('F');
	cout << "F inserted" << endl;
	bt.print();
	cout << "**********************************" << endl << endl;

	bt.del('F');
	cout << "F deleted" << endl;
	bt.print();
	cout << "**********************************" << endl << endl;

	bt.del('M');
	cout << "M deleted" << endl;
	bt.print();
	cout << "**********************************" << endl << endl;

	bt.del('G');
	cout << "G deleted" << endl;
	bt.print();
	cout << "**********************************" << endl << endl;

	bt.del('D');
	cout << "D deleted" << endl;
	bt.print();
	cout << "**********************************" << endl << endl;

	bt.del('B');
	cout << "B deleted" << endl;
	bt.print();
	cout << "**********************************" << endl << endl;

	return 0;
}
