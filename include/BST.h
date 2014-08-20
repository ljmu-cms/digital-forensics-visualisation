/*
	Name: M.Anýl Tuncel
	Copyright: M.Anýl Tuncel
	Author: M.Anýl Tuncel
	Date: 05.06.13 22:00
	Description: binarySearchTree.h
*/

#include <iostream>
#include <cassert>
#include "binaryTree.h"
using namespace std;

template <class T>
class BST:public binaryTree <T>
{
public:
	Node<T>*search(T&);
	void insert (T&);
	void deleteNode(T&);
private:
	void deleteFromTree(Node<T>*&p);
};

template <class T>
Node<T>* BST<T>::search(T&item)
{
	Node<T>*p;
	bool found=false;
	if(binaryTree<T>::root==NULL) 
		return NULL;	
	else
	{
		p=binaryTree<T>::root;
		while(p!=NULL && !found)
		{
			if(p->data==item)
			{
				found=true;
			}
			else if(p->data>item)
			{
				p=p->llink;
			}
			else
			{
				p=p->rlink;
			}
		}
		return p;
	}
}    

template <class T>
void BST<T>::insert(T&item)
{
	Node<T> *p,*q,*r;
	r=new Node<T>;
	r->data=item;
	r->llink=r->rlink=NULL;
	if(binaryTree<T>::root==NULL)	
		binaryTree<T>::root=r;
	else
	{
		p=binaryTree<T>::root;
		q=p;       // to prevent that error: potentially uninitialized local pointer variable 'q' used at line 63
		while(p!=NULL){
			q=p;
			if(item<p->data)
				p=p->llink;
			
			else
				p=p->rlink;
			
		}
		if(item<q->data)
		q->llink=r;
		
		else
		q->rlink=r;
		
	}
	
}

template <class T>
void BST<T>::deleteFromTree(Node<T>*&p)
{
    Node<T> *c,*prev,*temp;
	if(p==NULL)
		cerr<<"No Node to delete...";
	else if(p->llink==NULL && p->rlink==NULL)
	{
		temp=p;
		p=NULL;
		delete temp;
	}
	else if(p->llink==NULL)
	{
		temp=p;
		p=temp->rlink;
		delete temp;
	}
	else if(p->rlink==NULL)
	{
		temp=p;
		p=temp->llink;
		delete temp;
	}
	else
	{
		c=p->llink;
		prev=NULL;
		while(c->rlink!=NULL)
		{
			prev=c;
			c=c->rlink;
		}
		p->data=c->data;
		if(prev==NULL)
			p->llink=c->llink;
		else 
			prev->rlink=c->llink;
		delete c;
	}
}
     
template <class T>  
void BST<T>::deleteNode(T& item)
{
	Node<T> *c, *prev;
	bool found=false;
	if(binaryTree<T>::root==NULL)
		cerr<<"Empty Tree...";
	else
	{
		c=binaryTree<T>::root;
		prev=binaryTree<T>::root;
		while(c!=NULL && !found)
		{
			if(c->data==item)
				found=true;
			else
			{
				prev=c;
				if(c->data>item)
					c=c->llink;
				else
					c=c->rlink;
			}
		}
		if(!found) 
			cout<<"No such item...";
		else
		{
			if(c==binaryTree<T>::root)
				deleteFromTree(binaryTree<T>::root);
			else if(prev->data>item)
				deleteFromTree(prev->llink);
			else 
				deleteFromTree(prev->rlink);
		}
	}
}  