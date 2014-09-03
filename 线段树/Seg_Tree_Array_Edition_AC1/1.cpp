#include <iostream>
#include <algorithm>
using	namespace	std;

const int MAXN = 20;

int hash[MAXN];
int A[MAXN >> 1];
int B[MAXN >> 1];
int H[MAXN >> 1];

struct node {
	int left, right, mid, hight;
};

node seg_tree[3 * MAXN];

void make(int l, int r, int num) {
	seg_tree[num].left = l;
	seg_tree[num].right = r;
	seg_tree[num].mid = (l + r) / 2;
	seg_tree[num].hight = 0;
	if (l + 1 != r) {
		make(l, seg_tree[num].mid, 2 * num);
		make(seg_tree[num].mid, r, 2 * num + 1);
	}
}

void insert(int l, int r, int h, int num) {
	if (seg_tree[num].left == l && seg_tree[num].right == r) {
		if (seg_tree[num].hight < h || !seg_tree[num].hight)
            seg_tree[num].hight = h;
		return;
	}

	if (r <= seg_tree[num].mid) insert(l, r, h, 2 * num);
	else if (l >= seg_tree[num].mid) insert(l, r, h, 2 * num + 1);
	else {
		insert(l, seg_tree[num].mid, h, 2 * num);
		insert(seg_tree[num].mid, r, h, 2 * num + 1);
	}
}

int cal(int h, int num) {
	if (h > seg_tree[num].hight)
        seg_tree[num].hight = h;
	if (seg_tree[num].left + 1 == seg_tree[num].right)
        return seg_tree[num].hight * (hash[seg_tree[num].right] - hash[seg_tree[num].left]);
	return cal(seg_tree[num].hight, 2 * num) + cal(seg_tree[num].hight, 2 * num + 1);
}

inline int getn(int p, int n) {
	for (int i = 0; i < 2 * n; ++i)
		if (hash[i] == p) return i;
}

int
main(void) {
	int n;
	cin >> n;
    make(0, 2 * n, 1);
	for (int i = 0, j = 0; i < n; ++i, ++j) {
		cin >> A[i] >> B[i] >> H[i];
		hash[j] = A[i];
		hash[++j] = B[i];
	}

	sort(hash, hash + 2 * n);

	for (i = 0; i < n; ++i)
		insert(getn(A[i], n), getn(B[i], n), H[i], 1);
	cout << cal(seg_tree[1].hight, 1) << endl;

	return 0;
}
