#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "fiboheap.h"

typedef struct node * Node;

struct node{
    int key;
    Node parent;
    Node children; // double linked list
    Node left;
    Node right;
};

struct fiboheap{
    Node * nodes_by_degree;
    Node extreme; // either min or max
    Node roots; // double linked list
    Node * marked;
    int max_degree;
    bool (*compare)(int, int);
};

/* Static methods declarations */

static bool _max_compare(int a, int b);
static bool _min_compare(int a, int b);

static void _add_node(Node * list, Node node);
static void _consolidate(Fiboheap heap);
static void _free_node(Node node);
static void _free_nodes(Node node, Node last);
static Node _get_new_extreme(Fiboheap heap);
static int _length(Node node, Node first);
static void _meld_children_into_roots_list(Fiboheap heap, Node node);
static void _print_nodes(Node node, Node last);

/* Static definitions */

static bool
_max_compare(int a, int b){
    return a > b;
}

static bool
_min_compare(int a, int b){
    return a < b;
}

static void
_add_node(Node * list, Node node){

    if(*list == NULL){
        *list = node;
        (*list)->left = (*list)->right = node;
    }
    else{
        Node left = (*list)->left;
        left->right = node;
        node->left = left;
        node->right = (*list);
        (*list)->left = node;
    }
}

static int
_calculate_degree(Node node){
    if(node->children == NULL){
        return 0;
    }
    return _length(node->children->right, node->children);
}

static void
_consolidate(Fiboheap heap){
    Node current = heap->roots;
    Node last = heap->roots->left;

    do{
        int degree = _calculate_degree(current);

        if(degree > heap->max_degree){
            Node * new_nodes_by_degree = realloc(heap->nodes_by_degree, sizeof(Node) * (degree + 1));

            for (int i = heap->max_degree; i < degree + 1 ; i++ ){
                new_nodes_by_degree[i] = NULL;
            }

            if(new_nodes_by_degree == NULL){
                destroy(heap);
                exit(-1); //FIXME
            }
            heap->nodes_by_degree = new_nodes_by_degree;
            heap->max_degree = degree;
        }

        if(heap->nodes_by_degree[degree] != NULL){
            Node same_degree_node = heap->nodes_by_degree[degree];
            if(heap->compare(same_degree_node->key, current->key)){
                if(heap->roots == current){
                    heap->roots = current->right;
                    heap->roots->left = current->left;
                }
                current->left->right = current->right;
                current->right->left = current->left;
                _add_node(&same_degree_node->children, current);

                current = same_degree_node;
            }
            else{
                if(heap->roots == same_degree_node){
                    heap->roots = same_degree_node->right;
                    heap->roots->left = same_degree_node->left;
                }

                same_degree_node->left->right = same_degree_node->right;
                same_degree_node->right->left = same_degree_node->left;
                _add_node(&current->children, same_degree_node);

            }

            heap->nodes_by_degree[degree] = NULL;
            continue;
        }
        else{
            heap->nodes_by_degree[degree] = current;
        }

        current = current->right;

    }while(current != last);

    for (int i = 0; i < heap->max_degree + 1 ; i++ ){
        heap->nodes_by_degree[i] = NULL;
    }
}

static void
_free_node(Node node){
    if(node->children != NULL){
        _free_nodes(node->children, node->children->left);
    }
    free(node);

    return ;
}

static void
_free_nodes(Node node, Node last){
    if(node == last){
        _free_node(node);
        return ;
    }
    _free_nodes(node->right, last);

    _free_node(node);
}

static Node
_get_new_extreme(Fiboheap heap){
    Node current = heap->roots;
    Node last = heap->roots->left;
    Node max = current;

    do{
        current = current->right;

        if(heap->compare(current->key, max->key)){
            max = current;
        }
    }while(current != last);

    return max;
}

static int
_length(Node node, Node first){
    if(node == first){
        return 1;
    }
    return 1 + _length(node->right, first);
}

static void
_meld_children_into_roots_list(Fiboheap heap, Node node){
    if(node->children != NULL){
        Node first = node->children;
        Node last = node->children->left;

        Node left = heap->roots->left;
        left->right = first;
        first->left = left;
        last->right = heap->roots;
        heap->roots->left = last;
    }
    return ;
}

static void
_print_nodes(Node node, Node last){
    printf("%d ", node->key);
    if (node == last){
        return ;
    }
    _print_nodes(node->right, last);
}

/* Public API methods */

Fiboheap
create_heap(Strategy strategy){
    Fiboheap ret = calloc(sizeof(struct fiboheap), 1);

    if(ret == NULL){
        return NULL;
    }

    if(strategy == MAX){
        ret->compare = &_max_compare;
    }
    else{
        ret->compare = &_min_compare;
    }
    ret->nodes_by_degree = calloc(sizeof(Node), 2);
    ret->max_degree = 1;

    return ret;
}

void
print_roots(Fiboheap heap){
    printf("\nroots: \n");
    _print_nodes(heap->roots, heap->roots->left);
}

void
insert_key(Fiboheap heap, int key){
    Node new_node = calloc(sizeof(struct node), 1);

    if(new_node == NULL){
        // FIXME
        return ;
    }

    new_node->key = key;

    _add_node(&(heap->roots), new_node);

    if(heap->extreme == NULL || heap->compare(key, heap->extreme->key)){
        heap->extreme = new_node;
    }

    return ;
}

int
find_max_key(Fiboheap heap){
    return heap->extreme->key;
}

int
find_min_key(Fiboheap heap){
    //what's different is the strategy
    return find_max_key(heap);
}

int
pop_max(Fiboheap heap){
    int ret = heap->extreme->key;
    Node current_extreme = heap->extreme;
    // remove extreme from its current position connecting its adyacent
    // nodes btw them
    current_extreme->left->right = current_extreme->right;
    current_extreme->right->left = current_extreme->left;

    if(heap->roots == current_extreme){
        heap->roots = current_extreme->right;
        heap->roots->left = current_extreme->left;
    }

    _meld_children_into_roots_list(heap, current_extreme);
    heap->extreme = _get_new_extreme(heap);

    _consolidate(heap);

    if(heap->extreme == current_extreme){
        heap->roots = NULL;
    }

    free(current_extreme);
    return ret;
}

int
pop_min(Fiboheap heap){
    //what's different is the strategy
    return pop_max(heap);
}

void
destroy(Fiboheap heap){
    if(heap->roots){
        _free_nodes(heap->roots, heap->roots->left);
    }
    free(heap->nodes_by_degree);
    free(heap);
}
