#include <stdlib.h>
#include <string.h>

#include "location.h"
#include "plist.h"

struct plist
{
  size_t capacity;
  size_t size;
  location *items;
};

#define PLIST_LEFT_CHILD(i) 2*(i) + 1
#define PLIST_RIGHT_CHILD(i) 2*(i) + 2

plist *plist_create(size_t cap)
{
  plist *l = malloc(sizeof(plist));
  if (l != NULL)
    {
      l->items = malloc(sizeof(location) * cap);
      l->size = 0;
      l->capacity = cap;
    }
  return l;
}

void plist_destroy(plist *l)
{
  if (l == NULL)
    {
      return;
    }
  
  free(l->items);
  l->size = 0;
  l->items = NULL;
  l->capacity = 0;
  free(l);
}

size_t plist_size(const plist *l)
{
  if (l == NULL)
    {
      return 0;
    }

  return l->size;
}

bool plist_add_end(plist *l, const location *p)
{
  if (l == NULL || p == NULL)
    {
      return false;
    }

  if (l->capacity > l->size)
    {
      l->items[l->size] = *p;
      l->size++;
      return true;
    }
  else
    {
      return false;
    }
}

void plist_get(const plist *l, size_t i, location *p)
{
  if (l == NULL || p == NULL)
    {
      return;
    }

  if (i < l->size && i >= 0)
    {
      *p = l->items[i];
    }
}

// Helper functions for plist_sort

/**
 * Rearranges the points on the given list into a max-heap according to
 * the given comparison function.
 *
 * @param l a pointer to a list, non-NULL
 * @param compare a comparison function for locations
 */
void plist_build_heap(plist *l, int (*compare)(const location *, const location *));

/**
 * Restores the max-heap property in the given array according to the
 * given comparison function.
 *
 * @param l a pointer to a list that obeys the max-heap order property
 * up to but no including index n at all indices except one, non-NULL
 * @param i an index into the list that is less than n and  where a value
 * possibly violates the max-heap order property with its children
 * @param n an index into the list such that all values at lesser indices
 * other than i obey the max-heap property
 * @param a location comparison function
 */
void plist_reheap_down(plist *l, size_t i, size_t n, int (*compare)(const location *, const location *));

/**
 * Swaps the points at the given indices in the given list.
 * @param l a pointer to a list, non-NULL
 * @param i a valid index into l
 * @param j a valid index into l
 */
void plist_swap(plist *l, size_t i, size_t j);

void plist_sort(plist *l, int (*compare)(const location *, const location *))
{
  if (l == NULL || compare == NULL)
    {
      return;
    }

  plist_build_heap(l, compare);
  
  for (size_t n = l->size; n > 1; n--)
    {
      plist_swap(l, 0, n - 1);
      plist_reheap_down(l, 0, n - 1, compare);
    }
}

void plist_build_heap(plist *l, int (*compare)(const location *, const location *))
{
  for (size_t i = l->size / 2 + 1; i >= 1; i--)
    {
      plist_reheap_down(l, i - 1, l->size, compare);
    }
}

void plist_reheap_down(plist *l, size_t i, size_t n, int (*compare)(const location *, const location *))
{
  size_t left;
  while ((left = PLIST_LEFT_CHILD(i)) < n)
    {
      size_t right = left + 1;
      size_t largest_child = left;
      if (right < n && compare(&l->items[right], &l->items[left]) > 0)
	{
	  largest_child = right;
	}
      if (compare(&l->items[largest_child], &l->items[i]) > 0)
	{
	  plist_swap(l, i, largest_child);
	  i = largest_child;
	}
      else
	{
	  i = n;
	}
    }
}

void plist_swap(plist *l, size_t i, size_t j)
{
  if (i != j)
    {
      location temp = l->items[i];
      l->items[i] = l->items[j];
      l->items[j] = temp;
    }
}
