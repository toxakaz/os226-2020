#pragma once

typedef binary_heap;

binary_heap* binary_heap_init(unsigned int start_depth, int (*compare) (const void*, const void*));
int binary_heap_len(binary_heap* heap);
void** binary_heap_get_array(binary_heap* heap);
void binary_heap_insert(binary_heap* heap, void* element);
void* binary_heap_view_top(binary_heap* heap);
void* binary_heap_extract(binary_heap* heap);
void binary_heap_free(binary_heap* heap);