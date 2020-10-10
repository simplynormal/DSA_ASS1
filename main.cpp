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
    virtual void add(const T& element) = 0;
    virtual void add(int index, const T& element) = 0;
    virtual T removeAt(int index) = 0;
    virtual bool removeItem(const T& item) = 0;
    virtual bool empty() = 0;
    virtual int size() = 0;
    virtual void clear() = 0;
    virtual T& get(int index) = 0;
    virtual void set(int index, const T& element) = 0;
    virtual int indexOf(const T& item) = 0;
    virtual bool contains(const T& item) = 0;
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

public:
    FragmentLinkedList(int fragmentMaxSize = 5)
    {
        this->count = 0;
        this->fragmentMaxSize = fragmentMaxSize;
        this->fragmentPointers = new Node *[2];
        this->fragmentPointers[0] = NULL;
        this->fragmentPointers[1] = NULL;
    }
    virtual ~FragmentLinkedList();
    virtual void add(const T& element);
    virtual void add(int index, const T& element);
    virtual T removeAt(int index);
    virtual bool removeItem(const T& item);
    virtual bool empty();
    virtual int size();
    virtual void clear();
    virtual T& get(int index);
    virtual void set(int index, const T& element);
    virtual int indexOf(const T& item);
    virtual bool contains(const T& item);
    virtual string toString();
    Iterator begin(int index = 0);
    Iterator end(int index = -1);

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

    public:
        Iterator(FragmentLinkedList<T> *pList = 0, bool begin = true);
        Iterator(FragmentLinkedList<T> *pList = 0, int fragmentIndex = 0, bool begin = true);
        Iterator &operator=(const Iterator &iterator);
        T &operator*();
        bool operator!=(const Iterator &iterator);
        void remove();
        void set(const T& element);
        Iterator &operator++();
        Iterator operator++(int);
    };
};

//Destructor (Done)
template<class T>
FragmentLinkedList<T>::~FragmentLinkedList()
{
    if (empty()) return;

    this->fragmentMaxSize = 0;
    this->count = 0;
    Node *p = this->fragmentPointers[0], *tmp;
    while(p)
    {
        tmp = p;
        p = p->next;
        delete tmp;
    }
}

//Clear (Done)
template <class T>
void FragmentLinkedList<T>::clear()
{
    this->~FragmentLinkedList();
}

//Size (Done)
template <class T>
int FragmentLinkedList<T>::size()
{
    return count;
}

//Get (Done)
template<class T>
T& FragmentLinkedList<T>::get(int index)
{
    Node *p = this->fragmentPointers[0];
    int i = 0;
    while (i < index)
    {
        p = p->next;
        i++;
    }
    return p->data;
}

//Empty check (Done)
template<class T>
bool FragmentLinkedList<T>::empty()
{
    return (!count);
}

//Remove at (Done)
template <class T>
T FragmentLinkedList<T>::removeAt(int index)
{
    Node *p = this->fragmentPointers[0];
    if (index >= count || index < 0 || empty()) throw out_of_range("This index is out of range!");

    if (index == 0)
    {
        this->fragmentPointers[0] = p->next;
        this->count--;

        static T t = p->data;
        delete p;
        return t;
    }

    int i = 0;
    while(i < index) {p = p->next; i++;}
    if (i == count - 1) p->prev->next = nullptr;
    else
    {
        p->prev->next = p->next;
        p->next->prev = p->prev;
    }
    this->count--;

    static T t = p->data;
    delete p;
    return t;
}

//Remove item
template<class T>
bool FragmentLinkedList<T>::removeItem(const T& item)
{
    Node *p = this->fragmentPointers[0];
    int index = 0;
    while(index < count)
        if (p->data != item)
        {
            p = p->next;
            index++;
        } else break;
    if (index < count)
    {
        if (index == 0)
        {
            this->fragmentPointers[0] = p->next;
            this->count--;
            delete p;
            return true;
        }
        if (index == count - 1) p->prev->next = nullptr;
        else
        {
            p->prev->next = p->next;
            p->next->prev = p->prev;
            delete p;
        }

        this->count--;
        return true;
    } else return false;
}

//Set
template<class T>
void FragmentLinkedList<T>::set(int index, const T& element)
{

}

//Index of an element
template <class T>
int FragmentLinkedList<T>::indexOf(const T& item)
{
    Node *p = this->fragmentPointers[0];
    int index = 0;
    while(index < count)
        if (p->data != item)
        {
            p = p->next;
            index++;
        } else break;
    if (index < count) return index; else return -1;
}

//Contain
template <class T>
bool FragmentLinkedList<T>::contains(const T& item)
{
    Node *p = this->fragmentPointers[0];
    int index = 0;
    while(index < count)
        if (p->data != item)
        {
            p = p->next;
            index++;
        } else break;
    if (index < count) return true; else return false;
}

//Add element to the last position (Done)
template<class T>
void FragmentLinkedList<T>::add(const T& element)
{

    if (empty())
    {
        this->fragmentPointers[0] = new Node(element, nullptr, nullptr);
        this->count++;
        return;
    } else
    {
        Node *p = this->fragmentPointers[0];
        while (p->next) p = p->next;
        p->next = new Node(element, nullptr, p);
        this->count++;
    }
}

//Add element to the index position (Done)
template<class T>
void FragmentLinkedList<T>::add(int index, const T& element)
{
    if (index >= this->count)
    {
        add(element);
        this->count++;
    } else
    {
        Node *p = this->fragmentPointers[0];
        int i = 0;
        while(i < index) {p = p->next; i++;}
        Node *t = new Node(element, p, p->prev);
        p->prev->next = t;
        p->prev = t;
        this->count++;
    }
}

template <class T>
string FragmentLinkedList<T>::toString()
{
    stringstream ss;
    ss << "[";
    Node *ptr = this->fragmentPointers[0];

    if (this->count == 0)
        ss << "]";

    // TODO
    if (this->count == 0) return ss.str();
    while(1)
    {
        ss << ptr->data;
        if (ptr->next) ss << ", "; else break;
        ptr = ptr->next;
    }
    ss << "]";
    // END: TODO

    return ss.str();
}

#endif
// END: STUDENT ANSWER

int main()
{
    // TESTCASE INPUT
    // === Example
    FragmentLinkedList<int> fList;
    for(int i = 0; i < 10 ; i++)
    {
        fList.add(i*i);
    }
    cout << fList.toString();
    cout << endl;
    fList.add(4, 7);
    cout << fList.toString();
    cout << endl;
    cout << fList.removeAt(4) << endl;
    cout << fList.toString();
    cout << endl;
    cout << fList.removeAt(fList.size() - 1) << endl;
    cout << fList.toString();
    cout << endl;
    cout << fList.size() << endl;
    fList.removeItem(5);
    cout << fList.toString();
    cout << endl;
    cout << fList.size() << endl;
    cout << fList.indexOf(6) << ' ' << fList.indexOf(10) << ' ' << fList.contains(6) << ' ' << fList.contains(10);
    fList.clear();
    cout << fList.toString();

    // for(FragmentLinkedList<int>::Iterator it = fList.begin(); it != fList.end(); it++)
    //     cout << *it << " ";
    // cout << endl;
    // === END: Example
    // END: TESTCASE INPUT


    return 0;
}
