#include <QLinkedList>
#include <vector>

#ifndef GRAPH_H
#define GRAPH_H

// Sample Usage:
//
//Graph graph(9);
//graph.connect(0,1,2);
//graph.connect(1,2,3);
//graph.connect(2,3,3);

//graph.connect(6,7,3);
//graph.connect(7,8,3);

//graph.connect(0,6,3);
//graph.connect(1,7,4);
//graph.connect(2,4,2);
//graph.connect(4,8,2);
//graph.connect(3,5,2);

//QLinkedList<int> path = graph.shortestPath(6,2);
//foreach(int i, path)
//{
//    qDebug() << i << ",";
//}

class WeightedEdge
{
public:
    friend class Graph;
    WeightedEdge(int from, int to, int weight): from(from), to(to), weight(weight) {}

private:
    int from;
    int to;
    int weight;
};

class Graph
{
public:
    Graph(int num_node);
    ~Graph();

    void connect(int v, int w, int length);
    QLinkedList<WeightedEdge> edges(int v);

    QLinkedList<int> shortestPath(int from, int to);

private:
    void addEdge(WeightedEdge edge);
    int nextNodeInQueue(const std::vector<int> &queue);
    int N;
    int E;
    std::vector< QLinkedList<WeightedEdge> > adj;
};

#endif // GRAPH_H
