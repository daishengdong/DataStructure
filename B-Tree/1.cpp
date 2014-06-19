#include <iostream>
using	namespace	std;

#define m	3

struct BTNode {
	int keynum;
	struct BTNode* parent;
	int key[ m + 1 ];	// 0th key is not used
	struct BTNode *ptr[ m + 1 ];
	char* *recptr[ m + 1 ];
};

struct result {
	BTNode* pt;
	int i;
	int tag;

	result( BTNode* pt, int i, int tag ) : pt( pt ), i( i ), tag( tag ) {}
};

class BTree {
public:
	result search( int key );
	void insert( int key );

private:
	int _search_aux( BTNode* curNode, int key );
	void _insert_aux( int key, BTNode* curNode, int i );
	void _insert_to_tree( BTNode* curNode, int i, int key, BTNode* ptr );
	void _split( BTNode* curNode, int s, BTNode* ptr );

	BTNode* _root;
};

result BTree::search( int key ) {
	BTNode* curNode = _root, *parent = NULL;
	bool found = false;
	int i = 0;
	while ( curNode && !found ) {
		i = _search_aux( curNode, key );

		if ( i > 0 && curNode->key[ i ] == key ) found = true;
		else {
			parent = curNode;
			curNode = curNode->ptr[ i ];
		}
	}
	if ( found ) return result( curNode, i, 1 );
	else return result( parent, i, 0 );
}

int BTree::_search_aux( BTNode* curNode, int key ) {
	for ( int i = 1; i <= curNode->keynum; ++i )
		if ( key < curNode->key[ i ] )
			return i - 1;
		else if ( curNode->key[ i ] <= key && key < curNode->key[ i + 1 ] )
			return i;
	return curNode->keynum;
}

void BTree::insert( int key ) {
	result r = search( key );
	_insert_aux( key, r.pt, r.i );
}

void BTree::_insert_aux( int key, BTNode* curNode, int i ) {
	int x = key;
	BTNode* ap = NULL;
	bool finished = false;
	while ( curNode && !finished ) {
		_insert_to_tree( curNode, i, key, ap );
		if ( curNode->keynum < m ) finished = true;
		else {
			int s = ( m + 1 ) / 2;
			_split( curNode, s, ap );
			x = curNode->key[ s ];
			curNode = curNode->parent;
			if ( curNode ) i = _search_aux( curNode, x );
		}
	}
	// if ( !finished ) newRoot( T, q, x ap );
}

void BTree::_insert_to_tree( BTNode* curNode, int i, int key, BTNode* ptr ) {
	for ( int it = curNode->keynum; it > i; --it ) {
		curNode->key[ it + 1 ] = curNode->key[ it ];
	}
	curNode->key[ i + 1 ] = key;
	curNode->ptr[ i + 1 ] = ptr;
	++curNode->keynum;
}

void BTree::_split( BTNode* curNode, int s, BTNode* ptr ) {
	BTNode* newNode = new BTNode;
	for ( int i = s + 1, j = 1; i <= m; ++i, ++j ) {
		newNode->key[ j ] = curNode->key[ i ];
		newNode->ptr[ j - 1 ] = curNode->ptr[ i - 1 ];
		newNode->recptr[ j ] = curNode->recptr[ i ];
	}
	newNode->keynum = m - s;
	curNode->keynum = s - 1;

	ptr = newNode;
}


int
main( void ) {

	return 0;
}