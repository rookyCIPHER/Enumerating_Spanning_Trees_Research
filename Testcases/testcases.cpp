#include <iostream>
// #include "linked_list.h"  // Assuming you have defined your Linked_list class in a separate header file
#include "../Header_files/graph.h" // Assuming you have defined your graph class in a separate header file

int main()
{
    // Linked_list list;

    // // Test 1: Empty List Operations
    // std::cout << "Test 1: Empty List Operations" << std::endl;
    // list.printList();  // Should print {}
    // std::cout << "List length: " << list.length() << std::endl;  // Should be 0
    // std::cout << "Is list empty? " << (list.isEmpty() ? "Yes" : "No") << std::endl;  // Should be Yes

    // // Test 2: Adding and Removing Elements
    // std::cout << "\nTest 2: Adding and Removing Elements" << std::endl;
    // list.addFront(5);
    // list.addBack(10);
    // list.addBack(15);
    // list.printList();  // Should print 5 10 15
    // list.removeNode(10);
    // list.printList();  // Should print 5 15
    // list.removeNode(5);
    // list.removeNode(15);
    // list.printList();  // Should print {}

    // // Test 3: Memory Management
    // std::cout << "\nTest 3: Memory Management" << std::endl;
    // for (int i = 0; i < 100000; ++i) {
    //     list.addBack(i);
    // }
    // std::cout << "List length: " << list.length() << std::endl;  // Should be 100000

    // // Test 4: Performance Testing
    // std::cout << "\nTest 4: Performance Testing" << std::endl;
    // auto startTime = std::chrono::high_resolution_clock::now();
    // for (int i = 0; i < 100000; ++i) {
    //     list.removeNode(i);
    // }
    // auto endTime = std::chrono::high_resolution_clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    // std::cout << "Time taken to remove 100000 elements: " << duration.count() << " ms" << std::endl;

    // // Test 5: Boundary Cases
    // std::cout << "\nTest 5: Boundary Cases" << std::endl;
    // for (int i = 0; i < 1000000; ++i) {
    //     list.addBack(i);
    // }
    // std::cout << "List length: " << list.length() << std::endl;  // Should be 1000000

    // Test 1: Creating an empty graph
    Graph emptyGraph;
    cout << "TEST 1:" << endl;
    cout << "Empty graph created." << endl;
    cout << endl;

    // Test 2: Creating a graph from an adjacency matrix
    vector<vector<int>> adjMatrix = {
        {0, 1, 0, 1},
        {1, 0, 1, 0},
        {0, 1, 0, 1},
        {1, 0, 1, 0}};
    Graph adjMatrixGraph(adjMatrix);
    cout << "TEST 2:" << endl;
    cout << "Graph created from adjacency matrix." << endl;
    cout << endl;

    // Test 3: Adding and deleting edges
    pair<int, int> edgeToAdd(0, 2);
    pair<int, int> edgeToDelete(1, 3);

    adjMatrixGraph.adding_edge(edgeToAdd);
    cout << "TEST 3:" << endl;
    cout << "Edge (" << edgeToAdd.first << ", " << edgeToAdd.second << ") added." << endl;

    // adjMatrixGraph.deleting_edge(edgeToDelete);
    // cout << "Edge (" << edgeToDelete.first << ", " << edgeToDelete.second << ") deleted." << endl;
    cout << endl;

    // Test 4: Contracting an edge
    pair<int, int> edgeToContract(0, 1);
    adjMatrixGraph.contract_edge(edgeToContract);
    cout << "TEST 4:" << endl;
    cout << "Edge (" << edgeToContract.first << ", " << edgeToContract.second << ") contracted." << endl;
    cout << endl;

    // Test 5: Detecting a cycle
    pair<int, int> edgeForCycle(2, 3);
    Linked_list cycle = adjMatrixGraph.detect_cycle(edgeForCycle);

    cout << "TEST 5:" << endl;
    cout << "Cycle detected: ";
    cycle.printList();
    cout << endl;

    // Test 6: Searching for an edge
    cout << "TEST 6:" << endl;
    pair<int, int> edgeToSearch(2, 1);
    bool found = adjMatrixGraph.search_edge(edgeToSearch);
    cout << "Edge (" << edgeToSearch.first << ", " << edgeToSearch.second << ") found? " << (found ? "Yes" : "No") << endl;

    return 0;
}