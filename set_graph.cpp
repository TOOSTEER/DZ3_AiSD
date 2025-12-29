#include "set_graph.h"

SetGraph::SetGraph(int vertices) : adjacencySets(vertices), prevAdjacencySets(vertices) {}

SetGraph::SetGraph(const IGraph& other) : adjacencySets(other.VerticesCount()), prevAdjacencySets(other.VerticesCount()) {
    for (int i = 0; i < other.VerticesCount(); ++i) {
        for (int next : other.GetNextVertices(i)) {
            AddEdge(i, next);
        }
    }
}

void SetGraph::AddEdge(int from, int to) {
    adjacencySets[from].insert(to);
    prevAdjacencySets[to].insert(from);
}

int SetGraph::VerticesCount() const {
    return adjacencySets.size();
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const {
    return std::vector<int>(adjacencySets[vertex].begin(), adjacencySets[vertex].end());
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
    return std::vector<int>(prevAdjacencySets[vertex].begin(), prevAdjacencySets[vertex].end());
}