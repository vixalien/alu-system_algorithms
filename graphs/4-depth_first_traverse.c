#include "graphs.h"

/* Internal helper function declarations */
static vertex_t *find_vertex_at_index(const graph_t *graph,
				      size_t target_index);
static void perform_dfs_recursion(size_t vertex_id, size_t *visit_tracker,
				  size_t current_level, size_t *max_level,
				  const graph_t *graph,
				  void (*action)(const vertex_t *v, size_t depth));

/**
 * find_vertex_at_index - Locates a vertex by its index within the graph
 * @graph: Pointer to the graph structure
 * @target_index: Index value to search for
 *
 * Description: Traverses the vertex list to find a vertex with the
 *              specified index value
 * Return: Pointer to matching vertex, NULL if not found or invalid input
 */
static vertex_t *find_vertex_at_index(const graph_t *graph,
				      size_t target_index)
{
	vertex_t *current_node;

	if (!graph || target_index >= graph->nb_vertices)
		return (NULL);

	current_node = graph->vertices;
	while (current_node)
	{
		if (current_node->index == target_index)
			return (current_node);
		current_node = current_node->next;
	}

	return (NULL);
}

/**
 * perform_dfs_recursion - Recursive helper for depth-first traversal
 * @vertex_id: Current vertex index being processed
 * @visit_tracker: Array tracking visited status of all vertices
 * @current_level: Current depth level in the traversal
 * @max_level: Pointer to maximum depth reached so far
 * @graph: Graph structure being traversed
 * @action: Function to call for each visited vertex
 *
 * Description: Implements the core DFS algorithm recursively, marking
 *              vertices as visited and exploring their neighbors
 * Return: void
 */
static void perform_dfs_recursion(size_t vertex_id, size_t *visit_tracker,
				  size_t current_level, size_t *max_level,
				  const graph_t *graph,
				  void (*action)(const vertex_t *v, size_t depth))
{
	vertex_t *current_vertex, *neighbor_vertex;
	edge_t *current_edge;

	current_vertex = find_vertex_at_index(graph, vertex_id);

	/* Process vertex if valid and unvisited */
	if (current_vertex && visit_tracker[vertex_id] == UNEXPLORED)
	{
		/* Execute action function on current vertex */
		action(current_vertex, current_level);

		/* Update maximum depth if current level exceeds it */
		if (current_level > *max_level)
			*max_level = current_level;

		/* Mark current vertex as explored */
		visit_tracker[vertex_id] = EXPLORED;

		/* Explore all adjacent vertices */
		current_edge = current_vertex->edges;
		while (current_edge)
		{
			neighbor_vertex = current_edge->dest;

			/* Recursively visit unexplored neighbors */
			if (visit_tracker[neighbor_vertex->index] == UNEXPLORED)
			{
				perform_dfs_recursion(neighbor_vertex->index, visit_tracker,
						      current_level + 1, max_level,
						      graph, action);
			}
			current_edge = current_edge->next;
		}
	}
}

/**
 * depth_first_traverse - Performs depth-first traversal of entire graph
 * @graph: Graph structure to traverse
 * @action: Callback function executed for each visited vertex
 *
 * Description: Initiates DFS traversal from the first vertex, ensuring
 *              all connected components are visited. Tracks maximum depth.
 * Return: Maximum depth reached during traversal, 0 on failure
 */
size_t depth_first_traverse(const graph_t *graph,
			    void (*action)(const vertex_t *v, size_t depth))
{
	size_t maximum_depth = 0;
	size_t *visited_array;
	vertex_t *start_vertex;

	/* Validate input parameters */
	if (!graph || !action)
		return (0);

	/* Allocate tracking array for visited vertices */
	visited_array = calloc(graph->nb_vertices, sizeof(size_t));
	if (!visited_array)
		return (0);

	/* Begin traversal from first vertex if it exists */
	start_vertex = graph->vertices;
	if (start_vertex && visited_array[start_vertex->index] == UNEXPLORED)
	{
		perform_dfs_recursion(start_vertex->index, visited_array, 0,
				      &maximum_depth, graph, action);
	}

	/* Clean up allocated memory */
	free(visited_array);

	return (maximum_depth);
}
