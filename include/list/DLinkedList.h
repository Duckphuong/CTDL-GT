/*
 * File:   DLinkedList.h
 */

#ifndef DLINKEDLIST_H
#define DLINKEDLIST_H

#include "IList.h"

#include <sstream>
#include <iostream>
#include <type_traits>
using namespace std;

template <class T>
class DLinkedList : public IList<T>
{
public:
    class Node;        // Forward declaration
    class Iterator;    // Forward declaration
    class BWDIterator; // Forward declaration

protected:
    Node *head; // this node does not contain user's data
    Node *tail; // this node does not contain user's data
    int count;
    bool (*itemEqual)(T &lhs, T &rhs);        // function pointer: test if two items (type: T&) are equal or not
    void (*deleteUserData)(DLinkedList<T> *); // function pointer: be called to remove items (if they are pointer type)

public:
    DLinkedList(
        void (*deleteUserData)(DLinkedList<T> *) = 0,
        bool (*itemEqual)(T &, T &) = 0);
    DLinkedList(const DLinkedList<T> &list);
    DLinkedList<T> &operator=(const DLinkedList<T> &list);
    ~DLinkedList();

    // Inherit from IList: BEGIN
    void add(T e);
    void add(int index, T e);
    T removeAt(int index);
    bool removeItem(T item, void (*removeItemData)(T) = 0);
    bool empty();
    int size();
    void clear();
    T &get(int index);
    int indexOf(T item);
    bool contains(T item);
    string toString(string (*item2str)(T &) = 0);
    // Inherit from IList: END

    void println(string (*item2str)(T &) = 0)
    {
        cout << toString(item2str) << endl;
    }
    void setDeleteUserDataPtr(void (*deleteUserData)(DLinkedList<T> *) = 0)
    {
        this->deleteUserData = deleteUserData;
    }

    bool contains(T array[], int size)
    {
        int idx = 0;
        for (DLinkedList<T>::Iterator it = begin(); it != end(); it++)
        {
            if (!equals(*it, array[idx++], this->itemEqual))
                return false;
        }
        return true;
    }

    /*
     * free(DLinkedList<T> *list):
     *  + to remove user's data (type T, must be a pointer type, e.g.: int*, Point*)
     *  + if users want a DLinkedList removing their data,
     *      he/she must pass "free" to constructor of DLinkedList
     *      Example:
     *      DLinkedList<T> list(&DLinkedList<T>::free);
     */
    static void free(DLinkedList<T> *list)
    {
        typename DLinkedList<T>::Iterator it = list->begin();
        while (it != list->end())
        {
            delete *it;
            it++;
        }
    }

    /* begin, end and Iterator helps user to traverse a list forwardly
     * Example: assume "list" is object of DLinkedList

     DLinkedList<char>::Iterator it;
     for(it = list.begin(); it != list.end(); it++){
            char item = *it;
            std::cout << item; //print the item
     }
     */
    Iterator begin()
    {
        return Iterator(this, true);
    }
    Iterator end()
    {
        return Iterator(this, false);
    }

    /* last, beforeFirst and BWDIterator helps user to traverse a list backwardly
     * Example: assume "list" is object of DLinkedList

     DLinkedList<char>::BWDIterator it;
     for(it = list.last(); it != list.beforeFirst(); it--){
            char item = *it;
            std::cout << item; //print the item
     }
     */
    BWDIterator bbegin()
    {
        return BWDIterator(this, true);
    }
    BWDIterator bend()
    {
        return BWDIterator(this, false);
    }

protected:
    static bool equals(T &lhs, T &rhs, bool (*itemEqual)(T &, T &))
    {
        if (itemEqual == 0)
            return lhs == rhs;
        else
            return itemEqual(lhs, rhs);
    }
    void copyFrom(const DLinkedList<T> &list);
    void removeInternalData();
    Node *getPreviousNodeOf(int index);

    //////////////////////////////////////////////////////////////////////
    ////////////////////////  INNER CLASSES DEFNITION ////////////////////
    //////////////////////////////////////////////////////////////////////
public:
    class Node
    {
    public:
        T data;
        Node *next;
        Node *prev;
        friend class DLinkedList<T>;

    public:
        Node(Node *next = 0, Node *prev = 0)
        {
            this->next = next;
            this->prev = prev;
        }
        Node(T data, Node *next = 0, Node *prev = 0)
        {
            this->data = data;
            this->next = next;
            this->prev = prev;
        }
    };

    //////////////////////////////////////////////////////////////////////
    class Iterator
    {
    private:
        DLinkedList<T> *pList;
        Node *pNode;

    public:
        Iterator(DLinkedList<T> *pList = 0, bool begin = true)
        {
            if (begin)
            {
                if (pList != 0)
                    this->pNode = pList->head->next;
                else
                    pNode = 0;
            }
            else
            {
                if (pList != 0)
                    this->pNode = pList->tail;
                else
                    pNode = 0;
            }
            this->pList = pList;
        }

        Iterator &operator=(const Iterator &iterator)
        {
            this->pNode = iterator.pNode;
            this->pList = iterator.pList;
            return *this;
        }
        void remove(void (*removeItemData)(T) = 0)
        {
            pNode->prev->next = pNode->next;
            pNode->next->prev = pNode->prev;
            Node *pNext = pNode->prev; // MUST prev, so iterator++ will go to end
            if (removeItemData != 0)
                removeItemData(pNode->data);
            delete pNode;
            pNode = pNext;
            pList->count -= 1;
        }

        T &operator*()
        {
            return pNode->data;
        }
        bool operator!=(const Iterator &iterator)
        {
            return pNode != iterator.pNode;
        }
        // Prefix ++ overload
        Iterator &operator++()
        {
            pNode = pNode->next;
            return *this;
        }
        // Postfix ++ overload
        Iterator operator++(int)
        {
            Iterator iterator = *this;
            ++*this;
            return iterator;
        }
    };
};
//////////////////////////////////////////////////////////////////////
// Define a shorter name for DLinkedList:

template <class T>
using List = DLinkedList<T>;

//////////////////////////////////////////////////////////////////////
////////////////////////     METHOD DEFNITION      ///////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
DLinkedList<T>::DLinkedList(
    void (*deleteUserData)(DLinkedList<T> *),
    bool (*itemEqual)(T &, T &))
{
    head = new Node();
    tail = new Node();
    head->next = tail;
    tail->prev = head;
    count = 0;
    this->deleteUserData = deleteUserData;
    this->itemEqual = itemEqual;
}

template <class T>
DLinkedList<T>::DLinkedList(const DLinkedList<T> &list)
{
    head = new Node();
    tail = new Node();
    head->next = tail;
    tail->prev = head;
    count = 0;
    this->deleteUserData = list.deleteUserData;
    this->itemEqual = list.itemEqual;
    copyFrom(list);
}

template <class T>
DLinkedList<T> &DLinkedList<T>::operator=(const DLinkedList<T> &list)
{
    if (this != &list)
    {
        clear();
        copyFrom(list);
    }
    return *this;
}

template <class T>
DLinkedList<T>::~DLinkedList()
{
    clear();
    delete head;
    delete tail;
}

template <class T>
void DLinkedList<T>::add(T e)
{
    Node *newNode = new Node(e, tail, tail->prev);
    tail->prev->next = newNode;
    tail->prev = newNode;
    count++;
}

template <class T>
void DLinkedList<T>::add(int index, T e)
{
    if (index < 0 || index > count)
        throw out_of_range("Index is out of range!");

    Node *prevNode = getPreviousNodeOf(index);
    Node *newNode = new Node(e, prevNode->next, prevNode);
    prevNode->next->prev = newNode;
    prevNode->next = newNode;
    count++;
}

template <class T>
typename DLinkedList<T>::Node *DLinkedList<T>::getPreviousNodeOf(int index)
{
    if (index < 0 || index > count)
        throw out_of_range("Index is out of range!");

    Node *cur;
    if (index < count / 2)
    {
        cur = head;
        for (int i = 0; i <= index; i++)
            cur = cur->next;
    }
    else
    {
        cur = tail;
        for (int i = count; i > index; i--)
            cur = cur->prev;
    }
    return cur->prev;
}

template <class T>
T DLinkedList<T>::removeAt(int index)
{
    if (index < 0 || index >= count)
        throw out_of_range("Index is out of range!");

    Node *prevNode = getPreviousNodeOf(index);
    Node *deleteNode = prevNode->next;
    T data = deleteNode->data;

    prevNode->next = deleteNode->next;
    deleteNode->next->prev = prevNode;

    delete deleteNode;
    count--;
    return data;
}

template <class T>
bool DLinkedList<T>::empty()
{
    return count == 0;
}

template <class T>
int DLinkedList<T>::size()
{
    return count;
}

template <class T>
void DLinkedList<T>::clear()
{
    while (!empty())
    {
        removeAt(0);
    }
}

template <class T>
T &DLinkedList<T>::get(int index)
{
    if (index < 0 || index >= count)
        throw out_of_range("Index is out of range!");

    Node *node = getPreviousNodeOf(index)->next;
    return node->data;
}

template <class T>
int DLinkedList<T>::indexOf(T item)
{
    int index = 0;
    for (Node *cur = head->next; cur != tail; cur = cur->next)
    {
        if (equals(cur->data, item, itemEqual))
            return index;
        index++;
    }
    return -1;
}

template <class T>
bool DLinkedList<T>::removeItem(T item, void (*removeItemData)(T))
{
    int index = indexOf(item);
    if (index != -1)
    {
        removeAt(index);
        if (removeItemData != 0)
            removeItemData(item);
        return true;
    }
    return false;
}

template <class T>
bool DLinkedList<T>::contains(T item)
{
    return indexOf(item) != -1;
}

template <class T>
string DLinkedList<T>::toString(string (*item2str)(T &))
{
    stringstream ss;
    ss << "[";
    Node *cur = head->next;
    while (cur != tail)
    {
        if (item2str)
            ss << item2str(cur->data);
        else
            ss << cur->data;

        cur = cur->next;
        if (cur != tail)
            ss << ", ";
    }
    ss << "]";
    return ss.str();
}

template <class T>
void DLinkedList<T>::copyFrom(const DLinkedList<T> &list)
{
    Node *cur = list.head->next;
    while (cur != list.tail)
    {
        add(cur->data);
        cur = cur->next;
    }
}

template <class T>
void DLinkedList<T>::removeInternalData()
{
    Node *cur = head->next;
    while (cur != tail)
    {
        Node *deleteNode = cur;
        cur = cur->next;
        delete deleteNode;
    }
    head->next = tail;
    tail->prev = head;
    count = 0;
}

#endif /* DLINKEDLIST_H */
