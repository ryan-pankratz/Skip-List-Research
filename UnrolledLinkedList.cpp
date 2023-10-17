#include<iostream>
#include<cstdlib>
#include <chrono>
#include <time.h>
#include "mVal.h"

void arrayCuckooInsert(int arr[], int size, int index, int val) {

  int item = arr[index];

  arr[index] = val;


  while (index + 1 < size) {

    int temp = arr[index + 1];

    arr[index + 1] = item;

    item = temp;

    index++;

  }

}



void removeAtIndex(int arr[], int size, int index) {

  size--;

  int item = arr[size];

  arr[size] = -1; // Put blank value

  while (size > index) {

    int temp = arr[size - 1];

    arr[size - 1] = item;

    item = temp;

    size--;

  }

  // We have removed arr[index]

}



typedef struct ULLNode {

    int items[M]; // 4 * 13 = 52 bytes

    int size {1}; // 4 bytes, only need max between 0 and 16

    struct ULLNode* next {nullptr}; // 8 bytes

} ULLNode;



ULLNode* createULLNode() {

  ULLNode* n = (ULLNode*) malloc(sizeof(ULLNode));

  n->size = 0;

  n->next = nullptr;

  return n;

};



void insertToNode(ULLNode* node, int index, int item) {

  if (node->size < M) {

    node->size++;

    arrayCuckooInsert(node->items, node->size, index, item);

  }

  else {

    // Cuckoo Insert and push end into next

    int itemAtEnd = node->items[M - 1];

    arrayCuckooInsert(node->items, node->size, index, item);

    if (node->next == nullptr || node->next->size == M) {

      // Put new node to avoid shifting all the indexes

      ULLNode* n = createULLNode();

      n->next = node->next;

      node->next = n;

    }

    insertToNode(node->next, 0, itemAtEnd);

  }

}



void deleteFromNode(ULLNode* node, int index) {

  removeAtIndex(node->items, node->size, index);

  node->size--;



  if (node->next != nullptr && node->next->size + node->size <= M) {

    // Absorb next node

    int c = 0;

    ULLNode* ptr = node->next;

    while (c < ptr->size) {

      node->items[node->size + c] = ptr->items[c];

      c++;

    }

    node->size += ptr->size;

    ptr->size = 0;

    node->next = ptr->next;

    free(ptr);

  }

}



class ULinkedList {

  public:

    int length;

    ULLNode* head;

    ULLNode* tail;



    ULinkedList() {

      this->length = 0;

      this->head = createULLNode();

      this->tail = this->head;

    }



    int get(int index) {

      if (index < 0 || index >= this->length) {

        return -1;

      }

      ULLNode* curr = this->head;



      while (index - curr->size >= 0) {

        index -= curr->size;

        curr = curr->next;

      }



      return curr->items[index];

    }



    void addAtHead(int val) {

      insertToNode(this->head, 0, val);



      this->length++;

      if (this->tail->next != nullptr) {

        this->tail = this->tail->next;

      }

    }



    void addAtTail(int val) {

      if (this->tail->size == M) {

        this->tail->next = createULLNode();

        this->tail = this->tail->next;

      }

      insertToNode(this->tail, this->tail->size, val);

      this->length++;

    }



    void addAtIndex(int index, int val) {

      if (index < 0 || index > this->length) {

        return; // Bad index

      }

      else if (index == this->length) {

        this->addAtTail(val); // Add to tail

        return; // Return, already inserted and maintained length

      }



      ULLNode* curr = this->head;



      while((curr->size == M && index - curr->size >= 0) || (curr->size < M && index - (curr->size + 1) >= 0)) {

        index -= curr->size;

        curr = curr->next;

      }



      insertToNode(curr, index, val);



      if (this->tail->next != nullptr) {

        this->tail = this->tail->next;

      }



      this->length++;



    }



    void deleteAtIndex(int index) {

      if (index < 0 || index >= this->length) {

        return;

      }



      ULLNode* prev = nullptr;

      ULLNode* curr = this->head;



      while (index - curr->size >= 0) {

        index -= curr->size;

        prev = curr;

        curr = curr->next;

      }



      deleteFromNode(curr, index);

      this->length--;



      // Check if tail needs to be updated

      if (prev != nullptr && curr == this->tail && curr->size == 0) {

        ULLNode* ptr = this->tail; 

        this->tail = prev;

        prev->next = nullptr;

        free(ptr);

      } 

      else if (curr->next == nullptr) {

        this->tail = curr;

      } 

    }

    

    int getLength() {

        return this->length;

    }

    void deleteAll() {
        ULLNode* curr = this->head;
        
        while (curr != nullptr) {
            ULLNode* ptr = curr;
            curr = curr->next;
            free(ptr);
        } 
    }


};


int main() {
    int n = 100000;
    int trials = 100;
    int c = 1;

    double avg = 0;

    while (c <= trials) {
      ULinkedList* ull = new ULinkedList();
    // std::cout << "=== Unrolled Linked List insert time ===\n";

      auto start = std::chrono::high_resolution_clock::now();

      ull->addAtIndex(0, rand() % n);

      for (int i = 1; i < n; i++) {

          ull->addAtTail(rand() % n);

      }

      

      auto end = std::chrono::high_resolution_clock::now();

      auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

      avg = avg + (double) ((duration.count() - avg) / c);

      // std::cout << "Average Runtime: ";

      // std::cout << (duration.count() / n);

      // std::cout << "\n";

      // std::cout << "Total Runtime: ";

      ull->deleteAll();
      delete ull;
      c++;
    }

    std::cout << avg;

    // std::cout << "\n";

    return 0;
}