#ifndef __STRUC_SET_H__
#define __STRUC_SET_H__

#include "set.h"
#include <stddef.h>
#include <stdlib.h>

struct set {
  void **s;
  size_t capacity;
  size_t size;
  int (*cmp)(const void *, const void *);
  void *(*copy)(const void *);
  void (*del)(void *);
};

/** Returns the first position p in s such that begin<=p<=end and s[p]>=c
if any, or end otherwise
We assume that s is sorted and that all indices in begin..end are valid in s*
*/
int find(const struct set *s, void *c, int begin, int end);

/**
Translates elements in s[begin..end] to s[begin+1..end+1] assuming all
the indices are valid. The value in s[begin] is left unchanged.
*/
void shift_right(void *s[], size_t begin, size_t end);

/** translates elements in s[begin..end] to [begin-1..end-1] assuming all
the indices are valid. The value in s[end] is left unchanged
*/
void shift_left(void *s[], size_t begin, size_t end);

#endif //__set_H__
