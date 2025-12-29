#include "matrix_graph.h"

MatrixGraph::MatrixGraph(int vertices) : adjacencyMatrix(vertices, std::vector<bool>(vertices, false)) {}

MatrixGraph::MatrixGraph(const IGraph& other) : adjacencyMatrix(other.VerticesCount(), std::vector<bool>(other.VerticesCount(), false)) {
    for (int i = 0; i < other.VerticesCount(); ++i) {
        for (int next : other.GetNextVertices(i)) {
            AddEdge(i, next);
        }
    }
}

void MatrixGraph::AddEdge(int from, int to) {
    adjacencyMatrix[from][to] = true;
}

int MatrixGraph::VerticesCount() const {
    return adjacencyMatrix.size();
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
    std::vector<int> result;
    for (int i = 0; i < adjacencyMatrix.size(); ++i) {
        if (adjacencyMatrix[vertex][i]) result.push_back(i);
    }
    return result;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
    std::vector<int> result;
    for (int i = 0; i < adjacencyMatrix.size(); ++i) {
        if (adjacencyMatrix[i][vertex]) result.push_back(i);
    }
    return result;
}