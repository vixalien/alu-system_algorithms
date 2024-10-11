#include "pathfinding.h"

/* Internal helper function declarations */
static vertex_t *find_minimum_distance_vertex(graph_t *graph, size_t *distances,
					      size_t *processed, size_t *vertex_idx);
static void reconstruct_shortest_path(graph_t *graph, queue_t *result_queue,
				      vertex_t **predecessor_map, vertex_t const *source,
				      vertex_t const *destination);
static void execute_dijkstra_iteration(graph_t *graph, size_t *distances, size_t *processed,
				       vertex_t **predecessor_map, vertex_t const *source,
				       vertex_t const *destination, size_t current_idx);
static int allocate_dijkstra_structures(graph_t *graph, size_t **distances,
					size_t **processed, vertex_t ***predecessor_map);

/**
 * find_minimum_distance_vertex - Locates unprocessed vertex with smallest distance
 * @graph: Pointer to the graph structure
 * @distances: Array containing current shortest distances from source
 * @processed: Array tracking which vertices have been processed
 * @vertex_idx: Pointer to store the index of the minimum distance vertex
 *
 * Description: Searches through all vertices to find the one with minimum
 *              distance that hasn't been processed yet
 * Return: Pointer to vertex with minimum distance, NULL if none found
 */
static vertex_t *find_minimum_distance_vertex(graph_t *graph, size_t *distances,
					      size_t *processed, size_t *vertex_idx)
{
	size_t minimum_dist = ULONG_MAX;
	size_t iterator;
	vertex_t *current_vertex = graph->vertices;

	if (!current_vertex)
		return (NULL);

	*vertex_idx = ULONG_MAX;

	/* Find vertex with minimum distance among unprocessed vertices */
	for (iterator = 0; iterator < graph->nb_vertices; iterator++)
	{
		if (processed[iterator] == 0 && distances[iterator] < minimum_dist)
		{
			minimum_dist = distances[iterator];
			*vertex_idx = iterator;
		}
	}

	if (*vertex_idx == ULONG_MAX)
		return (NULL);

	/* Locate the actual vertex structure with the found index */
	while (current_vertex)
	{
		if (current_vertex->index == *vertex_idx)
			return (current_vertex);
		current_vertex = current_vertex->next;
	}
	
	return (NULL);
}

/**
 * reconstruct_shortest_path - Builds path queue from source to destination
 * @graph: Pointer to the graph structure
 * @result_queue: Queue to store the reconstructed path
 * @predecessor_map: Array mapping each vertex to its predecessor in shortest path
 * @source: Starting vertex of the path
 * @destination: Target vertex of the path
 *
 * Description: Traces back through predecessor links to build complete path
 * Return: void
 */
static void reconstruct_shortest_path(graph_t *graph, queue_t *result_queue,
				      vertex_t **predecessor_map, vertex_t const *source,
				      vertex_t const *destination)
{
	size_t current_idx = destination->index;

	if (!predecessor_map[current_idx])
		return;

	/* Add destination to path */
	if (!queue_push_front(result_queue, strdup(destination->content)))
	{
		queue_delete(result_queue);
		return;
	}

	/* Trace back through predecessors until reaching source */
	while (predecessor_map[current_idx] && current_idx < graph->nb_vertices)
	{
		if (!queue_push_front(result_queue, strdup(predecessor_map[current_idx]->content)))
		{
			queue_delete(result_queue);
			return;
		}
		
		current_idx = predecessor_map[current_idx]->index;
		if (current_idx == source->index)
			return;
	}
}

/**
 * execute_dijkstra_iteration - Performs one iteration of Dijkstra's algorithm
 * @graph: Pointer to the graph structure
 * @distances: Array of current shortest distances
 * @processed: Array tracking processed vertices
 * @predecessor_map: Array mapping vertices to their predecessors
 * @source: Source vertex for pathfinding
 * @destination: Target vertex for pathfinding
 * @current_idx: Current vertex index being processed
 *
 * Description: Processes current vertex, updates distances to neighbors,
 *              and recursively continues until destination is reached
 * Return: void
 */
static void execute_dijkstra_iteration(graph_t *graph, size_t *distances, size_t *processed,
				       vertex_t **predecessor_map, vertex_t const *source,
				       vertex_t const *destination, size_t current_idx)
{
	vertex_t *active_vertex;
	edge_t *current_edge;
	size_t vertex_index, tentative_distance;

	active_vertex = find_minimum_distance_vertex(graph, distances, processed, &current_idx);
	if (!active_vertex)
		return;

	printf("Checking %s, distance from %s is %ld\n",
	       active_vertex->content, source->content, distances[active_vertex->index]);

	vertex_index = active_vertex->index;
	current_edge = active_vertex->edges;

	/* Process all neighbors of current vertex */
	while (current_edge && processed[vertex_index] == 0)
	{
		tentative_distance = distances[vertex_index] + current_edge->weight;
		
		/* Update distance if shorter path found */
		if (distances[current_edge->dest->index] > tentative_distance)
		{
			distances[current_edge->dest->index] = tentative_distance;
			predecessor_map[current_edge->dest->index] = active_vertex;
		}
		
		current_edge = current_edge->next;
	}

	/* Mark current vertex as processed */
	processed[vertex_index] = 1;

	/* Stop if destination has been processed */
	if (processed[destination->index] == 1)
		return;

	/* Continue with next iteration */
	execute_dijkstra_iteration(graph, distances, processed, predecessor_map,
				   source, destination, current_idx);
}

/**
 * allocate_dijkstra_structures - Initializes data structures for Dijkstra's algorithm
 * @graph: Pointer to the graph structure
 * @distances: Pointer to distance array to be allocated
 * @processed: Pointer to processed array to be allocated
 * @predecessor_map: Pointer to predecessor array to be allocated
 *
 * Description: Allocates and initializes all arrays needed for the algorithm
 * Return: 0 on success, 1 on allocation failure
 */
static int allocate_dijkstra_structures(graph_t *graph, size_t **distances,
					size_t **processed, vertex_t ***predecessor_map)
{
	size_t initialization_idx;

	/* Allocate processed tracking array */
	*processed = calloc(graph->nb_vertices, sizeof(**processed));
	if (!(*processed))
		return (1);

	/* Allocate predecessor mapping array */
	*predecessor_map = calloc(graph->nb_vertices, sizeof(**predecessor_map));
	if (!(*predecessor_map))
	{
		free(*processed);
		return (1);
	}

	/* Allocate distance tracking array */
	*distances = malloc(graph->nb_vertices * sizeof(**distances));
	if (!(*distances))
	{
		free(*processed);
		free(*predecessor_map);
		return (1);
	}

	/* Initialize all distances to infinity */
	for (initialization_idx = 0; initialization_idx < graph->nb_vertices; initialization_idx++)
		(*distances)[initialization_idx] = ULONG_MAX;

	return (0);
}

/**
 * dijkstra_graph - Finds shortest path between two vertices using Dijkstra's algorithm
 * @graph: Pointer to the weighted graph structure
 * @start: Pointer to the starting vertex
 * @target: Pointer to the destination vertex
 *
 * Description: Implements Dijkstra's shortest path algorithm to find the path
 *              with minimum total weight from start to target vertex
 * Return: Queue containing vertices in shortest path, NULL if no path exists
 */
queue_t *dijkstra_graph(graph_t *graph, vertex_t const *start, vertex_t const *target)
{
	size_t *distance_array = NULL, *processed_array = NULL;
	queue_t *path_queue = NULL;
	vertex_t **predecessor_array = NULL;

	/* Validate input parameters */
	if (!graph || !start || !target)
		return (NULL);

	/* Initialize algorithm data structures */
	if (allocate_dijkstra_structures(graph, &distance_array, &processed_array,
					 &predecessor_array) != 0)
		return (NULL);

	/* Create result queue */
	path_queue = queue_create();
	if (!path_queue)
	{
		free(processed_array);
		free(distance_array);
		free(predecessor_array);
		return (NULL);
	}

	/* Set source vertex distance to zero */
	distance_array[start->index] = 0;

	/* Execute Dijkstra's algorithm */
	execute_dijkstra_iteration(graph, distance_array, processed_array,
				   predecessor_array, start, target, 0);

	/* Reconstruct path from source to destination */
	reconstruct_shortest_path(graph, path_queue, predecessor_array, start, target);

	/* Cleanup allocated memory */
	free(processed_array);
	free(distance_array);
	free(predecessor_array);

	/* Return NULL if no path was found */
	if (!path_queue->front)
	{
		queue_delete(path_queue);
		return (NULL);
	}

	return (path_queue);
}