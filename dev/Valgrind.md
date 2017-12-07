# Valgrind

Valgrind is an excellent tool to detect and fix memory leaks, as well as additional functionality. However, it has a few limitations in that it cannot determine whether a memory leak with pointers still reachable at program shutdown are desired or undesirable for the application developer. In two of our dependent libraries, RE2 and libxml2, both use globally-allocated variables to avoid reinitializing state. For libxml2, calling `xmlCleanupParser()` to avoid these warnings, even during atexit handlers, can have disastrous consequences on other applications use libxml2. We therefore document known memory leaks that **should** not be fixed by any application developer.

## RE2

Using x86_64 GCC on Linux, we get `still reachable: 128 bytes in 3 blocks` for the RE2 tests, along with `Mismatched free() / delete / delete []` for a `std::unique_ptr<match_impl>` initialized with new and using the default deleter. In addition, we acquire numerous `Conditional jump or move depends on uninitialised value(s)` due to internal logic in RE2. None of these are errors.

## libxml2

Using x86_64 GCC on Linux, we get `still reachable: 1,162 bytes in 48 blocks` for the libxml2 tests, due to the creation of libxml2 contexts that are not freed until `xmlCleanupParser()`.

## Total

All in all, Valgrind of the test suite should produce the expected `still reachable: 1,290 bytes in 51 blocks` on x86_64 Linux using GCC, along with numerous warnings in RE2 and libxml2.
