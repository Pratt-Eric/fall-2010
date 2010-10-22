// -*- C++ -*-
/**
 * @file BSTree.h
 * @brief Definition of a BSTree and BTreeNode class.
 * BTreeNode class defines the structure of the node for BSTree. 
 * BSTree defines a binary search tree class. 
 * @author Shumin Guo (guo.18@wright.edu)
 * @version 1.0.0
 */
// $Log$

#ifndef _BSTreeClass_
#define _BSTreeClass_
#include <string>
#include <vector>
#include <iostream>
#include "Employee.h"
#include "LinkedSortedList.h"

using namespace std; 

class BTreeNode {
 private:
  int eid_; 			/* The Employee ID. */
  BTreeNode *parent; 		/* parent node. */
  BTreeNode *left; 		/* left child. */
  BTreeNode *right;		/* right child. */
  LinkedNode<Employee> *enode; /* employ node in the linked list. */

 public:
  // Constructors
  BTreeNode();
  BTreeNode(LinkedNode<Employee> *e); 

  // Destructor.
  ~BTreeNode(){killContent();}

  /* setters. */
  void setEid(int id) {eid_ = id;} 
  void setParent(BTreeNode *p) {parent = p;}
  void setLeftChild(BTreeNode *l) {left = l; 
    if(l) l->setParent(this);}
  void setRightChild(BTreeNode *r) {right = r;
    if(r) r->setParent(this);}

  void setEmployeeRecord(LinkedNode<Employee> *e) {enode = e;}

  /* getters. */
  int getEid() const {return eid_;}
  BTreeNode* getParent() const {return parent;}
  BTreeNode* getLeftChild() const {return left;}
  BTreeNode* getRightChild() const {return right;}
  LinkedNode<Employee>* getEmployeeRecord() const {return enode;}

  /* visitor */
  void visit() {cout << eid_ << endl;}

  /**
   * @brief Test if *this* node is leaf node.
   * @return true on success and false on failure.
   */
  bool isLeafNode();

  /**
   * @brief Test if *this* node is internal node.
   * @return true on success and false on failure.
   */
  bool isInternalNode(); 

  /**
   * @brief kill the content stored within *this* node.
   * This function is used to delete the Employee record. 
   * @param elist The employee list to work on.
   * @return true on success and false on failure.
   */
  bool killContent();

  /**
   * @brief Output BTreeNode info into output stream. 
   * 
   * @param out Output stream. 
   * @param n BTreeNode object to be output. 
   * @return Output stream.
   */
  friend ostream& operator<<(ostream& out, BTreeNode& n);
};

class BSTree {
 private:
  BTreeNode *root_; 		/* root of tree. */
  int size; 			/* size of tree. */

 public:
  /**
   * @brief Default constructor. 
   */
  BSTree();
  // Destructor.
  ~BSTree();

  /**
   * @brief getters. 
   */
  BTreeNode* getRoot() {return root_;}
  int getSize() {return size;}


  /**
   * @brief Pick up node from tree.
   * This will release all the relations of this node from its parent,
   * its left child and right child.
   * 
   * @param node Pointer pointing to the node that will be picked. 
   * @return Output stream.
   */  
  BTreeNode* pickLeafNode(BTreeNode* node);

  /**
   * @brief Replce a node on tree with *this* node.
   * Actually, this function will copy the parent and children
   * information from the given node to *this* node.
   * @param snode Source node to be replaced. 
   * @param dnode Dest node that will replace source. 
   * @return true on success and false on failure.
   */  
  bool replaceNode(BTreeNode* snode, BTreeNode* dnode);

  // Construct tree, Insert, delete elements.
  /**
   * @brief Insert node into tree. 
   * @param eid the Employee Id to be inserted, a new BSTreeNode
   * object will be created implicitely. 
   * @param root root of BStree.
   * @param node node to be inserted. 
   * @return true on success and false on failure.
   */
  bool insertNode(BTreeNode *root, BTreeNode *node);

  /**
   * @brief Delete node from tree. 
   * @param eid of node to be deleted. 
   * @return true on success and false on failure.
   */
  bool deleteNode(BTreeNode *root, int eid);
  bool deleteNode(BTreeNode *node);

  /**
   * @brief Destroy the whole tree. 
   * @param root root of tree.
   * @return true on success and false on failure.
   */
  bool destroyTree(BTreeNode *root);

  /**
   * @brief Find node with given eid. 
   * @param root The root of tree to be processed. 
   * @param eid The Employee Id to be found. 
   * @return pointer to the found node.
   */
  BTreeNode* findNode(BTreeNode *root, int eid);
  BTreeNode* findNode(BTreeNode *node);

  /**
   * @brief Find node with smallest eid. 
   * @param root The root of tree to be processed. 
   * @return BTreeNode the element of this node should not be
   * altered. 
   */
  BTreeNode* getSmallest(BTreeNode *root) const;

  /**
   * @brief Find node with smallest eid. 
   * @param root The root of tree to be processed. 
   * @return BTreeNode the element of this node should not be
   * altered. 
   */
  BTreeNode* getSmallestNode(BTreeNode *root) const;

  /**
   * @brief Find node with largest eid. 
   * @param root The root of tree to be processed. 
   * @return BTreeNode the element of this node should not be
   * altered. 
   */
  BTreeNode* getLargest(BTreeNode *root) const;

  // Tree traversers. 
  /**
   * @brief Pre-Order traversal of tree. 
   * @param root root of BSTree.
   * @return void.
   */
  void preOrderTraverse(BTreeNode *root);

  /**
   * @brief In-Order traversal of tree. 
   * @param root root of BSTree.
   * @return void.
   */
  void inOrderTraverse(BTreeNode *root);

  /**
   * @brief Post-Order traversal of tree. 
   * @param root root of BSTree.
   * @return void.
   */
  void postOrderTraverse(BTreeNode *root);

  /**
   * @brief Output BSTree info into output stream. 
   * 
   * @param out Output stream. 
   * @param bstree BSTree object to be output. 
   * @return Output stream.
   */
  friend ostream& operator<<(ostream& out, BSTree& bstree); 
};
#endif	/* ifdef */
