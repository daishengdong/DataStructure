#include <iostream>
#include <stack>
using	namespace	std;

typedef struct _node {
	int value;
	_node* next;
	_node() {
		value = 0;
		next = NULL;
	}
} node, * ptr_node;

class list {
	public:
		list();
		ptr_node add(int);
		void add(ptr_node);
		void reverse(list&);
		void print();
	private:
		ptr_node head;		// head node
		int length;
};

list::list() {
	head = new node();
	length = 0;
}

ptr_node list::add(int value) {
	ptr_node tmp = head;
	while (tmp->next) tmp = tmp->next;

	ptr_node new_node = new node();
	new_node->value = value;
	tmp->next = new_node;
	++length;

	return new_node;
}

void list::add(ptr_node new_node) {
	ptr_node tmp = head;
	while (tmp->next) tmp = tmp->next;

	tmp->next = new_node;
	new_node->next = NULL;
	++length;
}

void list::reverse(list& lb) {
	stack< ptr_node > tmpStack;
	ptr_node tmp = head;
	while (tmp->next) {
		tmpStack.push(tmp->next);
		tmp = tmp->next;
	}

	while (!tmpStack.empty()) {
		lb.add(tmpStack.top());
		tmpStack.pop();
	}
}

void list::print() {
	ptr_node tmp = head;
	while (tmp->next) {
		cout << tmp->next->value << endl;
		tmp = tmp->next;
	}
}

int main(int argc, const char *argv[]) {
	list la, lb;
	for (int i = 0; i < 10; ++i) la.add(i);

	la.reverse(lb);
	lb.print();

	return 0;
}
