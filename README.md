# Реализации графов на C++

Этот проект содержит несколько реализаций интерфейса ориентированного графа на C++.

## Структура проекта

Проект состоит из следующих файлов:

### Основной интерфейс
- **igraph.h** - базовый интерфейс `IGraph` для представления ориентированного графа

### Реализации графов

1. **ListGraph**
   - `list_graph.h` / `list_graph.cpp`
   - Хранит граф в виде массива списков смежности

2. **MatrixGraph**
   - `matrix_graph.h` / `matrix_graph.cpp`
   - Хранит граф в виде матрицы смежности

3. **SetGraph**
   - `set_graph.h` / `set_graph.cpp`
   - Хранит граф в виде массива хэш-таблиц

4. **ArcGraph**
   - `arc_graph.h` / `arc_graph.cpp`
   - Хранит граф в виде массива пар {from, to}

## Интерфейс IGraph

Все реализации предоставляют одинаковый интерфейс:

```cpp
struct IGraph {
    virtual ~IGraph() = default;
    virtual void AddEdge(int from, int to) = 0;          // Добавить ребро от from к to
    virtual int VerticesCount() const = 0;              // Количество вершин
    virtual std::vector<int> GetNextVertices(int vertex) const = 0;  // Соседи по исходящим рёбрам
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;  // Соседи по входящим рёбрам
};