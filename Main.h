#pragma once
#include<cstdio>

struct Node
{
	int key;
	unsigned int height;
	char value[50];
	Node *left;
	Node *right;
	Node* parent;
};

struct Root
{
	Node* pointer;
};

void MainMenu(Root*);
void Controls(Root*, char);
Root* CreateRoot();
Node* CreateNode(int, char*);
Node* AddNode(Root*, Node*);
void FreeTree(Node*);
void PrintTree(Node*);
Node* SearchNode(Node*, int);
Node* MinimumNode(Node*);
Node* ScanNode();
void PrintFileNode(Node*);
void PrintFileNode(Node* print_node, FILE* file);
void PrintNode(Node*);
Node* ScanFileNode(FILE*);
Root* BuildTreeFromFile(Root*);
void DeleteNode(Root*);
void TreeToFile(Root*);
void PrintFileTreeWalk(Node*, FILE*);
unsigned int Height(Node*);
int Bfactor(Node*);
void UpdateHeight(Node*);
Node* RotateLeft(Node*,Root*);
Node* RotateRight(Node*,Root*);
Node* Balance(Node*,Root*);
Node* BalanceInsert(Node* p, Node* ins_item,Root*);
void TreeVisible(Node *r);
Node* RemoveMin(Node* p, Root* root);
Node* Remove(Node* p, int k, Root* root);
