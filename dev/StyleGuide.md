# Style Guide

PyCPP is a relatively large project, and therefore should try to maintain a consistent style internally.

**Table of Contents**

- [General Recommendations](#general-recommendations)

## General Recommendations

- Prefer `using x = y;` to `typedef y x;`, since the more important factor (the alias) is on the left size of the expression.
