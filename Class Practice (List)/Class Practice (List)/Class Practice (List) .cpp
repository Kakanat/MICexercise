#include <iostream>

class Mylist {
public:
	Mylist();
	~Mylist();

	void pushBackNode(double val);
	void replaceNode(double val, int pos);
	void insertNode(double val, int pos);
	void popBackNode();
	void deleteNode(int pos);
	void display();

private:
	struct Node {
		double node_value;
		Node *prev_node; //1.Why pointer?	2.Is it OK to use Node itself in the definition of Node?
		Node *next_node;
		Node() {
			prev_node = this;
			next_node = this;
		}
		Node(int value, Node* prev, Node* next) {
			node_value = value;
			prev_node = prev;
			next_node = next;
		}
	};
	Node *dummy_node;
	Node *current_node;
};

Mylist::Mylist() {
	dummy_node = new Node();
	current_node = new Node();
};

Mylist::~Mylist() {
	while (current_node != dummy_node) {
		Mylist::popBackNode();
	}
	delete current_node;
}

void Mylist::pushBackNode(double val) {
	current_node = dummy_node->prev_node;
	Node *added_node = new Node(val, current_node, dummy_node);
	dummy_node->prev_node = added_node;
	current_node->next_node = added_node;
}

void Mylist::replaceNode(double val, int pos) {
	current_node = dummy_node;
	int i = 0;
	while (i < pos) {
		current_node = current_node->next_node;
		i++;
	}
	current_node->node_value = val;
}

void Mylist::insertNode(double val, int pos) {
	current_node = dummy_node;
	if (pos > 0) {
		int i = 0;
		while (i < pos - 1) {
			current_node = current_node->next_node;
			i++;
		}
		Node *inserted_node = new Node(val, current_node, current_node->next_node);
		current_node->next_node->prev_node = inserted_node;
		current_node->next_node = inserted_node;
	}
	else if (pos < 0) {
		pos = -pos;
		int i = 0;
		while (i < pos) {
			current_node = current_node->prev_node;
			i++;
		}
		Node *inserted_node = new Node(val, current_node, current_node->next_node);
		current_node->next_node->prev_node = inserted_node;
		current_node->next_node = inserted_node;
	}
	else {
		std::cerr << "Do not enter 0 to the place you want to insert.\n";
	}
}

void Mylist::popBackNode() {
	current_node = dummy_node->prev_node;
	current_node->prev_node->next_node = dummy_node;
	dummy_node->prev_node = current_node;
	delete current_node;
	current_node = current_node->prev_node;
}

void Mylist::deleteNode(int pos) {
	current_node = dummy_node;
	if (pos > 0) {
		int i = 0;
		while (i < pos) {
			current_node = current_node->next_node;
			i++;
		}
		current_node->prev_node->next_node = current_node->next_node;
		current_node->next_node->prev_node = current_node->prev_node;
		delete current_node;
		current_node = current_node->prev_node;
	}
	else if (pos < 0) {
		pos = -pos;
		int i = 0;
		while (i < pos) {
			current_node = current_node->prev_node;
			i++;
		}
		current_node->prev_node->next_node = current_node->next_node;
		current_node->next_node->prev_node = current_node->prev_node;
		delete current_node;
		current_node = current_node->prev_node;
	}
	else {
		std::cerr << "Do not enter 0 to the place you want to insert.\n";
	}
}

void Mylist::display() {
	std::cout << "[";
	current_node = dummy_node->next_node;
	while (current_node != dummy_node) {
		std::cout << current_node->node_value << "";
		current_node = current_node->next_node;
		if (current_node != dummy_node) { std::cout << " , "; }
	}
	std::cout << "]\n";
}

int main(int argc, const char * argv[]) 
{
	Mylist mylist;
	mylist.pushBackNode(0);
	mylist.display();
	mylist.pushBackNode(5);
	mylist.display();
	mylist.replaceNode(4, 1);
	mylist.display();
	mylist.insertNode(2, 1);
	mylist.display();
	mylist.insertNode(3, 2);
	mylist.display();
	mylist.insertNode(7, 5);
	mylist.display();
	mylist.insertNode(8, -1);
	mylist.display();
	mylist.insertNode(6, -3);
	mylist.display();
	mylist.insertNode(1, -8);
	mylist.display();
	mylist.popBackNode();
	mylist.display();
	mylist.deleteNode(2);
	mylist.display();
	mylist.deleteNode(6);
	mylist.display();
	mylist.deleteNode(-1);
	mylist.display();
	mylist.deleteNode(-2);
	mylist.display();
	mylist.deleteNode(-3);
	mylist.display();

	system("pause");
	return 0;
}