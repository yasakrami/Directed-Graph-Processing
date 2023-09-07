#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

enum { TIME, DISTANCE, COST, INVALID };
enum color { White, Gray, Black };
// struct of the list of the nodes
typedef struct list_node {
    int index;
    int time;
    int distance;
    int cost;
    struct list_node* next;
} list_node;
// the struct of each node
typedef struct node {
    int data;
    enum color color;
    list_node* head;
} node;
// the struct of the graphs
typedef struct graph {
    int nVertices;
    node heads[];
} graph;
// the struct of the edges
typedef struct Edge {
    int source;
    int dest;
    int time;
    int distance;
    int cost;
} Edge;
// the array of edge lists
Edge edgeList[100];  
int edgeCount = 0;  
// for adding new nodes to the graph
node* new_node(int data) {
    node* z = (node*)malloc(sizeof(node));
    z->data = data;
    z->head = NULL;
    z->color = White;
    return z;
}

list_node* new_list_node(int item_index, int time, int distance, int cost) {
    list_node* z = (list_node*)malloc(sizeof(list_node));
    z->index = item_index;
    z->time = time;
    z->distance = distance;
    z->cost = cost;
    z->next = NULL;
    return z;
}
// adding new graphs
graph* new_graph(int nVertices) {
    graph* g = (graph*)malloc(sizeof(graph) + (nVertices * sizeof(node)));
    g->nVertices = nVertices;
    int i;
    for (i = 0; i < nVertices; i++) {
        node* z = new_node(-1);
        g->heads[i] = *z;
    }
    return g;
}
// for adding new nodes to the graph
void add_node_to_graph(graph* g, int data) {
    node* z = new_node(data);
    int i;
    for (i = 0; i < g->nVertices; i++) {
        if (g->heads[i].data < 0) {
            g->heads[i] = *z;
            break;
        }
    }
}

int in_graph_head_list(graph* g, int data) {
    int i;
    for (i = 0; i < g->nVertices; i++) {
        if (g->heads[i].data == data)
            return 1;
    }
    return 0;
}

int compareEdges(const Edge* edge1, const Edge* edge2, int choice) {
    // sort based on the user-selected criteria
    switch (choice) {
        //comparing based on the criteria
        case TIME: return (edge1->time > edge2->time) - (edge1->time < edge2->time);
        case DISTANCE: return (edge1->distance > edge2->distance) - (edge1->distance < edge2->distance);
        case COST: return (edge1->cost > edge2->cost) - (edge1->cost < edge2->cost);
        default: return 0; 
    }
}
void update(graph* g, graph* g2, Edge* edge, int choice) {
    //defining the criterias of each edge
    int source = edge->source;
    int dest = edge->dest;
    int time = edge->time;
    int distance = edge->distance;
    int cost = edge->cost;

    if (!in_graph_head_list(g2, source)) {
        add_node_to_graph(g2, source);
    }
    if (!in_graph_head_list(g2, dest)) {
        add_node_to_graph(g2, dest);
    }
    // Exclude self-loop edges
    if (source == dest) {
        return;
    }
    // Check if an edge with the same source and destination already exists in g2
    int exist = 0;
    node* firstNode = NULL;
    node* lastNode = NULL;

    for (int i = 0; i < g2->nVertices; i++) {
        if (g2->heads[i].data == source) {
            firstNode = &(g2->heads[i]);
        }
        if (g2->heads[i].data == dest) {
            lastNode = &(g2->heads[i]);
        }
    }
    if (firstNode != NULL && lastNode != NULL) {
        list_node* temp = firstNode->head;
        while (temp != NULL) {
            if (temp->index == lastNode - g2->heads) {
                exist = 1;
                break;
            }
            temp = temp->next;
        }
    }

    if (!exist) {
        // if the edge was not added before, Add the edge to g2
        list_node* n = new_list_node(lastNode - g2->heads, time, distance, cost);
        if (firstNode->head == NULL) {
            firstNode->head = n;
        } else {
            list_node* temp = firstNode->head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = n;
        }
    }
}
//printing the graph
void printgraph(graph* g) {
    int i;
    for (i = 0; i < g->nVertices; i++) {
        node* curr_node = &(g->heads[i]);
        printf("Node %d: ", curr_node->data);
        list_node* temp = curr_node->head;
        if (temp == NULL) {
            printf("No adjacent nodes.\n");
        } else {
            while (temp != NULL) {
                int adjacent_vertex = g->heads[temp->index].data;
                printf("%d (T=%d, D=%d, C=%d)", adjacent_vertex, temp->time, temp->distance, temp->cost);
                temp = temp->next;
                if (temp != NULL) {
                    printf(" --> ");
                }
            }
            printf("\n");
        }
    }
}

int compareTime(const void* temp1, const void* temp2) {
    return compareEdges(temp1, temp2, TIME);
}

int compareDistance(const void* temp1, const void* temp2) {
    return compareEdges(temp1, temp2, DISTANCE);
}

int compareCost(const void* temp1, const void* temp2) {
    return compareEdges(temp1, temp2, COST);
}

void add_edge(graph* g, int source, int dest, int time, int distance, int cost) {
    if (!in_graph_head_list(g, source)) {
        add_node_to_graph(g, source);
    }
    if (!in_graph_head_list(g, dest)) {
        add_node_to_graph(g, dest);
    }
    int i, j;
    for (i = 0; i < g->nVertices; i++) {
        if (g->heads[i].data == source) {
            int indexDest = -1;
            for (j = 0; j < g->nVertices; j++) {
                if (g->heads[j].data == dest) {
                    indexDest = j;
                    break;
                }
            }
            if (indexDest != -1) {
                //adding to the list node, the items
                list_node* n = new_list_node(indexDest, time, distance, cost);
                if (g->heads[i].head == NULL) {
                    g->heads[i].head = n;
                } else {
                    list_node* temp = g->heads[i].head;
                    while (temp->next != NULL) {
                        temp = temp->next;
                    }
                    temp->next = n;
                }
                //making an array of edge list and add the properties to it
                edgeList[edgeCount].source = source;
                edgeList[edgeCount].dest = dest;
                edgeList[edgeCount].time = time;
                edgeList[edgeCount].distance = distance;
                edgeList[edgeCount].cost = cost;
                edgeCount++;
            }
            break;
        }
    }
}
// function for user choice
int userChoice() {
    printf("\nBased on what do you want to do the elimination?\n");
    printf("1. Time 2. Distance 3. Cost\n");
    int choice;
    scanf("%d", &choice);
    return choice;
}
// struct for making queue, enqueue, dequeue and printing queue
typedef struct queue_node {
  node *n;
  struct queue_node *next;
}queue_node;

 
struct queue
{
    int count;
    queue_node *front;
    queue_node *rear;
};
typedef struct queue queue;

 

int is_empty_queue(queue *q)
{
  return !(q->count);
}

 

void enqueue(queue *q, node *n)
{
  queue_node *new_queue_node;
  new_queue_node = malloc(sizeof(queue_node));
  new_queue_node->n = n;
  new_queue_node->next = NULL;
  if(!is_empty_queue(q))
  {
    q->rear->next = new_queue_node;
    q->rear = new_queue_node;
  }
  else
  {
    q->front = q->rear = new_queue_node;
  }
  q->count++;
}

 

queue_node* dequeue(queue *q)
{
  queue_node *tmp;
  tmp = q->front;
  q->front = q->front->next;
  q->count--;
  return tmp;
}

 

queue* make_queue()
{
  queue *q;
  q = malloc(sizeof(queue));
  q->count = 0;
  q->front = NULL;
  q->rear = NULL;
  return q;
}


void cycleRemoving(graph* g, int start, int end) {
    // Allocate memory
    int* queue = malloc(g->nVertices * sizeof(int));
    int front = 0;
    int rear = 0;
    start--;
    g->heads[start].color = Gray;
    queue[rear++] = start;

    // Process nodes in the queue
    while (front < rear) {
        int current = queue[front++];
        printf("Node %d)\n", g->heads[current].data);
        list_node* temp = g->heads[current].head;
        list_node* pre = NULL;

        // Traverse the adjacency list of the current node
        while (temp != NULL) {
            int adjacent = temp->index;
            if (g->heads[adjacent].color != White) {
                printf("Elimination of %d -> %d\n", g->heads[current].data, g->heads[adjacent].data);
                // Remove the adjacent node from the list
                if (pre != NULL) {
                    pre->next = temp->next;
                    free(temp);
                    temp = pre->next;
                } else {
                    g->heads[current].head = temp->next;
                    free(temp);
                    temp = g->heads[current].head;
                }
            } else {
                g->heads[adjacent].color = Gray;
                queue[rear++] = adjacent;
                pre = temp;
                temp = temp->next;
            }
        }
        g->heads[current].color = Black;
        printf("Result of elimination cycle:\n");
        printgraph(g);
        printf("...........\n");
        // Check if the current node is equal to the end
        if (g->heads[current].data == end) {
            break;
        }
    }
    free(queue);
}

// for deleting the elements of the graph and making the allocated memory free.
void deletion(graph* g) {
    for (int i = 0; i < g->nVertices; i++) {
        list_node* temp = g->heads[i].head;
        while (temp != NULL) {
            list_node* pre = temp;
            temp = temp->next;
            free(pre);
        }
    }
    free(g);
}
void AddEdges(graph *g) {
    add_edge(g, 1, 2, 1, 1, 1);
    add_edge(g, 1, 3, 1, 1, 1);
    add_edge(g, 1, 4, 2, 1, 9);
    add_edge(g, 1, 2, 1, 8, 4);
    add_edge(g, 2, 4, 1, 4, 5);
    add_edge(g, 2, 3, 3, 2, 1);
    add_edge(g, 2, 1, 2, 1, 3);
    add_edge(g, 3, 1, 7, 1, 4);
    add_edge(g, 3, 1, 1, 3, 2);
    add_edge(g, 3, 4, 4, 3, 1);
    add_edge(g, 3, 4, 1, 7, 6);
    add_edge(g, 4, 4, 5, 3, 2);
    add_edge(g, 4, 3, 1, 1, 1);
    add_edge(g, 4, 2, 1, 4, 5);
    
}
int main(void) {
    graph* g = new_graph(4);
    graph* g2 = new_graph(4);
    printf("Welcome to the trip graph.\n");
    printf("The trip roads are as below.\n\n");
    AddEdges(g);
    printgraph(g);

    int start, end;
    printf("Enter the start node: ");
    scanf("%d", &start);
    printf("Enter the end node: ");
    scanf("%d", &end);
    
    int choice = userChoice();
    if (choice < 1 || choice > 3) {
        printf("Please enter your choice again.\n");
        return 0;
    }
    // Sort the edge list based on the user-selected criteria
    switch (choice-1) {
        case TIME:
            qsort(edgeList, edgeCount, sizeof(Edge), compareTime);
            break;
        case DISTANCE:
            qsort(edgeList, edgeCount, sizeof(Edge), compareDistance);
            break;
        case COST:
            qsort(edgeList, edgeCount, sizeof(Edge), compareCost);
            break;
    }
    printf("...........\n");
    printf("\nGraph after elimination of edges:\n");
   
    ////Update the graph
    for (int i = 0; i < edgeCount; i++) {
        update(g, g2, &edgeList[i], choice);
    }
    printgraph(g2);
    printf("...........\n");
    deletion(g);
    printf("\nCycle Removing:\n");
    cycleRemoving(g2, start, g2->nVertices - 1);
    printf("\nThe result is a DAG!\n");


    deletion(g2);

    return 0;
}

/*PSEUDOCODE

DEFINE enum { TIME, DISTANCE, COST, INVALID }
DEFINE enum color { White, Gray, Black }

// struct of the list of the nodes
STRUCT list_node {
    INT index
    INT time
    INT distance
    INT cost
    STRUCT list_node* next
}

// the struct of each node
STRUCT node {
    INT data
    enum color color
    list_node* head
}

// the struct of the graphs
STRUCT graph {
    INT nVertices
    node heads[]
}

// the struct of the edges
STRUCT Edge {
    INT source
    INT dest
    INT time
    INT distance
    INT cost
}

// the array of edge lists
Edge edgeList[100]
INT edgeCount = 0

// for adding new nodes to the graph
FUNCTION new_node(data)
    CREATE node z
    z.data = data
    z.head = NULL
    z.color = White
    RETURN z

FUNCTION new_list_node(item_index, time, distance, cost)
    CREATE list_node z
    z.index = item_index
    z.time = time
    z.distance = distance
    z.cost = cost
    z.next = NULL
    RETURN z

// adding new graphs
FUNCTION new_graph(nVertices)
    CREATE graph g
    g.nVertices = nVertices
    FOR i = 0 TO nVertices
        node* z = new_node(-1)
        g.heads[i] = *z
    RETURN g

// for adding new nodes to the graph
FUNCTION add_node_to_graph(g, data)
    node* z = new_node(data)
    FOR i = 0 TO g.nVertices
        IF g.heads[i].data < 0 THEN
            g.heads[i] = *z
            BREAK
        END IF
    END FOR

FUNCTION in_graph_head_list(g, data)
    FOR i = 0 TO g.nVertices
        IF g.heads[i].data == data THEN
            RETURN 1
        END IF
    END FOR
    RETURN 0

FUNCTION compareEdges(edge1, edge2, choice)
    SWITCH choice
        CASE TIME:
            RETURN (edge1.time > edge2.time) - (edge1.time < edge2.time)
        CASE DISTANCE:
            RETURN (edge1.distance > edge2.distance) - (edge1.distance < edge2.distance)
        CASE COST:
            RETURN (edge1.cost > edge2.cost) - (edge1.cost < edge2.cost)
        DEFAULT:
            RETURN 0

FUNCTION update(g, g2, edge, choice)
    source = edge.source
    dest = edge.dest
    time = edge.time
    distance = edge.distance
    cost = edge.cost

    IF !in_graph_head_list(g2, source) THEN
        add_node_to_graph(g2, source)
    END IF
    IF !in_graph_head_list(g2, dest) THEN
        add_node_to_graph(g2, dest)
    END IF

    IF source == dest THEN
        RETURN
    END IF

    exist = 0
    firstNode = NULL
    lastNode = NULL

    FOR i = 0 TO g2.nVertices
        IF g2.heads[i].data == source THEN
            firstNode = &(g2.heads[i])
        END IF
        IF g2.heads[i].data == dest THEN
            lastNode = &(g2.heads[i])
        END IF
    END FOR

    IF firstNode != NULL AND lastNode != NULL THEN
        temp = firstNode.head
        WHILE temp != NULL
            IF temp.index == lastNode - g2.heads THEN
                exist = 1
                BREAK
            END IF
            temp = temp.next
        END WHILE
    END IF

    IF exist == 0 THEN
        n = new_list_node(lastNode - g2.heads, time, distance, cost)
        IF firstNode.head == NULL THEN
            firstNode.head = n
        ELSE
            temp = firstNode.head
            WHILE temp.next != NULL
                temp = temp.next
            END WHILE
            temp.next = n
        END IF
    END IF
END FUNCTION

FUNCTION printgraph(g)
    FOR i = 0 TO g.nVertices
        curr_node = &(g.heads[i])
        PRINT "Node " + curr_node.data + ": "
        temp = curr_node.head
        IF temp == NULL THEN
            PRINT "No adjacent nodes."
        ELSE
            WHILE temp != NULL
                adjacent_vertex = g.heads[temp.index].data
                PRINT adjacent_vertex + "(T=" + temp.time + ", D=" + temp.distance + ", C=" + temp.cost + ")"
                temp = temp.next
                IF temp != NULL THEN
                    PRINT " --> "
                END IF
            END WHILE
        END IF
    END FOR
END FUNCTION

FUNCTION compareTime(temp1, temp2)
    RETURN compareEdges(temp1, temp2, TIME)
END FUNCTION

FUNCTION compareDistance(temp1, temp2)
    RETURN compareEdges(temp1, temp2, DISTANCE)
END FUNCTION

FUNCTION compareCost(temp1, temp2)
    RETURN compareEdges(temp1, temp2, COST)
END FUNCTION

FUNCTION add_edge(g, source, dest, time, distance, cost)
    IF !in_graph_head_list(g, source) THEN
        add_node_to_graph(g, source)
    END IF
    IF !in_graph_head_list(g, dest) THEN
        add_node_to_graph(g, dest)
    END IF

    FOR i = 0 TO g.nVertices
        IF g.heads[i].data == source THEN
            indexDest = -1
            FOR j = 0 TO g.nVertices
                IF g.heads[j].data == dest THEN
                    indexDest = j
                    BREAK
                END IF
            END FOR
            IF indexDest != -1 THEN
                n = new_list_node(indexDest, time, distance, cost)
                IF g.heads[i].head == NULL THEN
                    g.heads[i].head = n
                ELSE
                    temp = g.heads[i].head
                    WHILE temp.next != NULL
                        temp = temp.next
                    END WHILE
                    temp.next = n
                END IF
                edgeList[edgeCount].source = source
                edgeList[edgeCount].dest = dest
                edgeList[edgeCount].time = time
                edgeList[edgeCount].distance = distance
                edgeList[edgeCount].cost = cost
                edgeCount++
            END IF
            BREAK
        END IF
    END FOR
END FUNCTION

FUNCTION userChoice()
    PRINT "Based on what do you want to do the elimination?"
    PRINT "1. Time 2. Distance 3. Cost"
    READ choice
    RETURN choice
END FUNCTION

FUNCTION is_empty_queue(q)
    RETURN q.count == 0
END FUNCTION

FUNCTION enqueue(q, n)
    new_queue_node = allocate memory for queue_node
    new_queue_node.n = n
    new_queue_node.next = NULL
    IF !is_empty_queue(q) THEN
        q.rear.next = new_queue_node
        q.rear = new_queue_node
    ELSE
        q.front = q.rear = new_queue_node
    END IF
    q.count++
END FUNCTION

FUNCTION dequeue(q)
    tmp = q.front
    q.front = q.front.next
    q.count--
    RETURN tmp
END FUNCTION

FUNCTION make_queue()
    q = allocate memory for queue
    q.count = 0
    q.front = NULL
    q.rear = NULL
    RETURN q
END FUNCTION

FUNCTION cycleRemoving(g, start, end)
    queue = allocate memory for integer array of size g.nVertices
    front = 0
    rear = 0
    start = start - 1
    g.heads[start].color = Gray
    queue[rear++] = start

    WHILE front < rear
        current = queue[front++]
        PRINT "Node " + g.heads[current].data + ")"
        temp = g.heads[current].head
        pre = NULL

        WHILE temp != NULL
            adjacent = temp.index
            IF g.heads[adjacent].color != White THEN
                PRINT "Elimination of " + g.heads[current].data + " -> " + g.heads[adjacent].data
                IF pre != NULL THEN
                    pre.next = temp.next
                    free(temp)
                    temp = pre.next
                ELSE
                    g.heads[current].head = temp.next
                    free(temp)
                    temp = g.heads[current].head
                END IF
            ELSE
                g.heads[adjacent].color = Gray
                queue[rear++] = adjacent
                pre = temp
                temp = temp.next
            END IF
        END WHILE

        g.heads[current].color = Black
        PRINT "Result of elimination cycle:"
        printgraph(g)
        IF g.heads[current].data == end THEN
            BREAK
        END IF
    END WHILE

    free(queue)
END FUNCTION

FUNCTION deletion(g)
    FOR i = 0 TO g.nVertices
        temp = g.heads[i].head
        WHILE temp != NULL
            pre = temp
            temp = temp.next
            free(pre)
        END WHILE
    END FOR
    free(g)
END FUNCTION

FUNCTION main()
    g = new_graph(4)
    g2 = new_graph(4)
    add_edge()
    PRINT "Enter the start node: "
    READ start
    PRINT "Enter the end node: "
    READ end
    choice = userChoice()
    IF choice < 1 OR choice > 3 THEN
        PRINT "Please enter your choice again."
        RETURN 0
    END IF
    // Sort the edge list based on the user-selected criteria
    SWITCH choice-1
        CASE TIME:
            qsort(edgeList, edgeCount, sizeof(Edge), compareTime)
        CASE DISTANCE:
            qsort(edgeList, edgeCount, sizeof(Edge), compareDistance)
        CASE COST:
            qsort(edgeList, edgeCount, sizeof(Edge), compareCost)
    END SWITCH
    PRINT "Graph after elimination of edges:"
    // Update the graph
    FOR i = 0 TO edgeCount
        update(g, g2, edgeList[i], choice)
    END FOR
    printgraph(g2)
    deletion(g)
    PRINT "Cycle Removing:"
    cycleRemoving(g2, start, g2.nVertices - 1)
    deletion(g2)
    RETURN 0
END FUNCTION


        
*/