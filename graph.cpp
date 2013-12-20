#include <QtGlobal>
#include <QStack>
#include <vector>
#include "graph.h"

#define NOT_IN_QUEUE 0

using namespace std;

Graph::Graph(int num_node)
{
    this->N = num_node;
    this->E = 0;
    for (int i = 0; i < num_node; ++i)
        this->adj.push_back(QLinkedList<WeightedEdge>());
}

Graph::~Graph()
{
}

void Graph::connect(int v, int w, int length)
{
    this->addEdge(WeightedEdge(v, w, length));
    this->addEdge(WeightedEdge(w, v, length));
    this->E ++;
}

void Graph::addEdge(WeightedEdge edge)
{
    this->adj[edge.from] << edge;
}

QLinkedList<WeightedEdge> Graph::edges(int v)
{
    return this->adj[v];
}

// assume from != to and 0 <= from|to < N
QLinkedList<int> Graph::shortestPath(int from, int to)
{
    QLinkedList<int> path;
    if (from == to)
    {
        path << from;
        return path;
    }

    vector<int> parent(this->N);
    vector<int> dist(this->N);
    vector<bool> computed(this->N);
    vector<int> queue(this->N); // defaults to all NOT_IN_QUEUE

    computed[from] = true;
    parent[from] = from;
    foreach(WeightedEdge edge, this->edges(from))
    {
        parent[edge.to] = edge.from;
        queue[edge.to] = edge.weight;
    }

    while (true)
    {
        int v = this->nextNodeInQueue(queue);
        dist[v] = queue[v];
        computed[v] = true;
        queue[v] = NOT_IN_QUEUE;
        if (v == to)
            break;

        foreach(WeightedEdge edge, this->edges(v))
        {
            if (computed[edge.to])
                continue;
            if (queue[edge.to] == NOT_IN_QUEUE || queue[edge.to] > dist[v] + edge.weight)
            {
                queue[edge.to] = dist[v] + edge.weight;
                parent[edge.to] = v;
            }
        }
    }

    // get the path from `parent`
    QStack<int> stack;
    for (int v = to; parent[v] != v; v = parent[v])
        stack.push(v);
    stack.push(from);

    while (!stack.isEmpty())
        path << stack.pop();

    return path;
}

int Graph::nextNodeInQueue(const vector<int> &queue)
{
    int minVal = 999999;
    int minNode = -1;

    for(size_t i = 0; i < queue.size(); i++)
    {
        if (queue[i] != NOT_IN_QUEUE && minVal > queue[i])
        {
            minVal = queue[i];
            minNode = i;
        }
    }

    return minNode;
}
