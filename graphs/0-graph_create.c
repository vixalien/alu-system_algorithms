#include "graphs.h"

/**
 * graph_create - allocates memory to store a graph_t structure
 *
 * Return: If memory allocation fails, return NULL.
 * Otherwise, return a pointer to the allocated structure.
 */
graph_t *graph_create(void)
{
	graph_t *new_graph = malloc(sizeof(graph_t));

	if (new_graph == NULL)
		return (NULL);

	new_graph->nb_vertices = 0;
	new_graph->vertices = NULL;

	return (new_graph);
}
