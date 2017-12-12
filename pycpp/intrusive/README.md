# Instrusive

**Table of Contents**

- [Introduction](#introduction)

## Introduction

Intrusive contains intrusive and semi-intrusive containers, where
the user manages the memory for the intrusive container. 

// TODO: document

## Intrusive Containers

These containers are fully intrusive, where the user manages all memory associated with each container. These containers are non-copyable and non-assignable, since changes to each value culminates in changes to the container layout.

- [ForwardList](/pycpp/intrusive/forward_list.h)
- [List](/pycpp/intrusive/list.h)
- [Set](/pycpp/intrusive/set.h)
- [MultiSet](/pycpp/intrusive/multiset.h)
- [UnorderedSet](/pycpp/intrusive/unordered_set.h)
- [UnorderedMultiSet](/pycpp/intrusive/unordered_multiset.h)

## Semi-Intrusive Containers

These containers are partially intrusive, where the user manages memory associated with the value of each container, but any internal overhead is managed by the container. These containers are movable, and assignable, since each value is merely stored by reference, and does not alter the underlying container layout.

- [Deque](/pycpp/intrusive/deque.h)
- [Vector](/pycpp/intrusive/vector.h)

// TODO: implement the rest as a function of...
// http://open-std.org/JTC1/SC22/WG21/docs/papers/2016/p0406r1.html#general
