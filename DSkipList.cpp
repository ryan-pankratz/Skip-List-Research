#include <iostream>
#include<cstdlib>
#include <iostream>

#include <chrono>

using namespace std;

#include<cstdlib>

#include<math.h>

#include <time.h>

#include <array>

#include <vector>

int INF = 2147483647;

class DSLNode

{



public:

    int *val;

    DSLNode *next;

    DSLNode *down;

    int height;

    int width;



    DSLNode(int *value) {

        this->val = value;

        this->next = nullptr;

        this->down = nullptr;

        this->height = 1;

        this->width = 0;

    }



    void setDown(DSLNode *n) {

        this->height = n->height;

        this->down = n;

        incrementHeight();

    }



    void decreaseHeight() {

        DSLNode *curr = this;



        while (curr != nullptr) {

            curr->height -= 1;

            curr = curr->down;

        }

    }



    void incrementHeight() {



        DSLNode *curr = this;

        while (curr != nullptr) {

            curr->height += 1;

            curr = curr->down;

        }

    }

};



class DSkipList {



    private:

    

        DSLNode *head; // Head

        DSLNode *tail; // Tail



        int levels;



        void make_new_level() {

            DSLNode *newHead = new DSLNode(this->head->val);

            DSLNode *newTail = new DSLNode(this->tail->val);

            newHead->setDown(this->head);

            newHead->width = this->head->width + this->head->next->width + 1;

            newTail->setDown(this->tail);

            this->head = newHead;

            this->tail = newTail;

            this->head->next = this->tail;

            this->levels++;

        }



        void delete_top_level() {

            DSLNode *ptrHead = this->head;

            DSLNode *ptrTail = this->tail;

            this->head = ptrHead->down;

            this->tail = ptrTail->down;

            delete ptrHead;

            delete ptrTail;

            this->head->decreaseHeight();

            this->tail->decreaseHeight();

            this->levels--;

        }



    public:

        DSkipList() {

            int *negative = (int *)malloc(sizeof(int));

            int *positive = (int *)malloc(sizeof(int));

            *negative = -INF - 1;

            *positive = INF;

            this->head = new DSLNode(negative);

            this->tail = new DSLNode(positive);

            this->head->width = 1;

            this->head->next = this->tail;



            DSLNode *levelOneHead = new DSLNode(negative);

            levelOneHead->width = 1;

            DSLNode *levelOneTail = new DSLNode(positive);

            this->head->setDown(levelOneHead);

            this->tail->setDown(levelOneTail);

            levelOneHead->next = levelOneTail;

            this->levels = 1;

        }



        int get(int index) {

            // Check if the index is out of bounds

            if (index < 0 || index >= this->getLength()) {

                return -1;

            }



            // Start from the head of the list

            DSLNode *curr = this->head;



            // Traverse the list vertically until reaching the appropriate level

            while (curr->down != nullptr) {

                while (index >= curr->width) {

                    index -= curr->width;

                    curr = curr->next;

                }

                curr = curr->down;

            }



            // Traverse the list horizontally to the desired element

            while (index >= 0) {

                index -= curr->width;

                curr = curr->next;

            }



            // Return the value of the node at the given index

            return *(curr->val);

        }



        void addAtHead(int val) {

            addAtIndex(0, val);

        }



        void addAtTail(int val) {

            addAtIndex(this->getLength(), val);

        }



        void addAtIndex(int index, int value) {

            // Check if the index is out of bounds

            if (index < 0 || index > this->getLength()) {

                return;

            }



            DSLNode *curr = this->head;



            // Traverse the list vertically to the appropriate level

            while (curr->down != nullptr) {

                while (index - curr->width >= 0) {

                    index -= curr->width;

                    curr = curr->next;

                }



                DSLNode *tempCurr = curr->down->next;

                int h = tempCurr->height;



                // Check if three nodes in a row have the same height

                if (tempCurr->next != nullptr && tempCurr->next->next != nullptr &&

                    tempCurr->next->height == h && tempCurr->next->next->height == h) {

                    // Create a new node and insert it between the second and third nodes

                    DSLNode *newNode = new DSLNode(tempCurr->next->val);

                    newNode->setDown(tempCurr->next);

                    newNode->width = tempCurr->next->width + tempCurr->next->next->width;

                    newNode->next = curr->next;

                    curr->next = newNode;



                    // Update width of the current node

                    curr->width -= (tempCurr->next->width + tempCurr->next->next->width);



                    // Move up if possible

                    if (index - curr->width >= 0) {

                        index -= curr->width;

                        curr = curr->next;

                    }



                    // Create a new level if the height of the new node is the highest

                    if (newNode->height == this->levels + 1) {

                        make_new_level();

                    }

                }



                // Add one to width for the value we are adding

                curr->width++;

                curr = curr->down;

            }



            // Traverse the list horizontally to the desired index

            while (index > 0) {

                index -= curr->width;

                curr = curr->next;

            }



            // Create a new node and insert it at the desired index

            int *insertVal = (int *)malloc(sizeof(int));

            *insertVal = value;

            DSLNode *newNode = new DSLNode(insertVal);

            newNode->next = curr->next;

            curr->next = newNode;

            newNode->width = 1;

        }



        void deleteAtIndex(int index) {

            if (index < 0 || index >= this->getLength()) {

                return; // Check if the index is out of bounds

            }



            this->head->width--; // Subtract off width from the head node



            DSLNode *curr = this->head->down;



            // Traverse the Skip List vertically

            while (curr->down != nullptr) {

                bool deleteCurr = false;



                if (index - curr->width >= 0) {

                    DSLNode *prev;



                    // Move horizontally

                    while (index - curr->width >= 0)

                    {

                        prev = curr;

                        index -= curr->width;

                        curr = curr->next;

                    }



                    DSLNode *currTemp = curr->down->next;



                    if (currTemp->next != nullptr && currTemp->next->height != currTemp->height)

                    {

                        // Count the number of nodes with the same height in a row

                        currTemp = prev->down->next;

                        int num_at_height = 1;

                        while (currTemp->height == currTemp->next->height) {

                            num_at_height++;

                            currTemp = currTemp->next;

                        }



                        if (num_at_height >= 2) {

                            // Create a new node above currTemp

                            DSLNode *newNode = new DSLNode(currTemp->val);

                            newNode->setDown(currTemp);

                            newNode->width = currTemp->width; // Should be the same width

                            prev->next = newNode;

                            prev->width -= currTemp->width;

                            newNode->next = curr;

                            prev = newNode;

                        }



                        curr->decreaseHeight();

                        prev->width += curr->width; // Add the width of the current node

                        prev->width--;              // Because we are deleting in this gap, remove 1

                        prev->next = curr->next;

                        deleteCurr = true;

                    }

                }

                else if (*(curr->next->val) != INF) {

                    DSLNode *currTemp = curr->down->next;



                    if (currTemp->next != nullptr && currTemp->next->height != currTemp->height) {

                        currTemp = curr->next->down->next;



                        // Lower divider

                        DSLNode *tempNode = curr->next;

                        int widthTemp = tempNode->width;

                        int widthTemp2 = curr->next->down->width;



                        tempNode->decreaseHeight();

                        curr->width += widthTemp; // Add temp node's width

                        curr->next = tempNode->next;

                        delete tempNode;



                        if (currTemp->next->height == currTemp->height) {

                            // Raise currTemp

                            DSLNode *newNode = new DSLNode(currTemp->val);

                            newNode->setDown(currTemp);

                            newNode->next = curr->next;

                            newNode->width = widthTemp - widthTemp2; // Since it is one in front, the same width - 1

                            curr->next = newNode;

                            curr->width -= (widthTemp - widthTemp2);

                        }

                    }

                }



                if (this->head->down->next == this->tail->down) {

                    // We have a blank level under the top, remove it

                    this->delete_top_level();

                }



                if (!deleteCurr) {

                    curr->width--; // Remove 1

                    curr = curr->down;

                }

                else {

                    DSLNode *ptr = curr->down;

                    delete curr;

                    curr = ptr;

                }

            }



            DSLNode *prev;



            // Traverse the list horizontally to the desired index

            while (index > 0) {

                prev = curr;

                index -= curr->width;

                curr = curr->next;

            }



            if (curr->next->height == 1) {

                DSLNode *ptr = curr->next;

                curr->next = ptr->next;

                free(ptr->val);

                delete ptr;

            }

            else {

                *(curr->next->val) = *(curr->val);

                prev->next = curr->next;



                // Free curr

                free(curr->val);

                delete curr;

            }

        }



        void deleteAll() {

            // Since there is a top level that has no nodes, delete it

            delete_top_level();

            DSLNode *curr = this->head;



            while (curr->down != nullptr) {

                curr = curr->next; // Move to next node;



                while (curr->next != nullptr) {

                    DSLNode *ptr = curr;

                    curr = curr->next;

                    delete ptr; // Free the node

                }



                // All nodes should be freed now on this level

                curr = this->head->down; // Move down to next level

                delete_top_level();

            }



            while (curr != nullptr) {

                DSLNode *ptr = curr;

                curr = curr->next;

                free(ptr->val); // Free the value pointer

                delete ptr;

            }

        }



        int getLength() {

            return this->head->width - 1; // Should be the length

        }

};