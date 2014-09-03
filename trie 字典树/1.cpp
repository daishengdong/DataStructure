/* ===========================================
 * trie 树实现
 * 功能：输入字典，建立字典树，实现快速查询
 * Date: 2012.9.11   
 * Author: daishengdong
 * School: HDU
 * Email: daishengdong@gmail.com
 ===========================================*/

#include <iostream>
#include <fstream>
#include <string.h>
#include <windows.h>
using	namespace	std;

#define	MAXSIZE		26
#define	NAMELENGTH	30
#define	REMOVE		0
#define	SEARCH		1

typedef struct node {
	bool is_ultimate;
	int count;
	node* children[MAXSIZE];

	node() {
		is_ultimate = false;
		count = 0;
		memset(children, NULL, sizeof(children));
	}
} trie_node, *ptr_trie_node;

class trie_tree{
public:
	ptr_trie_node root;
	
	trie_tree();
	void insert(char* content);
	void func(char* content, int flag);
	void destroy();

private:
	bool search(char* content);
	bool remove(char* content);
	void release(ptr_trie_node cur_root);
};

trie_tree::trie_tree() {
	root = new trie_node();
}

void trie_tree::insert(char* content) {
	ptr_trie_node cur_node = root;
	for (int i = 0; i < strlen(content); ++i) {
		int index = content[i] - 'a';
		if (!cur_node->children[index]) {
			// if current node's index-th child is NULL
			cur_node->children[index] = new trie_node();
		}
		cur_node = cur_node->children[index];
		if (i == strlen(content) - 1) {
			// if this is the last letter
			cur_node->is_ultimate = true;
			++cur_node->count;
		}
	}
}

void trie_tree::func(char* content, int flag) {
	if (flag == REMOVE) {
		if (remove(content)) {
			cout << "remove " << content << " successfully!" << endl;
		} else {
			cout << "remove " << content << " failed!" << endl;
		}
	} else if (flag == SEARCH) {
		if (search(content)) {
			cout << content << " exits!" << endl;
		} else {
			cout << content << " not found!" << endl;
		}
	} else {
		cout << flag << " is not a legal param!" << endl;
	}
}

bool trie_tree::remove(char* content) {
	if (!search(content)) {
		cout << content << " does not exits, cannot remove!" << endl;
		return false;
	}

	ptr_trie_node cur_node = root;
	for (int i = 0; i < strlen(content); ++i) {
		int index = content[i] - 'a';
		cur_node = cur_node->children[index];
	}
	cur_node->is_ultimate = false;
	return true;
}

bool trie_tree::search(char* content) {
	ptr_trie_node cur_node = root;
	for (int i = 0; i < strlen(content); ++i) {
		int index = content[i] - 'a';
		if (cur_node->children[index]) {
			// if the index-th child of current node is not NULL
			// stands for the content[i] exits in the children of current node

			if (i == strlen(content) - 1) {
				// if this is the last letter of the content
				if (cur_node->children[index]->is_ultimate) {
					// if this is an ultimate node, return true
					return true;
				} else {
					return false;
				} // IF to judge if ultimate
			} else {
				cur_node = cur_node->children[index];
			} // IF to judge if last
		} else {
			return false;
		} // IF to judge if exits
	} // FOR
	return false;
}

void trie_tree::destroy() {
	release(root);
}

void trie_tree::release(ptr_trie_node cur_root) {
	for (int i = 0; i < MAXSIZE; ++i) {
		if (cur_root->children[i]) {
			release(cur_root->children[i]);
		}
	}
	delete cur_root;
}

int main(void) {
	char name[NAMELENGTH];
	trie_tree* name_dic = new trie_tree();
	ifstream ifs("dic.txt");

	if (!ifs) {
		cout << "open dic file failed!" << endl;
		system("pause");
		exit(1);
	}

	while (ifs.peek() != -1) {
		ifs >> name;
		name_dic->insert(name);
	}
	cout << "trie tree build complete!" << endl;
	cout << "==========================" << endl << endl;

	char name_to_run[NAMELENGTH];
	int flag;
	bool go_on = true;
	while (go_on) {
		cin >> name_to_run >> flag;
		name_dic->func(name_to_run, flag);
		cout << "continue ? (y/n) ";
		char yes_no;
		cin >> yes_no;
		go_on = (yes_no == 'n' ? false : true);
	}

	name_dic->destroy();

	return 0;
}
