#include <iostream>
#include <cmath>
#include <cstdlib>
#include <climits>
#include<vector>
#include <chrono>

#include<limits>
#include<cstdlib>
#include<iostream>

float INF = std::numeric_limits<float>::infinity();

class DSLNode {
  public:
    float* val;

    DSLNode* next;
    DSLNode* down;
    int height;

    DSLNode(float* value) {
      this->val = value;
      this->next = nullptr;
      this->down = nullptr;
      this->height = 1;
    }

    void setDown(DSLNode* n) {
      this->height = n->height;
      this->down = n;
      this->incrementHeight();
    }

    void decreaseHeight() {
      DSLNode* curr = this;
      while (curr != nullptr) {
        curr->height -= 1;
        curr = curr->down;
      }
    }

  private:
    void incrementHeight() {
      DSLNode* curr = this;
      while (curr != nullptr) {
        curr->height += 1;
        curr = curr->down;
      }
    }
};

class DSkiplist {
  private:
    DSLNode* head; // Head
    DSLNode* tail; // Tail
    int levels;
    int length; // Number of nodes

    DSLNode* search_on_level(DSLNode* curr, float value) {
      while (*(curr->next->val) < value) { // End is inf, so won't hit that
        curr = curr->next;
      }
      return curr;
    }

    void make_new_level() {
      DSLNode* newHead = new DSLNode(this->head->val);
      DSLNode* newTail = new DSLNode(this->tail->val);
      newHead->setDown(this->head);
      newTail->setDown(this->tail);
      this->head = newHead;
      this->tail = newTail;
      this->head->next = this->tail;

      this->levels++;
    }

    void delete_top_level() {
      DSLNode* ptrHead = this->head;
      DSLNode* ptrTail = this->tail;
      this->head = ptrHead->down;
      this->tail = ptrTail->down;

      delete ptrHead;
      delete ptrTail;

      this->head->decreaseHeight();
      this->tail->decreaseHeight();

      this->levels--;
    }
  
  public:
    DSkiplist() {
      float* negative = (float*) malloc(sizeof(float));
      float* positive = (float*) malloc(sizeof(float));

      *negative = -INF;
      *positive = INF;

      this->head = new DSLNode(negative);
      this->tail = new DSLNode(positive);

      this->head->next = this->tail;

      DSLNode* levelOneHead = new DSLNode(negative);
      DSLNode* levelOneTail = new DSLNode(positive);

      this->head->setDown(levelOneHead);
      this->tail->setDown(levelOneTail);

      levelOneHead->next = levelOneTail;
      this->levels = 1;
      this->length = 0;
    }

    bool search(float value) {
      DSLNode* curr = this->head;

      while (curr->down != nullptr) {
        curr = search_on_level(curr, value);
        curr = curr->down;
      }

      curr = search_on_level(curr, value);

      return *(curr->next->val) == value;
    }

    void add(float value) {
      DSLNode* curr = this->head;

      while (curr->down != nullptr) {
        curr = search_on_level(curr, value);

        DSLNode* tempCurr = curr->down->next;
        int h = tempCurr->height;

        // If 3 in a row of same height
        if (tempCurr->next != nullptr && tempCurr->next->next != nullptr && tempCurr->next->height == h && tempCurr->next->next->height == h) {
          DSLNode* newNode = new DSLNode(tempCurr->next->val);
          newNode->setDown(tempCurr->next);
          newNode->next = curr->next;
          curr->next = newNode;

          if (newNode->height == this->levels + 1) {
            make_new_level();
          }
        }

        curr = curr->down;
      }

      curr = search_on_level(curr, value);
      float* insertVal = (float*) malloc(sizeof(float));
      *insertVal = value;
      DSLNode* newNode = new DSLNode(insertVal);
      newNode->next = curr->next;
      curr->next = newNode;

      this->length++;
    }

    bool erase(float value) {
      // StaticStack s = StaticStack(this->levels);

      if (!search(value)) {
        return false;
      }

      DSLNode* curr = this->head->down;

      while (curr->down != nullptr) {
        bool deleteCurr = false;
        if (*(curr->next->val) < value) {
          DSLNode* prev;

          while (*(curr->next->val) < value) {
            prev = curr;
            curr = curr->next;
          }

          DSLNode* currTemp = curr->down->next;

          if (currTemp->next != nullptr && currTemp->next->height != currTemp->height) {
            currTemp = prev->down->next;
            int num_at_height = 1;

            while (currTemp->height == currTemp->next->height) {
              num_at_height++;
              currTemp = currTemp->next;
            }

            if (num_at_height >= 2) {
              DSLNode* newNode = new DSLNode(currTemp->val);
              newNode->setDown(currTemp);
              prev->next = newNode;
              newNode->next = curr;
              prev = newNode;
            }

            curr->decreaseHeight();
            prev->next = curr->next;
            deleteCurr = true;

            // if (prev->next->val == value) {
            //   s.push(prev->next);
            // }
          }
          // else if (curr->next->val == value) {
          //   s.push(curr->next);
          // }
        }
        else if (*(curr->next->val) != INF) {
          DSLNode* currTemp = curr->down->next;
          if (currTemp->next != nullptr && currTemp->next->height != currTemp->height) {
            currTemp = curr->next->down->next;
            // Lower divider
            DSLNode* tempNode = curr->next;
            tempNode->decreaseHeight();
            curr->next = tempNode->next;
            delete tempNode;

            if (currTemp->next->height == currTemp->height) {
              // raise currTemp

              DSLNode* newNode = new DSLNode(currTemp->val);
              newNode->setDown(currTemp);
              newNode->next = curr->next;
              curr->next = newNode;
            }
          }
          // else if (curr->next->val == value) {
          //   s.push(curr->next);
          // }
        }
        if (!deleteCurr) {
          curr = curr->down;
        }
        else {
          DSLNode* ptr = curr->down;
          delete curr;
          curr = ptr;
        }
      }

      DSLNode* prev;

      while (*(curr->next->val) < value) {
        prev = curr;
        curr = curr->next;
      }

      if (curr->next->height == 1) {
          DSLNode* ptr = curr->next;
          curr->next = ptr->next;
          free(ptr->val);
          delete ptr;
      }
      else {
        *(curr->next->val) = *(curr->val);
        prev->next = curr->next;

        //Free curr
        free(curr->val);
        delete curr;

      }

      this->length--;

      return true;
    }

    void printList() {
      int i = 0;

      while (i < this->levels + 1) {
        DSLNode* curr = this->head;
        int j = 0;

        while (j < i) {
          curr = curr->down;
          j++;
        }

        while (curr != nullptr) {
          std::cout << *(curr->next->val);
          curr = curr->next;
        }

        std::cout << '\n';
        i++;
      }
    }

    ~DSkiplist() {
      // Since there is a top level that has no nodes, delete it
      delete_top_level();

      DSLNode* curr = this->head;

      while (curr->down != nullptr) {

        curr = curr->next; // Move to next node;

        while (curr->next != nullptr) {
          DSLNode* ptr = curr;
          curr = curr->next;
          delete ptr; // Free the node
        }

        // All nodes should be freed now on this level
        curr = this->head->down; // Move down to next level

        delete_top_level();

      }

      while (curr != nullptr) {
        DSLNode* ptr = curr;
        curr = curr->next;
        free(ptr->val); // Free the value pointer
        delete ptr;
      }
      
    }

};

/* A Skiplist node class */
class SLNode {
private:
    int val;
    std::vector<SLNode*> next; // Size O(logn) in worst case

public:
    SLNode(int val) {
        this->val = val;
    }

    SLNode* getNext(int level) {
        return this->next[level];
    }

    void addNext(SLNode* node) {
        this->next.push_back(node);
    }

    void setNext(SLNode* node, int level) {
        this->next[level] = node;
    }

    int getValue() {
        return this->val;
    }

    int getHeight() {
        return this->next.size();
    }

    void deleteTopLevel() {
        this->next.pop_back();
    }

    void * operator new(size_t size) {
        void *p = malloc(size);
        return p;
    }

    void operator delete(void *p) {
        ((SLNode*) p)->next.clear();
        free(p);
    }
};

class VSkiplist {
private:
    size_t length;
    SLNode* head;

    int coinflip() {
        return rand() % 2;
    }

public:
    VSkiplist() {
        this->length = 0;
        this->head = new SLNode(INT_MIN);
    }
    
    bool search(int target) {
        SLNode* curr = this->head;
        int level = this->head->getHeight() - 1; // Get current level
        while (level >= 0) {
            while (curr->getNext(level) != nullptr && curr->getNext(level)->getValue() < target) {
                curr = curr->getNext(level);
            }

            if (curr->getNext(level) != nullptr && curr->getNext(level)->getValue() == target) {
                return true;
            }
            level--;
        }

        return false;
    }
    
    void add(int num) {
        std::vector<SLNode*> stack;
        stack.push_back(this->head); // In case we add a new level
        SLNode* curr = this->head;
        int level = this->head->getHeight() - 1;

        // Find places to add
        while (level >= 0) {
            while (curr->getNext(level) != nullptr && curr->getNext(level)->getValue() < num) {
                curr = curr->getNext(level);
            }

            stack.push_back(curr);
            level--;
        }
        level = 0;
        SLNode* node = new SLNode(num);
        int newHeight = log2(this->length + 1);

        if (newHeight == 0 || newHeight > this->head->getHeight()) {
            this->head->addNext(nullptr); // Add new level
        }

        // Add item with coinflips
        while (level < this->head->getHeight() && (level == 0 || this->coinflip())) {
            SLNode* prev = stack.back();
            stack.pop_back();
            node->addNext(prev->getNext(level));
            prev->setNext(node, level);
            level++;
        }

        stack.clear();
        this->length++;
    }
    
    bool erase(int num) {
        SLNode* curr = this->head;
        int level = this->head->getHeight() - 1;
        bool found = false;

        while (level >= 0) {
            while (curr->getNext(level) != nullptr && curr->getNext(level)->getValue() < num) {
                curr = curr->getNext(level);
            }

            if (curr->getNext(level) != nullptr && curr->getNext(level)->getValue() == num) {
                SLNode* temp = curr->getNext(level);
                curr->setNext(temp->getNext(level), level);
                temp->deleteTopLevel();

                if (level == 0) {
                    found = true;
                    this->length--;
                    delete temp;
                }
            }
            level--;
        }

        int newHeight = log2(this->length);

        if (this->length != 1 && this->head->getHeight() > newHeight) {
            /* Clear top level */
            int top = this->head->getHeight() - 1;
            curr = this->head;
            while (curr != nullptr) {
                SLNode* temp = curr;
                curr = curr->getNext(top);
                temp->deleteTopLevel();
            }
        }

        return found;
    }

    void * operator new(size_t size) {
        void *p = malloc(size);
        return p;
    }

    void operator delete(void *p) {
        SLNode* curr = ((VSkiplist*) p)->head;
        while (curr != nullptr) {
            SLNode* temp = curr;
            curr = curr->getNext(0);
            delete temp;
        }
        free(p);
    }
};

/**
 * Your Skiplist object will be instantiated and called as such:
 * Skiplist* obj = new Skiplist();
 * bool param_1 = obj->search(target);
 * obj->add(num);
 * bool param_3 = obj->erase(num);
 */

template <typename T>
class StaticStack {
public:
    StaticStack(int size) : size(size), top(0), values(new T[size]) {}

    ~StaticStack() {
        delete[] values;
    }

    void push(T item) {
        if (top < size) {
            values[top] = item;
            top++;
        }
    }

    T pop() {
        if (top > 0) {
            top--;
            return values[top];
        }
        return T(); // Default value for T (0 for numeric types, NULL for pointers, etc.)
    }

    bool is_empty() const {
        return top == 0;
    }

private:
    int size;
    int top;
    T* values;
};

class Node {
public:
    int val;
    Node* next;
    Node* down;

    Node(int val) : val(val), next(NULL), down(NULL) {}
};

class Skiplist {
public:
    Skiplist();
    ~Skiplist();
    bool search(int target);
    void add(int num);
    bool erase(int num);

private:
    Node* search_on_level(Node* curr, int value);
    void make_new_level();
    void delete_top_level();
    bool coinflip();

    Node* head;
    int levels;
    int length;
};

Skiplist::Skiplist() {
    head = new Node(INT_MIN);
    head->next = NULL;
    levels = 1;
    length = 0;
}

Skiplist::~Skiplist() {
    while (levels != 0) {
        delete_top_level();
    }
}

bool Skiplist::search(int target) {
    Node* curr = head;
    while (curr->down != NULL) {
        curr = search_on_level(curr, target);
        curr = curr->down;
    }
    curr = search_on_level(curr, target);

    return curr->next != NULL && curr->next->val == target;
}

void Skiplist::add(int num) {
    if (floor(log2(length + 1)) > levels) {
        make_new_level();
    }

    StaticStack<Node*> s(levels);

    Node* curr = head;
    while (curr->down != NULL) {
        curr = search_on_level(curr, num);
        s.push(curr);
        curr = curr->down;
    }
    curr = search_on_level(curr, num);
    Node* new_node = new Node(num);
    new_node->next = curr->next;
    curr->next = new_node;
    Node* temp = new_node;

    length += 1;
    int i = 1;
    while (i < levels && coinflip()) {
        curr = s.pop();
        Node* new_node = new Node(num);
        new_node->next = curr->next;
        curr->next = new_node;
        new_node->down = temp;
        temp = new_node;
        i += 1;
    }
}

void Skiplist::make_new_level() {
    Node* new_head = new Node(INT_MIN);
    new_head->down = head;
    head = new_head;
    levels += 1;
}

bool Skiplist::coinflip() {
    return (static_cast<float>(rand()) / RAND_MAX) < 0.5;
}

bool Skiplist::erase(int num) {
    if (length > 2 && floor(log2(length - 1)) < levels) {
        delete_top_level();
    }
    bool foundVal = false;

    Node* curr = head;
    while (curr != NULL) {
        curr = search_on_level(curr, num);
        if (curr->next != NULL && curr->next->val == num) {
            foundVal = true;
            Node* temp = curr->next;
            curr->next = curr->next->next;
            delete temp;
        }
        curr = curr->down;
    }

    if (foundVal)
        length -= 1;

    return foundVal;
}

void Skiplist::delete_top_level() {
    Node* curr = head;
    head = head->down;
    levels -= 1;
    /* Free the top level */
    while (curr != NULL) {
        Node* temp = curr;
        curr = curr->next;
        delete temp;
    }
}

Node* Skiplist::search_on_level(Node* curr, int value) {
    while (curr->next != NULL && curr->next->val < value) {
        curr = curr->next;
    }
    return curr;
}

int main() {
    DSkiplist* dsl = new DSkiplist();
    VSkiplist* vsl = new VSkiplist();
    Skiplist* sl = new Skiplist();
    int n = 10000000;
    std::vector<int> nums;

    for (int i = 0; i < n; i++) {
        nums.push_back(rand() % n);
    }

    std::cout << "=== Deterministic Skiplist Insert Time ===\n";

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < n; i++) {
        dsl->add(nums[i]);
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

    std::cout << "Total Runtime: " << duration.count() << "\nAverage Runtime: " << duration.count() / n << "\n\n";

    std::cout << "=== Vector Skiplist Insert Time ===\n";

    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < n; i++) {
        vsl->add(nums[i]);
    }

    end = std::chrono::high_resolution_clock::now();

    duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

    std::cout << "Total Runtime: " << duration.count() << "\nAverage Runtime: " << duration.count() / n << "\n\n";

    std::cout << "=== Regular Skiplist Insert Time ===\n";

    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < n; i++) {
        sl->add(nums[i]);
    }

    end = std::chrono::high_resolution_clock::now();

    duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

    std::cout << "Total Runtime: " << duration.count() << "\nAverage Runtime: " << duration.count() / n << "\n\n";

    std::cout << "=== Deterministic Skiplist Delete Time ===\n";

    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < n; i++) {
        dsl->erase(nums[i]);
    }

    end = std::chrono::high_resolution_clock::now();

    duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

    std::cout << "Total Runtime: " << duration.count() << "\nAverage Runtime: " << duration.count() / n << "\n\n";

    std::cout << "=== Vector Skiplist Delete Time === \n";

    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < n; i++) {
        vsl->erase(nums[i]);
    }

    end = std::chrono::high_resolution_clock::now();

    duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

    std::cout << "Total Runtime: " << duration.count() << "\nAverage Runtime: " << duration.count() / n << "\n\n";

    std::cout << "=== Regular Skiplist Delete Time === \n";

    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < n; i++) {
        sl->erase(nums[i]);
    }

    end = std::chrono::high_resolution_clock::now();

    duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

    std::cout << "Total Runtime: " << duration.count() << "\nAverage Runtime: " << duration.count() / n << "\n\n";

    delete vsl;
    delete sl;
    delete dsl;
    nums.clear();
}