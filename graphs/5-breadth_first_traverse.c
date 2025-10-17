#include "graphs.h"

/* Internal helper function declarations */
static vertex_t *locate_vertex_by_index(const graph_t *graph,
					size_t target_idx);
static queue_t *initialize_bfs_queue(void);
static void enqueue_vertex(queue_t *queue, int vertex_index);
static int dequeue_vertex(queue_t *queue, int peek_only);
static void cleanup_bfs_resources(queue_t *queue,
				  size_t *visited_tracker);

/**
 * initialize_bfs_queue - Creates and initializes a new queue for BFS
 *
 * Description: Allocates memory for a queue structure and sets initial values
 * Return: Pointer to newly created queue, NULL on allocation failure
 */
static queue_t *initialize_bfs_queue(void)
{
	queue_t *new_queue;

	new_queue = malloc(sizeof(queue_t));
	if (!new_queue)
		return (NULL);

	new_queue->front = -1;
	new_queue->rear = -1;
	return (new_queue);
}

/**
 * enqueue_vertex - Adds a vertex index to the end of the queue
 * @queue: Target queue structure
 * @vertex_index: Index value to be added
 *
 * Description: Inserts vertex index at rear of queue, handles queue expansion
 * Return: void
 */
static void enqueue_vertex(queue_t *queue, int vertex_index)
{
	if (queue->rear >= (SIZE - 1))
		return; /* Queue full, silently ignore */

	if (queue->front == -1)
		queue->front = 0;

	queue->rear++;
	queue->items[queue->rear] = vertex_index;
}

/**
 * dequeue_vertex - Removes and returns front element from queue
 * @queue: Source queue structure
 * @peek_only: If 1, only peek at front without removing
 *
 * Description: Retrieves front element, optionally removing it from queue
 * Return: Front element value, -1 if queue empty
 */
static int dequeue_vertex(queue_t *queue, int peek_only)
{
	int front_element;

	if (queue->rear == -1)
		return (-1);

	if (peek_only)
		return (queue->items[queue->front]);

	front_element = queue->items[queue->front];
	queue->front++;

	/* Reset queue if empty */
	if (queue->front > queue->rear)
		queue->front = queue->rear = -1;

	return (front_element);
}

/**
 * locate_vertex_by_index - Finds vertex with specified index in graph
 * @graph: Graph structure to search
 * @target_idx: Index to locate
 *
 * Description: Linear search through vertex list for matching index
 * Return: Pointer to matching vertex, NULL if not found
 */
static vertex_t *locate_vertex_by_index(const graph_t *graph,
					size_t target_idx)
{
	vertex_t *current_vertex;

	if (!graph || target_idx >= graph->nb_vertices)
		return (NULL);

	current_vertex = graph->vertices;
	while (current_vertex)
	{
		if (current_vertex->index == target_idx)
			return (current_vertex);
		current_vertex = current_vertex->next;
	}

	return (NULL);
}

/**
 * cleanup_bfs_resources - Frees memory allocated for BFS traversal
 * @queue: Queue structure to free
 * @visited_tracker: Visited array to free
 *
 * Return: void
 */
static void cleanup_bfs_resources(queue_t *queue,
				  size_t *visited_tracker)
{
	if (queue)
		free(queue);
	if (visited_tracker)
		free(visited_tracker);
}

/**
 * breadth_first_traverse - Performs breadth-first traversal of graph
 * @graph: Graph structure to traverse
 * @action: Callback function for each visited vertex
 *
 * Description: Implements BFS algorithm using queue, tracking depth levels
 *              and calling action function for each vertex at current level
 * Return: Maximum breadth (depth level) reached, 0 on failure
 */
size_t breadth_first_traverse(const graph_t *graph,
			      void (*action)(const vertex_t *v, size_t breadth))
{
	size_t current_breadth = 0, *visited_tracker;
	int current_index;
	vertex_t *current_vertex, *neighbor_vertex;
	edge_t *current_edge;
	queue_t *bfs_queue;

	/* Input validation */
	if (!graph || !graph->vertices || graph->nb_vertices == 0 || !action)
		return (0);

	/* Initialize data structures */
	visited_tracker = calloc(graph->nb_vertices, sizeof(size_t));
	bfs_queue = initialize_bfs_queue();

	if (!visited_tracker || !bfs_queue)
	{
		cleanup_bfs_resources(bfs_queue, visited_tracker);
		return (0);
	}

	/* Start BFS from first vertex */
	current_vertex = graph->vertices;
	enqueue_vertex(bfs_queue, current_vertex->index);
	enqueue_vertex(bfs_queue, LEVELBREAK);

	/* Main BFS loop */
	while (bfs_queue->rear != -1)
	{
		current_index = dequeue_vertex(bfs_queue, 0);

		/* Handle level separator */
		if (current_index == LEVELBREAK)
		{
			current_breadth++;
			enqueue_vertex(bfs_queue, LEVELBREAK);

			/* Check if queue only contains level breaks */
			if (dequeue_vertex(bfs_queue, 1) == LEVELBREAK)
				break;
			continue;
		}

		/* Process current vertex */
		current_vertex = locate_vertex_by_index(graph, current_index);
		if (!current_vertex ||
		    visited_tracker[current_vertex->index] == EXPLORED)
			continue;

		/* Mark as visited and execute action */
		visited_tracker[current_vertex->index] = EXPLORED;
		action(current_vertex, current_breadth);

		/* Add unvisited neighbors to queue */
		current_edge = current_vertex->edges;
		while (current_edge)
		{
			neighbor_vertex = current_edge->dest;
			if (neighbor_vertex &&
			    visited_tracker[neighbor_vertex->index] == UNEXPLORED)
			{
				enqueue_vertex(bfs_queue, neighbor_vertex->index);
				visited_tracker[neighbor_vertex->index] = BACKTRACK;
			}
			current_edge = current_edge->next;
		}
	}

	/* Cleanup and return result */
	cleanup_bfs_resources(bfs_queue, visited_tracker);
	return (current_breadth > 0 ? current_breadth - 1 : 0);
}
