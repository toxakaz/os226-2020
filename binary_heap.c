#include<stdio.h>
#include<stdlib.h>

typedef struct binary_heap
{
	void** element;
	int size;
	int len;
	int (*compare) (const void*, const void*);

} binary_heap;

unsigned required_size(unsigned x) {
	x = x − 1;
	x = x | (x >> 1);
	x = x | (x >> 2);
	x = x | (x >> 4);
	x = x | (x >> 8);
	x = x | (x >> 16);
	return x;
}

binary_heap* binary_heap_init(unsigned int start_size, int (*compare) (const void*, const void*))
{
	binary_heap* new_heap = (binary_heap*)malloc(sizeof(binary_heap));
	new_heap->size = required_size(start_size);
	new_heap->element = (void**)malloc(sizeof(void*) * new_heap->size);
	new_heap->len = 0;
	new_heap->compare = compare;
	return new_heap;
}

int binary_heap_len(binary_heap* heap)
{
	return heap->len;
}

void binary_heap_heapify_up(binary_heap* heap, int changed_ind)
{
	if (changed_ind > heap->len || changed_ind <= 0)
		return;

	int futher_ind = ((changed_ind + 1) >> 1) - 1;

	if (heap->compare(heap->element[futher_ind], heap->element[changed_ind]) < 0)
	{
		void* hub = heap->element[changed_ind];
		heap->element[changed_ind] = heap->element[futher_ind];
		heap->element[futher_ind] = hub;
		binary_heap_heapify_up(heap, futher_ind);
	}

	return;
}

void binary_heap_heapify_down(binary_heap* heap, int futher_ind)
{
	if (futher_ind < 0)
		return;

	int f_son = futher_ind * 2 + 1;

	if (f_son > heap->len)
		return;

	int eldest_son;
	if (f_son == heap->len)
		eldest_son = f_son;
	else if (heap->compare(heap->element[f_son], heap->element[f_son + 1]) > 0)
		eldest_son = f_son;
	else
		eldest_son = f_son + 1;

	if (heap->compare(heap->element[futher_ind], heap->element[eldest_son]) < 0)
	{
		void* hub = heap->element[futher_ind];
		heap->element[futher_ind] = heap->element[eldest_son];
		heap->element[eldest_son] = hub;
		binary_heap_heapify_down(heap, eldest_son);
	}

	return;
}

void** binary_heap_get_array(binary_heap* heap)
{
	return heap->element;
}

void binary_heap_insert(binary_heap* heap, void* element)
{
	if (heap->len == heap->size)
	{
		int new_size = (heap->size << 1) + 1;
		void** new_element = (void**)realloc(heap->element, sizeof(void*) * new_size);
		if (new_element == NULL)
		{
			//some code for error
			return;
		}
		heap->size = new_size;
		heap->element = new_element;
	}
	heap->element[heap->len] = element;
	binary_heap_heapify_up(heap, heap->len);
	heap->len = heap->len + 1;
	return;
}

void* binary_heap_view_top(binary_heap* heap)
{
	if (heap->len > 0)
		return heap->element[0];
	else
		return NULL;
}

void* binary_heap_extract(binary_heap* heap)
{
	if (!heap->len)
		return NULL;

	void* out_element = heap->element[0];
	heap->element[0] = heap->element[heap->len - 1];
	heap->len = heap->len - 1;
	binary_heap_heapify_down(heap, 0);

	return out_element;
}

void binary_heap_free(binary_heap* heap)
{
	free(heap->element);
	free(heap);
}