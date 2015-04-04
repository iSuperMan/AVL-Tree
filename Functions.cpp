#include"Main.h"
#include<iostream>
#include<string.h>

void MainMenu(Root* root)
{
	BuildTreeFromFile(root);
	char k;
	do{
		std::cout << "\n\nAdd Node --> 1\n";
		std::cout << "Delete Node --> 2\n";
		std::cout << "Print All Nodes -->3\n";
		std::cout << "Exit --> 0\n\n";
		std::cin >> k;
		Controls(root, k);
	} while (k != '0');
	FreeTree(root->pointer);
}

void Controls(Root* root, char k)
{
	switch (k)
	{
	case '1':{
		Node* new_node = ScanNode();
		PrintFileNode(new_node);
		AddNode(root, new_node);
	}
		break;
	case '2':
		DeleteNode(root);
		break;
	case '3':
		if (root->pointer == NULL)
			std::cout << "\nTree is empty\n";
		else 
			PrintTree(root->pointer);
		break;
	}
}

Node* CreateNode(int new_key, char *new_value)
{
	Node *new_node = new Node();
	new_node->key = new_key;
	strcpy_s(new_node->value, new_value);
	new_node->left = new_node->right=new_node->parent = NULL;
	new_node->height = 1;
	return new_node;
}

Root* CreateRoot()
{
	Root* new_root= new Root();
	new_root->pointer = NULL;
	return new_root;
}

Node* AddNode(Root *root, Node *add_element)
{
	if (root->pointer == NULL)
		root->pointer = add_element;
	else
	{
		//std::cout << "\nDO: "; TreeVisible(root->pointer);
		BalanceInsert(root->pointer, add_element,root);
		//std::cout << "\nPOSLE: "; TreeVisible(root->pointer);
	}
	return add_element;
}

void FreeTree(Node *root)
{
	if (root == NULL) return;
	if (root->left != NULL)
		FreeTree(root->left);
	if (root->right != NULL)
		FreeTree(root->right);
	if (root!=NULL)
		delete root;
}

void PrintTree(Node* root)
{
	if (root == NULL)
		return;
	PrintTree(root->left);
	PrintNode(root);
	PrintTree(root->right);
}

//возвращает NULL, если узел не найден
Node* SearchNode(Node* root, int k)
{
	if (root == NULL || k == root->key)
		return root;
	if (k < root->key)
		SearchNode(root->left, k);
	else
		SearchNode(root->right, k);
}

Node* MinimumNode(Node* root)
{
	while (root->left != NULL)
		root = root->left;
	return root;
}

//scan needed values from console and return ready Node 
Node* ScanNode()
{
	int k;
	char value[50];
	std::cout << std::endl << "Value: ";
	std::cin >> value;
	std::cout << "Key: ";
	std::cin >> k;
	return CreateNode(k,value);
}

void PrintNode(Node* item)
{
	std::cout << std::endl << "Value: " << item->value;
	std::cout << std::endl << "Key: " << item->key << std::endl << std::endl;
}

void PrintFileNode(Node* print_node)
{
	FILE* f;
	f = fopen("phonebook.txt", "a");
	fprintf(f, "\n%d\n", print_node->key);
	fprintf(f, "%s", print_node->value);
	fclose(f);
}

void PrintFileNode(Node* print_node, FILE* file)
{
	fprintf(file, "\n%d\n", print_node->key);
	fprintf(file, "%s", print_node->value);
}

Node* ScanFileNode(FILE* f)
{
	int key;
	char value[50];
	fscanf(f, "%d", &key);
	fscanf(f, "%s", value);
	return CreateNode(key, value);
}

Root* BuildTreeFromFile(Root* root)
{
	FILE* file;
	if ((file = fopen("phonebook.txt", "r")) == NULL)
	{
		std::cout << "Error to open file\n";
		return root;
	}
	char *ch;
	if (getc(file) == EOF)
	{
		std::cout << "File is empty\n";
		return root;
	}
	rewind(file);
	while (!feof(file))
		AddNode(root, ScanFileNode(file));
	return root;
}

void DeleteNode(Root* root)
{
	Node* del_item;
	int k;
	std::cout << "\nInput key: ";
	std::cin >> k;
	if ( SearchNode(root->pointer, k) == NULL)
		std::cout << "\nThis node doesn't exist\n";
	else
	{
		Remove(root->pointer, k, root);
		TreeToFile(root);
		std::cout << "\nDelete is success\n";
	}
}

void TreeToFile(Root* root)
{
	FILE* file;
	file = fopen("phonebook.txt", "w");
	if (root->pointer!=NULL)
		PrintFileTreeWalk(root->pointer, file);
	fclose(file);
}

void PrintFileTreeWalk(Node* root,FILE* file)
{
	if (root == NULL)
		return;
	PrintFileTreeWalk(root->left, file);
	PrintFileNode(root,file);
	PrintFileTreeWalk(root->right, file);
}

unsigned int Height(Node* p)
{
	return p ? p->height : 0;
}

int Bfactor(Node* p)
{
	return Height(p->right) - Height(p->left);
}

void UpdateHeight(Node* p)
{
	unsigned int hl = Height(p->left);
	unsigned int hr = Height(p->right);
	p->height = (hl > hr ? hl : hr) + 1;
}

Node* RotateRight(Node* p, Root *root)
{
	Node* q = p->left;
	p->left = q->right;
	q->right = p;
	if (p == root->pointer)
		root->pointer = q;
	UpdateHeight(p);
	UpdateHeight(q);
	return q;
}

Node* RotateLeft(Node* q, Root *root)
{
	Node* p = q->right;
	q->right = p->left;
	p->left = q;
	if (q == root->pointer)
		root->pointer = p;
	UpdateHeight(q);
	UpdateHeight(p);
	return p;
}

Node* Balance(Node* p, Root *root)
{
	UpdateHeight(p);
	if (Bfactor(p) == 2)
	{
		if (Bfactor(p->right)<0)
			p->right = RotateRight(p->right,root);
		return RotateLeft(p,root);
	}
	if (Bfactor(p) == -2)
	{
		if (Bfactor(p->left) > 0)
			p->left = RotateLeft(p->left,root);
		return RotateRight(p,root);
	}
	return p;
}

Node* BalanceInsert(Node* p, Node* ins_item, Root *root)
{
	if (!p) return ins_item;
	if (ins_item->key<p->key)
		p->left = BalanceInsert(p->left, ins_item,root);
	else
		p->right = BalanceInsert(p->right, ins_item,root);
	return Balance(p,root);
}

//display tree structure (needed only for debug)
void TreeVisible(Node *r)
{
	if (r->left == NULL)
		std::cout << "{N}";
	else
	{
		std::cout << "{ ";
		TreeVisible(r->left);
		std::cout << " }";
	}
	std::cout << "{" << r->key << "}";
	if (r->right == NULL)
		std::cout << "{N}";
	else
	{
		std::cout << "{ ";
		TreeVisible(r->right);
		std::cout << " }";
	}
}

//удаление минимального узла из дерева "p", с выполнением балансировки
Node* RemoveMin(Node* p,Root* root)
{
	if (p->left == NULL)
		return p->right;
	p->left = RemoveMin(p->left,root);
	return Balance(p,root);
}

//удаление из балансированного дерева
Node* Remove(Node* p, int k, Root* root)
{
	if (!p) return NULL;
	if (k < p->key)
		p->left = Remove(p->left, k, root);
	else if (k > p->key)
		p->right = Remove(p->right, k, root);
	else //  k == p->key 
	{
		Node* q = p->left;
		Node* r = p->right;
		
		if (!r) 
		{
			if (p == root->pointer)
				root->pointer = q;
			delete p;
			return q;
		}
		Node* min = MinimumNode(r);
		min->right = RemoveMin(r, root);
		min->left = q;
		if (p == root->pointer)
			root->pointer = min;
		delete p;
		return Balance(min,root);
	}
	return Balance(p,root);
}
