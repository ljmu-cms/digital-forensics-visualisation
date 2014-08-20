/*
	Name: M.Anýl Tuncel
	Copyright: M.Anýl Tuncel
	Author: M.Anýl Tuncel
	Date: 05.06.13 22:00
	Description: binaryTree.h
*/
#include <iostream>
#include <cassert>
using namespace std;

template <class T>
class Node
{
public:
	bool isRed;
    T data;
    Node<T> *llink, *rlink, *parent;	
	Node()
	{
	
		llink = rlink = parent = NULL;			
		isRed = false;
			
	}
};

template <class T>
class binaryTree
{

private:
	void destroy(Node<T>*&);
	void inordert(Node<T>*);
	void preordert(Node<T>*);
	void postordert(Node<T>*);
	int height(Node<T>*);
	int max(int,int);
	int nodecount(Node<T>*);
	int leavescount(Node<T>*);    

public:
	Node<T>* root;	 
	binaryTree() {root=NULL;}
	~binaryTree() {destroy(root);}
	bool isEmpty() {return root==NULL;}
	void inorder() {inordert (root);}
	void preorder() {preordert (root);}
	void postorder() {postordert (root);}
	int treeheight(Node<T>*) {height(root);}
	int ncount() {nodecount(root);}
	int lcount() {leavescount(root);}

};

template <class T>
void binaryTree<T>::inordert(Node<T>*p)
{
	if(p!=NULL)
	{ 
		inordert(p->llink);
		cout<<p->data<<endl;
		inordert(p->rlink);
	}
}

template <class T>
void binaryTree<T>::preordert(Node<T> *p)
{
	if(p!=NULL)
	{
	cout<<p->data;
	preordert(p->llink);
	preordert(p->rlink);
	}

}

template <class T>
void binaryTree<T>::postordert(Node<T>*p)
{
	if(p!=NULL)
	{
		postordert(p->llink);
		postordert(p->rlink);
		cout<<p->data;
	}
}

template <class T>
int binaryTree<T>::nodecount(Node<T>*p)
{
    if(p == NULL)
		return 0;
    else
		return 1 + nodecount(p->llink) + nodecount(p->rlink);
}

template <class T>
int binaryTree<T>::leavescount(Node<T>*p)
{
    if(p==NULL)
		return 0;
    else if((p->llink==NULL)&&(p->rlink==NULL))
		return 1;
    else
		return leavescount(p->llink)+leavescount(p->rlink);
}

template <class T>
int binaryTree<T>::max(int x , int y)
{
    if(x > y)
		return x;
    else
		return y;
}

template <class T>
int binaryTree<T>::height(Node<T>*p)
{
    if(p==NULL)
		return 0;
    else
		return 1 + max(treeheight(p->llink),(p->rlink));
}

template <class T>
void binaryTree<T>::destroy(Node<T>*&p)
{
	if(p!=NULL)
	{
		destroy(p->llink);
		destroy(p->rlink);
		delete p;
		p=NULL;
	}

}