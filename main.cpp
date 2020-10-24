#include <sstream>
#include <string>
#include <iostream>
#include <type_traits>
using namespace std;

#ifndef ILIST_H
#define ILIST_H

template <class T>
class IList
{
public:
    virtual void add(const T &element) = 0;
    virtual void add(int index, const T &element) = 0;
    virtual T removeAt(int index) = 0;
    virtual bool removeItem(const T &item) = 0;
    virtual bool empty() = 0;
    virtual int size() = 0;
    virtual void clear() = 0;
    virtual T get(int index) = 0;
    virtual void set(int index, const T &element) = 0;
    virtual int indexOf(const T &item) = 0;
    virtual bool contains(const T &item) = 0;
    virtual string toString() = 0;
};

#endif

// STUDENT ANSWER
#ifndef FRAGMENT_LINKED_LIST
#define FRAGMENT_LINKED_LIST
template <class T>
class FragmentLinkedList : IList<T>
{
public:
    class Node;
    class Iterator;

protected:
    Node **fragmentPointers;
    int fragmentMaxSize;
    int count;
    int fragmentCount;
    Node *DummyNode;

    void fragmentRebuild(bool add_rem, const int &index);

public:
    FragmentLinkedList(int fragmentMaxSize = 5)
    {
        this->DummyNode = nullptr;
        this->count = 0;
        this->fragmentCount = 1;
        this->fragmentMaxSize = fragmentMaxSize;
        this->fragmentPointers = new Node *[1];
        this->fragmentPointers[0] = NULL;
    }
    virtual ~FragmentLinkedList();
    virtual void add(const T &element);
    virtual void add(int index, const T &element);
    virtual T removeAt(int index);
    virtual bool removeItem(const T &item);
    virtual bool empty();
    virtual int size();
    virtual void clear();
    virtual T get(int index);
    virtual void set(int index, const T &element);
    virtual int indexOf(const T &item);
    virtual bool contains(const T &item);
    virtual string toString();
    Iterator begin(int index = 0);
    Iterator end(int index = -1);

    //Custom
    string fragmentCheck();

public:
    class Node
    {
    private:
        T data;
        Node *next;
        Node *prev;
        friend class FragmentLinkedList<T>;

    public:
        Node()
        {
            next = 0;
            prev = 0;
        }
        Node(Node *next, Node *prev)
        {
            this->next = next;
            this->prev = prev;
        }
        Node(T data, Node *next, Node *prev)
        {
            this->data = data;
            this->next = next;
            this->prev = prev;
        }
    };

    class Iterator
    {
    private:
        FragmentLinkedList<T> *pList;
        Node *pNode;
        int Head;

    public:
        Iterator(FragmentLinkedList<T> *pList = 0, bool begin = true);
        Iterator(FragmentLinkedList<T> *pList = 0, int fragmentIndex = 0, bool begin = true);
        Iterator &operator=(const Iterator &iterator);
        T &operator*();
        bool operator!=(const Iterator &iterator);
        void remove();
        void set(const T &element);
        Iterator &operator++();
        Iterator operator++(int);
    };
};

//Destructor (Done)---------------------------------------------------------
template <class T>
FragmentLinkedList<T>::~FragmentLinkedList()
{
    if (empty())
        return;

    this->count = 0;
    this->fragmentCount = 1;

    Node *p = this->fragmentPointers[0], *tmp;

    while (p)
    {
        tmp = p;
        p = p->next;
        delete tmp;
    }

    delete[] this->fragmentPointers;
}

//Clear (Done)---------------------------------------------------------
template <class T>
void FragmentLinkedList<T>::clear()
{
    this->~FragmentLinkedList();
}

//Size (Done)---------------------------------------------------------
template <class T>
int FragmentLinkedList<T>::size()
{
    return count;
}

//Get (Done)---------------------------------------------------------
template <class T>
T FragmentLinkedList<T>::get(int index)
{
    if (index >= count || index < 0 || empty())
        throw out_of_range("This index is out of range!");

    Node *p = this->fragmentPointers[0];
    int i = 0;

    while (i < index)
    {
        p = p->next;
        i++;
    }

    return p->data;
}

//Empty check (Done)---------------------------------------------------------
template <class T>
bool FragmentLinkedList<T>::empty()
{
    return (!count);
}

//Set (Done)---------------------------------------------------------
template <class T>
void FragmentLinkedList<T>::set(int index, const T &element)
{
    if (index >= count || index < 0 || empty())
        throw out_of_range("This index is out of range!");

    Node *p = this->fragmentPointers[0];
    int i = 0;

    while (i < index)
    {
        p = p->next;
        i++;
    }
    p->data = element;
}

//Index of an element (Done)---------------------------------------------------------
template <class T>
int FragmentLinkedList<T>::indexOf(const T &item)
{
    Node *p = this->fragmentPointers[0];
    int index = 0;

    while (index < count)
        if (p->data != item)
        {
            p = p->next;
            index++;
        }
        else
            break;

    if (index < count)
        return index;
    else
        return -1;
}

//Contain (Done)---------------------------------------------------------
template <class T>
bool FragmentLinkedList<T>::contains(const T &item)
{
    Node *p = this->fragmentPointers[0];
    int index = 0;

    while (index < count)
        if (p->data != item)
        {
            p = p->next;
            index++;
        }
        else
            break;

    if (index < count)
        return true;
    else
        return false;
}

//Remove at (Done)---------------------------------------------------------
template <class T>
T FragmentLinkedList<T>::removeAt(int index)
{
    Node *p = this->fragmentPointers[index / fragmentMaxSize];

    if (index >= count || index < 0 || empty())
        throw out_of_range("This index is out of range!");

    if (index == 0)
    {
        this->fragmentPointers[0] = p->next;
        this->fragmentPointers[0]->prev = nullptr;
        this->count--;

        T t = p->data;
        delete p;
        return t;
    }

    int i = index / fragmentMaxSize * fragmentMaxSize;

    while (i < index)
    {
        p = p->next;
        i++;
    }
    if (i == count - 1)
        p->prev->next = DummyNode;
    else
    {
        p->prev->next = p->next;
        p->next->prev = p->prev;
    }
    T t = p->data;
    this->count--;

    fragmentRebuild(0, index);

    delete p;
    return t;
}

//Remove item (Done)---------------------------------------------------------
template <class T>
bool FragmentLinkedList<T>::removeItem(const T &item)
{
    Node *p = this->fragmentPointers[0];
    int index = 0;

    while (index < count)
        if (p->data != item)
        {
            p = p->next;
            index++;
        }
        else
            break;

    if (index < count)
    {
        if (index == 0)
        {
            this->fragmentPointers[0] = p->next;
            this->fragmentPointers[0]->prev = nullptr;
            this->count--;
            delete p;
            return true;
        }
        if (index == count - 1)
            p->prev->next = DummyNode;
        else
        {
            p->prev->next = p->next;
            p->next->prev = p->prev;
            delete p;
        }

        this->count--;
        fragmentRebuild(0, index);

        return true;
    }
    else
        return false;
}

//Add element to the last position (Done)---------------------------------------------------------
template <class T>
void FragmentLinkedList<T>::add(const T &element)
{

    if (empty())
    {
        if (!DummyNode)
            this->DummyNode = new Node(-1, nullptr, nullptr);
        this->fragmentPointers[0] = new Node(element, DummyNode, nullptr);
        DummyNode->prev = this->fragmentPointers[0];

        this->count++;
        return;
    }
    else
    {
        Node *p = this->fragmentPointers[fragmentCount - 1];

        while (p->next && p->next != DummyNode)
            p = p->next;
        p->next = new Node(element, DummyNode, p);
        DummyNode->prev = p;

        this->count++;
    }

    fragmentRebuild(1, count - 1);
}

//Add element to the index position (Done)---------------------------------------------------------
template <class T>
void FragmentLinkedList<T>::add(int index, const T &element)
{
    if (index >= this->count)
        add(element);
    else
    {
        int i = index / fragmentMaxSize * fragmentMaxSize;
        Node *p = this->fragmentPointers[index / fragmentMaxSize], *t;

        while (i < index)
        {
            p = p->next;
            i++;
        }
        t = new Node(element, p, p->prev);
        if (i != 0)
            p->prev->next = t;
        p->prev = t;

        this->count++;
    }

    fragmentRebuild(1, index);
}

//To string (Done)---------------------------------------------------------
template <class T>
string FragmentLinkedList<T>::toString()
{
    stringstream ss;
    ss << "[";
    Node *ptr = this->fragmentPointers[0];

    if (this->count == 0)
        ss << "]";

    // TODO
    if (this->count == 0)
        return ss.str();
    while (1)
    {
        ss << ptr->data;
        if (ptr->next && ptr->next != DummyNode)
            ss << ", ";
        else
            break;
        ptr = ptr->next;
    }
    ss << "]";
    // END: TODO

    return ss.str();
}

//Fragment Rebuild (Done)---------------------------------------------------------
//add_rem: 0 is remove, 1 is add.
template <class T>
void FragmentLinkedList<T>::fragmentRebuild(bool add_rem, const int &index)
{
    bool tf = false;

    /*If we add an element when out of range, the last element will always be marked.
    We have to rebuild the Fragment list (increase its size by 1). */
    if (count / fragmentMaxSize >= fragmentCount && count % fragmentMaxSize != 0)
    {
        tf = true;
        Node **t = this->fragmentPointers;
        this->fragmentCount++;
        this->fragmentPointers = new Node *[fragmentCount];

        for (int i = 0; i < fragmentCount - 1; i++)
        {
            this->fragmentPointers[i] = t[i];
        }

        Node *p = this->fragmentPointers[fragmentCount - 2];
        while (p->next && p->next != DummyNode)
            p = p->next;
        this->fragmentPointers[fragmentCount - 1] = p;

        delete[] t;
    }

    /*If we remove the last element which is also marked by the last Fragment Pointer,
    we must decrease the Fragment Count */
    if (!add_rem && index % fragmentMaxSize == 0 && index == count)
    {
        //delete fragmentPointers[fragmentCount - 1];
        this->fragmentCount--;
    }

    /* Elements in the last Fragment, element marked by the last Fragment Pointer
    excluded, are not significant. */
    if (index / fragmentMaxSize == fragmentCount - 1 && index % fragmentMaxSize != 0 + tf)
        return;

    /* If an element is not from the cases above, we just need to move the Fragment
    Pointers from the index to the last by 1 depends on it is added (move backward)
    or removed (move forward). */
    int i = index / fragmentMaxSize + (index % fragmentMaxSize != 0), lim = fragmentCount - 1 - tf;
    while (i <= lim)
    {
        if (add_rem)
            fragmentPointers[i] = fragmentPointers[i]->prev;
        else
            fragmentPointers[i] = fragmentPointers[i]->next;
        i++;
    }
}

//Fragment check (Done)---------------------------------------------------------
template <class T>
string FragmentLinkedList<T>::fragmentCheck()
{
    stringstream ss;

    ss << "[ ";
    for (int i = 0; i < fragmentCount; i++)
    {
        if (indexOf(fragmentPointers[i]->data) == i * fragmentMaxSize)
            ss << fragmentPointers[i]->data << " ";
        else
            return "FALSE";
    }
    ss << "]   --True--";
    return ss.str();
}

//Iterator---------------------------------------/****/------------------------------------------------
//Constructor (Done)--------------------------------------------------------------------------------
template <typename T>
FragmentLinkedList<T>::Iterator::Iterator(FragmentLinkedList<T> *pList, bool begin)
    : pList(pList)
{
    if (begin)
        pNode = pList->fragmentPointers[0];
    else
        pNode = pList->DummyNode;
    this->Head = false;
}

template <typename T>
FragmentLinkedList<T>::Iterator::Iterator(FragmentLinkedList<T> *pList, int fragmentIndex, bool begin)
    : pList(pList)
{
    if (begin)
        pNode = pList->fragmentPointers[fragmentIndex];
    else
    {
        if (pList->fragmentCount > fragmentIndex + 1)
            pNode = pList->fragmentPointers[fragmentIndex + 1];
        else
            pNode = pList->DummyNode;
    }
    this->Head = false;
}

//Begin (Done)-------------------------------------------------------------------------------
template <typename T>
typename FragmentLinkedList<T>::Iterator FragmentLinkedList<T>::begin(int index)
{
    if (index == 0)
        return Iterator(this, true);
    return Iterator(this, index, true);
}

//End (Done)------------------------------------------------------------------------------
template <typename T>
typename FragmentLinkedList<T>::Iterator FragmentLinkedList<T>::end(int index)
{
    if (index == -1)
        return Iterator(this, false);
    return Iterator(this, index, false);
}

//Copy operator (Done)----------------------------------------------------------------------
template <class T>
typename FragmentLinkedList<T>::Iterator &FragmentLinkedList<T>::Iterator::operator=(const Iterator &iterator)
{
    this->pNode = iterator.pNode;
    this->pList = iterator.pList;
    this->Head = iterator.Head;
    return *this;
}

//Prefix operator (Done)-----------------------------------------------------------------------
template <typename T>
typename FragmentLinkedList<T>::Iterator &FragmentLinkedList<T>::Iterator::operator++()
{

    if (!Head)
        this->pNode = this->pNode->next;
    else
    {
        this->pNode = pList->fragmentPointers[0];
        Head = false;
    }
    return *this;
}

//Postfix operator  -----------------------------------------------------------------------
template <typename T>
typename FragmentLinkedList<T>::Iterator FragmentLinkedList<T>::Iterator::operator++(int)
{
    Iterator tmp(pList, true);
    tmp.pNode = this->pNode;
    if (!Head)
        this->pNode = this->pNode->next;
    else
    {
        this->pNode = pList->fragmentPointers[0];
        Head = false;
    }
    return tmp;
}

//Dereference operator  -----------------------------------------------------------------------
template <typename T>
T &FragmentLinkedList<T>::Iterator::operator*()
{
    return pNode->data;
}

//Check equality operator  -----------------------------------------------------------------------
template <typename T>
bool FragmentLinkedList<T>::Iterator::operator!=(const Iterator &iterator)
{
    return (this->pNode != iterator.pNode);
}

//Remove   -----------------------------------------------------------------------
template <typename T>
void FragmentLinkedList<T>::Iterator::remove()
{
    /* Problem M
       If an iterator is defined as an begin iterator, there is no guarantee
    that iterator is still pointing to the first node (we can add an element to
    the list after the iterator is defined). We have to update the index of the
    node pointed by the iterator to consider if it is the first node or not.
       If the iterator does point to the first node, var Head will be 1.
    */

    Node *p = pList->fragmentPointers[0];
    int i = 0;

    while (p != pNode)
    {
        p = p->next;
        i++;
    }
    if (i == 0)
        this->Head = true;
    this->pNode = this->pNode->prev;
    pList->removeAt(i);
}

//Set  -----------------------------------------------------------------------
template <class T>
void FragmentLinkedList<T>::Iterator::set(const T &element)
{
    this->pNode->data = element;
}

#endif
// END: STUDENT ANSWER

int main()
{
    // TESTCASE INPUT
    // === Example
    int a = 5, n = 16;
    FragmentLinkedList<int> fList(a);
    for (int i = 0; i < n; i++)
    {
        fList.add(i * i);
    }
    cout << "- Add i*i to list (i from 0 to " << n - 1 << "): " << endl;
    cout << fList.toString() << endl;
    cout << "Check fragment list with Fragment max size of " << a << ":" << endl
         << fList.fragmentCheck() << endl
         << endl;

    fList.add(4, 7);
    cout << "- Add 7 to index 4:" << endl
         << fList.toString() << endl;
    cout << "Check fragment list with Fragment max size of " << a << ":" << endl
         << fList.fragmentCheck() << endl
         << endl;

    cout << "- Remove item 4, return: " << fList.removeAt(4) << endl;
    cout << fList.toString() << endl;
    cout << "Check fragment list with Fragment max size of " << a << ":" << endl
         << fList.fragmentCheck() << endl
         << endl;

    cout << "- Remove item last, return: " << fList.removeAt(fList.size() - 1) << endl;
    cout << fList.toString() << endl;
    cout << "Check fragment list with Fragment max size of " << a << ":" << endl
         << fList.fragmentCheck() << endl
         << endl;

    cout << "- Remove item 5, return: " << fList.removeAt(5) << endl;
    cout << fList.toString() << endl;
    cout << "Check fragment list with Fragment max size of " << a << ":" << endl
         << fList.fragmentCheck() << endl
         << endl;

    fList.set(4, 69);
    cout << "- Set index 4 to 69:" << endl
         << fList.toString() << endl;
    cout << "Check fragment list with Fragment max size of " << a << ":" << endl
         << fList.fragmentCheck() << endl
         << endl;

    cout << "- Check index 6: " << fList.indexOf(6) << endl
         << endl;
    cout << "- Check index 10: " << fList.indexOf(10) << endl
         << endl;
    cout << "- Check item with value of 6: " << fList.contains(6) << endl
         << endl;
    cout << "- Check item with value of 64: " << fList.contains(64) << endl
         << endl;
    cout << "- Get value of index 5: " << fList.get(3) << endl
         << endl;

    using iterator = FragmentLinkedList<int>::Iterator;

    cout << "Original list:     " << fList.toString() << endl;
    cout << "Iterator checking: "
         << "[";
    string x = ", ";
    for (iterator i = fList.begin(); i != fList.end(); i++)
    {
        if (*i == fList.get(fList.size() - 1))
        {
            i.set(96);
            x = "";
        }
        cout << *i << x;
    }
    cout << "]" << endl
         << endl;

    iterator it = fList.begin();
    cout << "Prefix checking: " << *(it++) << " = ";
    cout << *it << endl
         << endl;

    it = fList.begin();
    it.remove();
    it++;
    cout << "List after modified: " << endl
         << fList.toString() << endl;
    cout << "Remove & Increase the first node checking: " << *it;
    if (*it == fList.get(0))
        cout << "   --TRUE--";
    else
        cout << "  --FALSE--";
    cout << endl
         << endl;

    //Problem M
    cout << "- Problem M" << endl;
    it = fList.begin();
    fList.add(0, 69);
    it.remove();
    it++;
    cout << "List after modified: " << endl
         << fList.toString() << endl;
    cout << "Remove & Increase the first iterator points to the first node which later does not checking: " << *it;
    cout << endl
         << endl;

    //Problem M
    cout << "- Repeat again Problem M" << endl;
    it = fList.begin();
    fList.add(0, 6969);
    it.remove();
    it++;
    cout << "List after modified: " << endl
         << fList.toString() << endl;
    cout << "Remove & Increase the first iterator points to the first node which later does not checking: " << *it;
    cout << endl
         << endl;

    // for(FragmentLinkedList<int>::Iterator it = fList.begin(); it != fList.end(); it++)
    //     cout << *it << " ";
    // cout << endl;
    // === END: Example
    // END: TESTCASE INPUT

    fList.clear();
    cout << "Check clear:  " << fList.toString() << endl
         << endl;

    return 0;
}
