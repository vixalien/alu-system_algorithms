#include "graphs.h"
#include <string.h>

edge_t *add_single_edge(vertex_t *source_vertex, vertex_t *dest_vertex);

/**
 * graph_add_edge - adds an edge between vertices in the graph
 * @graph: the graph to which the edge should be added
 * @src: the content of the source vertex
 * @dest: the content of the destination vertex
 * @type: the type of edge (UNIDIRECTIONAL or BIDIRECTIONAL)
 * Return: 1 on success, 0 on failure
 */
int graph_add_edge(graph_t *graph, const char *src, const char *dest, edge_type_t type)
{
	vertex_t *current_vertex = NULL, *source_vertex = NULL, *dest_vertex = NULL;

	if (!graph || !src || !dest || type < UNIDIRECTIONAL || type > BIDIRECTIONAL)
		return (0);

	for (current_vertex = graph->vertices; current_vertex; current_vertex = current_vertex->next)
	{
		if (strcmp(current_vertex->content, src) == 0)
			source_vertex = current_vertex;
		else if (strcmp(current_vertex->content, dest) == 0)
			dest_vertex = current_vertex;
	}

	if (!source_vertex || !dest_vertex)
		return (0);

	if (add_single_edge(source_vertex, dest_vertex) == NULL)
		return (0);

	if (type == BIDIRECTIONAL)
	{
		if (add_single_edge(dest_vertex, source_vertex) == NULL)
			return (0);
	}

	return (1);
}

/**
 * add_single_edge - adds a single directed edge to a vertex
 * @source_vertex: the source vertex for the edge
 * @dest_vertex: the destination vertex for the edge
 * Return: a pointer to the created edge, or NULL on failure
 */
edge_t *add_single_edge(vertex_t *source_vertex, vertex_t *dest_vertex)
{
	edge_t *new_edge = NULL, *current_edge = NULL;

	if (!source_vertex || !dest_vertex)
		return (NULL);

	new_edge = malloc(sizeof(edge_t));
	if (!new_edge)
		return (NULL);

	new_edge->dest = dest_vertex;
	new_edge->next = NULL;

	current_edge = source_vertex->edges;
	while (current_edge && current_edge->next)
		current_edge = current_edge->next;

	if (current_edge)
		current_edge->next = new_edge;
	else
		source_vertex->edges = new_edge;

	source_vertex->nb_edges++;

	return (new_edge);
}