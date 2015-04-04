#include"Main.h"
#include<iostream>
#include<string.h>

void MainMenu(Root* root)
{
	// open file with data and build tree
	BuildTreeFromFile(root);
	
	// display options of menu and scan value from console
	char k;
	do{
		std::cout << "\n\nAdd Node --> 1\n";
		std::cout << "Delete Node --> 2\n";
		std::cout << "Print All Nodes -->3\n";
		std::cout << "Exit --> 0\n\n";
		std::cin >> k;
		
		//process the input value
		Controls(root, k);
	} while (k != '0');
	
	// before leaving, don't forget free memory
	FreeTree(root->pointer);
}

void Controls(Root* root, char k)
{
	switch (k)
	{
	case '1':{
		Node* new_node = ScanNode(); // create node
		PrintFileNode(new_node);  // record node to file
		AddNode(root, new_node);  // add node to current tree
	}
		break;
	case '2':
		DeleteNode(root); // delete node
		break;
	case '3':
		if (root->pointer == NULL)
			std::cout << "\nTree is empty\n";
		else 
			PrintTree(root->pointer);  //print all nodes to console
		break;
	}
}

Node* CreateNode(int new_key, char *new_value)
{
	Node *new_node = new Node(); 
	new_node->key = new_key;
	strcpy_s(new_node->value, new_value); // new_node->value=new_value
	new_node->left = new_node->right=new_node->parent = NULL; // reset pointers on left child, right child and parent
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
	if (root->pointer == NULL)		// if tree is empty
		root->pointer = add_element; 	// then add_element is root
	else
	{
		BalanceInsert(root->pointer, add_element,root);  // inserts an element into a tree while maintaining balance
	}
	return add_element;
}

// frees all memory of a tree
void FreeTree(Node *root)
{
	if (root == NULL) return;
	if (root->left != NULL)
		FreeTree(root->left);
	if (root->right != NULL)
		FreeTree(root->right);
	delete root;
}

//print all nodes to console
void PrintTree(Node* root)
{
	if (root == NULL)
		return;
	PrintTree(root->left);
	PrintNode(root);	// print node
	PrintTree(root->right);
}

// return node with key, equal k, if it's exist else return NULL
Node* SearchNode(Node* root, int k)
{
	if (root == NULL || k == root->key)
		return root;
	if (k < root->key)
		SearchNode(root->left, k);
	else
		SearchNode(root->right, k);
}

// return minimum node from the subtree 
Node* MinimumNode(Node* root)
{
	while (root->left != NULL)
		root = root->left;
	return root;
}

//scan key and value from console and return ready Node 
Node* ScanNode()
{
	int k;
	char value[50];
	std::cout << "\nValue: ";
	std::cin >> value;
	std::cout << "Key: ";
	std::cin >> k;
	return CreateNode(k,value);
}

// display node's value and key on console
void PrintNode(Node* item)
{
	std::cout << "\nValue: " << item->value;
	std::cout << "\nKey: " << item->key << std::endl << std::endl;
}

// record to file node's key and value
void PrintFileNode(Node* print_node)
{
	FILE* f;
	f = fopen("phonebook.txt", "a");
	fprintf(f, "\n%d\n", print_node->key);
	fprintf(f, "%s", print_node->value);
	fclose(f);
}

// record to file node's key and value
void PrintFileNode(Node* print_node, FILE* file)
{
	fprintf(file, "\n%d\n", print_node->key);
	fprintf(file, "%s", print_node->value);
}

// scan node from file and return him
Node* ScanFileNode(FILE* f)
{
	int key;
	char value[50];
	fscanf(f, "%d", &key);
	fscanf(f, "%s", value);
	return CreateNode(key, value);
}

// open file and return built tree
Root* BuildTreeFromFile(Root* root)
{
	FILE* file;
	
	// if we can't open file then we display a message and out of the function
	if ((file = fopen("phonebook.txt", "r")) == NULL)
	{
		std::cout << "Error to open file\n";
		return root;
	}
	char *ch;
	// if we were able to open the file but it's empty then display a message and out of the function
	if (getc(file) == EOF)
	{
		std::cout << "File is empty\n";
		return root;
	}
	// if file isn't empty then sets the position indicator associated with stream to the beginning of the file
	rewind(file);
	// builds tree 
	while (!feof(file))
		AddNode(root, ScanFileNode(file)); // scans node from file and adds it to tree
	return root;
}

// delete node from tree and from file
void DeleteNode(Root* root)
{
	Node* del_item;
	int k;
	std::cout << "\nInput key: ";
	std::cin >> k;	// scan key 
	// if node with this key isn't exist then display message
	if ( SearchNode(root->pointer, k) == NULL)
		std::cout << "\nThis node doesn't exist\n";
	// if node with this key is exist then remove this node from tree and record changed tree in file 
	else
	{
		Remove(root->pointer, k, root); // remove node from tree
		TreeToFile(root); // record changed tree to file
		std::cout << "\nDelete is success\n"; // display message
	}
}

// record tree to file
void TreeToFile(Root* root)
{
	FILE* file;
	file = fopen("phonebook.txt", "w");
	if (root->pointer!=NULL)
		PrintFileTreeWalk(root->pointer, file); // record in order
	fclose(file);
}

// record tree in file in order
void PrintFileTreeWalk(Node* root,FILE* file)
{
	if (root == NULL)
		return;
	PrintFileTreeWalk(root->left, file);
	PrintFileNode(root,file); // record node to file
	PrintFileTreeWalk(root->right, file);
}

// return height of subtree with root in node 'p'
unsigned int Height(Node* p)
{
	return p ? p->height : 0;
}

// return balance factor(height difference between right and left subtrees) of node 
int Bfactor(Node* p)
{
	return Height(p->right) - Height(p->left);
}

// updates heigth of subtree with root in node 'p'
void UpdateHeight(Node* p)
{
	unsigned int hl = Height(p->left); // height of left subtree
	unsigned int hr = Height(p->right); // height of right subtree
	p->height = (hl > hr ? hl : hr) + 1; // height of larger subtree +1
}

// does a right rotation of subtree with root in node 'p'
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

//does a left rotation of subtree with root in node 'p'
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

//does balancing of subtree with root in node 'p'
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

// inserts 'ins_item' into a subtree with root in node 'p' and balances this subtree
Node* BalanceInsert(Node* p, Node* ins_item, Root *root)
{
	if (!p) return ins_item;
	if (ins_item->key<p->key)
		p->left = BalanceInsert(p->left, ins_item,root);
	else
		p->right = BalanceInsert(p->right, ins_item,root);
	return Balance(p,root);
}

//deletes minimum node from subtree with root in node 'p' and balances this subtree
Node* RemoveMin(Node* p,Root* root)
{
	if (p->left == NULL)
		return p->right;
	p->left = RemoveMin(p->left,root);
	return Balance(p,root);
}

//deletes node with key, equal 'k', from subtree with root in node 'p'
Node* Remove(Node* p, int k, Root* root)
{
	if (!p) return NULL;
	
	// finding node with key, equal 'k'
	if (k < p->key)
		p->left = Remove(p->left, k, root);
	else if (k > p->key)
		p->right = Remove(p->right, k, root);
	
	//when we find needed node ( k == p->key)
	else 
	{
		Node* q = p->left;  // remember left subtree
		Node* r = p->right; // remember right subtree
		
		// if right subtree is empty 
		if (!r) 
		{
			//if p is root of the entire tree then this root will equal left subtree
			if (p == root->pointer)
				root->pointer = q;
			delete p;
			return q;
		}
		
		//if right subtree isn't empty
		//find in right subtree minimum node
		Node* min = MinimumNode(r);
		//remove this minimum node from tree
		min->right = RemoveMin(r, root);
		min->left = q;
		if (p == root->pointer)
			root->pointer = min;
		delete p;
		return Balance(min,root);
	}
	return Balance(p,root);
}
