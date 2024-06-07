#include <iostream>
using namespace std;
void menu() {
	cout << "\n1 - префиксный порядок\n";
	cout << "2 - инфиксный порядок\n";
	cout << "3 - постфиксный порядок\n";
	cout << "4 - вывод сверху вниз, слева направо\n";
	cout << "5 - вывод сверху вниз, справа налево\n";
	cout << "6 - вывод снизу вверх, слева направо\n";
	cout << "7 - вывод снизу вверх, справа налево\n\n";
}
void instruction() {
	cout << "1 - добавить элемент в дерево\n";
	cout << "2 - посчитать количество элементов в дереве\n";
	cout << "3 - проверить, содержится ли указанный элемент в дереве\n";
	cout << "4 - очистить дерево\n";
	cout << "5 - удалить указанный элемент из дерева\n";
	cout << "6 - вывести дерево одним массивом\n";
	cout << "7 - повернуть дерево влево относительно узла\n";
	cout << "8 - повернуть дерево вправо относительно узла\n";
	cout << "9 - сбалансировать дерево\n";
	cout << "0 - завершение работы программы\n\n";
}

struct Node {
	int param = 0;
	Node* left = nullptr;
	Node* right = nullptr;
	Node* up = nullptr;
};
struct Tree {
	int counting = 0;
	Node* root = nullptr;
	enum Order { Prefix, Infix, Postfix, LevelsUpLeft, LevelsUpRight, LevelsDownLeft, LevelsDownRight };

	struct NodeQ {
		Node* value = nullptr;
		NodeQ* next = nullptr;
	};
	struct Queue {
		NodeQ* first = nullptr;
		NodeQ* last = nullptr;
		int counting = 0;

		void queue(Node* Node) {
			NodeQ* newNode = new NodeQ();
			newNode->value = Node;
			if (counting == 0) {
				first = newNode;
				last = newNode;
			}
			else {
				last->next = newNode;
				last = newNode;
			}
			counting++;
		}
		int unqueue() {
			if (counting > 0) {
				NodeQ* newNode = first->next;
				int value = first->value->param;
				delete first;
				first = newNode;
				counting--;
				return value;
			}
			else { return 0; }
		}
		int count() { return counting; }
	};

	void BalanceTree() {
		int* arr = ToArray(Postfix);
		int deep = 0;
		for (int i = 0; i < count(); i++) {
			deep = 0;
			Node* node = ToNode(arr[i]);
			Deep(node, deep);
			if (deep > 2) {
				int right = 0, left = 0;
				Deep(node->right, right);
				Deep(node->left, left);
				while (abs(right - left) > 1) {
					if (left > right) {
						right = 0;
						left = 0;
						Deep(node->left->right, right);
						Deep(node->left->left, left);
						if (right > left) { ToLeft(node->left->param); }
						ToRight(node->param);
					}
					else {
						right = 0;
						left = 0;
						Deep(node->right->right, right);
						Deep(node->right->left, left);
						if (left > right) { ToRight(node->right->param); }
						ToLeft(node->param);
					}
					node = node->up;
					right = 0;
					left = 0;
					Deep(node->right, right);
					Deep(node->left, left);
				}
			}
		}
	}

	void Deep(Node* node, int& maximum, int deep = 1) {
		if (node == nullptr) { return; }
		if (node->right != nullptr) { Deep(node->right, maximum, deep + 1); }
		if (node->left != nullptr) { Deep(node->left, maximum, deep + 1); }
		maximum = max(deep, maximum);
	}

	void isBalance(int& value, Node* node = nullptr) {
		if (node == nullptr) { node = root; }
		if ((counting == 0) || (node->left == nullptr && node->right == nullptr)) { return; }
		int right = 0, left = 0;
		Deep(node->right, right);
		Deep(node->left, left);
		if (abs(left - right) > 1) { value++; }
		if (node->left != nullptr) { isBalance(value, node->left); }
		if (node->right != nullptr) { isBalance(value, node->right); }
	}

	void Balance() {
		while (true) {
			int value = 0;
			isBalance(value);
			if (value == 0) { break; }
			BalanceTree();
		}
	}

	int* ToArray(Order order = Infix) {
		if (counting == 0) { return 0; }
		int* arr = new int[counting];
		int index = 0;
		if (order == LevelsUpLeft) {
			arr = ToLevels(LevelsUpLeft);
			return arr;
		}
		if (order == LevelsUpRight) {
			arr = ToLevels(LevelsUpRight);
			return arr;
		}
		if (order == LevelsDownLeft) {
			arr = ToLevels(LevelsDownLeft);
			return arr;
		}
		if (order == LevelsDownRight) {
			arr = ToLevels(LevelsDownRight);
			return arr;
		}
		input(arr, 0, order, root);
		return arr;
	}
	int input(int* arr, int index, Order order, Node* node) {
		if (node == nullptr) return index;
		if (order == Prefix) {
			arr[index] = node->param;
			index++;
		}
		index = input(arr, index, order, node->left);
		if (order == Infix) {
			arr[index] = node->param;
			index++;
		}
		index = input(arr, index, order, node->right);
		if (order == Postfix) {
			arr[index] = node->param;
			index++;
		}
	}

	int* ToLevels(Order order) {
		int index = 0;
		int* arr = new int[counting];
		Queue queue;
		queue.queue(root);
		while (index != counting) {
			for (int i = queue.count(); i > 0; i--) {
				if (order == LevelsUpLeft || order == LevelsDownRight)
					if (queue.first->value->left != nullptr) {
						queue.queue(queue.first->value->left);
					}
				if (queue.first->value->right != nullptr) {
					queue.queue(queue.first->value->right);
				}
				if (order == LevelsUpRight || order == LevelsDownLeft) {
					if (queue.first->value->left != nullptr) {
						queue.queue(queue.first->value->left);
					}
				}
				arr[index] = queue.unqueue();
				index++;
			}
		}
		if (order == LevelsDownLeft || order == LevelsDownRight) {
			reverse(arr, arr + counting);
		}
		return arr;
	}

	Node* ToNode(int value) {
		Node* node = root;
		while (node->param != value) {
			if (value > node->param) { node = node->right; }
			else { node = node->left; }
		}
		return node;
	}
	void ToLeft(int value) {
		if (!contains(value)) { return; }
		Node* node = ToNode(value);
		if (node->right == nullptr) { return; }
		if (node->right->left == nullptr) {
			node->right->up = node->up;
			node->up = node->right;
			node->right = nullptr;
			node->up->left = node;
		}
		else {
			node->right = node->right->left;
			node->right->up->left = node;
			node->right->up->up = node->up;
			node->up = node->right->up;
			node->right->up = node;
		}
		if (node == root) { root = node->up; }
		else {
			if (node->up->up->right == node) { node->up->up->right = node->up; }
			else { node->up->up->left = node->up; }
		}
	}
	void ToRight(int value) {
		if (!contains(value)) { return; }
		Node* node = ToNode(value);
		if (node->left == nullptr) { return; }
		if (node->left->right == nullptr) {
			node->left->up = node->up;
			node->up = node->left;
			node->left = nullptr;
			node->up->right = node;
		}
		else {
			node->left = node->left->right;
			node->left->up->right = node;
			node->left->up->up = node->up;
			node->up = node->left->up;
			node->left->up = node;
		}
		if (node == root) { root = node->up; }
		else {
			if (node->up->up->left == node) { node->up->up->left = node->up; }
			else { node->up->up->right = node->up; }
		}
	}

	int count() { return counting; }

	void add(int value) {
		if (contains(value)) { return; }
		Node* newNode = new Node();
		newNode->param = value;
		counting++;
		if (counting == 1) {
			root = newNode;
			return;
		}
		Node* node = root;
		while (true) {
			if (value > node->param) {
				if (node->right == nullptr) {
					node->right = newNode;
					newNode->up = node;
					return;
				}
				else { node = node->right; }
			}
			else {
				if (node->left == nullptr) {
					node->left = newNode;
					newNode->up = node;
					return;
				}
				else { node = node->left; }
			}
		}
	}

	bool contains(int value) {
		Node* node = root;
		if (counting == 0) { return false; }
		while (true) {
			if (node->param == value) { return true; }
			if (value > node->param) {
				if (node->right == nullptr) { return false; }
				else { node = node->right; }
			}
			else {
				if (node->left == nullptr) { return false; }
				else { node = node->left; }
			}
		}
	}

	int minimum(Node* node) {
		if (node->left == nullptr) { return node->param; }
		else { minimum(node->left); }
	}
	void remove(int value, Node* node = nullptr) {
		if (node == nullptr) {
			node = root;
			if (!contains(value)) { return; }
			if (counting == 1) {
				clear();
				return;
			}
		}
		if (value > node->param) { remove(value, node->right); }
		else if (value < node->param) { remove(value, node->left); }
		else {
			if (node->right != nullptr && node->left != nullptr) {
				node->param = minimum(node->right);
				remove(node->param, node->right);
			}
			else {
				if (node == root) {
					if (node->left == nullptr) { root = node->right; }
					else { root = node->left; }
					root->up = nullptr;
				}
				else if (node->left == nullptr) {
					if (node->up->left == node) { node->up->left = node->right; }
					else { node->up->right = node->right; }
					if (node->right != nullptr) { node->right->up = node->up; }
				}
				else if (node->right == nullptr) {
					if (node->up->right == node) { node->up->right = node->left; }
					else { node->up->left = node->left; }
					node->left->up = node->up;
				}
				delete node;
				counting--;
			}
		}
	}

	void clear(Node* node = nullptr) {
		if (node == nullptr) {
			node = root;
			if (counting == 0) { return; }
		}
		if (node->left != nullptr) { clear(node->left); }
		if (node->right != nullptr) { clear(node->right); }
		if (node == root) {
			root = nullptr;
			counting = 0;
		}
		delete node;
	}
};

int main() {
	system("chcp 1251>NULL");
	instruction();
	Tree tree;
	while (true) {
		cout << "\nВведите команду: ";
		int par;
		int* arr = 0;
		cin >> par;
		switch (par) {
		case 1:
			cout << "Введите число: ";
			cin >> par;
			tree.add(par);
			break;
		case 2:
			cout << "Количество элементов: " << tree.count() << endl;
			break;
		case 3:
			cout << "Введите число: ";
			cin >> par;
			if (tree.contains(par)) { cout << "Число содержится в дереве\n"; }
			else { cout << "Число не содержится в дереве\n"; }
			break;
		case 4:
			tree.clear();
			break;
		case 5:
			cout << "Введите число: ";
			cin >> par;
			tree.remove(par);
			break;
		case 6:
			menu();
			cin >> par;
			if (par == 1) { arr = tree.ToArray(tree.Prefix); }
			if (par == 2) { arr = tree.ToArray(tree.Infix); }
			if (par == 3) { arr = tree.ToArray(tree.Postfix); }
			if (par == 4) { arr = tree.ToArray(tree.LevelsUpLeft); }
			if (par == 5) { arr = tree.ToArray(tree.LevelsUpRight); }
			if (par == 6) { arr = tree.ToArray(tree.LevelsDownLeft); }
			if (par == 7) { arr = tree.ToArray(tree.LevelsDownRight); }
			cout << "\n";
			for (int i = 0; i < tree.counting; i++) { cout << arr[i] << " "; }
			cout << "\n";
			break;
		case 7:
			cout << "Введите значение узла: ";
			cin >> par;
			tree.ToLeft(par);
			break;
		case 8:
			cout << "Введите значение узла: ";
			cin >> par;
			tree.ToRight(par);
			break;
		case 9:
			tree.Balance();
			break;
		case 0:
			return 0;
		default:
			cout << "Некорректная команда!";
		}
	}
}