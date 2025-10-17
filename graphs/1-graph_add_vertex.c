#include "graphs.h"
#include <string.h>
#include <stdlib.h>

vertex_t *create_new_vertex(const char *str);

/**
 * graph_add_vertex - adds a vertex to the graph
 * @graph: pointer to graph type
 * @str: string value for new vertex
 * Return: pointer to vertex or null
 */
vertex_t *graph_add_vertex(graph_t *graph, const char *str)
{
	vertex_t *current_vertex, *last_vertex, *new_vertex;

	if (graph == NULL || str == NULL)
		return (NULL);

	current_vertex = graph->vertices;

	while (current_vertex)
	{
		if (strcmp(current_vertex->content, str) == 0)
			return (NULL);

		last_vertex = current_vertex;
		current_vertex = current_vertex->next;
	}

	new_vertex = create_new_vertex(str);
	if (new_vertex == NULL)
		return (NULL);

	if (graph->nb_vertices == 0)
	{
		graph->vertices = new_vertex;
		new_vertex->index = 0;
	}
	else
	{
		last_vertex->next = new_vertex;
		new_vertex->index = last_vertex->index + 1;
	}

	graph->nb_vertices += 1;

	return (new_vertex);
}

/**
 * create_new_vertex - create a new vertex
 * @str: string value for the new vertex
 * Return: a pointer to the new vertex
 */
vertex_t *create_new_vertex(const char *str)
{
	vertex_t *new_vertex = malloc(sizeof(vertex_t));

	if (new_vertex == NULL)
		return (NULL);

	new_vertex->content = strdup(str);
	new_vertex->index = 0;
	new_vertex->edges = NULL;
	new_vertex->nb_edges = 0;
	new_vertex->next = NULL;

	return (new_vertex);
}
