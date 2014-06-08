stplib
===

Each operation in stplib is defined as a function object and a function template that constructs and returns the object. This allows the compiler to infer the types of the constructor arguments, rather than having them stated explicitly.

The following sections will list the function template signatures that construct each operation, as well as the requirements for each parameter and a small example. The 'sequence' refers to the input given to the transformation, which is passed to each operation. Any predicate can be either function objects or lambdas. To pass a regular function as an argument to an operation, use std::function from the &lt;functional&gt; header.

Boolean Reductions
---

**all**
```c++
template <typename Predicate>
detail::all_type<Predicate> all(const Predicate &pred)
```
*all* returns true if *pred* holds for all elements in the sequence. The parameters must satisfy the following requirements:
* The sequence must define *SequenceType::value_type*, *SequenceType.begin()* and *SequenceType.end()*.
* The result of calling *pred* on an element of type *SequenceType::value_type* must be implicitly convertible to bool.

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
* The result of calling *pred* on an element of type *SequenceType::value_type* must be implicitly convertible to bool.

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
* *SequenceType::value_type* must define the equality operator.

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
* *SequenceType::value_type* must define the equality operator.

```c++
std::vector<int> int_vec({1, 2, 3, 4});
auto equal_obj = equal(int_vec);
bool result = equal_obj(int_vec); //result = true
```

Filters
---

**drop**
```c++
detail::drop_type drop(const size_t &n)
```
*drop* removes the first *n* elements from the sequence. The parameters must satisfy the following requirements:
* The sequence must define *SequenceType::iterator*, *SequenceType.begin()*, *SequenceType.end()* and *SequenceType.erase()*.

```c++
std::vector<int> int_vec({1, 2, 3, 4});
auto drop_obj = drop(2);
bool result = drop_obj(int_vec); //result = {3, 4}
```

**drop_while**
```c++
template <typename Predicate>
detail::drop_while_type<Predicate> drop_while(const Predicate &pred)
```
*drop_while* removes elements from the sequence until *pred* returns true for an element. The parameters must satisfy the following requirements:
* The sequence must define *SequenceType::value_typ*, *SequenceType::iterator*, *SequenceType.begin()*, *SequenceType.end()* and *SequenceType.erase()*.
* The result of calling *pred* on an element of type *SequenceType::value_type* must be implicitly convertible to bool.

```c++
std::vector<int> int_vec({1, 2, 3, 4});
auto drop_while_obj = drop([](const int &i){return i < 3;});
bool result = drop_while_obj(int_vec); //result = {3, 4}
```

**take**
```c++
detail::take_type take(const size_t &n)
```
*take* keeps the first *n* elements from the sequence, and removes the remaining elements. The parameters must satisfy the following requirements:
* The sequence must define *SequenceType::iterator*, *SequenceType.begin()*, *SequenceType.end()* and *SequenceType.erase()*.

```c++
std::vector<int> int_vec({1, 2, 3, 4});
auto take_obj = take(2);
bool result = take_obj(int_vec); //result = {1, 2}
```

**take_while**
```c++
template <typename Predicate>
detail::take_while_type<Predicate> take_while(const Predicate &pred)
```
*take_while* keeps elements from the sequence until *pred* returns true for an element, it then removes the remaining elements. The parameters must satisfy the following requirements:
* The sequence must define *SequenceType::value_type*, *SequenceType::iterator*, *SequenceType.begin()*, *SequenceType.end()* and *SequenceType.erase()*.
* The result of calling *pred* on an element of type *SequenceType::value_type* must be implicitly convertible to bool.

```c++
std::vector<int> int_vec({1, 2, 3, 4});
auto take_while_obj = take_while([](const int &i){return i < 3;});
bool result = take_while_obj(int_vec); //result = {1, 2}
```

**where**
```c++
template <typename Predicate>
detail::where_type<Predicate> where(const Predicate &pred)
```
*where* removes all elements from the sequence for which *pred* does not return true. The parameters must satisfy the following requirements:
* The sequence must define *SequenceType::value_type*, *SequenceType.begin()* and *SequenceType.end()*.
* The result of calling *pred* on an element of type *SequenceType::value_type* must be implicitly convertible to bool.

```c++
std::vector<int> int_vec({1, 2, 3, 4});
auto where_obj = where([](const int &i){return i % 2 == 0;});
bool result = where_obj(int_vec); //result = {2, 4}
```

Miscellaneous
---

**map**
```c++
template <typename UnaryOperation>
detail::map_type<UnaryOperation> map(const UnaryOperation &unop)
```
*map* calls *unop* on each element of the sequence, and returns a new sequence composed of the resulting values. The parameters must satisfy the following requirements:
* The sequence must define *SequenceType::value_type*, *SequenceType.begin()* and *SequenceType.end()*.

The resulting sequence will be of type *std::vector&lt;OpType&gt;* where *OpType* is the type returned by calling *unop* with type *SequenceType::value_type* as the parameter.

```c++
std::vector<int> int_vec({1, 2, 3});
auto map_obj = map([](const int &i){return std::string(" ", i);});
std::vector<std::string> result = map_obj(int_vec); //result = {" ", "  ", "   "}
```

**to_list**
```c++
detail::to_list_type to_list()
```
*to_list* returns a *std::list* containing the elements of the sequence. The parameters must satisfy the following requirements:
* The sequence must define *SequenceType::value_type*, *SequenceType.begin()* and *SequenceType.end()*.

The resulting sequence will be of type *std::list&lt;SequenceType::value_type&gt;*.

```c++
std::vector<int> int_vec({1, 2, 3, 4});
auto to_list_obj = to_list();
std::list<int> result = to_list_obj(int_vec); //result = {1, 2, 3, 4}
```

**to_map**
```c++
detail::to_map_type to_map()
```
*to_map* returns a *std::map* containing the elements of the sequence. The parameters must satisfy the following requirements:
* The sequence must define *SequenceType::value_type*, *SequenceType.begin()* and *SequenceType.end()*.
* *SequenceType::value_type* must define *value_type::first_type* and *value_type::second_type* (such as *std::pair* does).

The resulting sequence will be of type *std::map&lt;SequenceType::value_type::first_type, SequenceType::value_type::second_type&gt;*.

```c++
std::vector<std::pair<std::string, int>> pair_vec({
    std::pair<std::string, int>("one", 1),
    std::pair<std::string, int>("two", 2),
    std::pair<std::string, int>("three", 3)});

auto to_map_obj = to_map();
std::map<std::string, int> result = to_map_obj(pair_vec);
//result = {"one" : 1, "two" : 2, "three" : 3}
```

**to_vector**
```c++
detail::to_vector_type to_vector()
```
*to_vector* returns a *std::vector* containing the elements of the sequence. The parameters must satisfy the following requirements:
* The sequence must define *SequenceType::value_type*, *SequenceType.begin()*, *SequenceType.end()* and *SequenceType.push_back()*.

The resulting sequence will be of type *std::vector&lt;SequenceType::value_type&gt;*.

```c++
std::list<int> int_list({1, 2, 3, 4});
auto to_vector_obj = to_vector();
std::vector<int> result = to_vector_obj(int_list); //result = {1, 2, 3, 4}
```

**unique**
```c++
detail::unique_type unique()
```
*unique* removes all duplicate elements from the sequence. The parameters must satisfy the following requirements:
* The sequence must define *SequenceType::value_type*, *SequenceType.begin()*, *SequenceType.end()* and *SequenceType.push_back()*.
* *SequenceType::value_type* must define the equality operator.

```c++
std::vector<int> int_vec({1, 2, 2, 1});
auto unique_obj = unique();
std::vector<int> result = unique_obj(int_vec); //result = {1, 2}
```

**zip**
```c++
template <typename SequenceType>
detail::zip_type<SequenceType> zip(const SequenceType &sequence)
```
*zip* combines the two sequences into a single sequence of type *std::pair*. The parameters must satisfy the following requirements:
* Both sequences must define *SequenceType::value_type*, *SequenceType.begin()* and *SequenceType.end()*.

The resulting sequence will be of type *std::vector&lt;std::pair&lt;value_type1, value_type2&gt;&gt;*. As *zip* uses *std::pair*, it is compatible with *to_map*.

```c++
std::vector<int> int_vec({1, 2, 3, 4});
std::vector<std::string> string_vec({"one", "two", "three", "four"});

auto zip_obj = zip(int_vec);
std::vector<std::pair<>> result = zip_obj(string_vec);
//result = {"one" : 1, "two" : 2, "three" : 3, "four" : 4}
```

Numerical Reductions
---

**avg**
```c++
detail::avg_type avg()
```
*avg* returns the average of the elements of the sequence as a double. The parameters must satisfy the following requirements:
* The sequence must define *SequenceType::value_type*, *SequenceType.begin()* and *SequenceType.end()*.
* *SequenceType::value_type* must be default-constructible.
* *SequenceType::value_type* must define the addition operator.

If the sequence is empty, *avg* will throw *std::range_error*.

```c++
std::vector<int> int_vec({1, 2, 3, 4});
auto avg_obj = avg();
double result = avg_obj(int_vec); //result = 2.5
```

**count**
```c++
template <typename ElementType>
detail::count_type<ElementType> count(const ElementType &val)
```
*count* returns the number of occurences of an element identical to *val* in the sequence. The parameters must satisfy the following requirements:
* The sequence must define *SequenceType::value_type*, *SequenceType.begin()*, *SequenceType.end()* and *SequenceType::iterator*.
* *ElementType* must be implicitly convertible to *SequenceType::value_type*.

```c++
std::vector<int> int_vec({1, 2, 2, 4});
auto count_obj = count(2);
unsigned int result = count_obj(int_vec); //result = 2
```

**fold**
```c++
template
<
    typename BinaryOperation,
    typename InitType
>
detail::fold_type<BinaryOperation, InitType> fold(const BinaryOperation &binop, const InitType &init)
```
*fold* performs an accumulation of the elements of the sequence, using *binop* starting with *init* and the first element of the list. The parameters must satisfy the following requirements:
* The sequence must define *SequenceType::value_type*, *SequenceType.begin()* and *SequenceType.end()*.
* The result of *binop(InitType, SequenceType::value_type)* must be implicitly convertible to *InitType*.

```c++
std::vector<int> int_vec({1, 2, 3, 4});
auto fold_obj = fold(std::plus<int>(), 5);
int result = fold_obj(int_vec); //result = 15
```

**max**
```c++
detail::max_type max()
```
*max* returns the maximum element of the sequence, as defined by the less-than operator. The parameters must satisfy the following requirements:
* The sequence must define *SequenceType::value_type*, *SequenceType.begin()* and *SequenceType.end()*.
* *SequenceType::value_type* must define the less-than operator.

If the sequence is empty, *max* will throw *std::range_error*.

```c++
std::vector<int> int_vec({1, 2, 3, 4});
auto max_obj = max();
int result = max_obj(int_vec); //result = 4
```

**min**
```c++
detail::min_type min()
```
*min* returns the minimum element of the sequence, as defined by the less-than operator. The parameters must satisfy the following requirements:
* The sequence must define *SequenceType::value_type*, *SequenceType.begin()* and *SequenceType.end()*.
* *SequenceType::value_type* must define the less-than operator.

If the sequence is empty, *min* will throw *std::range_error*.

```c++
std::vector<int> int_vec({1, 2, 3, 4});
auto min_obj = min();
int result = min_obj(int_vec); //result = 1
```

**size**
```c++
detail::size_type size()
```
*size* returns the amount of elements in the sequence. The parameters must satisfy the following requirements:
* The sequence must define *SequenceType::iterator*, *SequenceType.begin()* and *SequenceType.end()*.

```c++
std::vector<int> int_vec({1, 2, 3, 4});
auto size_obj = size();
int result = size_obj(int_vec); //result = 4
```

**sum**
```c++
detail::sum_type sum()
```
*sum* returns the sum of the elements in the sequence as defined by the addition operator. The parameters must satisfy the following requirements:
* The sequence must define *SequenceType::value_type*, *SequenceType.begin()* and *SequenceType.end()*.
* *SequenceType::value_type* must be default-constructible.

```c++
std::vector<int> int_vec({1, 2, 3, 4});
auto sum_obj = sum();
int result = sum_obj(int_vec); //result = 10
```

Order Operations
---

**reverse**
```c++
detail::reverse_type reverse()
```
*reverse* reverses the order of elements in the sequence. The parameters must satisfy the following requirements:
* The sequence must define *SequenceType.begin()* and *SequenceType.end()*.

```c++
std::vector<int> int_vec({1, 2, 3, 4});
auto reverse_obj = reverse();
std::vector<int> result = reverse_obj(int_vec); //result = {4, 3, 2, 1}
```

**sort**
```c++
detail::sort_type sort()
```
*sort* sorts the sequence according to the less-than operator. The parameters must satisfy the following requirements:
* The sequence must define *SequenceType.begin()* and *SequenceType.end()*.

```c++
std::vector<int> int_vec({4, 2, 1, 3});
auto sort_obj = sort();
std::vector<int> result = sort_obj(int_vec); //result = {1, 2, 3, 4}
```

**sort_with**
```c++
template <typename Predicate>
detail::sort_with_type<Predicate> sort_with(const Predicate &pred)
```
*sort_with* sorts the sequence according to *pred*. The parameters must satisfy the following requirements:
* The sequence must define *SequenceType::value_type*, *SequenceType.begin()* and *SequenceType.end()*.
* The result of *pred(SequenceType::value_type, SequenceType::value_type)* must be implicitly convertible to bool.

```c++
std::vector<int> int_vec({4, 2, 1, 3});
auto sort_with_obj = sort_with([](const int &i, const int &j){return i < j;});
std::vector<int> result = sort_with_obj(int_vec); //result = {1, 2, 3, 4}
```

Set Operations
---

**difference**
```c++
template <typename SequenceType>
detail::difference_type<SequenceType> difference(const SequenceType &sequence)
```

*difference* returns a sequence consisting of all elements that are present in the first sequence while not being present at the same position of the second sequence. The parameters must satisfy the following requirements:
* Both sequences must define *SequenceType::value_type*, *SequenceType.push_back()*, *SequenceType.begin()* and *SequenceType.end()*.
* *SequenceType::value_type* must be the same for both sequences.

```c++
std::vector<int> int_vec({1, 2, 3, 4});
std::vector<int> other_int_vec({0, 2, 3, 5});
auto difference_obj = difference(other_int_vec);
std::vector<int> result = difference_obj(int_vec); //result = {0, 5}
```

**intersect**
```c++
template <typename SequenceType>
detail::intersect_type<SequenceType> intersect(const SequenceType &sequence)
```
*intersect* returns a sequence consisting of all elements that are present in the first sequence and also present in the second sequence in the same position. The parameters must satisfy the following requirements:
* Both sequences must define *SequenceType::value_type*, *SequenceType.push_back()*, *SequenceType.begin()* and *SequenceType.end()*.
* *SequenceType::value_type* must be the same for both sequences.

```c++
std::vector<int> int_vec({1, 2, 3, 4});
std::vector<int> other_int_vec({0, 2, 3, 5});
auto intersect_obj = intersect(other_int_vec);
std::vector<int> result = intersect_obj(int_vec); //result = {2, 3}
```

**join**
```c++
template <typename SequenceType>
detail::intersect_type<SequenceType> intersect(const SequenceType &sequence)
```
*join* returns the concatenation of the two sequences. The parameters must satisfy the following requirements:
* Both sequences must define *SequenceType::value_type*, *SequenceType.reserve()*, *SequenceType.insert()*, *SequenceType.begin()* and *SequenceType.end()*.
* *SequenceType::value_type* must be the same for both sequences.

```c++
std::vector<int> int_vec({1, 2, 3, 4});
std::vector<int> other_int_vec({4, 4});
auto join_obj = join(other_int_vec);
std::vector<int> result = join_obj(int_vec); //result = {1, 2, 3, 4, 4, 4}
```

Generators
---

Generators should not be part of a transformation pipeline, but can be used to generate sequences as input for any of the operations.

**range**
```c++
template
<
    typename IntervalType,
    typename StepType,
    typename RangeType = std::vector<IntervalType>
>
RangeType range(const IntervalType &start, const IntervalType &end, const StepType &step)
```
*range* will generate a sequence of elements, ranging from *start* to *end*, with each element being *step* larger than the previous element. *step* can be omitted in which case it will be fixed to 1. The parameters must satisfy the following requirements:
* Both *IntervalType* and *StepType* must be scalar types.

The resulting sequence will be of type *std::vector&lt;IntervalType&gt;*.

```c++
std::vector<int> result = range(1, 10, 3); //result = {1, 4, 7}
```

**repeat**
```c++
template
<
    typename ValueType,
    typename RepeatType = typename std::vector<ValueType>
>
RepeatType repeat(const ValueType &val, size_t n)
```
*repeat* will generate a sequence of *n* elements identical to *val*.

The resulting sequence will be of type *std::vector&lt;ValueType&gt;*.

```c++
std::vector<int> result = repeat(1, 3); //result = {1, 1, 1}
```

