#include <meta.hpp>

static_assert(!contains_duplicates<int, char, double>::value);
static_assert(contains_duplicates<int, char, double, int>::value);
static_assert(!contains_duplicates<int>::value);