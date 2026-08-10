#pragma once
#include <iostream>

struct Node{
    int data;
    Node *next = nullptr;
};

struct LinkedList{
    Node *head = nullptr;
    Node *tail = nullptr;
    size_t size = 0;
};


void clear(LinkedList& list);                                  // O(n) frees every node

// --- insertion ---
void pushFront(LinkedList& list, int value);                   // O(1)
void pushBack (LinkedList& list, int value);                   // O(1)  thanks to tail
bool insertAt (LinkedList& list, size_t index, int value);// O(n)

// --- removal ---
bool popFront (LinkedList& list);                              // O(1)
bool popBack  (LinkedList& list);                              // O(n)  <-- see note
bool removeAt (LinkedList& list, size_t index);           // O(n)
bool removeValue(LinkedList& list, int value);                 // O(n)  first match only

// --- access / query ---
Node* at      (const LinkedList& list, size_t index);     // O(n)  nullptr if OOB
Node* find    (const LinkedList& list, int value);             // O(n)  nullptr if absent
bool  contains(const LinkedList& list, int value);             // O(n)
bool  empty   (const LinkedList& list);                        // O(1)

// --- utility ---
void reverse(const LinkedList& list);                          // O(1) space, O(n) time
void print  (const LinkedList& list);                          // O(n)

void testList();