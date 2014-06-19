#include <iostream>
using	namespace	std;

// NONE stands for no pre or no next
#define NONE	-1
#define MAX		1000000

struct node {
	int key;
	node* left;
	node* right;
	node* parent;

	node( int key ) : key( key ) {}
};

class Splay_Tree {
public:
	Splay_Tree() {
		_root = NULL;
	}

	void print() {
		_print_aux( _root );
	}
	void _print_aux( node* curRoot ) {
		if ( curRoot == NULL ) return;

		if ( curRoot->left != NULL ) _print_aux( curRoot->left );
		cout << curRoot->key << " ";
		if ( curRoot->right != NULL ) _print_aux( curRoot->right );
	}

	void insert( int key );

	int pre() {
		if ( _root->left == NULL ) return NONE;
		node* curRoot = _root->left;
		int pre = curRoot->key;
		while ( curRoot->right != NULL ) {
			pre = curRoot->right->key;
			curRoot = curRoot->right;
		}
		return pre;
	}

	int next() {
		if ( _root->right == NULL ) return NONE;
		node* curRoot = _root->right;
		int next = curRoot->key;
		while ( curRoot->left != NULL ) {
			next = curRoot->left->key;
			curRoot = curRoot->left;
		}
		return next;
	}
private:
	void _splay( node* x );

	void _insert_aux( node* z );
	node* _find_aux( node* cur_root, int key );

	void _left_rotate( node* x );
	void _right_rotate( node* x );

	node* _root;
};

void Splay_Tree::_splay( node* x ) {
	while ( x != _root ) {
		// case 1: x's parent is _root
		if ( x->parent == _root ) {
			if ( x == x->parent->left ) {
				_right_rotate( x->parent );
			} else {
				_left_rotate( x->parent );
			}
		} else {
			node* y = x->parent;
			if ( y == y->parent->left && x == x->parent->left ) {
				// case 2: x, y are both the left child of their parents
				_right_rotate( y->parent );
				_right_rotate( x->parent );
			} else if ( y == y->parent->right && x == x->parent->right  ) {
				// case 2: x, y are both the right child of their parents
				_left_rotate( y->parent );
				_left_rotate( x->parent );
			} else {
				if ( y == y->parent->right ) {
					// case 3: y is the right child of its parent, while x is the left
					_right_rotate( y );
					_left_rotate( x->parent );
				} else {
					// case 3: y is the left child of its parent, while x is the right
					_left_rotate( y );
					_right_rotate( x->parent );
				}
			}
		}		
	}
}

void Splay_Tree::insert( int key ) {
	_insert_aux( new node( key ) );
}

void Splay_Tree::_insert_aux( node* z ) {
	node* y = NULL;
	node* x = _root;
	while ( x != NULL ) {
		y = x;
		if ( z->key < x->key ) x = x->left;
		else x = x->right;
	}
	z->parent = y;
	if ( y == NULL ) _root = z;
	else if ( z->key < y->key ) y->left = z;
	else y->right = z;
	z->left = NULL;
	z->right = NULL;
	_splay( z );
}

void Splay_Tree::_left_rotate( node* x ) {
	node* y = x->right;
	x->right = y->left;
	if ( y->left != NULL ) y->left->parent = x;
	y->parent = x->parent;
	if ( x->parent == NULL ) _root = y;
	else if ( x == x->parent->left ) x->parent->left = y;
	else x->parent->right = y;
	y->left = x;
	x->parent = y;
}

void Splay_Tree::_right_rotate( node* x ) {
	node* y = x->left;
	x->left = y->right;
	if ( y->right != NULL ) y->right->parent = x;
	y->parent = x->parent;
	if ( x->parent == NULL ) _root = y;
	else if ( x == x->parent->left ) x->parent->left = y;
	else x->parent->right = y;
	y->right = x;
	x->parent = y;
}

int
main( void ) {
	int n, ai, sigma = 0;;
	Splay_Tree splay_tree;

	cin >> n;
	cin >> ai;
	splay_tree.insert( ai );
	sigma = ai;
	n--;
	while ( n-- ) {
		cin >> ai;
		splay_tree.insert( ai );
		int min, delta_pre, delta_next;
		int pre = splay_tree.pre();
		int next = splay_tree.next();

		if ( pre != NONE ) delta_pre = abs( ai - pre );
		else delta_pre = MAX;

		if ( next != NONE ) delta_next = abs( ai - next );
		else delta_next = MAX;
		
		sigma += delta_pre < delta_next ? delta_pre : delta_next;
	}

	cout << sigma << endl;;
	
	return 0;
}