#include "huffman.h"

/**
 * huffman_codes - generates Huffman codes for characters based on
 * their frequencies and prints them
 * @data: an array of characters
 * @freq: an array of frequencies associated with characters
 * @size: the size of the arrays
 * Return: 1 on success, 0 on failure
 */
int huffman_codes(char *data, size_t *freq, size_t size)
{
	binary_tree_node_t *tree_root;
	char *code_string;
	size_t max_depth, i;

	tree_root = huffman_tree(data, freq, size);
	if (tree_root == NULL)
		return (0);

	max_depth = find_tree_depth(tree_root, 0);
	code_string = malloc(sizeof(char) * (max_depth + 1));

	for (i = 0; i < max_depth + 1; i++)
		code_string[i] = '\0';

	generate_codes_recursive(tree_root, code_string, 0);

	free(code_string);
	cleanup_huffman_tree(tree_root);

	return (1);
}

/**
 * cleanup_huffman_tree - recursively frees the memory allocated
 * for the Huffman tree
 * @root: the root node of the Huffman tree
 * Return: nothing (void)
 */
void cleanup_huffman_tree(binary_tree_node_t *root)
{
	if (root->left)
		cleanup_huffman_tree(root->left);

	if (root->right)
		cleanup_huffman_tree(root->right);

	free(root->data);
	free(root);
}

/**
 * find_tree_depth - calculates the depth of a binary tree
 * @root: the root node of the binary tree
 * @depth: the current depth of the node
 * Return: the depth of the tree
 */
size_t find_tree_depth(binary_tree_node_t *root, size_t depth)
{
	int left_depth, right_depth;

	left_depth = right_depth = -1;

	if (root->left)
		left_depth = find_tree_depth(root->left, depth + 1);
	if (root->right)
		right_depth = find_tree_depth(root->right, depth + 1);
	else
		return (depth);

	if (left_depth > right_depth)
		return (left_depth);

	return (right_depth);
}

/**
 * generate_codes_recursive - recursively generates
 * and prints Huffman codes for characters
 * @root: the current node in the Huffman tree
 * @code: the current code generated during traversal
 * @depth: the current depth of the node
 * Return: nothing (void)
 */
void generate_codes_recursive(binary_tree_node_t *root, char *code, size_t depth)
{
	symbol_t *symbol = (symbol_t *)root->data;

	if (root->left)
	{
		code[depth] = '0';
		generate_codes_recursive(root->left, code, depth + 1);
	}
	if (root->right)
	{
		code[depth] = '1';
		generate_codes_recursive(root->right, code, depth + 1);
	}
	else
	{
		printf("%c: %s\n", symbol->data, code);
	}
}