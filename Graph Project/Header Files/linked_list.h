#include <bits/stdc++.h>
using namespace std;

class Node
{
private:
    int value;
    Node *next;
    Node *prev;

public:
    Node(int element, Node *ptr)
    {
        value = element;
        next = ptr;
        prev = NULL;
    }
    Node(int element)
    {

        value = element;
        next = NULL;
        prev = NULL;
    }
    friend class Linked_list;
    friend class Graph;
};

class Linked_list
{
private:
    int size = 0;
    Node *head;
    Node *tail;

public:
    void addFront(int element);
    void addBack(int element);
    int length();
    Node *search(int element);
    void printList();
    void removeNode(int element);
    int isEmpty();

    Linked_list();
    friend class Graph;
};

Linked_list::Linked_list()
{

    head = NULL;
    tail = NULL;
}

void Linked_list::addFront(int element)
{
    size++;
    Node *n1 = new Node(element);
    n1->next = head;
    n1->prev = NULL;
    if (size == 1)
    {
        tail = n1;
    }
    head = n1;
}

int Linked_list::length()
{
    return size;
}

void Linked_list::printList()
{
    if (isEmpty() == 1)
    {
        cout << "{}" << endl;
        return;
    }

    Node *iter = head;
    while (iter != NULL)
    {
        cout << iter->value << " ";
        iter = iter->next;
    }
    cout << endl;
}

int Linked_list::isEmpty()
{
    if (head == NULL)
    {
        return 1;
    }
    return 0;
}

void Linked_list::addBack(int element)
{
    size++;
    // cout << "Hey again " << element << endl;
    Node *n2 = new Node(element);

    if (tail != NULL)
    {
        tail->next = n2;
    }
    n2->prev = tail;
    tail = n2;

    if (size == 1)
    {
        head = n2;
    }
}

Node *Linked_list::search(int element)
{

    Node *iter = head;
    while (iter != NULL)
    {
        if (iter->value == element)
        {
            return iter;
        }
        else
        {
            iter = iter->next;
        }
    }
    return iter;
}

void Linked_list::removeNode(int element)
{

    if (head->value == element)
    {
        if (head->next != NULL)
        {
            head->next->prev = NULL;
        }
        head = head->next;
        size--;
    }
    else if (tail->value == element)
    {
        if (tail->prev != NULL)
        {
            tail->prev->next = NULL;
        }
        tail = tail->prev;
        size--;
    }
    else
    {
        Node *iter = head;

        while (iter != NULL)
        {
            if (iter->value == element)
            {
                if (iter->prev != NULL)
                {
                    iter->prev->next = iter->next;
                }
                if (iter->next != NULL)
                {
                    iter->next->prev = iter->prev;
                }
                size--;
                break;
            }

            iter = iter->next;
        }
    }

    return;
}