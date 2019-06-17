#include <iostream>

using namespace std;

class AdjListNode {
    public:
        int destination;
        int weight;

        AdjListNode *next;
};

class AdjList {
    public:
        AdjListNode *head;
};

class Graph {
    public:
        int numberofvertices;
        AdjList *array;
};

AdjListNode *newAdjListNode(int destination, int weight) {
    AdjListNode *newNode = new AdjListNode();
    newNode->destination = destination;
    newNode->weight      = weight;
    newNode->next        = NULL;

    return newNode;
}

Graph *createGraph(int numberofvertices) {
    Graph *graph = new Graph();

    graph->numberofvertices = numberofvertices;

    graph->array = new AdjList[numberofvertices];

    for(int i=0; i<numberofvertices; i++) {
        graph->array[i].head = NULL;
    }

    return graph;
}

void addEdge(Graph *graph, int source, int destination, int weight) {
    AdjListNode *newNode = newAdjListNode(destination, weight);

    newNode->next = graph->array[source].head;
    graph->array[source].head = newNode;

    newNode = newAdjListNode(source, weight);

    newNode->next = graph->array[destination].head;
    graph->array[destination].head = newNode;
}

class MinHeapNode {
    public:
        int vertex;
        int key;
};

class MinHeap {
    public:
        int size;
        int capacity;
        int *pos;
        MinHeapNode **array;
};

MinHeapNode *newMinHeapNode(int vertex, int key) {
    MinHeapNode *minHeapNode = new MinHeapNode();
    minHeapNode->vertex = vertex;
    minHeapNode->key = key;
    return minHeapNode;
}

MinHeap *createMinHeap(int capacity) {
    MinHeap *minHeap = new MinHeap();
    minHeap->pos = new int[capacity];
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = new MinHeapNode*[capacity];
    return minHeap;
}

void swapMinHeapNode(MinHeapNode **a, MinHeapNode **b) {
    MinHeapNode *t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(MinHeap *minHeap, int idx) {
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if(left < minHeap->size && minHeap->array[left]->key < minHeap->array[smallest]->key) {
        smallest = left;
    }
    if(right < minHeap->size && minHeap->array[right]->key < minHeap->array[smallest]->key) {
        smallest = right;
    }

    if(smallest != idx) {
        MinHeapNode *smallestNode = minHeap->array[smallest];
        MinHeapNode *idxNode      = minHeap->array[idx];

        minHeap->pos[smallestNode->vertex] = idx;
        minHeap->pos[idxNode->vertex] = smallest;

        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

        minHeapify(minHeap, smallest);
    }
}

int isEmpty(MinHeap *minHeap) {
    return minHeap->size == 0;
}

MinHeapNode *extractMin(MinHeap *minHeap) {
    if (isEmpty(minHeap)) {
        return NULL;
    }

    MinHeapNode *root = minHeap->array[0];

    MinHeapNode *lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;

    minHeap->pos[root->vertex] = minHeap->size - 1;
    minHeap->pos[lastNode->vertex] = 0;

    --minHeap->size;
    minHeapify(minHeap, 0);

    return root;
}

void decreaseKey(MinHeap *minHeap, int vertex, int key) {
    int i = minHeap->pos[vertex];

    minHeap->array[i]->key = key;

    while(i && minHeap->array[i]->key <  minHeap->array[(i-1)/2]->key) {
        minHeap->pos[minHeap->array[i]->vertex] = (i-1)/2;
        minHeap->pos[minHeap->array[(i-1)/2]->vertex] = i;

        swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i-1)/2]);

        i = (i-1)/2;
    }
}

bool isInMinHeap(MinHeap *minHeap, int vertex) {
    if(minHeap->pos[vertex] < minHeap->size) {
        return true;
    }

    return false;
}

void print(int arr[], int n) {
    for(int i=1; i<n; ++i) {
        cout << i << " - " << arr[i] << endl;
    }
}

void dijkstra(Graph *graph, int src) {
    int V = graph->numberofvertices;

    int dist[V];

    MinHeap *minHeap = createMinHeap(V);

    for(int v=0; v<V; ++v) {
        dist[v] = INT_MAX;

        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->pos[v] = v;
    }

    minHeap->array[src] = newMinHeapNode(src, dist[src]);
    minHeap->pos[src]   = src;
    dist[src] = 0;
    decreaseKey(minHeap, src, dist[src]);

    minHeap->size = V;

    while(!isEmpty(minHeap)) {
        MinHeapNode *minHeapNode = extractMin(minHeap);
        int u = minHeapNode->vertex;

        AdjListNode *pCrawl = graph->array[u].head;
        while(pCrawl != NULL) {
            int v = pCrawl->destination;

            if(isInMinHeap(minHeap, v) && dist[u] != INT_MAX
                                       && pCrawl->weight + dist[u] < dist[v]) {
                dist[v] = dist[u] + pCrawl->weight;
                decreaseKey(minHeap, v, dist[v]);
            }

            pCrawl = pCrawl->next;
        }
    }

    print(dist, V);
}

int main() {
    int V = 9;
    Graph *graph = createGraph(V);

    addEdge(graph, 0, 1, 4);
    addEdge(graph, 0, 7, 8);
    addEdge(graph, 1, 2, 8);
    addEdge(graph, 1, 7, 11);
    addEdge(graph, 2, 3, 7);
    addEdge(graph, 2, 8, 2);
    addEdge(graph, 2, 5, 4);
    addEdge(graph, 3, 4, 9);
    addEdge(graph, 3, 5, 14);
    addEdge(graph, 4, 5, 10);
    addEdge(graph, 5, 6, 2);
    addEdge(graph, 6, 7, 1);
    addEdge(graph, 6, 8, 6);
    addEdge(graph, 7, 8, 7);

    dijkstra(graph, 0);

    return 0;
}
