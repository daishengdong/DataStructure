#include <iostream>
using	namespace	std;

struct node {
	int left;
	int right;
	int mid;
	bool covered;
	node* lchild;
	node* rchild;

	node( int left, int right ) : left( left ), right( right ), mid( left + ( ( right - left ) >> 1 ) ), covered( false ), lchild( NULL ), rchild( NULL ) {}
};

class SegTree {
public:
	SegTree( int left, int right ) : _left( left ), _right( right ), _root( new node( left, right ) ) {}

	void build();
	void insert( int left, int right );
	bool query( int x );
	bool query( int left, int right );
	bool del( int left, int right );
	int cal();

	void print() {
		_print_aux( _root );
	}
	void _print_aux( node* curNode ) {
		if ( curNode == NULL ) return;

		cout << "[ " << curNode->left << "," << curNode->right << " ]  " << curNode->covered << endl;
		if ( curNode->lchild != NULL ) _print_aux( curNode->lchild );
		if ( curNode->rchild != NULL ) _print_aux( curNode->rchild );
	}

private:
	void _build_aux( node* curNode, int left, int right );
	void _insert_aux( node* curNode, int left, int right );
	bool _query_aux( node* curNode, int x );
	bool _query_aux( node* curNode, int left, int right );
	bool _del_aux( node* curNode, int left, int right );
	int _cal( node* curNode );

	int _left, _right;
	node* _root;
};

void SegTree::build() {
	_build_aux( _root, _left, _right );
}

void SegTree::_build_aux( node* curNode, int left, int right ) {
	if ( left + 1 == right ) return;
	node* left_child = new node( left, curNode->mid );
	node* right_child = new node( curNode->mid, right );
	curNode->lchild = left_child;
	curNode->rchild = right_child;
	_build_aux( left_child, left, curNode->mid );
	_build_aux( right_child, curNode->mid, right );
}

void SegTree::insert( int left, int right ) {
	_insert_aux( _root, left, right );
}

void SegTree::_insert_aux( node* curNode, int left, int right ) {
	if ( curNode->left == left && curNode->right == right ) {
		curNode->covered = true;
		return;
	}
	if ( right <= curNode->mid ) {
		_insert_aux( curNode->lchild, left, right );
	} else if ( left >= curNode->mid ) {
		_insert_aux( curNode->rchild, left, right );
	} else {
		_insert_aux( curNode->lchild, left, curNode->mid );
		_insert_aux( curNode->rchild, curNode->mid, right );
	}
}

bool SegTree::query( int x ) {
	return _query_aux( _root, x );
}

bool SegTree::_query_aux( node* curNode, int x ) {
	if ( curNode->left + 1 == curNode->right ) {
		return curNode->covered;
	}
	if ( x <= curNode->mid ) {
		return _query_aux( curNode->lchild, x );
	}
	return _query_aux( curNode->rchild, x );
}

bool SegTree::query( int left, int right ) {
	return _query_aux( _root, left, right );
}

bool SegTree::_query_aux( node* curNode, int left, int right ) {
	if ( curNode->left + 1 == curNode->right ) return curNode->covered;
	if ( curNode->covered ) return curNode->covered;

	if ( right <= curNode->mid ) {
		return _query_aux( curNode->lchild, left, right );
	} else if ( left >= curNode->mid ) {
		return _query_aux( curNode->rchild, left, right );
	} else {
		return _query_aux( curNode->lchild, left, curNode->mid ) && _query_aux( curNode->rchild, curNode->mid, right );
	}
}

bool SegTree::del( int left, int right ) {
	return _del_aux( _root, left, right );
}

bool SegTree::_del_aux( node* curNode, int left, int right ) {
	if ( curNode->left + 1 == curNode->right ) {
		bool ret = curNode->covered;
		curNode->covered = false;
		return ret;
	}

	if ( curNode->covered ) {
		curNode->covered = false;
		curNode->lchild->covered = true;
		curNode->rchild->covered = true;
	}

	if ( right <= curNode->mid ) {
		return _del_aux( curNode->lchild, left, right );
	} else if ( left >= curNode->mid ) {
		return _del_aux( curNode->rchild, left, right );
	} else {
		return _del_aux( curNode->lchild, left, curNode->mid ) &&
		_del_aux( curNode->rchild, curNode->mid, right );
	}
}

int SegTree::cal() {
	return _cal( _root );
}

int SegTree::_cal( node* curNode ) {
	if ( curNode->covered ) return curNode->right - curNode->left;
	if ( curNode->left + 1 == curNode->right ) return 0;
	return _cal( curNode->lchild ) + _cal( curNode->rchild );
}

int
main( void ) {
	SegTree st( 0, 9 );
	st.build();
	st.print();
	// st.insert( 1, 3 );
	st.print();
	cout << "==========" << endl;
	st.insert( 1, 6 );
	st.print();
	cout << st.query( 2, 4 ) << endl;
	cout << st.cal() << endl;

	return 0;
}