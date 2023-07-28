#include<limits>

#include<cstdlib>

#include<iostream>



float INF = std::numeric_limits<float>::infinity();



class Node {

  public:

    float* val;



    Node* next;

    Node* down;

    int height;



    Node(float* value) {

      this->val = value;

      this->next = nullptr;

      this->down = nullptr;

      this->height = 1;

    }



    void setDown(Node* n) {

      this->height = n->height;

      this->down = n;

      incrementHeight();

    }



    void decreaseHeight() {

      Node* curr = this;

      while (curr != nullptr) {

        curr->height -= 1;

        curr = curr->down;

      }

    }



  void incrementHeight() {

    Node* curr = this;

    while (curr != nullptr) {

      curr->height += 1;

      curr = curr->down;

    }

  }

};

class SkipList {

  private:

    Node* head; // Head

    Node* tail; // Tail

    int levels;

    int length; // Number of nodes



    Node* search_on_level(Node* curr, float value) {

      while (*(curr->next->val) < value) { // End is inf, so won't hit that

        curr = curr->next;

      }

      return curr;

    }



    void make_new_level() {

      Node* newHead = new Node(this->head->val);

      Node* newTail = new Node(this->tail->val);

      newHead->setDown(this->head);

      newTail->setDown(this->tail);

      this->head = newHead;

      this->tail = newTail;

      this->head->next = this->tail;



      this->levels++;

    }



    void delete_top_level() {

      Node* ptrHead = this->head;

      Node* ptrTail = this->tail;

      this->head = ptrHead->down;

      this->tail = ptrTail->down;



      delete ptrHead;

      delete ptrTail;



      this->head->decreaseHeight();

      this->tail->decreaseHeight();



      this->levels--;

    }

  

  public:

    SkipList() {

      float* negative = (float*) malloc(sizeof(float));

      float* positive = (float*) malloc(sizeof(float));



      *negative = -INF;

      *positive = INF;



      this->head = new Node(negative);

      this->tail = new Node(positive);



      this->head->next = this->tail;



      Node* levelOneHead = new Node(negative);

      Node* levelOneTail = new Node(positive);



      this->head->setDown(levelOneHead);

      this->tail->setDown(levelOneTail);



      levelOneHead->next = levelOneTail;

      this->levels = 1;

      this->length = 0;

    }



    bool search(float value) {

      Node* curr = this->head;



      while (curr->down != nullptr) {

        curr = search_on_level(curr, value);

        curr = curr->down;

      }


      curr = search_on_level(curr, value);


      return *(curr->next->val) == value;

    }



    void insert(float value) {

      Node* curr = this->head;



      while (curr->down != nullptr) {

        curr = search_on_level(curr, value);



        Node* tempCurr = curr->down->next;

        int h = tempCurr->height;



        // If 3 in a row of same height

        if (tempCurr->next != nullptr && tempCurr->next->next != nullptr && tempCurr->next->height == h && tempCurr->next->next->height == h) {

          Node* newNode = new Node(tempCurr->next->val);

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

      Node* newNode = new Node(insertVal);

      newNode->next = curr->next;

      curr->next = newNode;



      this->length++;

    }



    bool erase(float value) {

      // StaticStack s = StaticStack(this->levels);
    
      if (!search(value)) {
        return false;
      }

      Node* curr = this->head->down;



      while (curr->down != nullptr) {

        bool deleteCurr = false;

        if (*(curr->next->val) < value) {

          Node* prev;



          while (*(curr->next->val) < value) {

            prev = curr;

            curr = curr->next;

          }



          Node* currTemp = curr->down->next;



          if (currTemp->next != nullptr && currTemp->next->height != currTemp->height) {

            currTemp = prev->down->next;

            int num_at_height = 1;



            while (currTemp->height == currTemp->next->height) {

              num_at_height++;

              currTemp = currTemp->next;

            }



            if (num_at_height >= 2) {

              Node* newNode = new Node(currTemp->val);

              newNode->setDown(currTemp);

              prev->next = newNode;

              newNode->next = curr;

              prev = newNode;

            }



            curr->decreaseHeight();

            prev->next = curr->next;

            deleteCurr = true;


          }

        }

        else if (*(curr->next->val) != INF) {

          Node* currTemp = curr->down->next;

          if (currTemp->next != nullptr && currTemp->next->height != currTemp->height) {

            currTemp = curr->next->down->next;

            // Lower divider

            Node* tempNode = curr->next;

            tempNode->decreaseHeight();

            curr->next = tempNode->next;

            delete tempNode;



            if (currTemp->next->height == currTemp->height) {

              // raise currTemp



              Node* newNode = new Node(currTemp->val);

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

          Node* ptr = curr->down;

          delete curr;

          curr = ptr;

        }

      }



      Node* prev;



      while (*(curr->next->val) < value) {

        prev = curr;

        curr = curr->next;

      }



      if (curr->next->height == 1) {

        Node* ptr = curr->next;

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

        Node* curr = this->head;

        int j = 0;



        while (j < i) {

          curr = curr->down;

          j++;

        }



        while (curr != nullptr) {

          std::cout << *(curr->val);

          std::cout << ", ";

          curr = curr->next;

        }



        std::cout << '\n';

        i++;

      }

    }



    void deleteAll() {

      // Since there is a top level that has no nodes, delete it

      delete_top_level();



      Node* curr = this->head;



      while (curr->down != nullptr) {



        curr = curr->next; // Move to next node;



        while (curr->next != nullptr) {

          Node* ptr = curr;

          curr = curr->next;

          delete ptr; // Free the node

        }



        // All nodes should be freed now on this level

        curr = this->head->down; // Move down to next level



        delete_top_level();



      }



      while (curr != nullptr) {

        Node* ptr = curr;

        curr = curr->next;

        free(ptr->val); // Free the value pointer

        delete ptr;

      }

      

    }



};



int main() {

  SkipList* sl = new SkipList();



  sl->printList();



  std::cout << "---------------\n";



  sl->insert(9);

  sl->insert(4);

  sl->insert(5);

  sl->insert(6);

  sl->insert(9);

  sl->printList();

  sl->erase(2);

  sl->erase(1);

  std::cout << "-------------\n";

  sl->printList();

  std::cout << "-------------\n";

  sl->insert(2);

  sl->insert(5);

  sl->erase(6);

  sl->printList();

  std::cout << "---------------\n";

  sl->insert(5);

  sl->insert(6);

  sl->insert(7);

  sl->insert(4);

  sl->printList();

  std::cout << "---------------\n";

  sl->erase(3);

  sl->erase(3);

  sl->erase(7);

  sl->erase(6);

  sl->erase(4);

  sl->printList();

  std::cout << "---------------\n";

  sl->deleteAll();
  

  delete sl;


  return 0;

   

}

