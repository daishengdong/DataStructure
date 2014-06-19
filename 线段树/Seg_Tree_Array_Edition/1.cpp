#include <iostream>
using	namespace	std;

const int MAXN = 8;

struct node {
	int left, right, mid, cover;
};

node seg_tree[ 3 * MAXN ];

void make( int l, int r, int num ) {
	seg_tree[ num ].left = l;
	seg_tree[ num ].right = r;
	seg_tree[ num ].mid = ( l + r ) / 2;
	if ( l + 1 != r ) {
		make( l, seg_tree[ num ].mid, 2 * num );
		make( seg_tree[ num ].mid, r, 2 * num + 1 );
	}
}

void insert( int l, int r, int num ) {
	if ( seg_tree[ num ].left == l && seg_tree[ num ].right == r ) {
		seg_tree[ num ].cover = 1;
		return;
	}

	if ( r <= seg_tree[ num ].mid ) insert( l, r, 2 * num );
	else if ( l >= seg_tree[ num ].mid ) insert( l, r, 2 * num + 1 );
	else {
		insert( l, seg_tree[ num ].mid, 2 * num );
		insert( seg_tree[ num ].mid, r, 2 * num + 1 );
	}
}

bool del( int l, int r, int num ) {
	if ( seg_tree[ num ].left + 1 == seg_tree[ num ].right ) {
		int cover = seg_tree[ num ].cover;
		seg_tree[ num ].cover = 0;
		return cover;
	}

	if ( seg_tree[ num ].cover == 1 ) {
		seg_tree[ num ].cover = 0;
		seg_tree[ 2* num ].cover = 1;
		seg_tree[ 2 * num + 1 ].cover = 1;
	}

	if ( r <= seg_tree[ num ].mid ) return del( l, r, 2 * num );
	else if ( l >= seg_tree[ num ].mid ) return del( l, r, 2 * num + 1 );
	else return del( l, seg_tree[ num ].mid, 2 * num ) &&
				del( seg_tree[ num ].mid, r, 2 * num + 1 );

}

int cal( int num ) {
	if ( seg_tree[ num ].cover ) return seg_tree[ num ].right - seg_tree[ num ].left + 1;
	if ( seg_tree[ num ].left + 1 == seg_tree[ num ].right ) return 0;
	return cal( 2 * num ) + cal( 2 * num + 1 );

}

int
main( void ) {

	return 0;
}