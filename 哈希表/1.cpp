#include <iostream>
#include <string>
using	namespace	std;

const int P = 97;
const int SIZE = 100;
const int nameCount = 30;
const int RANDOM_SIZE = 10;
const int RANDOM[RANDOM_SIZE] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19 };

const string NAME[nameCount] = { 
	"Huang Xiaodong", "Dai Shengdong", "Chen qiu", "Zheng Jiefeng", "Li Zhiqi",
	"Fu Tingting", "Wan Jian", "Xu Xianghua", "Yan Fei", "Zhou Jielun",
	"Cai Yilin", "Wang Lihong", "Xu Zhimo", "Li Qingzhao", "Wang Xiaobo",
	"Han Han", "Chai Jing", "Bai Yansong", "Huang Xiang", "Li Xiaofeng",
	"Lu Weiliang", "Jian Sheng", "Shang Jin", "Bing nv", "Zhou si",
	"Xiao Xiao", "Chao Xi", "Sheng Ling", "Ka er", "Sheng Niu" };

typedef class hashTable {
public:
	void buildTable();
	void search(string name);
private:
	int hash(int index, int d);
	int hash(string name);
	string table[SIZE];
} * HashTable;

void hashTable::buildTable() {
	int i;
	for (i = 0; i < SIZE; ++i) {
		table[i] = "#";
	}
	for (i = 0; i < nameCount; ++i) {
		int j = 0;
		int index = hash(NAME[i]);

		while (table[index] != "#") {
			index = hash(index, RANDOM[j]);
			++j;
			if (j == RANDOM_SIZE || index >= SIZE) {
				cout << "ERROR: table is full!" << endl;
				exit(0);
			}
		}
		table[index] = NAME[i];
	}
	cout << "INFO: table build done!" << endl;
}

void hashTable::search(string name) {
	int j = 0;
	int index = hash(name);
	while (table[index] != name) {
		index = hash(index, RANDOM[j]);
		++j;
		if (j == RANDOM_SIZE || index >= SIZE) {
			cout << "INFO: no such person!" << endl;
			return;
		}
	}
	cout << "INFO: " << name << " is found!" << endl;
	cout << "INDEX: " << index << endl;
}

int hashTable::hash(int index, int d) {
	return (index + d) % P;
}

int hashTable::hash(string name) {
	int sum = 0;
	for (int i = 0; i < name.length(); ++i) {
		sum += name[i];
	}

	return sum % P;
}

int main() {
	HashTable ht = new hashTable();
	ht->buildTable();

	char name[30];
	while (gets(name)) {
		ht->search(name);
	}

	return 0;
}