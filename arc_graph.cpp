#include "arc_graph.h"

ArcGraph::ArcGraph(int vertices) : verticesCount(vertices) {}

ArcGraph::ArcGraph(const IGraph& other) : verticesCount(other.VerticesCount()) {
    for (int i = 0; i < other.VerticesCount(); ++i) {
        for (int next : other.GetNextVertices(i)) {
            edges.emplace_back(i, next);
        }
    }
}

void ArcGraph::AddEdge(int from, int to) {
    edges.emplace_back(from, to);
}

int ArcGraph::VerticesCount() const {
    return verticesCount;
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
    std::vector<int> result;
    for (const auto& edge : edges) {
        if (edge.first == vertex) result.push_back(edge.second);
    }
    return result;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
    std::vector<int> result;
    for (const auto& edge : edges) {
        if (edge.second == vertex) result.push_back(edge.first);
    }
    return result;
}