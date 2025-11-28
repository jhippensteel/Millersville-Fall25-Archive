#ifndef LIST_HPP_
#define LIST_HPP_

#include <functional>
#include <iterator>
#include <limits>
#include <memory>
#include <utility>

template<typename T>
class List
{
public: /* should be private */
  struct Node;
  struct Iterator;
  struct ConstIterator;

public:
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type&;
  using const_reference = value_type const&;
  using pointer = value_type*;
  using const_pointer = value_type const*;
  using iterator = Iterator;
  using const_iterator = ConstIterator;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public: /* should be private */
  struct Node
  {
    Node () : data ()
    {
    }

    Node (T const& v) : data (v)
    {
    }

    Node (T const& v, Node* n, Node* p) : data (v), next (n), prev (p)
    {
    }

    Node (T&& v) : data (std::move (v))
    {
    }

    Node (T&& v, Node* n, Node* p) : data (std::move (v)), next (n), prev (p)
    {
    }

    // this is start of range to remove
    // end is inclusive end of range to remove
    static void
    unhook (Node* begin, Node* end)
    {
      begin->prev->next = last->next;
      last->next->prev = begin->prev;
    }

    // insert [first,last] before this
    void
    hook (Node* first, Node* last)
    {
      last->next = this;
      this->prev->next = first;
      first->prev = this->prev;
      this->prev = last;
    }

    // insert first before this
    void
    hook (Node* first)
    {
      hook (first, first);
    }

    void
    unhook ()
    {
      Node::unhook (this, this);
    }

    T data;
    Node* next{nullptr};
    Node* prev{nullptr};
  };

  struct Iterator
  {

    using value_type = List::value_type;
    using pointer = List::pointer;
    using reference = List::reference;
    using difference_type = List::difference_type;
    using iterator_category = std::bidirectional_iterator_tag;

  public:
    Iterator () noexcept = default;
    Iterator (Iterator const&) noexcept = default;
    Iterator (Iterator&&) noexcept = default;
    ~Iterator () = default;
    Iterator&
    operator= (Iterator const&) noexcept = default;
    Iterator&
    operator= (Iterator&&) noexcept = default;

    Iterator (Node const* n) : m_nodePtr (const_cast<Node*> (n))
    {
    }

    reference operator* () const
    {
      return m_nodePtr->data;
    }

    pointer operator-> () const
    {
      return &(m_nodePtr->data);
    }

    // advances to the "next" pointer, returns reference to self
    Iterator&
    operator++ ()
    {
      m_nodePtr = m_nodePtr->next;
      return *this;
    }

    // advances to the "next" pointer, returns copy of self prior to advancement
    Iterator
    operator++ (int)
    {
      auto itcopy = new Iterator(*this);
      m_nodePtr = m_nodePtr->next;
      return itcopy;
    }

    // advances to the "prev" pointer, returns reference to self
    Iterator&
    operator-- ()
    {
      m_nodePtr = m_nodePtr->prev;
      return *this;
    }

    // advances to the "prev" pointer, returns copy of self prior to advancement
    Iterator
    operator-- (int)
    {
      auto itcopy = new Iterator(*this);
      m_nodePtr = m_nodePtr->prev;
      return itcopy;
    }

    // compares the underlying pointers for equality
    friend bool
    operator== (Iterator const& i, Iterator const& j)
    {
      return i.m_nodePtr == j.m_nodePtr;
    }

    friend bool
    operator!= (Iterator const& i, Iterator const& j)
    {
      return !(i == j);
    }

  private:
    Node* m_nodePtr{nullptr};

    friend class List;
  };

  struct ConstIterator
  {
    using value_type = List::value_type;
    using pointer = List::const_pointer;
    using reference = List::const_reference;
    using difference_type = List::difference_type;
    using iterator_category = std::bidirectional_iterator_tag;

  public:
    ConstIterator () noexcept = default;
    ConstIterator (ConstIterator const&) noexcept = default;
    ConstIterator (ConstIterator&&) noexcept = default;
    ~ConstIterator () = default;
    ConstIterator&
    operator= (ConstIterator const&) noexcept = default;
    ConstIterator&
    operator= (ConstIterator&&) noexcept = default;

    ConstIterator (Node const* n) : m_nodePtr (const_cast<Node*> (n))
    {
    }

    ConstIterator (Iterator const& i) : m_nodePtr (i.m_nodePtr)
    {
    }

    reference operator* () const
    {
      return m_nodePtr->data;
    }

    pointer operator-> () const
    {
      return &(m_nodePtr->data);
    }

    ConstIterator&
    operator++ ()
    {
      m_nodePtr = m_nodePtr->next;
      return *this;
    }

    ConstIterator
    operator++ (int)
    {
      auto itcopy = new ConstIterator(*this);
      m_nodePtr = m_nodePtr->next;
      return itcopy;
    }

    ConstIterator&
    operator-- ()
    {
      m_nodePtr = m_nodePtr -> prev;
      return *this;
    }

    ConstIterator
    operator-- (int)
    {
      auto itcopy = new ConstIterator(*this);
      m_nodePtr = m_nodePtr->prev;
      return itcopy;
    }

    friend bool
    operator== (ConstIterator const& i, ConstIterator const& j)
    {
      return i.m_nodePtr == j.m_nodePtr;
    }

    friend bool
    operator!= (ConstIterator const& i, ConstIterator const& j)
    {
      return !(i == j);
    }

  private:
    Node* m_nodePtr{nullptr};

    friend class List;
  };

  // transfers [first, last) to before pos and sets all links
  static void
  transfer (const_iterator pos, const_iterator first, const_iterator last)
  {
    if(first != last) {
      Node::unhook(first.m_nodePtr, --last.m_nodePtr);
      pos.m_nodePtr->hook(first.m_nodePtr, --last.m_nodePtr);

    }

    // check if there is an empty range
    //   if not empty, unhook the inclusive range [first, std::prev(last)],
    //   and hook that range before pos
  }

public:

  // default constructor
  List ():
  m_size(0),
  m_header(new Node())
  /* remember to include the member initializer list */
  {
    m_header.next = &m_header;
    m_header.prev = &m_header;
  }

  // size-value constructor
  explicit List (size_type count, T const& value) : List ()
  {
    for(size_t i = 0;i<count;i++){
      m_header.hook(new Node(value));
    }
  }

  explicit List (size_type count) : List ()
  {
    while (count--)
    {
      emplace_back ();
    }
  }

  // range constructor
  template<std::forward_iterator InputIt>
  List (InputIt first, InputIt last) : List ()
  {
    transfer(m_header, first, last);
  }

  // copy constructor
  List (List const& other) : List (other.begin (), other.end ())
  {
  }

  // move constructor
  List (List&& other)
    : m_header (std::exchange (other.m_header, Node ()))
    , m_size (std::exchange (other.m_size, 0))
  {
  }

  // intializer_list constructor
  List (std::initializer_list<T> init) : List (init.begin (), init.end ())
  {
  }


  // destructor
  ~List ()
  {
    
    clear();
    // Remember to delete all allocated nodes!
  }

  // copy assignment
  List&
  operator= (List const& other)
  {
    if (&other != this){
      clear()
      
    }//TODO
    // Remember to check for self-assignment
    // Hint: look at versions of assign() below...
  }

  // move assignment
  List&
  operator= (List&& other) noexcept
  {
    if (&other != this)
    {
      clear ();
      m_header.next = std::exchange (other.m_header.next, &(other.m_header));
      m_header.prev = std::exchange (other.m_header.prev, &(other.m_header));
      m_size = std::exchange (other.m_size, 0);
    }
    return *this;
  }

  // initializer_list assignment
  List&
  operator= (std::initializer_list<T> ilist)
  {
    assign (ilist);
    return *this;
  }

  void
  assign (size_type count, T const& value)
  {
    List l (count, value);
    swap (l);
  }

  template<std::forward_iterator InputIt>
  void
  assign (InputIt first, InputIt last)
  {
    List l (first, last);
    swap (l);
  }

  void
  assign (std::initializer_list<T> ilist)
  {
    // TODO
  }

  // return a reference to the first element in the list
  reference
  front ()
  {
    return *begin()
  }

  // return a reference to the first element in the list
  const_reference
  front () const
  {
    return *begin()
  }

  // return a reference to the last element in the list
  reference
  back ()
  {
    return *--end();
  }

  // return a reference to the last element in the list
  const_reference
  back () const
  {
    return *--end();
  }

  // returns an iterator to the first element in the list
  iterator
  begin () noexcept
  {
    return {m_header.next};
  }

  const_iterator
  begin () const noexcept
  {
    return {m_header.next}
  }

  const_iterator
  cbegin () const noexcept
  {
    return begin();
  }

  // returns an iterator to one-past-the-last element in the list
  iterator
  end () noexcept
  {
    return {m_header};
  }

  const_iterator
  end () const noexcept
  {
    return {m_header};
  }

  const_iterator
  cend () const noexcept
  {
    return end();
  }

  reverse_iterator
  rbegin () noexcept
  {
    return reverse_iterator{end ()};
  }

  const_reverse_iterator
  rbegin () const noexcept
  {
    return const_reverse_iterator{end ()};
  }

  const_reverse_iterator
  crbegin () const noexcept
  {
    return const_reverse_iterator{cend ()};
  }

  reverse_iterator
  rend () noexcept
  {
    return reverse_iterator{begin ()};
  }

  const_reverse_iterator
  rend () const noexcept
  {
    return const_reverse_iterator{begin ()};
  }

  const_reverse_iterator
  crend () const noexcept
  {
    return const_reverse_iterator{cbegin ()};
  }

  bool
  empty () const noexcept
  {
    return m_size == 0;
  }

  size_type
  size () const noexcept
  {
    return m_size;
  }

  size_type
  max_size () const noexcept
  {
    return std::numeric_limits<size_type>::max ();
  }

  // removes all elements from the list
  void
  clear () noexcept
  {
    while (--end() != end()){
      pop_back()
    }
    // Remember to delete all nodes
  }

  // inserts "value" before "pos" -- returns iterator pointing to newly inserted element
  iterator
  insert (const_iterator pos, T const& value)
  {
    Node n = new Node(value);
    pos.m_nodePtr->hook(n);
    return Iterator(n);
    // Hint: create a new Node and "hook" it in place
  }

  // inserts "value" before "pos" -- returns iterator pointing to newly inserted element
  iterator
  insert (const_iterator pos, T&& value)
  {
    Node n = new Node(value);
    pos.m_nodePtr->hook(n)
    return Iterator(n)
    // Hint: same as above but call Node ctor with std::move(value)
  }

  iterator
  insert (const_iterator pos, size_type count, T const& value)
  {
    List l (count, value);
    iterator iter = l.begin ();
    splice (pos, l);
    return iter;
  }

  template<std::forward_iterator InputIt>
  iterator
  insert (const_iterator pos, InputIt first, InputIt last)
  {
    List l (first, last);
    iterator iter = l.begin ();
    splice (pos, l);
    return iter;
  }

  iterator
  insert (const_iterator pos, std::initializer_list<T> ilist)
  {
    List l (ilist);
    iterator iter = l.begin ();
    splice (pos, ilist);
    return iter;
  }

  template<typename... Args>
  iterator
  emplace (const_iterator pos, Args&&... args)
  {
    thread_local std::allocator<Node> nalloc;
    Node* node = nalloc.allocate (1);
    new (&(node->data)) T (std::forward<Args> (args)...);
    pos.m_nodePtr->hook (node);
    ++m_size;
    return iterator{node};
  }

  // inserts an element at the end of the list
  void
  push_back (T const& value)
  {
    insert(end(), value);
    // Hint: could be one-line call to insert()
  }

  void
  push_back (T&& value)
  {
    insert(end(), std::move(value));
    // Hint: same as above but say std::move(value) on right-hand-side of assignment
  }

  // inserts an element at the front of the list
  void
  push_front (T const& value)
  {
    // TODO
    // Hint: could be one-line call to insert()
  }

  void
  push_front (T&& value)
  {
    // TODO
    // Hint: same as above but say std::move(value) on right-hand-side of assignment
  }

  template<typename... Args>
  reference
  emplace_back (Args&&... args)
  {
    return *emplace (end (), std::forward<Args> (args)...);
  }

  template<typename... Args>
  reference
  emplace_front (Args&&... args)
  {
    return *emplace (begin (), std::forward<Args> (args)...);
  }

  // erase element pointed to by "pos" -- returns iterator to next element
  iterator
  erase (const_iterator pos)
  {
    iterator nextPos = ++pos;
    pos.m_nodePtr.unhook();
    return nextPos;
  }

  // erase elements in the range [first, last) -- returns an iterator that points to the same element as "last"
  iterator
  erase (const_iterator first, const_iterator last)
  {
    while (first != last){
      first = erase(first);
    }
    return first;
  }

  void
  pop_back ()
  {
    erase (--end ());
  }

  void
  pop_front ()
  {
    erase (begin ());
  }

  // resize the list to contain count elements
  void
  resize (size_type count)
  {
    // TODO
    // if count > size(), then add new elements with emplace_back()
    // if count < size(), then remove elements from the end
  }

  // resize the list to contain count elements, using "value" if count > size()
  void
  resize (size_type count, value_type const& value)
  {
    // TODO
    // similar to above
  }

  void
  swap (List& other)
  {
    using std::swap;
    // swap pointers
    swap (m_header.prev, other.m_header.prev);
    swap (m_header.next, other.m_header.next);
    // fix links that should now point to m_header
    m_header.next->prev = &m_header;
    m_header.prev->next = &m_header;
    // fix links that should now point to other.m_header
    other.m_header.next->prev = &other.m_header;
    other.m_header.prev->next = &other.m_header;
    // finally, swap sizes
    swap (m_size, other.m_size);
  }

  template<typename Compare>
  void
  merge (List& other, Compare comp)
  {
    if (this == &other)
    {
      return;
    }
    iterator first1 = begin ();
    iterator last1 = end ();
    iterator first2 = other.begin ();
    iterator last2 = other.end ();
    try
    {
      while (first1 != last1 && first2 != last2)
      {
        if (comp (*first2, *first1))
        {
          List::transfer (first1, first2, ++first2);
        }
        else
        {
          ++first1;
        }
      }
      if (first2 != last2)
      {
        List::transfer (last1, first2, last2);
      }
      m_size += std::exchange (other.m_size, 0);
    }
    catch (...)
    {
      size_type const dist = std::distance (first2, last2);
      m_size += (other.m_size - dist);
      other.m_size = dist;
      throw;
    }
  }

  template<typename Compare>
  void
  merge (List&& other, Compare comp)
  {
    merge (other, comp);
  }

  void
  merge (List& other)
  {
    merge (other, std::less<>{});
  }

  void
  merge (List&& other)
  {
    merge (other, std::less<>{});
  }

  void
  splice (const_iterator pos, List& other, const_iterator first,
          const_iterator last)
  {
    if (&other != this)
    {
      size_type const dist = std::distance (first, last);
      other.m_size -= dist;
      m_size += dist;
    }
    List::transfer (pos, first, last);
  }

  void
  splice (const_iterator pos, List&& other, const_iterator first,
          const_iterator last)
  {
    splice (pos, other, first, last);
  }

  void
  splice (const_iterator pos, List& other, const_iterator it)
  {
    splice (pos, other, it, std::next (it));
  }

  void
  splice (const_iterator pos, List&& other, const_iterator it)
  {
    splice (pos, other, it, std::next (it));
  }

  void
  splice (const_iterator pos, List& other)
  {
    splice (pos, other, other.begin (), other.end ());
  }

  void
  splice (const_iterator pos, List&& other)
  {
    splice (pos, other, other.begin (), other.end ());
  }

  size_type
  remove (T const& value)
  {
    size_type removed = 0;
    iterator first = begin ();
    iterator last = end ();
    while (first != last)
    {
      iterator next = first;
      ++next;
      if (*first == value)
      {
        erase (first);
        ++removed;
      }
      first = next;
    }
    return removed;
  }

  template<typename UnaryPredicate>
  size_type
  remove_if (UnaryPredicate p)
  {
    size_type removed = 0;
    iterator first = begin ();
    iterator last = end ();
    while (first != last)
    {
      iterator next = first;
      ++next;
      if (p (*first))
      {
        erase (first);
        ++removed;
      }
      first = next;
    }
    return removed;
  }

  template<typename BinaryPredicate>
  size_type
  unique (BinaryPredicate p)
  {
    size_type removed = 0;
    if (!empty ())
    {
      iterator first = begin ();
      iterator last = end ();
      iterator next = first;
      while (++next != last)
      {
        if (p (*first, *next))
        {
          erase (next);
          ++removed;
        }
        else
        {
          first = next;
        }
        next = first;
      }
    }
    return removed;
  }

  size_type
  unique ()
  {
    return unique (std::equal_to<>{});
  }

  template<typename BinaryPredicate>
  void
  sort (BinaryPredicate p)
  {
    if (m_size <= 1)
    {
      return;
    }
    List carry, tmp[64], *counter, *fill = &tmp[0];
    try
    {
      do
      {
        carry.splice (carry.cbegin (), *this, cbegin ());
        for (counter = &tmp[0]; counter != fill && !counter->empty ();
             ++counter)
        {
          counter->merge (carry, p);
          counter->swap (carry);
        }
        counter->swap (carry);
        if (counter == fill)
        {
          ++fill;
        }
      } while (!empty ());
      for (counter = &tmp[1]; counter != fill; ++counter)
      {
        counter->merge (*(counter - 1), p);
      }
      swap (*(fill - 1));
    }
    catch (...)
    {
      splice (cend (), carry);
      for (List& t : tmp)
      {
        splice (cend (), t);
      }
      throw;
    }
  }

  void
  sort ()
  {
    sort (std::less<>{});
  }

  // Reverses the elements of the list without invalidating/changing any iterators/values
  void
  reverse ()
  {
    // TODO -- EXTRA CREDIT [+5]
    // Hint: fixup all next/prev links
  }

public: /*should be private*/
  Node m_header;
  size_type m_size;
};

#endif
