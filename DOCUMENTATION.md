stplib
===

Each operation in stplib is defined as a function object and a function template that constructs and returns the object. This allows the compiler to infer the types of the constructor arguments, rather than having them stated explicitly.

Boolean Reductions
---

**all**
```c++
template <typename Predicate>
detail::all_type<Predicate> all(const Predicate &pred)
```
*all* returns true if *pred* holds for all elements in the sequence. The sequence must define *value_type*, *begin()* and *end()*. The result of calling *pred* on an element of *value_type* must be implicitly convertible to bool.

**any**
```c++
template <typename Predicate>
detail::any_type<Predicate> any(const Predicate &pred)
```
*any* returns true if *pred* holds for any element in the sequence. The sequence must define *value_type*, *begin()* and *end()*. The result of calling *pred* on an element of *value_type* must be implicitly convertible to bool.

**contains**
```c++
template <typename ElementType>
detail::contains_type<ElementType> contains(const ElementType &val)
```
*contains* returns true if any element in the sequence is identical to *val*. The sequence must define *value_type*, *begin()* and *end()*. *ElementType* must be implicitly convertible to *value_type*. *value_type* must define operator==.

**equal**
```c++
template <typename SequenceType>
detail::equal_type<SequenceType> equal(const SequenceType &sequence)
```
*equal* returns true if both sequences are identical. Both sequences must define *value_type*, *begin()* and *end()*. The *value_type* of both sequences must be identical. *value_type* must define operator==.

Filters
---

Generators
---

Miscellaneous
---

Numerical Reductions
---

Order Operations
---

Set Operations
---

Transformations
---
