

typedef enum strategy {MAX, MIN} Strategy;
typedef enum bool {FALSE, TRUE} bool;
typedef struct fiboheap * Fiboheap;

Fiboheap create_heap(Strategy strategy);

void insert_key(Fiboheap heap, int key);

int find_max_key(Fiboheap heap);

int find_min_key(Fiboheap heap);

int pop_max(Fiboheap heap);

int pop_min(Fiboheap heap);

void print_roots(Fiboheap heap);

void destroy(Fiboheap heap);