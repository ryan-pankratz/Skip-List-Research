# Skip List Research

This repository is a WIP for my personal Skip List research. This is a version of the deterministic skip list by J. Ian Munro, Thomas Papadakis, and Robert Sedgewick. The original paper can be found here https://www.ic.unicamp.br/~celio/peer2peer/skip-net-graph/deterministic-skip-lists-munro.pdf.

This skip list is modified to be unsorted. Instead, it uses William Pughs indexing of a skip list in order to achieve logarithmic index search. This idea is used in the skip list to produce a skip list that has logarithmic runtime in most operations besides search.

Here are the runtimes listed below:

Search for number - O(n)
Search for index - O(log(n))
Delete at Index - O(log(n))
Add at Index - O(log(n))
Add at end - O(log(n))

The next aim of this, is to create a method for which the add at end operation runs in constant O(1) (amortized) runtime (Worst case would still be logarithmic). I have yet to prove my idea, but I believe that it is possible. Using a randomized skip list instead of the deterministic one, it is possible to achieve an average case O(1). 

The goal of this research is to improve the Linked List. I find the linked list to be a fascinating data structure, but due to the speed increases cache gives. it rarely sees use compared to the dynamic array (Or vector) except for in operating systems. In addition, I would also like to incorportate some other linked list techniques such as the unrolled linked list to try and produce the ultimate linked list.
