#include "igraph.h"
#include <vector>

class ArcGraph : public IGraph {
public:
    explicit ArcGraph(int vertices);
    explicit ArcGraph(const IGraph& other);
    ~ArcGraph() override = default;

    void AddEdge(int from, int to) override;
    int VerticesCount() const override;
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    int verticesCount;
    std::vector<std::pair<int, int>> edges;
};