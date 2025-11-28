#ifndef TEST_VECTOR_HPP_
#define TEST_VECTOR_HPP_

#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_DISABLE_MATCHERS
#define CATCH_CONFIG_CPP11_TO_STRING
#define CATCH_CONFIG_CPP17_UNCAUGHT_EXCEPTIONS
#define CATCH_CONFIG_CPP17_STRING_VIEW
#define CATCH_CONFIG_RUNNER
#include <catch2/catch_all.hpp>

#include <forward_list>
#include <initializer_list>
#include <vector>
#include <cstdlib>

#include <list>

#include "../List.hpp"

template <typename T>
struct Counter {
  T data;
  static int ctor, dtor, cctor, mctor, cassign, massign;
  Counter() { ++ctor; }
  Counter(Counter const &) { ++cctor; }
  Counter(Counter &&) { ++mctor; }
  Counter& operator=(Counter const&) { ++cassign; return *this; }
  Counter& operator=(Counter &&) { ++massign; return *this; }
  ~Counter() { ++dtor; }
  static void reset() {
    ctor = dtor = cctor = mctor = cassign = massign = 0;
  }
};

template <> int Counter<int>::ctor = 0;
template <> int Counter<int>::dtor = 0;
template <> int Counter<int>::cctor = 0;
template <> int Counter<int>::mctor = 0;
template <> int Counter<int>::cassign = 0;
template <> int Counter<int>::massign = 0;

class Range {
private:
  size_t low;
  size_t high;
public:
  Range () = delete;
  Range (size_t low, size_t high)
    : low (low), high (high) { }
  Range (size_t high)
    : low (0), high (high) { }
  size_t operator() () {
    return low + rand() % (high - low + 1);
  }
  static Range exactly (size_t n) {
    return Range (n, n);
  }
};

SCENARIO ("unhook() should remove elements in the range [first,last]", "[Node][unhook]")
{
  GIVEN ("A sequence of circular, doubly-linked Nodes")
  {
    constexpr int SIZE = 20;

    using Node = List<int>::Node;

    Node nodes[SIZE];

    auto NEXT_INDEX = [] (int i) { return (i + 1) % SIZE; };
    auto PREV_INDEX = [] (int i) { return (i + SIZE - 1) % SIZE; };

    auto PREV = [&] (Node* n) {
                  return [&] (int i) -> Node* {
                           return n + PREV_INDEX(i);
                         };
                };
    auto NEXT = [&] (Node* n) {
                  return [&] (int i) -> Node* {
                           return n + NEXT_INDEX(i);
                         };
                };

    auto IN_RANGE = [] (int begin, int end) {
                      return [=] (int i ) -> bool {
                               return begin <= i && i <= end;
                             };
                    };

    auto NODE = [&] (int i) -> Node* {
                  return nodes + i;
                };

    for (int i = 0; i < SIZE; ++i) {
      nodes[i].data = i;
      nodes[i].next = nodes + (i + 1) % SIZE;
      nodes[i].prev = nodes + (i + SIZE - 1) % SIZE;
    }
    Node* header = &nodes[0];
    WHEN ("we unhook a single element")
    {
      int INDEX = 10;
      Node* removed = nodes + INDEX;

      Node* prev = removed->prev;
      Node* next = removed->next;

      Node::unhook (removed, removed);
      THEN ("[1] the rest of the list is still intact")
      {
        REQUIRE (prev->next == next);
        REQUIRE (next->prev == prev);
        for (int i = 0; i < SIZE; ++i)
        {
          if (i != 0 && i != INDEX) {
            REQUIRE (NODE(i)->data == i);
          }
          if (NEXT_INDEX(i) != INDEX) {
            REQUIRE (NODE(i)->next == NODE(NEXT_INDEX(i)));
          }
          if (PREV_INDEX(i) != INDEX) {
            REQUIRE (NODE(i)->prev == NODE(PREV_INDEX(i)));
          }
        }
      }
      AND_THEN ("[1] the removed element is still accessible")
      {
        REQUIRE (INDEX == removed->data);
        REQUIRE ((removed->prev == prev || removed->prev == nullptr));
        REQUIRE ((removed->next == next || removed->next == nullptr));
      }
    }

    WHEN ("we unhook a range of elements")
    {
      int BEGIN_INDEX = 5;
      int END_INDEX = 15;

      Node* begin = nodes + BEGIN_INDEX;
      Node* end = nodes + END_INDEX;

      Node* prev = begin->prev;
      Node* next = end->next;

      Node::unhook (begin, end);
      THEN ("[1] the rest of the list is still intact")
      {
        auto in_range = IN_RANGE (BEGIN_INDEX, END_INDEX);
        REQUIRE (prev->next == next);
        REQUIRE (next->prev == prev);
        for (int i = 0; i < SIZE; ++i)
        {
          if (i != 0 && in_range (i)) {
            REQUIRE (NODE(i)->data == i);
          }
          if (!in_range(NEXT_INDEX(i))) {
            REQUIRE (NODE(i)->next == NODE(NEXT_INDEX(i)));
          }
          if (!in_range(PREV_INDEX(i))) {
            REQUIRE (NODE(i)->prev == NODE(PREV_INDEX(i)));
          }
        }
      }
      AND_THEN ("[1] the removed elements are still accessible")
      {
        for (int i = BEGIN_INDEX; i <= END_INDEX; ++i) {
          REQUIRE (i == NODE(i)->data);
          if (i != BEGIN_INDEX) {
            REQUIRE (NODE(i)->prev == NODE(PREV_INDEX(i)));
          } else {
            REQUIRE ((NODE(i)->prev == nullptr || NODE(i)->prev == prev));
          }
          if (i != END_INDEX) {
            REQUIRE (NODE(i)->next == NODE(NEXT_INDEX(i)));
          } else {
            REQUIRE ((NODE(i)->next == nullptr || NODE(i)->next == next));
          }
        }
      }
    }
  }
}

SCENARIO ("hook() should insert elements in the range [first,last] prior to a Node", "[Node][hook]")
{
  GIVEN ("A sequence of circular, doubly-linked Nodes")
  {
    constexpr int SIZE = 5;

    using Node = List<int>::Node;

    Node nodes[SIZE];

    for (int i = 0; i < SIZE; ++i) {
      nodes[i].data = i;
      nodes[i].next = nodes + (i + 1) % SIZE;
      nodes[i].prev = nodes + (i + SIZE - 1) % SIZE;
    }
    Node* header = &nodes[0];
    Node newNode (99);
    Node* prev = header->prev;
    Node* next = header;

    WHEN ("we hook a single element before header")
    {
      header->hook(&newNode);

      THEN ("[1] the new node's links were set")
      {
        REQUIRE (newNode.prev == prev);
        REQUIRE (newNode.next == header);

      }
      AND_THEN ("[1] the node was inserted where it should be and the rest of the list is unchanged")
      {
        REQUIRE (newNode.data == 99);
        REQUIRE (header->data == 0);
        REQUIRE (newNode.prev->data == SIZE - 1);
        REQUIRE (newNode.next->data == 0);

        REQUIRE (header->prev == &newNode);
        REQUIRE (prev->next == &newNode);
        REQUIRE (prev->prev->next == prev);
        REQUIRE (next->next->prev == next);
      }
    }

    WHEN ("we hook a double-linked range of Nodes before header")
    {
      size_t const N = 3;
      Node arr[N];
      for (int i = 0; i < N; ++i) {
        arr[i].data = 111 * (i + 1);
        arr[i].next = arr[i].prev = nullptr;
        if (i + 1 < N)
          arr[i].next = &arr[i + 1];
        if (i - 1 >= 0)
          arr[i].prev = &arr[i - 1];
      }

      header->hook(arr, arr + N - 1);

      THEN ("[1] the range's links were set")
      {
        REQUIRE (arr[0].prev == prev);
        REQUIRE (arr[0].next == &arr[1]);
        REQUIRE (arr[N - 1].next == header);
        REQUIRE (arr[N - 1].prev == &arr[N - 2]);
      }
      AND_THEN ("[1] the node was inserted where it should be and the rest of the list is unchanged")
      {
        REQUIRE (header->prev == &arr[N - 1]);
        REQUIRE (prev->next == &arr[0]);
        REQUIRE (prev->prev->next == prev);
        REQUIRE (next->next->prev == next);
      }
    }
  }
}

using list = List<int>;
using node = list::Node;
using nodeptr = node*;
using const_iterator = list::ConstIterator;

auto header = [] (list & l) {
                return &l.m_header;
              };
auto begin = [] (list & l) {
               return const_iterator { header(l)->next };
             };
auto end = [] (list & l) {
             return const_iterator { header(l) };
           };
auto nth = [] (list & l) {
             return [ptr = header(l)->next] (int n) {
                      nodeptr p = ptr;
                      if (n < 0)
                        do p = p->prev; while (n++);
                      else if (n > 0)
                        while (n--) p = p->next;
                      return const_iterator {p};
                    };
           };

auto ptr = [] (const_iterator i) {
             return *reinterpret_cast<nodeptr*>(&i);
           };

SCENARIO ("List::transfer should move the iter range [first,last) out of their list and into pos' list before pos", "[List][transfer]")
{
  GIVEN ("A list with some elements")
  {
    constexpr int LSIZE = 10;
    list l;
    nodeptr lNodes[LSIZE];
    nodeptr lHeader = header (l);
    for (int i = 0; i < LSIZE; ++i)
    {
      lNodes[i] = new node (i + 1);
    }
    lHeader->next = lNodes[0];
    lHeader->prev = lNodes[LSIZE - 1];
    for (int i = 0; i < LSIZE; ++i)
    {
      if (i == LSIZE - 1) {
        lNodes[i]->next = lHeader;
      } else {
        lNodes[i]->next = lNodes[i + 1];
      }
      if (i == 0) {
        lNodes[i]->prev = lHeader;
      } else {
        lNodes[i]->prev = lNodes[i - 1];
      }
    }

    GIVEN ("Another list with some elements")
    {
      constexpr int MSIZE = 5;
      list m;
      nodeptr mHeader = header(m);
      nodeptr mNodes[MSIZE];
      for (int i = 0; i < MSIZE; ++i)
      {
        mNodes[i] = new node (11 * (i + 1));
      }

      mHeader->next = mNodes[0];
      mHeader->prev = mNodes[MSIZE - 1];
      for (int i = 0; i < MSIZE; ++i)
      {
        if (i == MSIZE - 1) {
          mNodes[i]->next = mHeader;
        } else {
          mNodes[i]->next = mNodes[i + 1];
        }
        if (i == 0) {
          mNodes[i]->prev = mHeader;
        } else {
          mNodes[i]->prev = mNodes[i - 1];
        }
      }

      WHEN ("We transfer an empty range")
      {
        auto pos = ptr(nth(l)(LSIZE / 2));
        auto loc = nth(m)(MSIZE / 2);

        auto posPtr = ptr(pos);
        auto prevOfPosPtr = posPtr->prev;
        auto nextOfPosPtr = posPtr->next;

        auto beforeLocPtr = ptr(loc)->prev;
        auto locPtr = ptr(loc);
        list::transfer (pos, loc, loc);

        THEN ("[1] Nothing should change")
        {
          REQUIRE (posPtr->next == nextOfPosPtr);
          REQUIRE (posPtr->prev == prevOfPosPtr);
          REQUIRE (nextOfPosPtr->prev == posPtr);
          REQUIRE (prevOfPosPtr->next == posPtr);
          REQUIRE (beforeLocPtr->next == locPtr);
          REQUIRE (locPtr->prev == beforeLocPtr);
          REQUIRE (locPtr->next->prev == locPtr);
          REQUIRE (beforeLocPtr->prev->next == beforeLocPtr);
        }

      }
      WHEN ("We transfer a single element")
      {
        auto pos = nth(l)(LSIZE / 2);
        auto first = nth(m)(MSIZE / 2);
        auto last = nth(m)(MSIZE / 2 + 1);

        auto posPtr = ptr(pos);
        auto prevOfPosPtr = posPtr->prev;
        auto nextOfPosPtr = posPtr->next;

        auto beforeFirstPtr = ptr(first)->prev;
        auto beforeLastPtr = ptr(last)->prev;
        auto lastPtr = ptr(last);

        list::transfer (pos, first, last);

        THEN ("[1] the element should have been unlinked")
        {
          REQUIRE (beforeFirstPtr->next == lastPtr);
          REQUIRE (lastPtr->prev == beforeFirstPtr);
          REQUIRE (beforeFirstPtr->prev->next == beforeFirstPtr);
          REQUIRE (lastPtr->next->prev == lastPtr);
        }
        AND_THEN ("[1] the element should have been inserted before pos with no side effects")
        {
          REQUIRE (prevOfPosPtr->next == ptr(first));
          REQUIRE (ptr(first)->prev == prevOfPosPtr);

          REQUIRE (beforeLastPtr->next == posPtr);
          REQUIRE (posPtr->prev == beforeLastPtr);

          REQUIRE (posPtr->next == nextOfPosPtr);
        }
      }
      WHEN ("We transfer a range")
      {
        auto pos = nth(l)(LSIZE / 2);
        auto first = begin(m);
        auto last = end(m);

        auto posPtr = ptr(pos);
        auto prevOfPosPtr = posPtr->prev;
        auto nextOfPosPtr = posPtr->next;

        auto beforeFirstPtr = ptr(first)->prev;
        auto beforeLastPtr = ptr(last)->prev;
        auto lastPtr = ptr(last);

        list::transfer (pos, first, last);

        THEN ("[1] first and last should have been unlinked")
        {
          REQUIRE (beforeFirstPtr->next == lastPtr);
          REQUIRE (lastPtr->prev == beforeFirstPtr);

          REQUIRE (beforeFirstPtr->prev->next == beforeFirstPtr);
          REQUIRE (lastPtr->next->prev == lastPtr);
        }
        AND_THEN ("[1] first and last should have been inserted before pos with no side effects")
        {
          REQUIRE (prevOfPosPtr->next == ptr(first));
          REQUIRE (ptr(first)->prev == prevOfPosPtr);

          REQUIRE (beforeLastPtr->next == posPtr);
          REQUIRE (posPtr->prev == beforeLastPtr);

          REQUIRE (posPtr->next == nextOfPosPtr);
        }
      }

      mHeader->next = mHeader->prev = mHeader;
      for (int i = 0; i < MSIZE; ++i)
      {
        delete mNodes[i];
      }
    }
    lHeader->next = lHeader->prev = lHeader;
    for (int i = 0; i < LSIZE; ++i)
    {
      delete lNodes[i];
    }
  }
}

SCENARIO ("Iterators can be incremented, decremented, and compared", "[Iterator]")
{
  GIVEN ("A doubly-linked circular list containing 1-2-3")
  {
    using node = List<int>::Node;
    using iter = List<int>::Iterator;
    node header;
    node n1 (1);
    node n2 (2);
    node n3 (3);

    header.next = &n1;
    n1.next = &n2;
    n2.next = &n3;
    n3.next = &header;

    header.prev = &n3;
    n1.prev = &header;
    n2.prev = &n1;
    n3.prev = &n2;

    iter begin (&n1);
    iter end (&header);

    WHEN ("We preincrement an iterator")
    {
      THEN ("[1] We update the current iterator and return")
      {
        REQUIRE (*++begin == 2);
        REQUIRE (*begin == 2);
      }
    }
    WHEN ("We postincrement an iterator")
    {
      THEN ("[1] We return a copy of the iterator and then update")
      {
        REQUIRE (*begin++ == 1);
        REQUIRE (*begin == 2);
      }
    }
    WHEN ("We predecrement an iterator")
    {
      THEN ("[1] We update the current iterator and return")
      {
        REQUIRE (*--end == 3);
        REQUIRE (*end == 3);
      }
    }
    WHEN ("We postdecrement an iterator")
    {
      THEN ("[1] We return a copy of the iterator and then update")
      {
        REQUIRE (*begin-- == 1);
        REQUIRE (*begin == 0);
      }
    }
    WHEN ("We compare two iterators")
    {
      THEN ("[1] They return true IFF the two iterators point to the same Node")
      {
        REQUIRE_FALSE (begin == end);
        REQUIRE (++end == begin);
      }
    }
  }
}
SCENARIO ("ConstIterators can be incremented, decremented, and compared", "[ConstIterator]")
{
  GIVEN ("A doubly-linked circular list containing 1-2-3")
  {
    using node = List<int>::Node;
    using iter = List<int>::ConstIterator;
    node header;
    node n1 (1);
    node n2 (2);
    node n3 (3);

    header.next = &n1;
    n1.next = &n2;
    n2.next = &n3;
    n3.next = &header;

    header.prev = &n3;
    n1.prev = &header;
    n2.prev = &n1;
    n3.prev = &n2;

    iter begin (&n1);
    iter end (&header);

    WHEN ("We preincrement an iterator")
    {
      THEN ("[1] We update the current iterator and return")
      {
        REQUIRE (*++begin == 2);
        REQUIRE (*begin == 2);
      }
    }
    WHEN ("We postincrement an iterator")
    {
      THEN ("[1] We return a copy of the iterator and then update")
      {
        REQUIRE (*begin++ == 1);
        REQUIRE (*begin == 2);
      }
    }
    WHEN ("We predecrement an iterator")
    {
      THEN ("[1] We update the current iterator and return")
      {
        REQUIRE (*--end == 3);
        REQUIRE (*end == 3);
      }
    }
    WHEN ("We postdecrement an iterator")
    {
      THEN ("[1] We return a copy of the iterator and then update")
      {
        REQUIRE (*begin-- == 1);
        REQUIRE (*begin == 0);
      }
    }
    WHEN ("We compare two iterators")
    {
      THEN ("[1] They return true IFF the two iterators point to the same Node")
      {
        REQUIRE_FALSE (begin == end);
        REQUIRE (++end == begin);
      }
    }
  }
}

SCENARIO ("List has begin() and end()", "[List][iterator]")
{
  GIVEN("A List with some elements")
  {
    List<int> l;
    List<int>::Node* n = new List<int>::Node(123);
    l.m_header.next = n;
    n->next = &l.m_header;
    List<int>::iterator end { &l.m_header };
    List<int>::const_iterator cend { &l.m_header };
    List<int>::iterator begin { n };
    List<int>::const_iterator cbegin { n };

    List<int> const& cref = l;

    WHEN ("We call begin()")
    {
      auto yourBegin = l.begin();
      THEN ("[1] The iterators match")
      {
        REQUIRE (yourBegin == begin);
      }
    }
    WHEN ("We call begin() const")
    {
      auto yourBegin = cref.begin();
      THEN ("[1] The iterators match")
      {
        REQUIRE (yourBegin == cbegin);
      }
    }
    WHEN ("We call end()")
    {
      auto yourEnd = l.end();
      THEN ("[1] The iterators match")
      {
        REQUIRE (yourEnd == end);
      }
    }
    WHEN ("We call end() const")
    {
      auto yourEnd = cref.end();
      THEN ("[1] The iterators match")
      {
        REQUIRE (yourEnd == cend);
      }
    }

    l.m_header.next = l.m_header.prev = &l.m_header;
    delete n;
  }
}

SCENARIO ("List can be default-constructed", "[List][ctor]")
{
  WHEN ("We construct a list")
  {
    List<int> l;
    THEN ("[1] header is circularly linked and size is 0")
    {
      REQUIRE (l.m_header.next == &l.m_header);
      REQUIRE (l.m_header.prev == &l.m_header);
      REQUIRE (l.m_size == 0);
    }
  }
}

SCENARIO ("List can be range-constructed", "[List][ctor]")
{
  std::forward_list<int> list { 1, 2, 3 };
  WHEN ("We construct a list from an empty Range")
  {
    List<int> l (list.end(), list.end());
    THEN ("[1] The list should be empty")
    {
      REQUIRE (l.m_header.next == &l.m_header);
      REQUIRE (l.m_header.prev == &l.m_header);
      REQUIRE (l.m_size == 0);
    }
  }
  WHEN ("We construct a list from a Range")
  {
    List<int> l (list.begin(), list.end());
    std::list <int> list2 (list.begin(), list.end());
    THEN ("[1] The list should contain the correct number of elements")
    {
      REQUIRE (l.m_size == std::distance (list2.begin(), list2.end()));
    }
    AND_THEN ("[1] The list should contain all of the elements")
    {
      REQUIRE (std::equal (list2.begin(), list2.end(), l.begin(), l.end()));
      REQUIRE (std::equal (list2.rbegin(), list2.rend(), l.rbegin(), l.rend()));
    }
  }
}

SCENARIO ("List has front() and back()", "[List][access]")
{
  GIVEN ("A List with some elements")
  {
    List<int> list { 11, 22, 33, 44 };

    WHEN ("We call front()")
    {
      THEN ("[1] the first element is returned")
      {
        REQUIRE (list.front() == 11);
        list.front() = 99;
        REQUIRE (*list.begin() == 99);
      }
    }
    AND_WHEN ("We call back()")
    {
      THEN ("[1] the last element is returned")
      {
        REQUIRE (list.back() == 44);
        list.back() = 99;
        REQUIRE (*--list.end() == 99);
      }
    }
  }

  GIVEN ("A const List with some elements")
  {
    List<int> const list { 11, 22, 33, 44 };

    WHEN ("We call front()")
    {
      THEN ("[1] the first element is returned")
      {
        REQUIRE (list.front() == 11);
      }
    }
    AND_WHEN ("We call back()")
    {
      THEN ("[1] the last element is returned")
      {
        REQUIRE (list.back() == 44);
      }
    }
  }
}

SCENARIO ("List has push_front and push_back", "[List][push]")
{
  GIVEN ("A list with some elements")
  {
    List<int> yours { 1, 2, 3, 4 };
    std::list<int> mine { 1, 2, 3, 4 };
    int value =  99;
    WHEN ("We push_back a const reference")
    {
      yours.push_back (value);
      mine.push_back (value);
      THEN ("[2] The value is added to the back and size increases by one")
      {
        REQUIRE (yours.size() == mine.size());
        REQUIRE (std::equal (yours.begin(), yours.end(), mine.begin(), mine.end()));
        REQUIRE (std::equal (yours.rbegin(), yours.rend(), mine.rbegin(), mine.rend()));
        REQUIRE (yours.back() == value);
      }
    }
    WHEN ("We push_back an r-value reference")
    {
      yours.push_back (std::move(value));
      mine.push_back (std::move(value));
      THEN ("[2] The value is added to the back and size increases by one")
      {
        REQUIRE (yours.size() == mine.size());
        REQUIRE (std::equal (yours.begin(), yours.end(), mine.begin(), mine.end()));
        REQUIRE (std::equal (yours.rbegin(), yours.rend(), mine.rbegin(), mine.rend()));
        REQUIRE (yours.back() == value);
      }
    }
    WHEN ("We push_front a const reference")
    {
      yours.push_front (value);
      mine.push_front (value);
      THEN ("[2] The value is added to the front and size increases by one")
      {
        REQUIRE (yours.size() == mine.size());
        REQUIRE (std::equal (yours.begin(), yours.end(), mine.begin(), mine.end()));
        REQUIRE (std::equal (yours.rbegin(), yours.rend(), mine.rbegin(), mine.rend()));
        REQUIRE (yours.front() == value);
      }
    }
    WHEN ("We push_front an r-value reference")
    {
      yours.push_front (std::move(value));
      mine.push_front (std::move(value));
      THEN ("[2] The value is added to the front and size increases by one")
      {
        REQUIRE (yours.size() == mine.size());
        REQUIRE (std::equal (yours.begin(), yours.end(), mine.begin(), mine.end()));
        REQUIRE (std::equal (yours.rbegin(), yours.rend(), mine.rbegin(), mine.rend()));
        REQUIRE (yours.front() == value);
      }
    }
  }
}

SCENARIO ("List has insert", "[List][insert]")
{
  GIVEN ("A List with some elements")
  {
    List<int> yours { 1, 2, 3, 4 };
    std::list<int> mine { 1, 2, 3, 4 };
    int value =  99;
    WHEN ("We insert a const reference")
    {
      auto your_iter = std::next (yours.begin(), yours.size() / 2);
      auto my_iter = std::next (mine.begin(), mine.size() / 2);

      auto your_returned_iter = yours.insert (your_iter, value);
      auto my_returned_iter = mine.insert (my_iter, value);
      THEN ("[3] The value is added to the back and size increases by one")
      {
        REQUIRE (yours.size() == mine.size());
        REQUIRE (std::equal (yours.begin(), yours.end(), mine.begin(), mine.end()));
        REQUIRE (std::equal (yours.rbegin(), yours.rend(), mine.rbegin(), mine.rend()));
        REQUIRE (*your_returned_iter == *my_returned_iter);
        REQUIRE (std::next(your_returned_iter) == your_iter);
      }
    }
    WHEN ("We insert an r-value reference")
    {
      auto your_iter = std::next (yours.begin(), yours.size() / 2);
      auto my_iter = std::next (mine.begin(), mine.size() / 2);

      auto your_returned_iter = yours.insert (your_iter, std::move(value));
      auto my_returned_iter = mine.insert (my_iter, std::move(value));
      THEN ("[3] The value is added to the back and size increases by one")
      {
        REQUIRE (yours.size() == mine.size());
        REQUIRE (std::equal (yours.begin(), yours.end(), mine.begin(), mine.end()));
        REQUIRE (std::equal (yours.rbegin(), yours.rend(), mine.rbegin(), mine.rend()));
        REQUIRE (*your_returned_iter == *my_returned_iter);
        REQUIRE (std::next(your_returned_iter) == your_iter);
      }
    }
  }
}

SCENARIO ("List can be size-constructed with default value", "[List][size-ctor]")
{
  WHEN ("We size-construct a List with a default value")
  {
    List<double> yours (99, 3.14);
    std::list<double> mine (99, 3.14);
    THEN ("[3] size is set and all elements are inserted")
    {
      REQUIRE (yours.size() == mine.size());
      REQUIRE (std::equal (yours.begin(), yours.end(), mine.begin(), mine.end()));
      REQUIRE (std::equal (yours.rbegin(), yours.rend(), mine.rbegin(), mine.rend()));
    }
  }
}

SCENARIO ("List can be destructed", "[List][dtor]")
{
  WHEN ("We destruct a List")
  {
    int SIZE = 99;
    {
      List<Counter<int>> a (SIZE);
      Counter<int>::reset();
    }

    THEN ("[3] All nodes are properly destroyed")
    {
      // each node + header
      REQUIRE (Counter<int>::dtor == SIZE + 1);
    }
  }
}

SCENARIO ("List has copy-assignment", "[List][copy-assign]")
{
  GIVEN ("A List with some elements")
  {
    List<int> l { 1, 2, 3, 4, 5 };
    List<int> copy (l);
    WHEN ("We copy-assign to itself")
    {
      l = l;
      THEN ("[2] Nothing should happen")
      {
        REQUIRE (l.size() == copy.size());
        REQUIRE (std::equal (l.begin(), l.end(), copy.begin(), copy.end()));
        REQUIRE (std::equal (l.rbegin(), l.rend(), copy.rbegin(), copy.rend()));
      }
    }
    WHEN ("We copy-assign from another list")
    {
      *l.end() = 99;

      List<int> other { 11, 22, 33, 44, 55, 66, 77, 88 };
      *other.end() = 999;

      l = other;
      THEN ("[2] size and all elements should be updated")
      {

        REQUIRE (l.size() == other.size());
        auto i = l.begin();
        auto j = other.begin();
        for (size_t k = 0; k < l.size(); ++k) {
          REQUIRE (*i++ == *j++);
        }

        REQUIRE (*i == *l.end());
        REQUIRE (j == other.end());
        for (size_t k = 0; k < l.size(); ++k) {
          REQUIRE (*--i == *--j);
        }
        REQUIRE (std::equal (l.begin(), l.end(), other.begin(), other.end()));
        REQUIRE (std::equal (l.rbegin(), l.rend(), other.rbegin(), other.rend()));

      }
      AND_THEN ("[2] all old elements should have been destroyed or overwritten")
      {
        int ORIG_SIZE = 10;
        int NEW_SIZE = 20;
        Counter<int> c;
        c.data = 99;
        List<Counter<int>> orig (ORIG_SIZE, c);
        List<Counter<int>> newList (NEW_SIZE, c);
        Counter<int>::reset();
        orig = newList;
        int constructors_called = (Counter<int>::ctor + Counter<int>::cctor + Counter<int>::mctor);
        int destructors_called = Counter<int>::dtor;
        REQUIRE ((constructors_called - destructors_called) == (NEW_SIZE - ORIG_SIZE));
      }
    }
  }
}

SCENARIO ("List has assign(std::initializer_list<T>)", "[List][assign]")
{
  GIVEN ("A List with elements")
  {
    List<int> l = { 1, 2, 3, 4 };
    WHEN ("We assign() a smaller initializer_list to the List")
    {
      std::initializer_list<int> ilist = {11, 22, 33};
      l.assign (ilist);
      THEN ("[1] Everything should be replaced")
      {
        REQUIRE (l.size() == ilist.size());
        REQUIRE (std::equal (l.begin(), l.end(), ilist.begin(), ilist.end()));
      }
    }
    WHEN ("We assign() a larger initializer_list to the List")
    {
      std::initializer_list<int> ilist = {11, 22, 33, 44, 55};
      l.assign (ilist);
      THEN ("[1] Everything should be replaced")
      {
        REQUIRE (l.size() == ilist.size());
        REQUIRE (std::equal (l.begin(), l.end(), ilist.begin(), ilist.end()));
      }
    }
  }
}

SCENARIO ("List has single-element erase()", "[List][erase]")
{
  GIVEN ("A List with some elements")
  {
    int SIZE = 5;
    Counter<int> v;
    List<Counter<int>> l (SIZE);
    auto n1 = l.begin();
    auto n2 = std::next(n1);
    auto n3 = std::next(n2);
    auto n4 = std::next(n3);
    auto n5 = std::next(n4);
    auto end = std::next(n5);

    {
      auto itr = n1;
      for (int i = 1; i <= SIZE; ++i) {
        (itr++)->data = i;
      }
    }
    Counter<int>::reset();

    WHEN ("We erase begin()")
    {
      auto returned_iterator = l.erase (n1);
      THEN ("[2] the element should be removed and size be decremented")
      {
        REQUIRE (returned_iterator == n2);
        REQUIRE (l.size() == SIZE - 1);
        REQUIRE (((Counter<int>::dtor == 1) && "destructor was called"));
        REQUIRE (std::next(end) == n2);
        REQUIRE (std::prev(n2) == end);
      }
    }
    WHEN ("We erase from the middle")
    {
      auto returned_iterator = l.erase (n3);
      THEN ("[2] the element should be removed and size be decremented")
      {
        REQUIRE (returned_iterator == n4);
        REQUIRE (l.size() == SIZE - 1);
        REQUIRE (((Counter<int>::dtor == 1) && "destructor was called"));
        REQUIRE (std::next(n2) == n4);
        REQUIRE (std::prev(n4) == n2);
      }
    }
    WHEN ("We erase the last element")
    {
      auto returned_iterator = l.erase (n5);
      THEN ("[2] the element should be removed and size be decremented")
      {
        REQUIRE (returned_iterator == end);
        REQUIRE (l.size() == SIZE - 1);
        REQUIRE (((Counter<int>::dtor == 1) && "destructor was called"));
        REQUIRE (std::next(n4) == end);
        REQUIRE (std::prev(end) == n4);
      }
    }
  }
}

SCENARIO ("List has range-based erase()", "[List][erase-range]")
{
  GIVEN ("A List with some elements")
  {
    int SIZE = 8;
    Counter<int> v;
    List<Counter<int>> l (SIZE);
    auto n1 = l.begin();
    auto n2 = std::next(n1);
    auto n3 = std::next(n2);
    auto n4 = std::next(n3);
    auto n5 = std::next(n4);
    auto n6 = std::next(n5);
    auto n7 = std::next(n6);
    auto n8 = std::next(n7);
    auto end = std::next(n8);

    {
      auto itr = n1;
      for (int i = 1; i <= SIZE; ++i) {
        (itr++)->data = i;
      }
    }
    Counter<int>::reset();

    WHEN ("We erase an empty range")
    {
      auto returned_iterator = l.erase (n1, n1);
      THEN ("[2] the element should be removed and size be decremented")
      {
        REQUIRE (returned_iterator == n1);
        REQUIRE (l.size() == SIZE);
        REQUIRE (((Counter<int>::dtor == 0) && "destructor was NOT called"));
        REQUIRE (std::next(end) == n1);
        REQUIRE (std::prev(n1) == end);
      }
    }
    WHEN ("We erase a range of length one")
    {
      auto returned_iterator = l.erase (n3, n4);
      THEN ("[2] the element should be removed and size be decremented")
      {
        REQUIRE (returned_iterator == n4);
        REQUIRE (l.size() == SIZE - 1);
        REQUIRE (((Counter<int>::dtor == 1) && "destructor was called"));
        REQUIRE (std::next(n2) == n4);
        REQUIRE (std::prev(n4) == n2);
      }
    }
    WHEN ("We erase half the elements")
    {
      auto returned_iterator = l.erase (n2, n6);
      THEN ("[2] the element should be removed and size be decremented")
      {
        REQUIRE (returned_iterator == n6);
        REQUIRE (l.size() == SIZE / 2);
        REQUIRE (((Counter<int>::dtor == SIZE / 2) && "destructor was called"));
        REQUIRE (std::next(n1) == n6);
        REQUIRE (std::prev(n6) == n1);
      }
    }
    WHEN ("We erase all elements")
    {
      auto returned_iterator = l.erase (n1, end);
      THEN ("[2] the element should be removed and size be decremented")
      {
        REQUIRE (returned_iterator == end);
        REQUIRE (l.size() == 0);
        REQUIRE (((Counter<int>::dtor == SIZE) && "destructor was called"));
        REQUIRE (std::next(end) == end);
        REQUIRE (std::prev(end) == end);
      }
    }
  }
}

SCENARIO ("List has clear()", "[List][clear]")
{
  GIVEN ("A List of some size")
  {
    int SIZE = 30;
    List<Counter<int>> l (SIZE);
    WHEN ("We clear the list")
    {
      Counter<int>::reset();
      l.clear();
      THEN ("[3] the list should be empty")
      {
        REQUIRE (l.size() == 0);
        REQUIRE (l.begin() == l.end());
        REQUIRE (++l.begin() == l.begin());
        REQUIRE (--l.begin() == l.begin());
      }
      AND_THEN ("[2] all elements should have been destructed")
      {
        REQUIRE (Counter<int>::dtor == SIZE);
      }
    }
  }
}

SCENARIO ("List can be resized", "[List][resize]")
{
  GIVEN ("A List of some size")
  {
    List<int> yours { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    std::list<int> mine { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    WHEN ("We resize the list to a smaller size with no default value")
    {
      yours.resize (yours.size() / 2);
      mine.resize (mine.size() / 2);
      THEN ("[1] size should be decreased and elements removed")
      {
        REQUIRE (yours.size() == mine.size());
        REQUIRE (std::equal (yours.begin(), yours.end(), mine.begin(), mine.end()));
        REQUIRE (std::equal (yours.rbegin(), yours.rend(), mine.rbegin(), mine.rend()));
      }
      AND_THEN ("[1] Elements should have been destructed")
      {
        int SIZE = 20;
        int NEW_SIZE = 10;
        List<Counter<int>> a (SIZE);
        Counter<int>::reset();
        a.resize (NEW_SIZE);
        REQUIRE (Counter<int>::dtor == (SIZE - NEW_SIZE));
      }
    }
    WHEN ("We resize the list to a smaller size with a value")
    {
      yours.resize (yours.size() / 2);
      mine.resize (mine.size() / 2);
      THEN ("[1] size should be decreased and elements removed")
      {
        REQUIRE (yours.size() == mine.size());
        REQUIRE (std::equal (yours.begin(), yours.end(), mine.begin(), mine.end()));
        REQUIRE (std::equal (yours.rbegin(), yours.rend(), mine.rbegin(), mine.rend()));
      }
      AND_THEN ("[1] Elements should have been destructed")
      {
        int SIZE = 20;
        int NEW_SIZE = 10;
        Counter<int> val;
        val.data = 10;
        List<Counter<int>> a (SIZE, val);
        Counter<int>::reset();
        a.resize (NEW_SIZE);
        REQUIRE (Counter<int>::dtor == (SIZE - NEW_SIZE));
      }
    }
    WHEN ("We resize the list to a larger size with no default value")
    {
      yours.resize (yours.size() * 2);
      mine.resize (mine.size() * 2);
      THEN ("[2] size should increase and elements added to the end")
      {
        REQUIRE (yours.size() == mine.size());
        REQUIRE (std::equal (yours.begin(), yours.end(), mine.begin(), mine.end()));
        REQUIRE (std::equal (yours.rbegin(), yours.rend(), mine.rbegin(), mine.rend()));
      }

    }
    WHEN ("We resize the list to a larger size with a value")
    {
      yours.resize (yours.size() * 2, 99);
      mine.resize (mine.size() * 2, 99);
      THEN ("[2] size should increase and elements added to the end")
      {
        REQUIRE (yours.size() == mine.size());
        REQUIRE (std::equal (yours.begin(), yours.end(), mine.begin(), mine.end()));
        REQUIRE (std::equal (yours.rbegin(), yours.rend(), mine.rbegin(), mine.rend()));
      }
    }
  }
}

SCENARIO ("reverse() maintains all iterators but reverses the order")
{
  GIVEN ("A List with values")
  {
    int SIZE = 13;
    List<int> list;
    for (int i = 1; i <= SIZE; ++i) {
      list.push_back (i);
    }
    std::vector<List<int>::iterator> iters;
    iters.push_back(list.end());
    for (auto i = list.begin(); i != list.end(); ++i) {
      iters.push_back (i);
    }
    iters.push_back(list.end());
    WHEN ("We reverse() the List")
    {
      list.reverse();
      THEN("[5] All iterators still refer to their original values and elements are reversed")
      {
        for (int i = 1; i <= SIZE; ++i) {
          REQUIRE (*iters[i] == i);
          REQUIRE (std::prev(iters[i]) == iters[i + 1]);
          REQUIRE (std::next(iters[i]) == iters[i - 1]);
        }
        REQUIRE (list.size() == SIZE);
      }
    }
  }
}


#endif
