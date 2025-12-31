#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>

// Подключаем все заголовочные файлы
#include "igraph.h"
#include "list_graph.h"
#include "matrix_graph.h"
#include "set_graph.h"
#include "arc_graph.h"

// ============================================
// Helper functions
// ============================================

void print_test_header(const std::string& test_name) {
    std::cout << "\n=============================================\n";
    std::cout << "  Test: " << test_name << "\n";
    std::cout << "=============================================\n";
}

void print_success() {
    std::cout << "  [SUCCESS]\n";
}

void print_failure(const std::string& message) {
    std::cout << "  [ERROR]: " << message << "\n";
}

template<typename T>
bool vectors_equal(const std::vector<T>& v1, const std::vector<T>& v2) {
    auto v1_sorted = v1;
    auto v2_sorted = v2;
    std::sort(v1_sorted.begin(), v1_sorted.end());
    std::sort(v2_sorted.begin(), v2_sorted.end());
    return v1_sorted == v2_sorted;
}

// ============================================
// Test 1: Empty graph
// ============================================

template<typename GraphType>
void test_empty_graph(const std::string& graph_name) {
    std::cout << "  " << graph_name << ": ";

    try {
        GraphType graph(5);
        assert(graph.VerticesCount() == 5);

        for (int i = 0; i < 5; ++i) {
            assert(graph.GetNextVertices(i).empty());
            assert(graph.GetPrevVertices(i).empty());
        }

        print_success();
    }
    catch (const std::exception& e) {
        print_failure(e.what());
        throw;
    }
}

void run_empty_graph_test() {
    print_test_header("Empty graph");

    test_empty_graph<ListGraph>("ListGraph");
    test_empty_graph<MatrixGraph>("MatrixGraph");
    test_empty_graph<SetGraph>("SetGraph");
    test_empty_graph<ArcGraph>("ArcGraph");
}

// ============================================
// Test 2: Single edge
// ============================================

template<typename GraphType>
void test_single_edge(const std::string& graph_name) {
    std::cout << "  " << graph_name << ": ";

    try {
        GraphType graph(3);
        graph.AddEdge(0, 1);

        auto next = graph.GetNextVertices(0);
        assert(next.size() == 1);
        assert(next[0] == 1);

        auto prev = graph.GetPrevVertices(1);
        assert(prev.size() == 1);
        assert(prev[0] == 0);

        // Check that other vertices are empty
        assert(graph.GetNextVertices(1).empty());
        assert(graph.GetPrevVertices(0).empty());

        print_success();
    }
    catch (const std::exception& e) {
        print_failure(e.what());
        throw;
    }
}

void run_single_edge_test() {
    print_test_header("Single edge");

    test_single_edge<ListGraph>("ListGraph");
    test_single_edge<MatrixGraph>("MatrixGraph");
    test_single_edge<SetGraph>("SetGraph");
    test_single_edge<ArcGraph>("ArcGraph");
}

// ============================================
// Test 3: Multiple edges
// ============================================

template<typename GraphType>
void test_multiple_edges(const std::string& graph_name) {
    std::cout << "  " << graph_name << ": ";

    try {
        GraphType graph(6);

        // Create structure: 0 -> 1, 0 -> 2, 1 -> 3, 2 -> 3, 3 -> 4, 4 -> 5
        graph.AddEdge(0, 1);
        graph.AddEdge(0, 2);
        graph.AddEdge(1, 3);
        graph.AddEdge(2, 3);
        graph.AddEdge(3, 4);
        graph.AddEdge(4, 5);

        // Check forward connections
        assert(vectors_equal(graph.GetNextVertices(0), { 1, 2 }));
        assert(vectors_equal(graph.GetNextVertices(1), { 3 }));
        assert(vectors_equal(graph.GetNextVertices(2), { 3 }));
        assert(vectors_equal(graph.GetNextVertices(3), { 4 }));
        assert(vectors_equal(graph.GetNextVertices(4), { 5 }));
        assert(graph.GetNextVertices(5).empty());

        // Check backward connections
        assert(graph.GetPrevVertices(0).empty());
        assert(vectors_equal(graph.GetPrevVertices(1), { 0 }));
        assert(vectors_equal(graph.GetPrevVertices(2), { 0 }));
        assert(vectors_equal(graph.GetPrevVertices(3), { 1, 2 }));
        assert(vectors_equal(graph.GetPrevVertices(4), { 3 }));
        assert(vectors_equal(graph.GetPrevVertices(5), { 4 }));

        print_success();
    }
    catch (const std::exception& e) {
        print_failure(e.what());
        throw;
    }
}

void run_multiple_edges_test() {
    print_test_header("Multiple edges");

    test_multiple_edges<ListGraph>("ListGraph");
    test_multiple_edges<MatrixGraph>("MatrixGraph");
    test_multiple_edges<SetGraph>("SetGraph");
    test_multiple_edges<ArcGraph>("ArcGraph");
}

// ============================================
// Test 4: Self-loop (edge from vertex to itself)
// ============================================

template<typename GraphType>
void test_self_loop(const std::string& graph_name) {
    std::cout << "  " << graph_name << ": ";

    try {
        GraphType graph(3);
        graph.AddEdge(0, 0);  // Self-loop
        graph.AddEdge(1, 2);

        // Self-loop should be visible both as next and prev
        auto next0 = graph.GetNextVertices(0);
        auto prev0 = graph.GetPrevVertices(0);

        assert(std::find(next0.begin(), next0.end(), 0) != next0.end());
        assert(std::find(prev0.begin(), prev0.end(), 0) != prev0.end());

        print_success();
    }
    catch (const std::exception& e) {
        print_failure(e.what());
        throw;
    }
}

void run_self_loop_test() {
    print_test_header("Self-loop");

    test_self_loop<ListGraph>("ListGraph");
    test_self_loop<MatrixGraph>("MatrixGraph");
    test_self_loop<SetGraph>("SetGraph");
    test_self_loop<ArcGraph>("ArcGraph");
}

// ============================================
// Test 5: Copy constructor
// ============================================

void test_copy_constructor_basic() {
    print_test_header("Basic copy constructor");

    try {
        // Create original graph
        ListGraph original(5);
        original.AddEdge(0, 1);
        original.AddEdge(0, 2);
        original.AddEdge(1, 3);
        original.AddEdge(2, 4);
        original.AddEdge(3, 4);

        std::cout << "  Copying ListGraph to other types:\n";

        // Copy to all graph types
        MatrixGraph matrixCopy(original);
        SetGraph setCopy(original);
        ArcGraph arcCopy(original);
        ListGraph listCopy(original);

        // Check vertex count
        assert(original.VerticesCount() == 5);
        assert(matrixCopy.VerticesCount() == 5);
        assert(setCopy.VerticesCount() == 5);
        assert(arcCopy.VerticesCount() == 5);
        assert(listCopy.VerticesCount() == 5);

        // Check all vertices
        for (int i = 0; i < 5; ++i) {
            auto orig_next = original.GetNextVertices(i);
            auto matrix_next = matrixCopy.GetNextVertices(i);
            auto set_next = setCopy.GetNextVertices(i);
            auto arc_next = arcCopy.GetNextVertices(i);
            auto list_next = listCopy.GetNextVertices(i);

            assert(vectors_equal(orig_next, matrix_next));
            assert(vectors_equal(orig_next, set_next));
            assert(vectors_equal(orig_next, arc_next));
            assert(vectors_equal(orig_next, list_next));
        }

        std::cout << "  All copies are identical to original: [SUCCESS]\n";

    }
    catch (const std::exception& e) {
        print_failure(e.what());
        throw;
    }
}

void test_copy_chain() {
    print_test_header("Copy chain");

    try {
        // Create copy chain: List -> Matrix -> Set -> Arc -> List
        ListGraph g1(4);
        g1.AddEdge(0, 1);
        g1.AddEdge(1, 2);
        g1.AddEdge(2, 3);
        g1.AddEdge(3, 0);  // Cycle

        MatrixGraph g2(g1);      // List -> Matrix
        SetGraph g3(g2);         // Matrix -> Set
        ArcGraph g4(g3);         // Set -> Arc
        ListGraph g5(g4);        // Arc -> List

        std::cout << "  Checking chain List -> Matrix -> Set -> Arc -> List:\n";

        // All graphs should be identical
        for (int i = 0; i < 4; ++i) {
            auto v1 = g1.GetNextVertices(i);
            auto v2 = g2.GetNextVertices(i);
            auto v3 = g3.GetNextVertices(i);
            auto v4 = g4.GetNextVertices(i);
            auto v5 = g5.GetNextVertices(i);

            assert(vectors_equal(v1, v2));
            assert(vectors_equal(v1, v3));
            assert(vectors_equal(v1, v4));
            assert(vectors_equal(v1, v5));
        }

        std::cout << "  Copy chain works: [SUCCESS]\n";

    }
    catch (const std::exception& e) {
        print_failure(e.what());
        throw;
    }
}

void run_copy_constructor_tests() {
    test_copy_constructor_basic();
    test_copy_chain();
}

// ============================================
// Test 6: Graph with zero vertices
// ============================================

void test_zero_vertices() {
    print_test_header("Graph with zero vertices (boundary case)");

    try {
        ListGraph lg(0);
        MatrixGraph mg(0);
        SetGraph sg(0);
        ArcGraph ag(0);

        assert(lg.VerticesCount() == 0);
        assert(mg.VerticesCount() == 0);
        assert(sg.VerticesCount() == 0);
        assert(ag.VerticesCount() == 0);

        std::cout << "  All implementations support 0 vertices: [SUCCESS]\n";

    }
    catch (const std::exception& e) {
        print_failure(e.what());
        throw;
    }
}

// ============================================
// Test 7: Performance (simplified)
// ============================================

template<typename GraphType>
void measure_performance(const std::string& graph_name, int vertices, int edges) {
    auto start = std::chrono::high_resolution_clock::now();

    GraphType graph(vertices);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, vertices - 1);

    // Add edges
    for (int i = 0; i < edges; ++i) {
        int from = dist(gen);
        int to = dist(gen);
        graph.AddEdge(from, to);
    }

    auto mid = std::chrono::high_resolution_clock::now();

    // Query neighbors
    long total_neighbors = 0;
    for (int i = 0; i < vertices; ++i) {
        total_neighbors += graph.GetNextVertices(i).size();
        total_neighbors += graph.GetPrevVertices(i).size();
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto add_time = std::chrono::duration_cast<std::chrono::milliseconds>(mid - start);
    auto query_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - mid);

    std::cout << "  " << graph_name << ":\n";
    std::cout << "    Adding " << edges << " edges: " << add_time.count() << " ms\n";
    std::cout << "    Querying neighbors: " << query_time.count() << " ms\n";
    std::cout << "    Total neighbors found: " << total_neighbors << "\n";
}

void run_performance_test() {
    print_test_header("Performance test (V=100, E=1000)");

    const int V = 100;
    const int E = 1000;

    measure_performance<ListGraph>("ListGraph", V, E);
    measure_performance<MatrixGraph>("MatrixGraph", V, E);
    measure_performance<SetGraph>("SetGraph", V, E);
    measure_performance<ArcGraph>("ArcGraph", V, E);
}

// ============================================
// Test 8: Independence of copies
// ============================================

void test_independence() {
    print_test_header("Independence of graph copies");

    try {
        // Create original
        ListGraph original(3);
        original.AddEdge(0, 1);
        original.AddEdge(1, 2);

        // Copy
        MatrixGraph copy(original);

        // Modify copy
        copy.AddEdge(0, 2);
        copy.AddEdge(2, 0);

        // Modify original
        original.AddEdge(2, 1);

        // Check they are independent
        auto orig_next0 = original.GetNextVertices(0);
        auto copy_next0 = copy.GetNextVertices(0);

        // Original: 0 -> 1
        // Copy: 0 -> 1, 0 -> 2

        assert(vectors_equal(orig_next0, { 1 }));
        assert(vectors_equal(copy_next0, { 1, 2 }));

        std::cout << "  Copies are independent from original: [SUCCESS]\n";

    }
    catch (const std::exception& e) {
        print_failure(e.what());
        throw;
    }
}

// ============================================
// MAIN TESTING FUNCTION
// ============================================

int main() {
    std::cout << "================================================\n";
    std::cout << "   TESTING GRAPH IMPLEMENTATIONS              \n";
    std::cout << "   ListGraph, MatrixGraph, SetGraph, ArcGraph  \n";
    std::cout << "================================================\n\n";

    int passed_tests = 0;
    int total_tests = 8;

    try {
        // Run all tests
        run_empty_graph_test();
        passed_tests++;

        run_single_edge_test();
        passed_tests++;

        run_multiple_edges_test();
        passed_tests++;

        run_self_loop_test();
        passed_tests++;

        run_copy_constructor_tests();
        passed_tests++;

        test_zero_vertices();
        passed_tests++;

        run_performance_test();
        passed_tests++;

        test_independence();
        passed_tests++;

        // Summary
        std::cout << "\n=============================================\n";
        std::cout << "  TEST RESULTS\n";
        std::cout << "=============================================\n";
        std::cout << "  Tests passed: " << passed_tests << " out of " << total_tests << "\n";

        if (passed_tests == total_tests) {
            std::cout << "\n  ALL TESTS PASSED SUCCESSFULLY!\n";
            std::cout << "  All graph implementations work correctly.\n";
        }
        else {
            std::cout << "\n  Not all tests passed!\n";
        }

        return 0;

    }
    catch (const std::exception& e) {
        std::cerr << "\n=============================================\n";
        std::cerr << "  CRITICAL ERROR!\n";
        std::cerr << "=============================================\n";
        std::cerr << "  Message: " << e.what() << "\n";
        std::cerr << "  Test interrupted.\n";

        return 1;
    }
}