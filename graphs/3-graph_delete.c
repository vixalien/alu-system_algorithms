#include "graphs.h"

/**
* graph_delete - deallocate memory for a graph
* @graph: pointer to graph type
*/
void graph_delete(graph_t *graph)
{
	vertex_t *current_vertex, *next_vertex;
	edge_t *current_edge, *next_edge;

	if (graph == NULL)
		return;

	current_vertex = graph->vertices;

	while (current_vertex)
	{
		next_vertex = current_vertex->next;
		free(current_vertex->content);

		current_edge = current_vertex->edges;
		while (current_edge)
		{
			next_edge = current_edge->next;
			free(current_edge);
			current_edge = next_edge;
		}
		free(current_vertex);
		current_vertex = next_vertex;
	}

	free(graph);
}
