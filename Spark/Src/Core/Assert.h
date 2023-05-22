#pragma once

#include <cassert>

#if 1
#define SP_CORE_ASSERT(...)  assert(__VA_ARGS__)
#else
#define SP_CORE_ASSERT(...)
#endif
