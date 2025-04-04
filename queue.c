#include "queue.h"
#include "tile_game.h"
#include <stdlib.h>
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
    size_t encoded = remove_from_head(&q->data);
    return deserialize(encoded);
}

//returns true if puzzle in solved state
bool is_solved(struct game_state state) 
{
    uint8_t expected_tile = 1;
    for (int r = 0; r < 4; ++r) 
    {
        for (int c = 0; c < 4; ++c) 
        {
            if (r == 3 && c == 3) 
            {
                if (state.tiles[r][c] != 0) 
                {
                    return false;
                }
            } else if (state.tiles[r][c] != expected_tile++) 
            {
                return false;
            }
        }
    }
    return true;
}

//makes all valid next moves from current state
int next_states_from_state(struct game_state state, struct game_state out_states[4]) 
{
int generated = 0;

    //try moving the empty tile up
    if (state.empty_row > 0) 
    {
        out_states[generated] = state;
        move_up(&out_states[generated]);
        out_states[generated].num_steps++;
        generated++;
    }

    //try moving down
    if (state.empty_row < 3) 
    {
        out_states[generated] = state;
        move_down(&out_states[generated]);
        out_states[generated].num_steps++;
        generated++;
    }

    //try moving left
    if (state.empty_col > 0) 
    {
        out_states[generated] = state;
        move_left(&out_states[generated]);
        out_states[generated].num_steps++;
        generated++;
    }

    //try moving right
    if (state.empty_col < 3) 
    {
        out_states[generated] = state;
        move_right(&out_states[generated]);
        out_states[generated].num_steps++;
        generated++;
    }
    return generated;
}

//returns the minimum number of moves
int number_of_moves(struct game_state start) 
{
    struct queue q = {0};
    enqueue(&q, start);

    //allocate visited array 
    bool *visited = calloc(1 << 24, sizeof(bool));
    if (!visited) 
    {
        fprintf(stderr, "Memory allocation failed.\n");
        return -1;
    }

    while (q.data.head) 
    {
        struct game_state current = dequeue(&q);
        size_t hash = serialize(current);
        size_t index = hash % (1 << 24);
        if (visited[index]) 
        {
            continue;
        }
        visited[index] = true;

        if (is_solved(current)) 
        {
            free_list(q.data);
            free(visited);
            return current.num_steps;
        }
        struct game_state next_states[4];
        int total = next_states_from_state(current, next_states);
        for (int i = 0; i < total; ++i) 
        {
            enqueue(&q, next_states[i]);
        }
    }
    free_list(q.data);
    free(visited);
    return -1;
}
