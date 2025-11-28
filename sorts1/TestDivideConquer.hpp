#include "DivideAndConquer.hpp"

#include <algorithm>
#include <iterator>
#include <random>
#include <vector>

#include <catch2/catch_all.hpp>

SCENARIO ("median3 works", "[median3]")
{
  GIVEN ("A vector with at least 3 elements")
  {
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> copy (v);
    auto first = v.begin ();
    auto last = --v.end ();
    auto mid = v.begin () + v.size () / 2;
    WHEN ("The ordering is [low] <= [mid] <= [high]")
    {
      AND_WHEN ("We call median3")
      {
        auto result = SortUtils::median3 (v.begin (), v.end ());
        THEN ("[1] nothing should be swapped")
        {
          REQUIRE (*first <= *mid);
          REQUIRE (*mid <= *last);
          REQUIRE (v == copy);
        }
        THEN ("[0.5] the correct iterator is returned")
        {
          REQUIRE (result == *mid);
        }
      }
    }
    WHEN ("The ordering is [low] <= [high] <= [mid]")
    {
      std::iter_swap (mid, last);
      AND_WHEN ("We call median3")
      {
        auto result = SortUtils::median3 (v.begin (), v.end ());
        THEN ("[1] values should be swapped")
        {
          REQUIRE (*first <= *mid);
          REQUIRE (*mid <= *last);
          REQUIRE (v == copy);
        }
        THEN ("[0.5] the correct iterator is returned")
        {
          REQUIRE (result == *mid);
        }
      }
    }
    WHEN ("The ordering is [mid] <= [low] <= [high]")
    {
      std::iter_swap (mid, last);
      AND_WHEN ("We call median3")
      {
        auto result = SortUtils::median3 (v.begin (), v.end ());
        THEN ("[1] values should be swapped")
        {
          REQUIRE (*first <= *mid);
          REQUIRE (*mid <= *last);
          REQUIRE (v == copy);
        }
        THEN ("[0.5] the correct iterator is returned")
        {
          REQUIRE (result == *mid);
        }
      }
    }
    WHEN ("The ordering is [mid] <= [high] <= [low]")
    {
      std::iter_swap (mid, last);
      std::iter_swap (first, last);
      AND_WHEN ("We call median3")
      {
        auto result = SortUtils::median3 (v.begin (), v.end ());
        THEN ("[1] values should be swapped")
        {
          REQUIRE (*first <= *mid);
          REQUIRE (*mid <= *last);
          REQUIRE (v == copy);
        }
        THEN ("[0.5] the correct iterator is returned")
        {
          REQUIRE (result == *mid);
        }
      }
    }
    WHEN ("The ordering is [high] <= [low] <= [mid]")
    {
      std::iter_swap (first, last);
      std::iter_swap (mid, last);
      AND_WHEN ("We call median3")
      {
        auto result = SortUtils::median3 (v.begin (), v.end ());
        THEN ("[1] values should be swapped")
        {
          REQUIRE (*first <= *mid);
          REQUIRE (*mid <= *last);
          REQUIRE (v == copy);
        }
        THEN ("[0.5] the correct iterator is returned")
        {
          REQUIRE (result == *mid);
        }
      }
    }
    WHEN ("The ordering is [high] <= [mid] <= [low]")
    {
      std::iter_swap (first, last);
      AND_WHEN ("We call median3")
      {
        auto result = SortUtils::median3 (v.begin (), v.end ());
        THEN ("[1] values should be swapped")
        {
          REQUIRE (*first <= *mid);
          REQUIRE (*mid <= *last);
          REQUIRE (v == copy);
        }
        THEN ("[0.5] the correct iterator is returned")
        {
          REQUIRE (result == *mid);
        }
      }
    }
  }
}

SCENARIO ("merge works", "[merge]")
{
  GIVEN ("Two sorted vectors")
  {
    std::vector<int> a (10);
    std::vector<int> b (4);
    std::vector<int> out (a.size () + b.size ());
    std::vector<int> copy_out (out);
    auto expected = out.end ();
    WHEN ("All elements in Range1 are less than all elements in Range2")
    {
      std::iota (a.begin (), a.end (), 0);
      std::iota (b.begin (), b.end (), a.size () * 2);
      AND_WHEN ("We call merge")
      {
        auto result = SortUtils::merge (a.begin (), a.end (), b.begin (),
                                        b.end (), out.begin ());
        std::merge (a.begin (), a.end (), b.begin (), b.end (),
                    copy_out.begin ());
        THEN ("[2] We expect all elements to be merged to out")
        {
          REQUIRE (out == copy_out);
        }
        THEN ("[0.5] The return value is correct")
        {
          REQUIRE (result == expected);
        }
      }
    }
    WHEN ("All elements in Range2 are less than all elements in Range1")
    {
      std::iota (a.begin (), a.end (), b.size () * 2);
      std::iota (b.begin (), b.end (), 0);
      AND_WHEN ("We call merge")
      {
        auto result = SortUtils::merge (a.begin (), a.end (), b.begin (),
                                        b.end (), out.begin ());
        std::merge (a.begin (), a.end (), b.begin (), b.end (),
                    copy_out.begin ());
        THEN ("[2] We expect all elements to be merged to out")
        {
          REQUIRE (out == copy_out);
        }
        THEN ("[0.5] The return value is correct")
        {
          REQUIRE (result == expected);
        }
      }
    }
    WHEN ("All elements are mixed between Range1 and Range2")
    {
      size_t i = 0;
      for (auto& v : a)
      {
        v = 2 * i++ + 1;
      }
      i = 0;
      for (auto& v : b)
      {
        v = 2 * i++;
      }
      AND_WHEN ("We call merge")
      {
        auto result = SortUtils::merge (a.begin (), a.end (), b.begin (),
                                        b.end (), out.begin ());
        std::merge (a.begin (), a.end (), b.begin (), b.end (),
                    copy_out.begin ());
        THEN ("[4] We expect all elements to be merged to out")
        {
          REQUIRE (out == copy_out);
        }
        THEN ("[1] The return value is correct")
        {
          REQUIRE (result == expected);
        }
      }
    }
  }
}

SCENARIO ("partition works", "[partition]")
{
  int const pivot = GENERATE (range (1, 11));
  std::vector<int> v;
  v.reserve (100);
  const int seed = 1337;
  std::minstd_rand rng (seed);
  for (int i = 1; i <= 10; ++i)
  {
    v.insert (v.end (), 1 + rng () % 10, i);
  }
  std::shuffle (v.begin (), v.end (), rng);
  GIVEN ("A vector with some duplicates")
  {
    std::vector<int> copy (v);
    WHEN ("We pick a random pivot")
    {
      INFO ("Before partition:");
      CAPTURE (v);
      auto result = SortUtils::partition (v.begin (), v.end (), pivot);
      INFO ("After partition:");
      auto dup_begin = result.first;
      auto greater_begin = result.second;
      CAPTURE (pivot, v);
      THEN ("[1.5] the vector is properly partitioned")
      {
        auto iter = v.begin ();
        while (iter != v.end () && *iter < pivot)
          ++iter;
        {
          INFO (
            "The first of the pair should be where iter is no longer < pivot");
          REQUIRE (std::distance (v.begin (), iter) ==
                   std::distance (v.begin (), dup_begin));
        }
        while (iter != v.end () && *iter == pivot)
          ++iter;
        {
          INFO (
            "The second of the pair should be where iter is no longer == "
            "pivot");
          REQUIRE (std::distance (v.begin (), iter) ==
                   std::distance (v.begin (), greater_begin));
        }
        while (iter != v.end () && *iter > pivot)
          ++iter;
        {
          INFO ("No elements should exist after testing > pivot");
          REQUIRE (iter == v.end ());
        }
        std::sort (v.begin (), v.end ());
        std::sort (copy.begin (), copy.end ());
        INFO ("There were no loss of elements");
        REQUIRE (v == copy);
      }
    }
  }
}

SCENARIO ("nth_element works", "[nth_element]")
{
  std::vector<int> v (40);
  std::iota (v.begin (), v.end (), 1);
  std::shuffle (v.begin (), v.end (), std::minstd_rand{2047});

  GIVEN ("A vector of some size")
  {
    WHEN ("We call nth_element")
    {
      int const index = GENERATE (0, 39, take (8, random (1, 38)));
      INFO ("Before call to nth_element");
      CAPTURE (v, index);
      auto copy = v;
      auto result = SortUtils::nth_element (v.begin (), v.end (), index);
      THEN ("[1] We get the correct value")
      {
        INFO ("After call to nth_element");
        CAPTURE (v);
        {
          INFO ("the value the iterator points to is correct");
          REQUIRE (*result == (index + 1));
        }
        {
          INFO ("the index of the iterator returned is correct");
          REQUIRE (std::distance (v.begin (), result) + 1 == *result);
        }
        std::sort (v.begin (), v.end ());
        std::sort (copy.begin (), copy.end ());
        {
          INFO ("No elements were lost");
          REQUIRE (v == copy);
        }
      }
    }
  }
}

SCENARIO ("merge_sort works", "[merge_sort]")
{
  GIVEN ("A vector of some size")
  {
    std::vector<int> v (31);
    std::iota (v.begin (), v.begin () + v.size() / 2, 1);
    std::iota (v.begin() + v.size() / 2, v.end(), v.size() / 4);
    std::vector<int> expected (v);
    std::sort (expected.begin(), expected.end());
    std::shuffle (v.begin (), v.end (), std::minstd_rand{2047});
    WHEN ("We call merge_sort")
    {
      SortUtils::merge_sort (v.begin (), v.end ());
      THEN ("[10] We get the right answer")
      {
        REQUIRE (expected == v);
      }
    }
  }
}


SCENARIO ("quick_sort works", "[quick_sort]")
{
  GIVEN ("A vector of some size")
  {
    std::vector<int> v (31);
    std::iota (v.begin (), v.begin () + v.size() / 2, 1);
    std::iota (v.begin() + v.size() / 2, v.end(), v.size() / 4);
    std::vector<int> expected (v);
    std::sort (expected.begin(), expected.end());
    std::shuffle (v.begin (), v.end (), std::minstd_rand{2047});
    WHEN ("We call quick_sort")
    {
      SortUtils::quick_sort (v.begin (), v.end ());
      THEN ("[10] We get the right answer")
      {
        REQUIRE (expected == v);
      }
    }
  }
}
