#include <iostream>
#include <algorithm>
#include <list>
#include <map>
using	namespace	std;

int min(int a, int b, int c) {
	int t[] = {a, b, c};
	sort(t, t + 3);
	return t[0];
}

int LevenshteinDistance(char *str1, char *str2) {
    int n1 = strlen(str1);
    int n2 = strlen(str2);
	if (n1 == 0) return n2;
	if (n2 == 0) return n1;
	if (*str1 == *str2) return LevenshteinDistance(str1 + 1, str2 + 1);
	else return 1 + min(LevenshteinDistance(str1 + 1, str2 + 1), LevenshteinDistance(str1 + 1, str2), LevenshteinDistance(str1, str2 + 1));
}

struct Node {
    char *word;
    map<int, Node*> children;

    Node(char *pword) : word(pword) {}

    bool containsKey(int key) {
		return children.count(key) > 0;
    }

    void addChild(int key, char *word) {
        children[key] = new Node(word);
    }

	list<int>& getKeys() {
		list<int>* lst = new list<int>();
		for(map<int, Node*>::iterator it = children.begin(); it != children.end(); ++it)
			lst->push_back(it->first);
		return *lst;
    }
};

class BKTree {
public:
	BKTree() : _root(NULL) {}
    void add(char *word);
    list<char*>& search(char *word, int d);
private:
    void _search_aux(Node *node, list<char*>& lst, char *word, int d);

    Node *_root;
};

void BKTree::add(char *word) {
    if (!_root) {
        _root = new Node(word);
        return;
    }
    Node *curNode = _root;

    int dist = LevenshteinDistance(curNode->word, word);
    while (curNode->containsKey(dist)) {
        if (dist == 0) return;
        curNode = curNode->children[dist];
        dist = LevenshteinDistance(curNode->word, word);
    }
    curNode->addChild(dist, word);
}

list<char*>& BKTree::search(char *word, int d) {
    list<char*>* lst = new list<char*>();
    _search_aux(_root, *lst, word, d);
	return *lst;
}

void BKTree::_search_aux(Node *node, list<char*>& lst, char *word, int d) {
    int curDist = LevenshteinDistance(node->word, word);
    int minDist = curDist - d;
    int maxDist = curDist + d;

    if (curDist <= d) lst.push_back(node->word);

    list<int> childrenDist = node->getKeys();
    for (list<int>::iterator it = childrenDist.begin(); it != childrenDist.end(); ++it)
        if (*it >= minDist && *it <= maxDist)
            _search_aux(node->children[*it], lst, word, d);
}

int main() {
	char *words[10] = {
		"GAME", "FAME", "SAME", "GATE",	"GAIN",
		"GAY", "FRAME", "HOME",	"AIM", "ACM",
	};

	BKTree bkt;

	for (int i = 0; i < 10; ++i)
		bkt.add(words[i]);

	list<char*> lst = bkt.search("GAIE", 1);

	for (list<char*>::iterator it = lst.begin(); it != lst.end(); ++it)
		cout << *it << endl;
	
	return 0;
}
