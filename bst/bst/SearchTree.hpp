/*
  Filename   : SearchTree.hpp
  Author     : Gary M. Zoppetti & Jaysen Hippensteel
  Course     : CSCI 362
  Description: Binary search tree class, 
                 a basis for implementing associative ADTs
		 like set and map. 
*/

/************************************************************/
// Macro guard

#ifndef SEARCHTREE_HPP
#define SEARCHTREE_HPP

/************************************************************/
// System includes

#include <iostream>
#include <algorithm>
#include <iterator>
#include <queue>

/************************************************************/
// Local includes

/************************************************************/
// Using declarations

using std::ostream;
using std::queue;

/************************************************************/

template<typename T>
struct Node
{
  using NodePtr = Node*;

  Node (const T& d = T ())
  : data(d), left(nullptr), right(nullptr), parent(nullptr)
  {
    // Initialize data, left, right, and parent in
    //   the member initialization list.
    // left, right, and parent should be set to nullptr.
    // The body of this constructor should be empty.
  }

  Node (const T& d, NodePtr l, NodePtr r, NodePtr p)
  : data(d), left(l), right(r), parent(p)
  {
    // Initialize data, left, right, and parent in
    //   the member initialization list.
    // The body of this constructor should be empty.
  }

  //**

  T        data;
  NodePtr  left;
  NodePtr  right;
  NodePtr  parent;
};

/************************************************************/

// Forward declaration
template <typename T>
class SearchTree;

/************************************************************/

template <typename T>
struct TreeIterator
{
  friend class SearchTree<T>;

  using Self = TreeIterator<T>;
  using NodePtr = Node<T>*;
  using ConstNodePtr = const Node<T>*;

  using difference_type = ptrdiff_t;
  using iterator_category = std::bidirectional_iterator_tag;

  using value_type = T;
  using pointer = const T*;
  using reference = const T&;

  TreeIterator ()
    : m_nodePtr ()
  { }

  explicit
  TreeIterator (ConstNodePtr n)
    : m_nodePtr (n)
  { }

  reference
  operator* () const
  {
    return m_nodePtr -> data;
  }

  // Return address of node's data member.
  pointer
  operator-> () const
  {
    return &*this;
  }

  // Pre-increment
  Self&
  operator++ ()
  {
    m_nodePtr = increment (m_nodePtr);

    return *this;
  }

  // Post-increment
  Self
  operator++ (int)
  {
    Self copy = *this;
    m_nodePtr = increment (m_nodePtr);
    return copy;
    // Increment iterator so it points to in-order successor.
    // Return appropriate iterator. 
  }

  // Pre-decrement
  Self&
  operator-- ()
  {
    m_nodePtr = decrement (m_nodePtr);

    return *this;
  }

  // Post-decrement
  Self
  operator-- (int)
  {
    Self copy = *this;
    m_nodePtr = decrement(m_nodePtr);
    return copy;
    // Decrement iterator so it points to in-order predecessor.
    // Return appropriate iterator. 
  }

  bool
  operator== (const Self& i) const
  {
    return m_nodePtr == i.m_nodePtr;
  }

  bool
  operator!= (const Self& i) const
  {
    return m_nodePtr != i.m_nodePtr;
  }

private:
  static ConstNodePtr
  increment (ConstNodePtr n)
  {
    if(n->right){
      n = n->right;
      while(n->left){
        n = n->left;
      }
      return n;
    }
    while(n == n->parent->right && n != n->parent->parent){
      n = n->parent;
    }
    return n->parent;
    // Return in-order successor of "n".
  }

  static ConstNodePtr
  decrement (ConstNodePtr n)
  {
    if(n->left){
      n = n->left;
      while(n->right){
        n = n->right;
      }
      return n;
    }
    while(n == n->parent->left && n != n->parent->parent){
      n = n->parent;
    }
    return n->parent;
    // Return in-order predecessor of "n".
  }

  // FOR TESTING, otherwise would be PRIVATE
public:

  ConstNodePtr m_nodePtr;
};

/************************************************************/

template <typename T>
class SearchTree
{
  friend class TreeIterator<T>;

public:

  // DO NOT MODIFY type aliases!
  using value_type = T;
  using pointer =  T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;

  using iterator = TreeIterator<T>;
  using const_iterator = TreeIterator<T>;

  // Header parent points to root of tree or is nullptr
  //   if the tree is empty.
  // Header left points to LARGEST node or is nullptr
  //   if the tree is empty.  
  // Header right points to SMALLEST node or is nullptr
  //   if the tree is empty.
  // size represents the number of elements in the tree.
  SearchTree ()
    : m_header (), m_size (0)
  {
  }

  // Copy constructor
  // TODO
  SearchTree (const SearchTree& t)
  : m_header (), m_size(t.size())
  {
    if(m_size > 0){
      m_header.parent = new Node();
      copy(m_header, *(m_header.parent), *(t.m_header.parent));
      m_header.left = maximum(m_header.parent);
      m_header.right = minimum(m_header.parent);
    }
  }


  ~SearchTree ()
  {
    clear ();
  }


  // Return whether the tree is empty.
  bool
  empty () const
  {
    return m_header.parent == nullptr;
  }

  // Return the size.
  size_t
  size () const
  {
    return m_size;
  }

  int
  depth () const
  {
    return depth (m_header.parent);
  }
  
  // Return an iterator pointing to the smallest element. 
  iterator
  begin ()
  {
    return TreeIterator(m_header.right);
  }

  const_iterator
  begin () const
  {
    return TreeIterator(m_header.right);
  }

  // Return an iterator pointing one beyond the last element,
  //   i.e. the header node. 
  iterator
  end ()
  {
    return TreeIterator(&m_header);
  }

  // TODO
  const_iterator
  end () const
  {
    return TreeIterator(&m_header);
  }

  iterator
  find (const T& v) const
  {
    // Call private helper method
    ConstNodePtr n = findHelper (v);
    if (n == nullptr)
    {
      // Wasn't found so return end ()
      n = &m_header;
    }
    return iterator (n);
  }

  std::pair<iterator, bool>
  insert (const T& v)
  {
    NodePtr insertedNode = insert (v, m_header.parent, &m_header);
    bool inserted = insertedNode != nullptr;
    if (inserted)
    {
      // Update header right to point to smallest node
      if (m_header.right == nullptr || v < m_header.right->data)
        m_header.right = insertedNode;
      // Update header left to point to largest node
      if (m_header.left == nullptr || v > m_header.left->data)
        m_header.left = insertedNode;
    }

    return { iterator (insertedNode), inserted };
  }

  size_t
  erase (const T& v)
  {
    // Update header right to point to smallest node
    if (m_header.right != nullptr && v == m_header.right->data)
      m_header.right =
        const_cast<NodePtr> (iterator::increment (m_header.right));
    // Update header left to point to largest node
    else if (m_header.left != nullptr && v == m_header.left->data)
      m_header.left =
        const_cast<NodePtr> (iterator::decrement (m_header.left));
    
    bool erased = erase (v, m_header.parent);
    // If we erased the last value set header left and right to nullptr
    if (erased && empty ())
      m_header.left = m_header.right = nullptr;
    
    return erased ? 1 : 0;
  }

  // Delete all nodes, set header's parent, left, and right links to nullptr,
  //   and set size to 0. Utilizes a private, recursive "clear"
  //   declared below. 
  // TODO
  void
  clear ()
  {
    clear(m_header.parent);
    m_header.left = nullptr;
    m_header.right = nullptr;
    m_header.parent = nullptr;
    m_size = 0;
  }

  void
  printInOrder (ostream& out) const
  {
    printInOrder (out, m_header.parent);
  }

  void
  printLevelOrder (ostream& out) const
  {
    printLevelOrder (out, m_header.parent);
  }

private:

  using Node = struct Node<T>;
  using NodePtr = Node*;
  using ConstNodePtr = const Node*;

  int
  depth (ConstNodePtr r) const
  {
    if (r == nullptr)
      return -1;
    return std::max (depth (r->left), depth (r->right)) + 1;
  }

  //Recursive copy helper
  void copy(Node& parent, Node& copyTo, Node& copyFrom){
    copyTo.data = copyFrom.data;
    copyTo.parent = &parent;
    if (copyFrom.left == nullptr){
      copyTo.left = nullptr;
    }
    else{
      copyTo.left = new Node();
      copy(copyTo, *copyTo.left, *copyFrom.left);
    }
    if (copyFrom.right == nullptr){
      copyTo.right = nullptr;
    }
    else{
      copyTo.right = new Node();
      copy(copyTo, *copyTo.right, *copyFrom.right);
    }
  }

  NodePtr
  minimum (NodePtr r) const
  {
    while (r->left != nullptr){
      r = r->left;
    }
    return r;
    // Return minimum node in tree rooted at "r".
  }

  NodePtr
  maximum (NodePtr r) const
  {
    while (r->right != nullptr){
      r = r->right;
    }
    return r;
    // Return maximum node in tree rooted at "r".
  }

  ConstNodePtr
  findHelper (const T& v) const
  {
    return findHelper(m_header.parent, v);
    // Return a pointer to the node that contains "v".
  }

  ConstNodePtr findHelper (NodePtr r, const T& v) const
  {
    if (r == nullptr){
      return nullptr;
    }
    if (r->data == v){
      return r;
    }
    if(v > r->data){
      return findHelper(r->right, v);
    }
    if(v < r->data){
      return findHelper(r->left, v);
    }
  }

  NodePtr
  insert (const T& v, NodePtr& r, NodePtr parent)
  {
    if (r == nullptr){
      r = new Node(v, nullptr, nullptr, parent);
      ++m_size;
      return r;
    }
    if (v < r->data){
      return insert(v, r->left, r);
    }
    if (v > r->data){
      return insert(v, r->right, r);
    }
    return nullptr;
    // Insert "v" into the tree rooted at "r".
    // Use "parent" for recursion and setting the parent of the
    //   node containing "v".
  }

  bool
  erase (const T& v, NodePtr& r)
  {
    if (r == nullptr) return false;
    if (v < r->data) return erase(v, r->left);
    if (v > r->data) return erase(v, r->right);
    if (r->left != nullptr && r->right != nullptr){
      r->data = minimum(r->right)->data;
      return erase(r->data, r->right);
    }
    if (r->left != nullptr){
      NodePtr child = r->left;
      delete r;
      --m_size;
      r = child;
      return true;
    }
    if (r->right != nullptr){
      NodePtr child = r->right;
      delete r;
      --m_size;
      r = child;
      return true;
    }
    delete r;
    --m_size;
    r = nullptr;
    return true;
    // Erase "v" from the tree rooted at "r".
    // Return whether the erase succeeded or not.
  }

  void
  clear (NodePtr& r)
  {
    if (r == nullptr) return;
    if (r->left != nullptr) clear (r->left);
    if (r->right != nullptr) clear (r->right);
    delete r;
    r = nullptr;
  }

  void
  printInOrder (ostream& out, NodePtr r) const
  {
    if (r != nullptr)
    {
      printInOrder (out, r->left);
      out << r->data << " ";
      printInOrder (out, r->right);
    }
  }

  // TODO
  // FIXME: This routine is INCORRECT and is only meant to
  //   show you some techniques you MAY want to employ. 
  // Rewrite this method to output elements in the form required
  //   by the operator<< below. 
  void
  printLevelOrder (ostream& out, NodePtr r) const
  {
    if(r == nullptr) return;
    queue<NodePtr> q;
    q.push (r);
    int nodesInRow = 1;
    int currNode = 0;
    int Rows = depth(r);
    int currRow = 0;
    while (!q.empty ())
    {
      r = q.front ();
      q.pop ();
      currNode++;
      if(r == nullptr){
        out << "- ";
        if(currNode == nodesInRow){
          if(currRow == Rows || Rows == -1){
            return;
          }
          out << "| ";
          nodesInRow *= 2;
          currNode = 0;
          currRow++;
        }
        q.push(nullptr);
        q.push(nullptr);
        continue;
      }
      out << r->data << " ";
      if(currNode == nodesInRow){
        if(currRow == Rows){
          return;
        }
        out << "| ";
        nodesInRow *= 2;
        currNode = 0;
        currRow++;
      }
      q.push(r->left);
      q.push(r->right);
    }
  }

private:

  Node   m_header;
  size_t m_size;
};

/************************************************************/

// Output tree as [ e1 e2 e3 ... en ]
//   with a '|' to indicate the end of a level,
//   and a '-' to indicate a missing node.
// E.g., the tree
//       4
//    2     7
//         6
// should be printed EXACTLY like so: [ 4 | 2 7 | - - 6 - ]
// ONLY print the levels that exist, and ensure each level contains
//   2^k entries (a T object or "-"), where "k" is the level number. 
template<typename T>
ostream&
operator<< (ostream& out, const SearchTree<T>& tree)
{
  out << "[ ";
  // For the version you submit, ensure you are using "printLevelOrder"!
  // You may find "printInOrder" useful for early testing, since it's
  //   been written for you.
  //tree.printInOrder (out);
  tree.printLevelOrder (out);
  out << "]";

  return out;
}

/************************************************************/

#endif

/************************************************************/
