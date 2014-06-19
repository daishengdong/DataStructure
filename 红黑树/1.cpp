#include <iostream>
using	namespace	std;

enum COLOR { BLACK, RED };

struct node {
	int key;
	COLOR color;
	node* left;
	node* right;
	node* parent;

	node() : color( BLACK ) {}
	node( int key ) : key( key ), color( RED ) {}
};

class RB_Tree {
public:
	RB_Tree() : nil( new node() ) {
		// cannot be root( nil )
		root = nil;
		root->parent = nil;
	}
	void insert( int key );
	node* del( int key );

	void print();
	
private:
	void _print_aux( node* curRoot );

	void _insert_aux( node* z );
	node* _del_aux( node* z );

	void _insert_fixup( node* z );
	void _del_fixup( node* x );

	void _left_rotate( node* x );
	void _right_rotate( node* x );

	node* _find( node* curRoot, int key );
	node* _tree_successor( node* x );
	node* _tree_minimum( node* x );

	node* root;
	node* nil;
};

void RB_Tree::print() {
	_print_aux( root );
}

void RB_Tree::_print_aux( node* curRoot ) {
	if ( curRoot == nil ) return;

	if ( curRoot->left != nil ) _print_aux( curRoot->left );
	cout << curRoot->key << " ";
	if ( curRoot->right != nil ) _print_aux( curRoot->right );
}

void RB_Tree::insert( int key ) {
	_insert_aux( new node( key ) );
}

void RB_Tree::_insert_aux( node* z ) {
	node* y = nil;
	node* x = root;
	while ( x != nil ) {
		y = x;
		if ( z->key < x->key ) x = x->left;
		else x = x->right;
	}
	z->parent = y;
	if ( y == nil ) root = z;
	else if ( z->key < y->key ) y->left = z;
	else y->right = z;
	z->left = nil;
	z->right = nil;
	z->color = RED;
	_insert_fixup( z );
}

void RB_Tree::_insert_fixup( node* z ) {
	while ( z->parent->color == RED ) {
		if ( z->parent == z->parent->parent->left ) {
			node* y = z->parent->parent->right;
			if ( y->color == RED ) {
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			} else {
				if ( z == z->parent->right ) {
					z = z->parent;
					_left_rotate( z );
				}
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				_right_rotate( z->parent->parent );
			}
		} else {
			node* y = z->parent->parent->left;
			if ( y->color == RED ) {
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			} else {
				if ( z == z->parent->left ) {
					z = z->parent;
					_right_rotate( z );
				}
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				_left_rotate( z->parent->parent );
			}
		}
	}
	root->color = BLACK;
}

void RB_Tree::_left_rotate( node* x ) {
	node* y = x->right;
	x->right = y->left;
	y->left->parent = x;
	y->parent = x->parent;
	if ( x->parent == nil ) root = y;
	else if ( x == x->parent->left ) x->parent->left = y;
	else x->parent->right = y;
	y->left = x;
	x->parent = y;
}

void RB_Tree::_right_rotate( node* x ) {
	node* y = x->left;
	x->left = y->right;
	y->right->parent = x;
	y->parent = x->parent;
	if ( x->parent == nil ) root = y;
	else if ( x == x->parent->left ) x->parent->left = y;
	else x->parent->right = y;
	y->right = x;
	x->parent = y;
}

node* RB_Tree::del( int key ) {
	node* z = _find( root, key );
	/*
	return NULL, not return nil
	*/
	if ( z == nil ) return NULL;
	return _del_aux( z );
}

node* RB_Tree::_del_aux( node* z ) {
	node* x;
	node* y;
	if ( z->left == nil || z->right == nil ) y = z;
	else y = _tree_successor( z );
	if ( y->left != nil ) x = y->left;
	else x = y->right;
	x->parent = y->parent;
	if ( y->parent == nil ) root = x;
	else if ( y == y->parent->left ) y->parent->left = x;
	else y->parent->right = x;
	if ( y != z ) z->key = y->key;
	if ( y->color == BLACK ) _del_fixup( x );
	return y;
}

void RB_Tree::_del_fixup( node* x ) {
	while ( x != root && x->color == BLACK ) {
		if ( x == x->parent->left ) {
			node* w = x->parent->right;
			if ( w->color == RED ) {
				w->color = BLACK;
				x->parent->color = RED;
				_left_rotate( x->parent );
				w = x->parent->right;
			}
			if ( w->left->color == BLACK && w->right->color == BLACK ) {
				w->color = RED;
				x = x->parent;
			} else {
				if ( w->right->color == BLACK ) {
					w->left->color = BLACK;
					w->color = RED;
					_right_rotate( w );
					w = x->parent->right;
				}
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->right->color = BLACK;
				_left_rotate( x->parent );
				x = root;
			}
		} else {
			node* w = x->parent->left;
			if ( w->color == RED ) {
				w->color = BLACK;
				x->parent->color = RED;
				_right_rotate( x->parent );
				w = x->parent->left;
			}
			if ( w->left->color == BLACK && w->right->color == BLACK ) {
				w->color = RED;
				x = x->parent;
			} else {
				if ( w->left->color == BLACK ) {
					w->right->color = BLACK;
					w->color = RED;
					_left_rotate( w );
					w = x->parent->left;
				}
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->left->color = BLACK;
				_right_rotate( x->parent );
				x = root;
			}
		}
	}
	x->color = BLACK;
}

node* RB_Tree::_tree_successor( node* x ) {
	if ( x->right != nil ) return _tree_minimum( x->right );
	node* y = x->parent;
	/*
	caution: y != nil
	*/
	while ( y != nil && x == y->right ) {
		x = y;
		y = y->parent;
	}
	return y;
}

node* RB_Tree::_find( node* curRoot, int key ) {
	if ( curRoot == nil || curRoot->key == key ) return curRoot;
	else if ( key < curRoot->key ) return _find( curRoot->left, key );
	else return _find( curRoot->right, key );
}

node* RB_Tree::_tree_minimum( node* x ) {
	while ( x->left != nil ) x = x->left;
	return x;
}

int
main( void ) {
	RB_Tree rb_tree;

	rb_tree.print();
	cout << endl;
	
	for ( int i = 0; i < 20; ++i ) {
		rb_tree.insert( i );

		rb_tree.print();
		cout << endl;
	}

	for ( i = 0; i < 20; ++i ) {
		node* n = rb_tree.del( i );

		cout << n->key << endl;
		rb_tree.print();
		cout << endl;
	}
	
	return 0;
}