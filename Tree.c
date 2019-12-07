/**
 * File: Tree.c
 * Qiyu Liu
 * CSE 12, Fall 2019
 * Nov 25 2019
 * cs12fa19ku
 * Assignment eight 
 * 
 * Description:
 * This file contains the algorithm for the implementation of a binary tree data
 * structure. Each tree is composed by nodes. We have some instance variables to
 * store information about the tree. Each node of the tree will have a left and
 * right children. Data is stored within the node. This data
 * structrue will be used to allow variable assignment. Everything is
 * implemented recursively. 
 */
#include <cstdlib>
#include <string>
#include "Tree.h"
using namespace std;

// messages
static const char AND[] = " and ";
static const char CHECK[] = " - Checking ";
static const char COMPARE[] = " - Comparing "; 
static const char DEALLOCATE[] = " - Deallocating]\n"; 
static const char INSERT[] = " - Inserting "; 
static const char REPLACE[] = " - Replacing ";
static const char UPDATE[] = " - Updating ";
static const char RIGHT_BRACKET[] = "]\n"; // Right Bracket

template <class Whatever>
int Tree<Whatever>::debug = 0;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define THRESHOLD 2

template <class Whatever>
ostream & operator << (ostream &, const TNode<Whatever> &);

/**
 * Name:
 * TNode.c
 * 
 * Description:
 * This struct define the necessary data fields and methods for a node inside a
 * binary search tree. The detail description of the data fields and public
 * method is included below.
 *
 * Data Fields:
 * data -> the information stored within the node
 * left -> left child of the node
 * right -> right child of the node
 * height -> 1+height of the tallest child, or 0 for leaf
 * balance -> left child's height - right child's height
 * occupancy -> how many nodes exist
 * tree_count -> the index of the tree
 * 
 * Public functions:
 * TNode -> the constructor function of the Node, initializes data
 * toString -> string representation fo the data
 * writeAllTNodes -> write out all the nodes and their data
 * Delete_AllTNodes -> delete all the nodes 
 * ReplaceAndRemoveMin -> remove the minimum child
 * SetHeightAndBalance -> set height and balance of the node
 */
template <class Whatever>
struct TNode {
	long balance;
	Whatever data;
	long height;
	TNode<Whatever> * left;
	long & occupancy;
	TNode<Whatever> * right;
	unsigned long & tree_count;

	/**
	 * Name: TNode 
	 * Purpose and description:
	 * This constructor initializes the memory associated with the TNode
	 * object.
	 * 
	 * @param element Whatever type -> the data that we wanna store. 
	 * @param theTree Tree<Whatever> type -> the tree itself
	 */
	TNode (const Whatever & element, Tree<Whatever> & theTree)
		: balance (0), data (element), height (0), left (0), 
		occupancy (theTree.occupancy), right (0), 
		tree_count (theTree.tree_count) {
			// increase occuapncy
			occupancy++;
		}

	/**
	 * Name: TNode 
	 * Purpose and description:
	 * This constructor initializes the memory associated with the TNode
	 * object.
	 * 
	 * @param element Whatever type -> the data that we wanna store. 
	 * @param parentTNode TNode<Whatever> type -> the parent TNode 
	 */
	TNode (const Whatever & element, TNode<Whatever> & parentTNode)
		: balance (0), data (element), height (0), left (0), 
		occupancy (parentTNode.occupancy), right (0), 
		tree_count (parentTNode.tree_count) {
			// increase occuapncy
			occupancy++;
		}

	/**
	 * Name: ~TNode 
	 * Purpose and description:
	 * This destructor deallocate the memory associated with the TNode
	 * object.
	 * 
	 */
	~TNode (void) {
		// decrease occuapncy
		occupancy--;
	}

	/**
	 * Name: delete_AllTNodes
	 * Purpose and description:
	 * This destructor deallocate the memory associated with the TNode
	 * object recursively.
	 * 
	 */
	void delete_AllTNodes (void) {
		// go left
		if (left)
			left -> delete_AllTNodes();
		// go right
		if (right)
			right -> delete_AllTNodes();
		// delete the curent one
		delete(this);
	}

	/**
	 * Name: Insert 
	 * Purpose and description:
	 * This method inserts an element into the tree. 1 for success, 2 for
	 * failure.
	 * 
	 * @param element Whatever type -> the data to be inserted
	 * @param PointerInParent TNode<Whatever> type -> reference to Parent's
	 *		 pointer
	 */
	unsigned long Insert (const Whatever & element, 
			TNode<Whatever> *& PointerInParent);

	/**
	 * Name: Lookup 
	 * Purpose and description:
	 * This method looks up an element from the tree.
	 * 
	 * @param element Whatever type -> the data to be inserted
	 * @return TRUE for success, FALSE for failure
	 */
	unsigned long Lookup(Whatever & element) const {
		unsigned long returnVal; // storing return value
		// debug statement
		if (Tree<Whatever>::debug && this){
			cerr<<TREE<<tree_count<<COMPARE<<(const char *) element<<AND<<(const
			char *) data<<RIGHT_BRACKET;
		}
		// cannot find the item
		if (this == NULL){
			return FALSE;
		}
		// found the item
		else if (data == element){
			element = data;
			returnVal = TRUE;
		}
		// going left if the element if smaller
		else if (element < data){
			returnVal = left -> Lookup(element);
		}
		// going right otherwise
		else{
			returnVal = right -> Lookup(element);
		}
		// return statement
		return returnVal;
	}

	/**
	 * Name: ReplaceAndRemoveMin 
	 * Purpose and description:
	 * This method removes an element with two children. 
	 * 
	 * @param targetTNode TNode<Whatever> -> the data to be inserted
	 * @param PointerInParent TNode<Whatever> type -> reference to Parent's
	 *		 pointer
	 */
	void ReplaceAndRemoveMin (TNode<Whatever> & targetTNode, 
			TNode<Whatever> *& PointerInParent) {
		// debug statement
		if (Tree<Whatever>::debug){
			cerr<<TREE<<tree_count<<CHECK<<(const char *)data<<RIGHT_BRACKET;
		}
		// if this is not null
		if (this){
			// if there is left child, recursively go left
			if (left){
				left -> ReplaceAndRemoveMin(targetTNode, left);
				// we need to set the Height and balance after removal
				SetHeightAndBalance(PointerInParent);
			}
			// if there is not left child, swap the data
			else {
				// debug statement
				if (Tree<Whatever>::debug){
					cerr<<TREE<<tree_count<<REPLACE<<(const char *)data
					<<RIGHT_BRACKET;
				}
				// swapping the data
				targetTNode.data = this -> data;
				// attach PointerInParent to the right child
				PointerInParent = right;
				// delete the minimal TNode
				delete this;
			}
		}
	}

	/**
	 * Name: Remove
	 * Purpose and description:
	 * This method removes an element into the tree.
	 * 
	 * @param element Whatever type -> the data to be inserted
	 * @param PointerInParent TNode<Whatever> type -> reference to Parent's
	 *		  pointer
	 */
	unsigned long Remove (TNode<Whatever> & elementTNode, 
			TNode<Whatever> *& PointerInParent,
			long fromSHB = FALSE) {
		unsigned long returnVal = TRUE; // storing return value
		bool deleted = false; // if TNode is deleted

		// when the current TNode is null
		if (this == NULL){
			return FALSE;
		}
		// when we find the matching data
		else if (data == elementTNode.data){
			elementTNode.data = data; // swap the data
			deleted = true; // turn the flag on
			// no children case
			if (left == NULL && right == NULL){
				PointerInParent = NULL;
				delete this;
				returnVal = TRUE;
			}
			// only right child case
			else if (left == NULL){
				PointerInParent = right; // re-attach it to the right
				delete this;
				returnVal = TRUE;
			}
			// only left child case
			else if (right == NULL){
				PointerInParent = left; // re-attach it the left
				delete this;
				returnVal = TRUE;
			}
			// two children case
			else {
				right -> ReplaceAndRemoveMin(*this, right); // right child call
				deleted = false; // set flag to false
			}
		}
		else if (elementTNode.data < data){
			// recursively calling to the left
			returnVal = left -> Remove(elementTNode, left, fromSHB);
		}
		else{
			// recursively calling to the right
			returnVal = right -> Remove(elementTNode, right, fromSHB);
		}
		if (!fromSHB && !deleted){
			// calling SHB if not called from SHB
			SetHeightAndBalance(PointerInParent); 
		}
		return returnVal;
	}

	/**
	 * Name: SetHeightAndBalance 
	 * Purpose and description:
	 * This method sets the height and balance of the node.
	 * 
	 * @param TNode<Whatever> PointerInParent-> reference to parent's pointer
	 *		  field 
	 */
	void SetHeightAndBalance (TNode<Whatever> *& PointerInParent) {
		long leftHeight = -1; // left child's height
		long rightHeight = -1; // right child's height
		// update left child's height if there's one
		if (left != NULL){
			leftHeight = left -> height;
		}
		// update right child's height if there's one
		if (right != NULL){
			rightHeight = right -> height;
		}
		// update the height of the current TNode
		if (leftHeight > rightHeight){
			height = leftHeight + 1;
		}
		else {
			height = rightHeight + 1;
		}
		balance = leftHeight - rightHeight; // update balance of current TNode
		// debug message
		if (Tree<Whatever>::debug){
			cerr<<TREE<<tree_count<<UPDATE<<(const char *) data<<RIGHT_BRACKET;
		}

		// keep the balance of the tree
		if (abs(balance) > THRESHOLD){
			Whatever element = this -> data;
			// Remove the off balance TNode
			Remove(*this, PointerInParent, TRUE);
			PointerInParent -> Insert(element, PointerInParent);
		}
	}

	ostream & Write_AllTNodes (ostream & stream) const {
		if (left)
			left->Write_AllTNodes (stream);
		stream << *this;
		if (right)
			right->Write_AllTNodes (stream);

		return stream;
	}
};

/**
 * Name: Tree<Whatever> :: Set_Debug_On (public)
 * Purpose and description:
 * This method sets the debug to true.
 *
 */
template <class Whatever>
void Tree<Whatever> :: Set_Debug_On (void)
{
	debug = true;
}

/**
 * Name: Tree<Whatever> :: Set_Debug_Off (public)
 * Purpose and description:
 * This method sets the debug to false.
 *
 */
template <class Whatever>
void Tree<Whatever> :: Set_Debug_Off (void)
{
	debug = false;
}

/**
 * Name: TNode<Whatever> :: Insert 
 * Performs insertion into the binary search tree. It will return true if the
 * insertion is successful, and false if the insertion is unseccessful.
 *
 * @param   element   Whatever type -> the element to te inserted 
 * @param  PointerInParent TNode<Whatever> -> PointerInParent
 * @return long type -> TRUE for success, FALSE for unsuccess
 */
template <class Whatever>
unsigned long TNode<Whatever> :: Insert (const Whatever & element, 
		TNode<Whatever> *& PointerInParent) {
	// debug message
	if (Tree<Whatever>::debug){
		cerr<<TREE<<tree_count<<COMPARE<<(const char *) element<<AND<<(const
		char *) data<<RIGHT_BRACKET;
	}
	// when we find the matching element
	if (data == element){
		data = element;
	}
	// go left if the element is smaller
	else if (element < data){
		// keeping going left if there is left child
		if (left){
			left -> Insert(element, left);
		}
		// insert it when there is no left child
		else {
			left = new TNode<Whatever>(element, *this);
			// debug message
			if (Tree<Whatever>::debug){
				cerr<<TREE<<tree_count<<INSERT<<(const char *) element 
				<<RIGHT_BRACKET;
			}
		}
	}
	// go right if the element is smaller
	else {
		// keeping going right if there is right child
		if (right){
			right -> Insert(element, right);
		}
		// insert it when there is no right child
		else {
			right = new TNode<Whatever>(element, *this);
			// debug
			if (Tree<Whatever>::debug){
				cerr<<TREE<<tree_count<<INSERT<<(const char *) element 
				<<RIGHT_BRACKET;
			}
		}
	}
	// set the H and B after insertion
	SetHeightAndBalance(PointerInParent);
	return 1;
}

template <class Whatever>
ostream & operator << (ostream & stream, const TNode<Whatever> & nnn) {
	stream << "at height:  :" << nnn.height << " with balance:  "
		<< nnn.balance << "  ";
	return stream << nnn.data << "\n";
}

/**
 * Name: Tree<Whatever> :: Insert (public)
 * Performs insertion into the binary search tree. It will return true if the
 * insertion is successful, and false if the insertion is unseccessful.
 *
 * @param   Whatever type -> the element to be inserted 
 * @return  long type -> TRUE for success, FALSE for unsuccess 
 */
template <class Whatever>
unsigned long Tree<Whatever> :: Insert (const Whatever & element) {

	// empty tree case
	if (root == NULL){
		root = new TNode<Whatever>(element, *this);
		// debug message
		if (debug){
			cerr<<TREE<<tree_count<<INSERT<<(const char *) element
				<<RIGHT_BRACKET;
		}
		return TRUE;
	}
	
	// TNode's Insert handles all other cases
	root -> Insert(element, root);

	return 1;
}

/**
 * Name: Tree<Whatever> :: Lookup (public) 
 * Purpose and description:
 * This method looks up the matching data in the binary tree. It returns the
 * object if found, null otherwise.
 *
 * @param   element   Whatever type -> the element to te inserted 
 * @return  unsigned long type -> 1 for success and 0 for failure
 */
template <class Whatever>
unsigned long Tree<Whatever> :: Lookup (Whatever & element) const {
	// delegate to TNode's lookup
	return root -> Lookup(element);
}

/**
 * Name: Tree<Whatever> :: Remove (public)
 * Purpose and description:
 * This method removes the matching data from the binary tree. It return the
 * deleted data if it is found, null otherwise.
 *
 * @param   element   Whatever type -> the element to te inserted 
 * @return  Base* type -> the item if found, NULL otherwise
 */
template <class Whatever>
unsigned long Tree<Whatever> :: Remove (Whatever & element) {
	// create a TNode object on RTS
	TNode<Whatever> temp = TNode<Whatever> (element, *this);
	// delegates to TNode's remove
	long returnVal = root -> Remove(temp, root, FALSE);
	// swap the data
	element = temp.data;
	return returnVal;
}

/**
 * Name: Tree<Whatever> :: Tree (public)
 * Purpose and description:
 * This is the constructor function for Tree. It initializes some of the
 * instance variables. It initializes the tree_count using a static counter. 
 * 
 */
template <class Whatever>
Tree<Whatever> :: Tree (void): occupancy (0), root (NULL)
{
	static long counter;
	tree_count = ++counter;

	if (debug) {
		cerr << TREE << tree_count << ALLOCATE;
	}
}

/**
 * Name: Tree<Whatever> :: ~Tree (public)
 * Purpose and description:
 * It deallocates memory associated with the Tree. It will also delete all the
 * memory of the elements within the table. 
 *
 */
template <class Whatever>
Tree<Whatever> :: ~Tree (void)
{
	// debug statement
	if (debug){
		cerr<<TREE<<tree_count<<DEALLOCATE;
	}
	// if the tree is not empty, call TNode's delete_AllTNodes
	if (root)
		root -> delete_AllTNodes();
}

/**
 * Name: Tree<Whatever> :: Write (public)
 * Purpose and description:
 * This function will output the contents of the Tree table to the stream
 * specified by the caller. The stream could be cerr, cout, or any other valid
 * stream.
 *
 * @param   stream -> A reference to the output stream 
 * @return  <return> -> A reference to the output stream 
 */
template <class Whatever>
ostream & Tree<Whatever> :: Write (ostream & stream) const
{
	stream << "Tree " << tree_count << ":\n"
		<< "occupancy is " << occupancy << " elements.\n";

	return (root) ? root->Write_AllTNodes (stream) : stream;
}
