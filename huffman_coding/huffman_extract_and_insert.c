#include "huffman.h"

/* Internal helper function prototypes */
static binary_tree_node_t *create_parent_node(binary_tree_node_t *left_child,
					       binary_tree_node_t *right_child);
static symbol_t *merge_symbol_frequencies(symbol_t *first_symbol,
					  symbol_t *second_symbol);

/**
 * huffman_extract_and_insert - Combines two minimum frequency nodes into parent
 * @priority_queue: Pointer to the min-heap managing Huffman tree nodes
 *
 * Description: Extracts the two nodes with lowest frequencies from the queue,
 *              creates a new parent node with combined frequency, and inserts
 *              it back into the priority queue. This process continues until
 *              only one root node remains in the Huffman tree construction.
 * Return: 1 on successful operation, 0 on failure
 */
int huffman_extract_and_insert(heap_t *priority_queue)
{
	void *first_node_ptr, *second_node_ptr;
	binary_tree_node_t *left_node, *right_node, *combined_node;

	if (!priority_queue)
		return (0);

	/* Extract two nodes with minimum frequencies */
	first_node_ptr = heap_extract(priority_queue);
	second_node_ptr = heap_extract(priority_queue);

	/* Validate extraction success */
	if (!first_node_ptr || !second_node_ptr)
		return (0);

	/* Cast extracted pointers to tree nodes */
	left_node = (binary_tree_node_t *)first_node_ptr;
	right_node = (binary_tree_node_t *)second_node_ptr;

	/* Create new parent node combining both children */
	combined_node = create_parent_node(left_node, right_node);
	if (!combined_node)
		return (0);

	/* Insert combined node back into priority queue */
	if (!heap_insert(priority_queue, combined_node))
	{
		/* Cleanup on insertion failure */
		if (combined_node->data)
			free(combined_node->data);
		free(combined_node);
		return (0);
	}

	return (1);
}

/**
 * create_parent_node - Constructs new internal node from two child nodes
 * @left_child: First child node (typically lower frequency)
 * @right_child: Second child node (typically higher frequency)
 *
 * Description: Creates a new binary tree node with combined frequency symbol
 *              and establishes parent-child relationships
 * Return: Pointer to new parent node, NULL on failure
 */
static binary_tree_node_t *create_parent_node(binary_tree_node_t *left_child,
					       binary_tree_node_t *right_child)
{
	binary_tree_node_t *parent_node;
	symbol_t *left_symbol, *right_symbol, *parent_symbol;

	if (!left_child || !right_child)
		return (NULL);

	/* Extract symbol data from child nodes */
	left_symbol = (symbol_t *)left_child->data;
	right_symbol = (symbol_t *)right_child->data;

	/* Create combined symbol with merged frequencies */
	parent_symbol = merge_symbol_frequencies(left_symbol, right_symbol);
	if (!parent_symbol)
		return (NULL);

	/* Allocate new parent node */
	parent_node = binary_tree_node(NULL, parent_symbol);
	if (!parent_node)
	{
		free(parent_symbol);
		return (NULL);
	}

	/* Establish tree relationships */
	parent_node->left = left_child;
	parent_node->right = right_child;
	left_child->parent = parent_node;
	right_child->parent = parent_node;

	return (parent_node);
}

/**
 * merge_symbol_frequencies - Creates symbol with combined frequency values
 * @first_symbol: First symbol contributing to frequency sum
 * @second_symbol: Second symbol contributing to frequency sum
 *
 * Description: Generates a new symbol structure with frequency equal to
 *              the sum of both input symbols, using sentinel character
 * Return: Pointer to new merged symbol, NULL on failure
 */
static symbol_t *merge_symbol_frequencies(symbol_t *first_symbol,
					  symbol_t *second_symbol)
{
	symbol_t *merged_symbol;
	size_t total_frequency;

	if (!first_symbol || !second_symbol)
		return (NULL);

	/* Calculate combined frequency */
	total_frequency = first_symbol->freq + second_symbol->freq;

	/* Create new symbol with sentinel character and combined frequency */
	merged_symbol = symbol_create(-1, total_frequency);

	return (merged_symbol);
}
