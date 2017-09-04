#pragma once

#include<iostream>
using namespace std;

#include<string.h>

enum PointFlag{LINK, THREAD};

template<class T>
struct BinaryTreeNodeThd
{
	BinaryTreeNodeThd(const T& data)
		: _data(data)
		, _pLeft(NULL)
		, _pRight(NULL)
		, _pParent(NULL)
		, _leftThread(LINK)
		, _rightThread(LINK)
	{}

	T _data;
	BinaryTreeNodeThd<T>* _pLeft;
	BinaryTreeNodeThd<T>* _pRight;
	BinaryTreeNodeThd<T>* _pParent;
	PointFlag _leftThread;
	PointFlag _rightThread;
};

template<class T>
class BinaryTreeThd
{
	typedef BinaryTreeNodeThd<T> Node;
public:
	BinaryTreeThd()
		: _pRoot(NULL)
	{}

	BinaryTreeThd(const T array[], size_t size, const T& invalid)
	{
		size_t index = 0;
		_CreateTree(_pRoot, array, size, index, invalid);
	}

	void PreThreading()
	{
		Node* prev = NULL;
		_PreThreading(_pRoot, prev);
	}

	void InThreading()
	{
		Node* prev = NULL;
		_InThreading(_pRoot, prev);
	}

	void PostThreading()
	{
		Node* prev = NULL;
		_PostThreading(_pRoot, prev);
	}

	void PreOrder()
	{
		if(NULL == _pRoot)
			return;

		while(_pRoot)
		{
			while(LINK == _pRoot->_leftThread)
			{
				cout<<_pRoot->_data<<" ";
				_pRoot = _pRoot->_pLeft;
 			}
			cout<<_pRoot->_data<<" ";

			while(THREAD == _pRoot->_rightThread)//处理连续的右线索
			{
				_pRoot = _pRoot->_pRight;
				cout<<_pRoot->_data<<" ";
			}
			if(LINK == _pRoot->_leftThread)
			{
				_pRoot = _pRoot->_pLeft;
			}
			if(LINK == _pRoot->_rightThread)
			{
				_pRoot = _pRoot->_pRight;
			}
		}
	}
	void InOrder()
	{
		if(_pRoot == NULL)
			return ;
		while(_pRoot)
		{
			while(LINK == _pRoot->_leftThread)
			{
				_pRoot = _pRoot->_pLeft; 
			}
			cout<<_pRoot->_data<<" ";

			while(THREAD == _pRoot->_rightThread)
			{
				_pRoot = _pRoot->_pRight;
				cout<<_pRoot->_data<<" ";
			}
			if(LINK == _pRoot->_rightThread)
			{
				_pRoot = _pRoot->_pRight;
			}

			else if(LINK == _pRoot->_leftThread)
			{
				_pRoot = _pRoot->_pLeft;
			}
		}
	}
	//void PostOrder();

private:
	void _CreateTree(Node* &pRoot, const T array[], size_t size, size_t& index, const T& invalid)//为什么会传入pRoot的引用？？？？
	{
		
		if(index<size && array[index]!= invalid)
		{
			pRoot = new Node(array[index]);
			_CreateTree(pRoot->_pLeft, array, size, ++index, invalid);
		    _CreateTree(pRoot->_pRight, array, size, ++index, invalid);
		}
		
 	}

	//按照前序遍历的方法来实现的
	void _PreThreading(Node* pRoot, Node*& prev)//prev是用来标记当前节点的前一个节点
	{
		if(NULL==pRoot)
			return ;

		if(NULL == pRoot->_pLeft)//线索化当前节点的左指针域
		{
			pRoot->_pLeft = prev;
			pRoot->_leftThread = THREAD;
		}

		if(prev && NULL == prev->_pRight )//线索化前一个节点(prev)的右指针域
		{
			prev->_pRight = pRoot;
			prev->_rightThread = THREAD;
		}
		prev = pRoot;


		if(LINK==pRoot->_leftThread)
		{
			_PreThreading(pRoot->_pLeft,prev);
		}


		if(LINK==pRoot->_rightThread)
		{
			_PreThreading(pRoot->_pRight,prev);
		}
	}

	void _InThreading(Node* pRoot, Node*& prev)
	{
		if(NULL == pRoot)
			return ;


		_InThreading(pRoot->_pLeft,prev);
		

		if(NULL == pRoot->_pLeft)//线索化当前结点的左指针域
		{
			pRoot->_pLeft =prev;
			pRoot->_leftThread = THREAD;
		}

		if(prev && NULL == prev->_pRight)//线索化前一个节点（prev）的右指针域
		{
			prev->_pRight = pRoot;
			prev->_rightThread = THREAD;
		}

		prev = pRoot;
	
		_InThreading(pRoot->_pRight,prev);
		

	}

	void _PostThreading(Node* pRoot, Node*& prev)
	{
		if(NULL == pRoot)
			return ;

		if(LINK == pRoot->_leftThread)
		{
			_InThreading(pRoot->_pLeft,prev);
		}


		if(LINK == pRoot->_rightThread)
		{
			_InThreading(pRoot->_pRight,prev);
		}

		if(NULL == pRoot->_pLeft)//线索化当前结点的左指针域
		{
			pRoot->_pLeft =prev;
			pRoot->_leftThread = THREAD;
		}
		
		if(prev && NULL == prev->_pRight)//线索化前一个节点（prev）的右指针域
		{
			prev->_pRight = pRoot;
			prev->_rightThread = THREAD;
		}
		prev = pRoot;
	}

private:
	Node* _pRoot;
};

void FunTest()
{
	char* array={"124###35##6"};
	BinaryTreeThd<char> Node(array,strlen(array),'#');
	//Node.PreThreading();
	Node.InThreading();
	//Node.PostThreading();

	//Node.PreOrder();

	Node.InOrder();

}