#include "huffman.h"

/**
 * symbol_frequency_compare - compares the frequencies
 * of two symbol nodes
 * @p1: a pointer to the first symbol node
 * @p2: a pointer to the second symbol node
 * Return: the difference between the frequencies of the symbols
 */
int symbol_frequency_compare(void *p1, void *p2)
{
	binary_tree_node_t *first_node, *second_node;
	symbol_t *first_symbol, *second_symbol;

	first_node = (binary_tree_node_t *)p1;
	second_node = (binary_tree_node_t *)p2;
	first_symbol = (symbol_t *)first_node->data;
	second_symbol = (symbol_t *)second_node->data;

	return (first_symbol->freq - second_symbol->freq);
}

/**
 * huffman_priority_queue - creates a min-heap priority queue
 * of symbols
 * @data: an array of characters
 * @freq: an array of frequencies associated with characters
 * @size: the size of the arrays
 * Return: a min-heapified version of the arrays
 */
heap_t *huffman_priority_queue(char *data, size_t *freq, size_t size)
{
	heap_t *min_heap;
	symbol_t *new_symbol;
	binary_tree_node_t *tree_node;
	size_t idx;

	min_heap = heap_create(symbol_frequency_compare);

	for (idx = 0; idx < size; idx++)
	{
		new_symbol = symbol_create(data[idx], freq[idx]);
		tree_node = binary_tree_node(NULL, new_symbol);
		tree_node = heap_insert(min_heap, tree_node);
	}
	return (min_heap);
}
