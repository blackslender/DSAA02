/*
 * =========================================================================================
 * Name        : dbLib.cpp
 * Author      : Duc Dung Nguyen
 * Student     : Vu Hoang Van
 * Email       : 1614063@hcmut.edu.vn
 * Copyright   : Faculty of Computer Science and Engineering - Bach Khoa University
 * Description : library for Assignment 2 - Data structures and Algorithms - Fall 2017
 *               This file implements functions used for database management
 * =========================================================================================
 */

#ifndef A02_DSALIB_H
#define A02_DSALIB_H

#pragma region // This part includes preprocessing statements and exception handlers
#include <string>
#include <math.h>
#include <vector>

using namespace std;

class DSAException // This class handles exception
{
    int _error;
    string _text;

  public:
    DSAException() : _error(0), _text("Success") {}
    DSAException(int err) : _error(err), _text("Unknown Error") {}
    DSAException(int err, const char *text) : _error(err), _text(text) {}

    int getError() { return _error; }
    string &getErrorText() { return _text; }
};

#pragma endregion
#pragma region // This section defines Linked list
template <class T>
struct L1Item
{
    T data;
    L1Item<T> *pNext;
    L1Item() : pNext(NULL) {}
    L1Item(T &a) : data(a), pNext(NULL) {}
};

template <class T>
class L1List
{
    L1Item<T> *_pHead;
    size_t _size;

  public:
    L1List()
    { //Default constructor
        _pHead = NULL;
        _size = 0;
    }
    L1List(const L1List<T> &obj)
    { 
    }
    ~L1List()
    {
    }

    bool IsEmpty()
    {
        return _pHead == NULL;
    }

    size_t Size()
    {
        return _size;
    }

    void Clean() 
    {
        while (_pHead) {
            L1List<T> *tmp = _pHead->pNext;
            delete _pHead;
            _pHead = tmp;
        }
        _size = 0;
    }
    bool InsertHead(T &value)
    { // Insert a value to the head of the list
        if (IsEmpty())
        {
            _pHead = new L1Item<T>(value);
            _size = 1;
            return true;
        }
        else
        {
            L1Item<T> *tmp = new L1Item<T>(value);
            tmp->pNext = _pHead;
            _pHead = tmp;
            _size++;
            return true;
        }
    }

    bool RemoveHead() 
    { // Remove the first element of the list
        if (IsEmpty()) return false;
        L1Item<T>  tmp = _pHead->pNext;
        delete _pHead;
        _pHead = tmp;
    }

    bool Reverse()
    { // Reverse the list
        if (IsEmpty()) return true;

        size_t tmpSize = _size;

        L1Item<T> *tmp = _pHead;
        L1Item<T> *p = _pHead;
        while (p)  
        {
            InsertHead(p->data);
            p = p->pNext;
        }

        p = _pHead;
        while (p->pNext != tmp) p = p->pNext;
        p->pNext = NULL;

        while (tmp) {
            p = tmp->pNext;
            delete tmp;
            tmp = p;
        }

        _size = tmpSize;
        return true;
    }

    void Traverse(void(*op)(T&)) 
    { // Traverse the list with void op
        L1Item<T> *p = _pHead;
        while (p) 
        {
            op(p->data);
            p = p->pNext;
        }
    }


};

#pragma endregion
#pragma region // This section defines AVL Tree

template<class T> typedef struct AVLNode{
    T data;

    AVLNode Father,LChild,RChild;
    int Height,Weight;

    AVLNode() {
        Father = NULL;
        LChild = NULL;
        RChild = NULL;
        height = 0;
        weight = 0;
    }

    AVLNode(T &value): AVLNode(),data(value){}
    
};

template class<T> class AVLTree 
{
    AVLNode<T> *_pRoot;
    
}
#pragma endregion
#endif