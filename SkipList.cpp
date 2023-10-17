// #include<limits>
#include<cstdlib>
#include<iostream>
#include <math.h>
#include <time.h>

typedef struct SLNode {
  // Value is not needed
  int val;
  int width;
  // size_t height;
  SLNode* next;
  SLNode* down;
} SLNode;

SLNode* createSLNode(int val) {
  SLNode* node = (SLNode*) malloc(sizeof(SLNode));
  node->val = val;
  node->width = 0;
  node->next = nullptr;
  node->down = nullptr;
  return node;
}

typedef struct LLNode {
  SLNode* val;
  LLNode* next;
  LLNode* prev;
} LLNode;

LLNode* createLLNode(SLNode* val) {
  LLNode* node = (LLNode*) malloc(sizeof(LLNode));
  node->val = val;
  node->next = nullptr;
  node->prev = nullptr;
  return node;
}

bool flipCoin() {
  return rand() % 2;
}

class SkipList {
  private:
    SLNode* head; // Head
    int levels;
    int length; // Number of nodes
    LLNode* bTail; // Doubly linked list of pointers to tail elements (starting from bottom)
    LLNode* tTail; // Points to top of the list above

    void make_new_level() {
      SLNode* newHead = createSLNode(this->head->val);
      newHead->down = this->head;
      SLNode* curr = this->head;
      while (curr != nullptr) {
        newHead->width += curr->width;
        curr = curr->next;  
      }
      this->head = newHead;
      this->head->next = nullptr;

      // update top of tail pointer
      this->tTail->next = createLLNode(this->head);
      this->tTail->next->prev = this->tTail;
      this->tTail = this->tTail->next;

      this->levels++;
    }
  
  public:
    SkipList() {
      srand(time(NULL));
      this->head = nullptr;
      this->tTail = nullptr;
      this->bTail = nullptr;
      this->levels = 0;
      this->length = 0;
    }

    int get(int index) {
      if (index < 0 || index >= this->length) {
        return -1;
      }

      SLNode* curr = this->head;

      while (curr->down != nullptr) {
        while (curr->next != nullptr && index - curr->width >= 0) { // index is right, so won't hit
          index -= curr->width;
          curr = curr->next;
        }
        if (index == 0) {
          return curr->val;
        }
        curr = curr->down;
      }

      while (index != 0) { // Length should be right, so no need to check end
        index -= curr->width;
        curr = curr->next;
      }

      return curr->val;
    }

    void addAtIndex(int index, int value) {
      if (index < 0 || index > this->length) {
        return ; // -1 
      }
      else if (index == 0) {
        // Need new Head
        addAtHead(value);
        return;
      }
      else {

        if (floor(log2(this->length + 1)) > this->levels) {
          this->make_new_level();
        }

        SLNode* curr = this->head;

        LLNode* nodes = nullptr;

        while (curr->down != nullptr) {
          while (curr->next != nullptr && index - curr->width > 0) {
            index -= curr->width;
            curr = curr->next;
          }
          // curr->width++; // Add one to width for the value we are adding
          if (nodes == nullptr) {
            nodes = createLLNode(curr);
          }
          else {
            LLNode* ptr = createLLNode(curr);
            ptr->next = nodes;
            nodes = ptr;
          }

          if (curr->next != nullptr) {
            curr->width += 1; // update width
          }
          curr = curr->down;
        }

        while (index - curr->width > 0) {
          index -= curr->width;
          curr = curr->next;
        }

        LLNode* currTail = bTail;

        SLNode* newNode = createSLNode(value);
        newNode->next = curr->next;
        curr->next = newNode;
        newNode->width = 1;
        curr = newNode;

        if (curr->next == nullptr) {
          currTail->val = currTail->val->next;
        }

        currTail = currTail->next;

        while (nodes != nullptr && flipCoin()) {
          SLNode* newNode = createSLNode(value);
          newNode->down = curr;
          newNode->next = nodes->val->next;
          nodes->val->next = newNode;

          int s = 0;
          curr = nodes->val->down;
          while (curr != newNode->down) {
            s += curr->width;
            curr = curr->next;
          }
          
          if (newNode->next == nullptr) {
            // Set width to 1 and update currTail
            newNode->width = 1;
            currTail->val = currTail->val->next;
          }
          else {
            newNode->width = nodes->val->width - s;
          }          
        
          nodes->val->width = s;

          LLNode* ptr = nodes;
          nodes = nodes->next;
          free(ptr);
          curr = newNode;
          currTail = currTail->next;
        }

        while (nodes != nullptr) {
          LLNode* ptr = nodes;
          nodes = nodes->next;
          free(ptr);
        }
        this->length++;
      }
    }

    void addAtHead(int value) {
      if (this->length == 0) {
        this->head = createSLNode(value);
        this->head->width = 1;
        this->tTail = createLLNode(this->head);
        this->bTail = this->tTail;
        this->levels++;
        this->length++;
      }
      else {
        // Swap value at head for new value, and insert head
        SLNode* curr = this->head;
        int headVal = curr->val;

        while (curr != nullptr) {
          curr->val = value;
          curr = curr->down;
        }

        // Insert old head at index 1
        this->addAtIndex(1, headVal);
      }
    }

    void addAtTail(int value) {
      if (this->length == 0) {
        this->addAtHead(value);
        return;
      }

      if (floor(log2(this->length + 1)) > this->levels) {
        this->make_new_level();
      }

      LLNode* currTail = this->bTail;
      SLNode* newNode = createSLNode(value);
      newNode->next = nullptr;
      newNode->width = 1;
      
      SLNode* curr = newNode;

      currTail->val->next = newNode;
      
      currTail->val = currTail->val->next;

      currTail = currTail->next;

      while (currTail != nullptr && flipCoin()) {
        
        newNode = createSLNode(value);
        newNode->down = curr;
        newNode->next = nullptr;
        newNode->width = 1; // Since at the end, move up
        currTail->val->next = newNode;

        // Get the length
        int s = 0;
        curr = currTail->val->down; // Set to be down of previous tail (currTail->val->down)
        while (curr != newNode->down) {
          s += curr->width;
          curr = curr->next;
        }     
        
        currTail->val->width = s;

        currTail->val = currTail->val->next; // Move to next value in tail

        curr = newNode;
        currTail = currTail->next;
      }

      this->length++;
    }

    void deleteAtIndex(int index) {

      if (index < 0 || index >= this->length) {
        return; // Invalid index
      }

      if (this->levels >= 2 && floor(log2(this->length - 1)) < this->levels) {
        this->delete_top_level();
      }

      if (index == 0) {
        // Move the value at index 1 head
        if (this->length == 1) {
          free(this->head);
          free(this->tTail);
          this->head = nullptr;
          this->tTail = nullptr;
          this->bTail = nullptr;
          this->length--;
          this->levels--;
          return;
        }
        else {
          SLNode* curr = this->head;
          while (curr->down != nullptr) {
            curr = curr->down;
          }
          int val = curr->next->val;

          curr = this->head;
          while (curr != nullptr) {
            curr->val = val;
            curr = curr->down;
          }
          // New value set, delete one in front

          index = 1; // Delete the value at index 1
        }
      }

      SLNode* curr = this->head;

      LLNode* currTail = this->tTail;

      index -= 1; // Want to stop right before it

      while (curr->down != nullptr) {
          
        while (curr->next != nullptr && index - curr->width >= 0) {
          index -= curr->width; // update index
          curr = curr->next;
        }

        if (curr->next != nullptr && index - curr->width == -1) { // If we are right in front of it
          SLNode* ptr = curr->next;
          if (ptr == currTail->val) { // Implies that next is nullptr
            currTail->val = curr;
          }
          curr->width += ptr->width;
          curr->next = ptr->next;
          free(ptr);
        }

        if (curr->next != nullptr) {
          curr->width--;
        }

        curr = curr->down;
        currTail = currTail->prev;
      }
      
      
      while (index != 0) {
        index -= curr->width; // update index
        curr = curr->next;
      }
        
      SLNode* ptr = curr->next;
      curr->next = ptr->next;

      if (currTail->val == ptr) {
        currTail->val = curr;
      }

      free(ptr);

      this->length--;
    }

    void delete_top_level() {
      SLNode* curr = this->head;
      this->head = this->head->down;
      this->levels--;

      // Free level
      while (curr != nullptr) {
        SLNode* ptr = curr;
        curr = curr->next;
        free(ptr);
      }

      LLNode* ptr = this->tTail;
      if (this->tTail == this->bTail) {
        free(this->tTail);
        this->tTail = nullptr;
        this->bTail = nullptr;
      }
      else {
        LLNode* ptr = this->tTail;
        this->tTail = ptr->prev;
        this->tTail->next = nullptr;
        free(ptr);
      }
      
    }
    
    void printList() {
        SLNode* front = this->head;
        while (front != nullptr) {
            SLNode* curr = front;
            while (curr->next != nullptr) {
                std::cout << curr->val;
                std::cout << ", ";
                int count = 0;
                while (count < curr->width - 1) {
                    std::cout << "  ";
                    count++;
                }
                curr = curr->next;
            }
            std::cout << curr->val;
            std::cout << "\n";
            front = front->down;
        }
    }

    void deleteAll() {
      // Since there is a top level that has no nodes, delete it
    //   delete_top_level();

    //   SLNode* curr = this->head;

    //   while (curr->down != nullptr) {

    //     curr = curr->next; // Move to next node;

    //     while (curr->next != nullptr) {
    //       SLNode* ptr = curr;
    //       curr = curr->next;
    //       delete ptr; // Free the node
    //     }

    //     // All nodes should be freed now on this level
    //     curr = this->head->down; // Move down to next level

    //     delete_top_level();

    //   }

    //   while (curr != nullptr) {
    //     SLNode* ptr = curr;
    //     curr = curr->next;
    //     free(ptr);
    //   }

    
    SLNode* front = this->head;

    while (front != nullptr) {
    
        SLNode* curr = front->next;

        while (curr != nullptr) {
            SLNode* ptr = curr;
            curr = curr->next;
            free(ptr);
        }

        SLNode* ptr = front;
        front = front->down;
        free(ptr);
    }

    LLNode* tailCurr = bTail;

    while (tailCurr != nullptr) {
        LLNode* ptr = tailCurr;
        tailCurr = tailCurr->next;
        free(ptr);
    }
      
    }

    int getLength() {
      return this->length; 
    }

};
