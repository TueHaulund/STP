stplib
===

Each operation in stplib is defined as a function object and a function template that constructs and returns the object. This allows the compiler to infer the types of the constructor arguments, rather than having them stated explicitly.

In the following sections, the function template signatures that construct each operation is listed. The 'sequence' refers to the input given to the transformation, which is passed to each operation. Any predicate can be either function objects or lambdas. To pass a regular function as an argument to an operation, use std::function from the &lt;functional&gt; header.

Boolean Reductions
---

**all**
```c++
template <typename Predicate>
detail::all_type<Predicate> all(const Predicate &pred)
```
*all* returns true if *pred* holds for all elements in the sequence. The parameters must satisfy the following requirements:
* The sequence must define *SequenceType::value_type*, *SequenceType.begin()* and *SequenceType.end()*. 
* The result of calling *pred* on an element of *SequenceType::value_type* must be implicitly convertible to bool.

```c++
std::vector<int> int_vec({1, 2, 3, 4});
auto all_obj = all([](const int &i){return i % 2 == 0;});
bool result = all_obj(int_vec); //result = false
```

**any**
```c++
template <typename Predicate>
detail::any_type<Predicate> any(const Predicate &pred)
```
*any* returns true if *pred* holds for any element in the sequence. The parameters must satisfy the following requirements:
* The sequence must define *SequenceType::value_type*, *SequenceType.begin()* and *SequenceType.end()*. 
* The result of calling *pred* on an element of *SequenceType::value_type* must be implicitly convertible to bool.

```c++
std::vector<int> int_vec({1, 2, 3, 4});
auto any_obj = any([](const int &i){return i % 2 == 0;});
bool result = any_obj(int_vec); //result = true
```

**contains**
```c++
template <typename ElementType>
detail::contains_type<ElementType> contains(const ElementType &val)
```
*contains* returns true if any element in the sequence is identical to *val*. The parameters must satisfy the following requirements:
* The sequence must define *SequenceType::value_type*, *SequenceType.begin()* and *SequenceType.end()*. 
* *ElementType* must be implicitly convertible to *SequenceType::value_type*.
* *SequenceType::value_type* must define operator ==.

```c++
std::vector<int> int_vec({1, 2, 3, 4});
auto contains_obj = contains(3);
bool result = contains_obj(int_vec); //result = true
```

**equal**
```c++
template <typename SequenceType>
detail::equal_type<SequenceType> equal(const SequenceType &sequence)
```
*equal* returns true if both sequences are identical. The parameters must satisfy the following requirements:
* Both sequences must define *SequenceType::value_type*, *SequenceType.begin()* and *SequenceType.end()*. 
* The *SequenceType::value_type* of both sequences must be identical.
* *SequenceType::value_type* must define operator ==.

```c++
std::vector<int> int_vec({1, 2, 3, 4});
auto equal_obj = equal(int_vec);
bool result = equal_obj(int_vec); //result = true
```

Filters
---

**drop**
```
detail::drop_type drop(const size_t &n)
```
*drop* removes the first *n* elements from the sequence. The sequence must define *iterator*, *begin()*, *end()* and *erase()*.

**drop_while**
```
template <typename Predicate>
detail::drop_while_type<Predicate> drop_while(const Predicate &pred)
```
*drop_while* removes elements from the sequence until *pred* returns true for an element. The sequence must define *iterator*, *value_type*, *begin()*, *end()* and *erase()*. The result of calling *pred* on an element of *value_type* must be implicitly convertible to bool.

**take**
```
detail::take_type take(const size_t &n)
```
*take* keeps the first *n* elements from the sequence, and removes the remaining elements. The sequence must define *iterator*, *begin()*, *end()* and *erase()*.

**take_while**
```
template <typename Predicate>
detail::take_while_type<Predicate> take_while(const Predicate &pred)
```
*take_while* keeps elements from the sequence until *pred* returns true for an element, it then removes the remaining elements. The sequence must define *iterator*, *value_type*, *begin()*, *end()* and *erase()*. The result of calling *pred* on an element of *value_type* must be implicitly convertible to bool.

**where**
```
template <typename Predicate>
detail::where_type<Predicate> where(const Predicate &pred)
```
*where* removes all elements from the sequence for which *pred* does not return true. The sequence must define *value_type*, *begin()* and *end()*. The result of calling *pred* on an element of *value_type* must be implicitly convertible to bool.

Miscellaneous
---

**map**
```
template <typename UnaryOperation>
detail::map_type<UnaryOperation> map(const UnaryOperation &unop)
```
*map* calls *unop* on each element of the sequence, and returns a new sequence composed of the resulting values. The sequence must define *value_type*, *begin()* and *end()*. The resulting sequence will be of type *std::vector&lt;OpType&gt;* where optype is the type returned by calling *unop* with *value_type* as parameter.

**to_list**
```
detail::to_list_type to_list()
```
*to_list* returns a std::list containing the elements of the sequence. The sequence must define *value_type*, *begin()* and *end()*. The resulting sequence will be of type *std::list&lt;value_type&gt;*.

**to_map**
```
detail::to_map_type to_map()
```
*to_map* returns a std::map containing the elements of the sequence. The sequence must define *value_type*, *begin()* and *end()*. *value_type* must define *first_type* and *second_type* (such as std::pair does), and the resulting sequence will be of type *std::map&lt;first_type, second_type&gt;*.

**to_vector**
```
detail::to_vector_type to_vector()
```
*to_vector* returns a std::vector containing the elements of the sequence. The sequence must define *value_type*, *begin()* and *end()*. The resulting sequence will be of type *std::vector&lt;value_type&gt;*.

**unique**
```
detail::unique_type unique()
```
*unique* removes all duplicate elements from the sequence. The sequence must define *value_type*, *begin()*, *end()* and *push_back()*. *value_type* must define operator ==.

**zip**
```
template <typename SequenceType>
detail::zip_type<SequenceType> zip(const SequenceType &sequence)
```
*zip* combines the two sequences into a single sequence of std::pair. Both sequences must define *value_type*, *begin()* and *end()*. The resulting sequence will be of type *std::vector&lt;std::pair&lt;value_type1, value_type2&gt;&gt;*. As *zip* uses std::pair, it is compatible with *to_map*.

Numerical Reductions
---

**avg**
```
detail::avg_type avg()
```
*avg* returns the average of the elements of the sequence as a double. The sequence must define *value_type*, *begin()* and *end()*. *value_type* must be default-constructible and must define operator +. If the sequence is empty, *avg* will throw *std::range_error*.

**count**
```
template <typename ElementType>
detail::count_type<ElementType> count(const ElementType &val)
```
*count* returns the number of occurences of an element identical to *val* in the sequence. The sequence must define *value_type*, *begin()*, *end()* and *iterator*. *ElementType* must be implicitly convertible to *value_type*.

**fold**
```
template
<
    typename BinaryOperation,
    typename InitType
>
detail::fold_type<BinaryOperation, InitType> fold(const BinaryOperation &binop, const InitType &init)
```
*fold* performs an accumulation of the elements of the sequence, using *binop* starting with *init* and the first element of the list. The sequence must define *value_type*, *begin()* and *end()*. The result of *binop(InitType, value_type)* must be implicitly convertible to *InitType*.

**max**
```
detail::max_type max()
```
*max* returns the maximum element of the sequence, as defined by operator &lt;. The sequence must define *value_type*, *begin()* and *end()*. *value_type* must define operator &lt;. If the sequence is empty, *max* will throw *std::range_error*.

**min**
```
detail::min_type min()
```
*min* returns the minimum element of the sequence, as defined by operator &lt;. The sequence must define *value_type*, *begin()* and *end()*. *value_type* must define operator &lt;. If the sequence is empty, *min* will throw *std::range_error*.

**size**
```
detail::size_type size()
```
*size* returns the amount of elements in the sequence. The sequence must define *iterator*, *begin()* and *end()*.

**sum**
```
detail::sum_type sum()
```
*sum* returns the sum of the elements in the sequence as defined by operator +. The sequence must define *value_type*, *begin()* and *end()*. *value_type* must be default-constructible.

Order Operations
---

**reverse**
```
detail::reverse_type reverse()
```
*reverse* reverses the order of elements in the sequence. The sequence must define *begin()* and *end()*.

**sort**
```
detail::sort_type sort()
```
*sort* sorts the sequence according to operator &lt;. The sequence must define *begin()* and *end()*.

**sort_with**
```
template <typename Predicate>
detail::sort_with_type<Predicate> sort_with(const Predicate &pred)
```
*sort_with* sorts the sequence according to *pred*. The sequence must define *value_type*, *begin()* and *end()*. The result of *pred(value_type, value_type)* must be implicitly convertible to bool.

Set Operations
---

**difference**
```
template <typename SequenceType>
detail::difference_type<SequenceType> difference(const SequenceType &sequence)
```
*difference* returns a sequence consisting of all elements that are present in the first sequence while not being present at the same position of the second sequence. Both sequences must define *value_type*, *push_back()*, *begin()* and *end()*. *value_type* must be the same for both sequences.

**intersect**
```
template <typename SequenceType>
detail::intersect_type<SequenceType> intersect(const SequenceType &sequence)
```
*intersect* returns a sequence consisting of all elements that are present in the first sequence and also present in the second sequence in the same position. Both sequences must define *value_type*, *push_back()*, *begin()* and *end()*. *value_type* must be the same for both sequences.

**join**
```
template <typename SequenceType>
detail::intersect_type<SequenceType> intersect(const SequenceType &sequence)
```
*join* returns the concatenation of the two sequences. Both sequences must define *value_type*, *reserve()*, *insert()*, *begin()* and *end()*. *value_type* must be the same for both sequences.

Generators
---

Generators should not be part of a transformation pipeline, but can be used to generate sequences as input for any of the operations.

**range**
```
template
<
    typename IntervalType,
    typename StepType,
    typename RangeType = std::vector<IntervalType>
>
RangeType range(const IntervalType &start, const IntervalType &end, const StepType &step)
```
*range* will generate a sequence of elements, ranging from *start* to *end*, with each element being *step* larger than the previous element. *step* can be omitted in which case it will be fixed to 1. Both *IntervalType* and *StepType* must be scalar types. The resulting sequence will be of type *std::vector&lt;IntervalType&gt;*.

**repeat**
```
template
<
    typename ValueType,
    typename RepeatType = typename std::vector<ValueType>
>
RepeatType repeat(const ValueType &val, size_t n)
```
*repeat* will generate a sequence of *n* elements identical to *val*. The resulting sequence will be of type *std::vector&lt;ValueType&gt;*.

