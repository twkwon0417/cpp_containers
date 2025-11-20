#ifndef TEST_NAMESPACE_H
#define TEST_NAMESPACE_H

// ============================================================================
//  Common header to automatically switch between STD and FT implementations
// ============================================================================

#include <algorithm>
#include <functional>
#include <iterator>
#include <map>
#include <memory>
#include <stack>
#include <type_traits>
#include <utility>
#include <vector>

#ifdef STD_MODE
// -----------------------------
// Use standard library headers
// -----------------------------
namespace ft = std;
#else
    // -----------------------------
    // Use custom ft headers
    // -----------------------------
    // TODO: include your implemented header file
    #include "enable_if.h"
    #include "equal.h"
    #include "is_integral.h"
    #include "iterator_traits.h"
    #include "lexicographical_compare.h"
    #include "map.h"
    #include "pair.h"
    #include "reverse_iterator.h"
    #include "stack.h"
    #include "vector.h"
#endif

#endif // TEST_NAMESPACE_H
