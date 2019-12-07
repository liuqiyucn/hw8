/**
 * File: Tree.h
 * Qiyu Liu
 * CSE 12, Fall 2019
 * Nov 26 2019
 * cs12fa19ku
 * Assignment eight 
 * 
 * Description:
 * This is the file that contains all the public information of the tree.c file
 * that we would like to show to the world. The hidden file is contained within
 * the .c file.
 */
#ifndef TREE_H
#define TREE_H

/* DO NOT ADD CODE: This file is used in evaluation
 * Changing function signatures will result in a 25% deduction.
 * adding comments is ok.
 */


#include <iostream>
using namespace std;

// debug message
static const char ALLOCATE[] = " - Allocating]\n";
static const char TREE[] = "[Tree ";

template <class Whatever>
struct TNode;
/**
 * Name:
 * Tree<class Whatever>
 * 
 * Description:
 * This file contains the code behind the implementation of a binary search
 * tree. Each tree is composed of nodes. We have some instance variables to
 * store information about the tree. We will outline the main data fields and
 * public functions within this class below.
 *
 * Data Fields:
 * root -> the root node of the tree
 * occupancy -> how many nodes are in the tree
 * debug -> whether we want debug statement
 * 
 * Public functions:
 * Tree -> constructor function 
 * Set_Debug_On -> set the debug_on TRUE
 * Set_Debug_Off -> set the debug to FALSE
 * Insert -> insert an node storing element within the tree
 * Loopup -> check if an element is stored within the tree
 * Remove -> remove an element from the tree
 * ostream & Write -> write out nodes
 */
template <class Whatever>
class Tree {
        friend struct TNode<Whatever>;
        long occupancy;
        TNode<Whatever> * root;
        unsigned long tree_count;
        static int debug;
public:
        
        Tree (void);
        ~Tree (void);
        
        static void Set_Debug_On (void);
        static void Set_Debug_Off (void);
        
        unsigned long Insert (const Whatever &);
        unsigned long Lookup (Whatever &) const;
        unsigned long Remove (Whatever &);
        ostream & Write (ostream &) const;
};

#include "Tree.c"

#endif
