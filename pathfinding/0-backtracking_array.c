#include "pathfinding.h"

/**
 * is_position_valid - Checks if the position is valid and hasn't been visited.
 * @map: 2D array to explore (0 is walkable, 1 is blocked)
 * @visited: Array that keeps track of where we've been
 * @x: X coordinate of the position to check
 * @y: Y coordinate of the position to check
 * @rows: Number of rows
 * @cols: Number of columns
 * Return: 1 if it's safe, 0 if not
 */
int is_position_valid(char **map, int **visited, int x, int y, int rows, int cols)
{
	if (x < 0 || y < 0 || x >= cols || y >= rows)
		return (0);
	if (map[y][x] == '1' || visited[y][x] == 1)
		return (0);
	return (1);
}

/**
 * setup_arrays - Creates path queue and visited map, and array of directions.
 * @path: Path queue
 * @visited: Pointer to visited 2D array
 * @direction: Array of directions
 * @rows: Number of rows
 * @cols: Number of columns
 * Return: 0 on success, -1 on failure
 */
int setup_arrays(queue_t **path, int ***visited, point_t *direction,
				 int rows, int cols)
{
	int i;

	*path = queue_create();
	if (*path == NULL)
		return (-1);

	*visited = malloc(rows * sizeof(**visited));
	if (*visited == NULL)
		return (-1);
	for (i = 0; i < rows; i++)
	{
		(*visited)[i] = calloc(cols, sizeof(**visited));
		if ((*visited)[i] == NULL)
			return (-1);
	}

	direction[BOTTOM].x = 0, direction[BOTTOM].y = 1;
	direction[RIGHT].x = 1, direction[RIGHT].y = 0;
	direction[LEFT].x = -1, direction[LEFT].y = 0;
	direction[TOP].x = 0, direction[TOP].y = -1;
	return (0);
}

/**
 * add_to_path - Adds a point to the queue.
 * @path: Current queue
 * @x: Current x coordinate
 * @y: Current y coordinate
 * Return: 1 on success, 0 on failure
 */
int add_to_path(queue_t **path, int x, int y)
{
	point_t *point;

	point = malloc(sizeof(*point));
	if (!point)
		return (0);
	point->x = x;
	point->y = y;

	queue_push_front(*path, point);
	return (1);
}

/**
 * backtrack_recursively - Recursive backtracking function.
 * @map: Pointer to the given 2D array to explore (0 is walkable, 1 is blocked)
 * @visited: Pointer to 2D array of coordinates visited
 * @x: Current x coordinate
 * @y: Current y coordinate
 * @target: Target coordinates
 * @path: Solution queue
 * @directions: Array of possible directions to travel
 * @rows: Number of rows
 * @cols: Number of columns
 * Return: 1 on success, 0 on failure
 */
int backtrack_recursively(char **map, int **visited, int x, int y,
						  point_t const *target, queue_t **path,
						  point_t *directions, int rows, int cols)
{
	int i;

	if (!is_position_valid(map, visited, x, y, rows, cols))
		return (0);

	printf("Checking coordinates [%d, %d]\n", x, y);

	if (x == target->x && y == target->y)
		return (add_to_path(path, x, y));

	visited[y][x] = 1;

	for (i = 0; i < NUM_DIRECTIONS; i++)
	{
		if (backtrack_recursively(map, visited, x + directions[i].x,
								  y + directions[i].y, target, path, directions, rows, cols))
			return (add_to_path(path, x, y));
	}
	return (0);
}

/**
 * backtracking_array - Searches for the first path from the starting point to
 *                      the target within a two-dimensional array.
 * @map: Pointer to read-only 2D array (0 is walkable, 1 is blocked cell)
 * @rows: Number of rows
 * @cols: Number of columns
 * @start: Coordinates of the starting point
 * @target: Coordinates of the ending point
 * Return: Queue in which each node is a point in the path from start to end
 */
queue_t *backtracking_array(char **map, int rows, int cols,
							point_t const *start, point_t const *target)
{
	int **visited;
	int i, found_path;
	queue_t *path;
	point_t directions[NUM_DIRECTIONS];

	if (!map || !*map || rows <= 0 || cols <= 0 || !start || !target)
		return (NULL);

	if (setup_arrays(&path, &visited, directions, rows, cols) == -1)
		return (NULL);

	found_path = backtrack_recursively(map, visited, start->x, start->y,
									   target, &path, directions, rows, cols);

	for (i = 0; i < rows; i++)
		free(visited[i]);
	free(visited);

	if (found_path)
		return (path);

	queue_delete(path);
	return (NULL);
}