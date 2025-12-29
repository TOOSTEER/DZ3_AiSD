#include "list_graph.h"

ListGraph::ListGraph(int vertices) : adjacencyLists(vertices), prevAdjacencyLists(vertices) {}

ListGraph::ListGraph(const IGraph& other) : adjacencyLists(other.VerticesCount()), prevAdjacencyLists(other.VerticesCount()) {
    for (int i = 0; i < other.VerticesCount(); ++i) {
        for (int next : other.GetNextVertices(i)) {
            AddEdge(i, next);
        }
    }
}

void ListGraph::AddEdge(int from, int to) {
    adjacencyLists[from].push_back(to);
    prevAdjacencyLists[to].push_back(from);
}

int ListGraph::VerticesCount() const {
    return adjacencyLists.size();
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const {
    return adjacencyLists[vertex];
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const {
    return prevAdjacencyLists[vertex];
}