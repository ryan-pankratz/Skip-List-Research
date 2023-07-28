#include <limits>

#include <cstdlib>

#include <algorithm>

#include <iostream>

#include <stdexcept>

#include "array.h"


const int NUMBER_OF_VALUES = 512;

class ArrayNode {

  private:

    float* values;

    int size;

    float max_value;



    /*

     * A helper to insert the value into this array

     * Precondition: this->size < NUMBER_OF_VALUES

     */

    void insert_to_array(float value) {

      if (this->max_value < value) {

        this->max_value = value;

      }



      insort(this->values, value, this->size); // Inserts the value in sorted order

      this->size++;



    }



    /*

     * A helper to insert the value into the array, pushes the max value out

     * Precondition: this->size == NUMBER_OF_VALUES

     */

    void cuckoo_insert(float value) {

      float pushedVal = this->max_value;



      insort(this->values, value, this->size); // Pushes the last value out of the array



      this->max_value = this->values[this->size - 1];



      if (this->next != nullptr && this->next->size < NUMBER_OF_VALUES) { 

        // We can push it into the next nest

        this->next->insert_to_array(pushedVal);

      }

      else {

        // To remove the slowness of moving all of memory like with an array,

        // put into it's own node



        ArrayNode* newNode = new ArrayNode();

        newNode->next = this->next;

        this->next = newNode;

        newNode->insert_to_array(pushedVal);

      }

    }



    /*

     * Adopts the array of values in this->next.

     * Precondition: this->size + this->next->size <= NUMBER_OF_VALUES

     */

    void adopt() {
        int i = size; // Count for size
        int j = 0; // Count for j
        
        while (j < this->next->size) {
            this->values[i] = this->next->values[j];
            i++;
            j++;
        }

        ArrayNode* ptr = this->next;
        this->next = ptr->next;
        
        delete ptr->values;
        delete ptr;
    }

  public:

    ArrayNode* next;



    ArrayNode() {

      this->values = new float[NUMBER_OF_VALUES];

      this->size = 0;

      this->next = nullptr;

      float max_value = -INF;

    }



    ArrayNode(float initVal) {

      this->values = new float[NUMBER_OF_VALUES];

      std::fill_n(this->values, NUMBER_OF_VALUES, initVal);

      this->size = NUMBER_OF_VALUES;

      this->max_value = initVal;

    }



    void insert_value(float value) {

      if (this->size < NUMBER_OF_VALUES) {

        this->insert_to_array(value);

      }

      else {

        this->cuckoo_insert(value);

      }

    }



    int getMaxValue() {

      return this->max_value;

    }



    int getSize() {

      return this->size;

    }



    /*

     * Erases <value> from this->next;

     */

    void remove(float value) {   

      try {

        removeVal(this->next->values, value, this->next->size);

              this->next->size--;



        if (this->next->size == 0) {
            ArrayNode* ptr = this->next;
            this->next = ptr->next;
            delete ptr->values;
            delete ptr;
        }

        else if (this->size + this->next->getSize() <= NUMBER_OF_VALUES) {

          this->adopt();

        }

        else if (this->next->next != nullptr && this->next->size + this->next->next->getSize() <= NUMBER_OF_VALUES) {

          this->next->adopt();

        }

      }

      catch (std::invalid_argument) {

        std::cout << "Error: Value not in array";

      }

    }

    void deleteNodesOnly() {
        ArrayNode* curr = this;
        while (curr != nullptr) {
        ArrayNode* ptr = curr;
        curr = curr->next;
        delete ptr;
        }
    }

    void deleteAllNodes() {
        ArrayNode* curr = this;
        while (curr != nullptr) {
            ArrayNode* ptr = curr;
            curr = curr->next;
            delete ptr->values;
            delete ptr;
        }
    }

};



class LinkedArrayList {

  private:

    ArrayNode* front;

    int length;

  public:

    LinkedArrayList() {

      this->front = new ArrayNode(-INF);

      this->length = 0;

    }



    void insert(float value) {

      ArrayNode* curr = this->front;

      float prev_max = -INF;



      while (curr->next != nullptr && !(curr->getMaxValue() >= value)) { // >= prev_max

        prev_max = curr->getMaxValue();

        curr = curr->next;

      }



      curr->insert_value(value);

      this->length++;

    }



    void erase(float value) {

      ArrayNode* curr = this->front;



      while (curr->next != nullptr && !(curr->next->getMaxValue() >= value)) {

        curr = curr->next;

      }



      if (curr->next != nullptr) {

        curr->remove(value); // Removes value from curr (prev->next)

      }

    }

    void deleteAll() {
        this->front->deleteAllNodes();
    }

};