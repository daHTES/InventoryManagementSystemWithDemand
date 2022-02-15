#include <iostream>

class ListNode
{
public:
    int data;
    ListNode* next;
    ListNode(int val) :data(val), next(NULL) {}
};

class List
{
public:
    ListNode* head;
    List() :head(NULL) {}
    void insertAtBegin(int val);
    void insertAtEnd(int val);
    void insertAtPos(int pos, int val);
    void remove(int val);
    void print();
    ~List();
};

void List::insertAtBegin(int val)//inserting at front of list
{
    ListNode* newnode = new ListNode(val);
    newnode->next = this->head;
    this->head = newnode;
}

void List::insertAtEnd(int val) //inserting at end of list
{
    if (head == NULL)
    {
        insertAtBegin(val);
        return;
    }
    ListNode* newnode = new  ListNode(val);
    ListNode* ptr = this->head;
    while (ptr->next != NULL)
    {
        ptr = ptr->next;
    }
    ptr->next = newnode;
}

void List::insertAtPos(int pos, int val)
{
    ListNode* newnode = new  ListNode(val);
    if (pos == 1)
    {
        //as head
        newnode->next = this->head;
        this->head = newnode;
        return;
    }
    pos--;
    ListNode* ptr = this->head;
    while (ptr != NULL && --pos)
    {
        ptr = ptr->next;
    }
    if (ptr == NULL)
        return;//not enough elements
    newnode->next = ptr->next;
    ptr->next = newnode;
}

void List::remove(int toBeRemoved)//removing an element
{
    if (this->head == NULL)
        return; //empty
    if (this->head->data == toBeRemoved)
    {
        //first node to be removed
        ListNode* temp = this->head;
        this->head = this->head->next;
        delete(temp);
        return;
    }
    ListNode* ptr = this->head;
    while (ptr->next != NULL && ptr->next->data != toBeRemoved)
        ptr = ptr->next;
    if (ptr->next == NULL)
        return;//not found
    ListNode* temp = ptr->next;
    ptr->next = ptr->next->next;
    delete(temp);
}

void List::print()//printing the list
{
    ListNode* ptr = this->head;
    while (ptr != NULL)
    {
        std::cout << ptr->data << " ";
        ptr = ptr->next;
    }
    std::cout << std::endl;
}

List::~List()
{
    ListNode* ptr = this->head, * next = NULL;
    while (ptr != NULL)
    {
        next = ptr->next;
        delete(ptr);
        ptr = next;
    }
}