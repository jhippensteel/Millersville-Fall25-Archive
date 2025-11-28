// File: DivideAndConquer.hpp
// Author: Gary Zoppetti and Will Killian and Chad Hogg and Jaysen Hippensteel

#ifndef DIVIDE_AND_CONQUER_HPP_
#define DIVIDE_AND_CONQUER_HPP_

#include <iterator>
#include <utility>
#include <vector>
#include <iostream>

// NOTE: you are forbidden from using anything from <algorithm> for this assignment
//       EXCEPT for std::copy

namespace SortUtils
{

// [9]
// Given a RandomAccessRange [first, last), determine where the "midpoint"
// would be and perform the following steps:
// order *first, *mid, *std::prev(last) in such a way such that
//   *first <= *mid <= *std::prev(last)
//
// returns the median value (NOT an iterator)
//
template<std::random_access_iterator Iter>
std::iter_value_t<Iter>
median3 (Iter first, Iter last)
{
  auto mid = first + (last - first) / 2;

  if (*first > *mid){
    std::iter_swap(first, mid);
  }
  if(*mid > *std::prev(last)){
    std::iter_swap(mid, std::prev(last));
  }
  if (*first > *mid){
    std::iter_swap(first, mid);
  }
  return *mid;
}

// [10]
// Takes two sorted ranges [first1, last1) and [first2, last2)
// and "merges" them by copying values into the iterator starting
// at "out". Uses operator< for comparing values
//
// Returns the iterator of one-past-the-last where we wrote to out
//
template<typename Iter1, typename Iter2, typename OIter>
OIter
merge (Iter1 first1, Iter1 last1, Iter2 first2, Iter2 last2, OIter out)
{
  while (first1 != last1 && first2 != last2){
    if (*first1 <= *first2){
      *out = *first1;
      ++out;
      ++first1;
    }
    if (*first2 < *first1){
      *out = *first2;
      ++out;
      ++first2;
    }
  }
  while (first1 != last1){
    *out = *first1;
    ++out;
    ++first1;
  }
  while (first2 != last2){
    *out = *first2;
    ++out;
    ++first2;
  }
  return out;
}

// [15]
// Takes a RandomAccessRange [first, last) and partitions the data into
// three groups -- this should be accomplished in a SINGLE PASS Big-O: O(N)
//
// Group 1: all values in [first, last) < pivot
// Group 2: all values in [first, last) == pivot
// Group 3: all values in [first, last) > pivot
//
// [ ... Group 1 ... | ... Group 2 ... | ... Group 3 ... ]
//                   ^                 ^
//                   p1                p2
//
// Returns a pair of iterators pointing to "p1" and "p2" above
//
// Hint: See separate Three Way Partition explanation in handout.
//
template<typename Iter, typename Value>
std::pair<Iter, Iter>
partition (Iter first, Iter last, Value const& pivot)
{
  auto low = first;
  auto eq = first;
  auto hi = last;
  while (eq != hi){
    if(*eq < pivot){
      std::iter_swap(low, eq);
      ++low;
      ++eq;
    }
    else if (*eq > pivot){
      --hi;
      std::iter_swap(eq, hi);
    }
    else {
      ++eq;
    }
  }
  return std::make_pair(low, hi);
}

// [10]
// Given a RandomAccessRange, recursively call partition on either the
// left half or right half until you have found the nth largest element
//
// A call to nth_element (v.begin(), v.end(), 0) will return the min
// A call to nth_element (v.begin(), v.end(), v.size() - 1) will return the max
// A call to nth_element (v.begin(), v.end(), v.size() / 2) will return the median
//
// Precondition:
//   std::distance (begin, end) > n
//
// Hints:
//  - n will change if you need to recurse on the right half
//  - No recursion happens if "index of" n is between "index of" p1 and p2
//    remember: p1 and p2 are the return values to partition.
//  - call median3 to get a pivot value
//
template<typename Iter>
Iter
nth_element (Iter first, Iter last, size_t n)
{
  // TODO
  auto pivot = SortUtils::median3(first, last);
  auto [p1, p2] = SortUtils::partition (first, last, pivot);
  if(first+n >= p1 && first+n < p2){
	return first+n;
  }
  if(first+n < p1){
  	  return SortUtils::nth_element(first, p1, n);
  }
  if(first+n >= p2){
  	  return SortUtils::nth_element(p2, last, n-(p2-first));
  }
}

// [10]
// Given a RandomAccessRange, sort using merge sort
//
// Precondition:
//   std::distance (begin, end) > 0
//
// Hints:
//   - You will need a vector to act as a temporary buffer.
//   - The merge function will expect that vector to already be big enough
//     to hold all of the elements.
//
template<typename Iter>
void
merge_sort (Iter first, Iter last)
{
  // TODO
  // T is the type of data we are sorting
  using T = std::iter_value_t<Iter>;
  size_t length = last - first;
  if (length == 1) return;
  size_t mid = length/2;
  SortUtils::merge_sort(first, first+mid);
  SortUtils::merge_sort(first+mid, last);
  std::vector<T> mergedVector(length);
  SortUtils::merge(first, first+mid, first+mid, last, mergedVector.begin());
  std::copy(mergedVector.begin(), mergedVector.end(), first);
}

// Provided for you -- no need to change.
template<typename Iter>
void
insertion_sort (Iter first, Iter last)
{
  for (Iter i = first; i != last; ++i)
  {
    for (Iter j = i; j != first; --j)
    {
      if (*(j - 1) > *j)
      {
        std::iter_swap (j - 1, j);
      }
      else
      {
        break;
      }
    }
  }
}


// [10]
// Given a RandomAccessRange, sort using quick sort
//
// Precondition:
//   std::distance (begin, end) > 0
//
// Hints:
//   - median3 will be called to find the pivot
//   - partition should be called
//   - if there are fewer than 16 elements, use the provided insertion sort instead
//
template<typename Iter>
void
quick_sort (Iter first, Iter last)
{
  // TODO
  // T is the type of data we are sorting
  using T = std::iter_value_t<Iter>;
  if(last-first < 16) {
  	  SortUtils::insertion_sort(first, last);
  	  return;
  }
  auto pivot = SortUtils::median3(first, last);
  auto [p1, p2] = SortUtils::partition(first, last, pivot);
  SortUtils::quick_sort(first, p1);
  SortUtils::quick_sort(p2, last);
}

} // end namespace util

#endif
