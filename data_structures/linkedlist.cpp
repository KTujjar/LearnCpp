#include "linkedlist.hpp"

void pushFront(LinkedList& list, int value){
    Node *t = new Node;

    t->data = value;
    t->next = list.head;

    if(empty(list)) list.tail = t;
    list.head = t;
    
    list.size += 1;
}    

void pushBack(LinkedList& list, int value){
    Node *t = new Node;
    t->data = value;
    t->next = nullptr;

    if(empty(list)) list.head = t;
    else list.tail->next = t;
    
    list.tail = t;

    list.size += 1;
}        
bool insertAt(LinkedList& list, size_t index, int value){
    
    if(index > list.size - 1) return false;
    Node *t = new Node;
    t->data = value;

    if(index == 0){
        t->next = list.head;
        list.head = t;

        list.size += 1;
        return true;
    }

    Node *p = at(list, index - 1);
    if(p->next == nullptr){
        return false;
    }
    t->next = p->next;

    p->next = t;

    list.size += 1;

    return true;
}

bool removeAt(LinkedList& list, size_t index){
    if(empty(list)) return false;

    Node *t;

    if(index == 0){
        t = list.head;
        list.head = list.head->next;
        if(list.size == 1) list.tail = nullptr;
        delete t;
        list.size -= 1;
        return true;
    } 
    t = at(list, index - 1);
    Node *d = t->next;
    if(d == list.tail) list.tail = t; 
    t->next = t->next->next;

    list.size -= 1;
    delete d;
    return true;
}

Node* at(const LinkedList& list, size_t index){
    size_t i = 0;
    for(Node *cur = list.head; cur != nullptr; cur = cur->next){
        if(i == index) return cur;
        i++;
    }
    return nullptr;
}


bool empty(const LinkedList& list){
    if(list.size == 0) return true;

    return false;
}


void print(const LinkedList& list){
    for(Node *cur = list.head; cur != nullptr; cur = cur->next){
        std::cout << cur->data << " ";
    }
    std::cout << " head: " << list.head->data << " tail: " << list.tail->data;
    std::cout << "\n";
}

void testList(){
    LinkedList list;

    std::cout << "pushFront \n";
    pushFront(list, 8);
    print(list);

    std::cout << "pushBack \n";
    pushBack(list, 6);
    print(list);

    std::cout << "insertAt \n";
    insertAt(list, 1, 2);
    print(list);

    std::cout << "removeAt \n";
    removeAt(list, 2);
    print(list);
    
}