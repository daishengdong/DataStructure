#include <iostream>
using	namespace	std;

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

	Splay_Tree( node* root ) {
		_root = root;
		_root->parent = NULL;
	}

	Splay_Tree& operator=( const Splay_Tree& s ) {
		_root = s._root;
		return *this;
	}

	friend void join( Splay_Tree& s1, Splay_Tree& s2 );

	node* find( int key );
	void insert( int key );
	node* del( int key );
	void split( int key, Splay_Tree& s1, Splay_Tree& s2 );
	
	void print();
	
private:
	void _print_aux( node* cur_root );

	void _splay( node* x );

	void _insert_aux( node* z );
	node* _del_aux( node* z );
	node* _find_aux( node* cur_root, int key );

	void _left_rotate( node* x );
	void _right_rotate( node* x );

	node* _tree_successor( node* x );
	node* _tree_minimum( node* x );
	node* _tree_maximum( node* x );

	node* _root;
};

void join( Splay_Tree& s1, Splay_Tree& s2 ) {
	// each element in s2 is greater than the one in s1
	node* x = s1._tree_maximum( s1._root );
	s1._splay( x );
	x->right = s2._root;
}

node* Splay_Tree::find( int key ) {
	node* x = _find_aux( _root, key );
	_splay( x );
	return x;
}

node* Splay_Tree::_find_aux( node* cur_root, int key ) {
	if ( cur_root == NULL || cur_root->key == key ) return cur_root;
	else if ( key < cur_root->key ) return _find_aux( cur_root->left, key );
	else return _find_aux( cur_root->right, key );
}

void Splay_Tree::_splay( node* x ) {
	while ( x != _root ) {
		// case 1: x's parent is _root
		if ( x->parent == _root ) {
			if ( x == x->parent->left ) {
				_right_rotate( x->parent );
			} else {
				_left_rotate( x->parent );
			}
			_root = x;
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

void Splay_Tree::print() {
	_print_aux( _root );
}

void Splay_Tree::_print_aux( node* cur_root ) {
	if ( cur_root == NULL ) return;

	if ( cur_root->left != NULL ) _print_aux( cur_root->left );
	cout << cur_root->key << " ";
	if ( cur_root->right != NULL ) _print_aux( cur_root->right );
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

node* Splay_Tree::del( int key ) {
	node* z = find( key );
	/*
	return NULL, not return NULL
	*/
	if ( z == NULL ) return NULL;
	return _del_aux( z );
}

node* Splay_Tree::_del_aux( node* z ) {
	node* x;
	node* y;
	if ( z->left == NULL || z->right == NULL ) y = z;
	else y = _tree_successor( z );
	if ( y->left != NULL ) x = y->left;
	else x = y->right;
	if ( x != NULL ) x->parent = y->parent;
	if ( y->parent == NULL ) _root = x;
	else if ( y == y->parent->left ) y->parent->left = x;
	else y->parent->right = x;
	if ( y != z ) {
		z->key = y->key;
		_splay( z );
	} else {
		_splay( x );
	}
	return y;
}

void Splay_Tree::split( int key, Splay_Tree& s1, Splay_Tree& s2 ) {
	node* x = find( key );
	_splay( x );
	s1 = Splay_Tree( x->left );
	s2 = Splay_Tree( x->right );
}

node* Splay_Tree::_tree_successor( node* x ) {
	if ( x->right != NULL ) return _tree_minimum( x->right );
	node* y = x->parent;
	/*
	caution: y != NULL
	*/
	while ( y != NULL && x == y->right ) {
		x = y;
		y = y->parent;
	}
	return y;
}

node* Splay_Tree::_tree_minimum( node* x ) {
	while ( x->left != NULL ) x = x->left;
	return x;
}

node* Splay_Tree::_tree_maximum( node* x ) {
	while ( x->right != NULL ) x = x->right;
	return x;
}

int
main( void ) {
	Splay_Tree splay_Tree;

	splay_Tree.print();
	cout << endl;
	
	for ( int i = 0; i < 5; ++i ) {
		splay_Tree.insert( i );

		splay_Tree.print();
		cout << endl;
	}

	for ( i = 0; i < 5; ++i ) {
		node* n = splay_Tree.del( i );

		cout << n->key << endl;
		splay_Tree.print();
		cout << endl;
	}
	
	return 0;
}