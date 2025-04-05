#include "queue.h"
#include "tile_game.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//adds game state to back of queue
void enqueue(struct queue *q, struct game_state state) 
{
    size_t encoded = serialize(state);
    insert_at_tail(&q->data, encoded);
}

//removes game state from the front of the queue
struct game_state dequeue(struct queue *q) 
{
    if (!q->data.head) 
    {
        struct game_state empty_state = {0};
        return empty_state;
    }
    size_t encoded = remove_from_head(&q->data);
    return deserialize(encoded);
}

//return min number of moves
int number_of_moves(struct game_state start) 
{
    //initialize queue
    struct queue q = {{NULL}};

    //allocate visited state
    #define MAX_VISITED 1000000
    static uint64_t visited[MAX_VISITED];
    static size_t visited_count = 0;
    memset(visited, 0, sizeof(visited));
    visited_count = 0;

    enqueue(&q, start);
    visited[visited_count++] = serialize(start);

    //breadth-first search loop
    while (q.data.head != NULL) 
    {
        struct game_state current = dequeue(&q);

        //quick check solution state
        static const uint8_t target[4][4] = {
            {1, 2, 3, 4},
            {5, 6, 7, 8},
            {9, 10, 11, 12},
            {13, 14, 15, 0}
        };
        if (memcmp(current.tiles, target, sizeof(target)) == 0) 
        {
            free_list(q.data);  //cleanup queue memory
            return current.num_steps;
        }

        //explore next possible moves
        struct game_state neighbors[4];
        int count = 0;

        struct game_state next = current;
        move_up(&next);
        if (memcmp(next.tiles, current.tiles, sizeof(next.tiles)) != 0)
            neighbors[count++] = next;

        next = current;
        move_down(&next);
        if (memcmp(next.tiles, current.tiles, sizeof(next.tiles)) != 0)
            neighbors[count++] = next;

        next = current;
        move_left(&next);
        if (memcmp(next.tiles, current.tiles, sizeof(next.tiles)) != 0)
            neighbors[count++] = next;

        next = current;
        move_right(&next);
        if (memcmp(next.tiles, current.tiles, sizeof(next.tiles)) != 0)
            neighbors[count++] = next;

        //check and enqueue unvisited states
        for (int i = 0; i < count; i++) 
        {
            uint64_t key = serialize(neighbors[i]);
            bool seen_before = false;

            for (size_t j = 0; j < visited_count; j++) 
            {
                if (visited[j] == key) 
                {
                    seen_before = true;
                    break;
                }
            }

            if (!seen_before && visited_count < MAX_VISITED) 
            {
                enqueue(&q, neighbors[i]);
                visited[visited_count++] = key;
            }
        }
    }

    //if no solution
    free_list(q.data);
    return -1;
}

