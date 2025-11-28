
[4] static Node::unhook (Node* first, Node* last)
    [2] When first == last (single element)
    [2] When first != last

[4] Node::hook (Node* first, Node* last)
    [2] When first != last
    [2] When first == last (single element)

[5] static List::transfer(ConstIterator pos, ConstIterator first, ConstIterator last)
    [1] when first == last (no-op)
    [2] when distance(first, last) == 1 (single element)
    [2] when distance(first, last) >> 1 (many elements)

[1] Iterator& Iterator::operator++()
[1] Iterator Iterator::operator++(int)
[1] Iterator& Iterator::operator--()
[1] Iterator Iterator::operator--(int)
[1] friend bool Iterator::operator==(Iterator const&, Iterator const&)

[1] ConstIterator& ConstIterator::operator++()
[1] ConstIterator ConstIterator::operator++(int)
[1] ConstIterator& ConstIterator::operator--()
[1] ConstIterator ConstIterator::operator--(int)
[1] friend bool ConstIterator::operator==(ConstIterator const&, ConstIterator const&)

[1] iterator       List::begin()
[1] const_iterator List::begin() const
[1] iterator       List::end()
[1] const_iterator List::end() const

[1] List::List()
[3] List::List(InputIter, InputIter)
    [1] when distance(first, last) == 0
    [2] when distance(first, last)  > 0

[1] reference       List::front()
[1] const_reference List::front() const
[1] reference       List::back()
[1] const_reference List::back() const

[3] iterator List::insert (const_iterator, T const&)
[2] void List::push_back (T const&)
[2] void List::push_front (T const&)

[3] iterator List::insert (const_iterator, T&&)
[2] void List::push_back (T&&)
[2] void List::push_front (T&&)

[3] List::List(size_type, T const&)
[3] List::~List()

[6] List& List::operator=(List const&)

[2] void List::assign(std::initializer_list<T>)

[6] iterator List::erase (const_iterator)
[8] iterator List::erase (const_iterator, const_iterator)

[5] void List::clear()

[4] void List::resize (size_type)
[4] void List::resize (size_type, T const&)
