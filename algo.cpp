// #include <bits/stdc++.h>
#include <iostream>
#include <stack>
#include <vector>
#include <map>
#include <utility>
#include <queue>
#include <algorithm>
#include "Header_files/graph.h"
#include <chrono>
#include <iomanip>
using namespace std;

// generates a spanning tree, from a dfs path given to us
Graph generate_spanning_tree(vector<pair<int, int>> path) // O(V-1)  because number of edges in the tree path will be v-1
{

    Graph sp_tree(path.size());

    for (int i = 1; i < path.size(); i++)
    {
        // not starting from index 0 because the first element of dfs path will be {0,-1} and -1 won't be a valid node in the graph
        // this is because the our dfs traversal started from 0

        sp_tree.adj_list[path[i].second].addBack(path[i].first);
        sp_tree.adj_list[path[i].first].addBack(path[i].second);
    }

    return sp_tree;
}

// getting an arbitary spanning tree and set of non tree edges

// but this functionc an only be used for generating non-tree edges only for the initial tree
Graph arbitary_spanning_tree(Graph G) // O(V+E)
{

    stack<pair<int, int>> s;
    s.push({0, -1});
    vector<bool> visited(G.number_of_nodes, false);
    vector<pair<int, int>> dfs;
    dfs.clear();

    while (!s.empty())
    {
        pair<int, int> curr = s.top();
        s.pop();

        if (!visited[curr.first])
        {
            visited[curr.first] = true;
            dfs.push_back(curr);

            Node *iter = G.adj_list[curr.first].head;

            while (iter != NULL)
            {
                if (!visited[iter->value])
                {
                    s.push({iter->value, curr.first});
                }
                iter = iter->next;
            }
        }
    }

    return generate_spanning_tree(dfs);
}

// takes set of all non-tree edges and the IN set and OUT set associated with every node x of the computation tree. And returns the set F, which contains all non-tree edges with respect to spanning tree on node x of computation Tree which are also in G_x i.e. they are not in OUT_x and S (spanning tree at node x, but not being in the spanning tree is already handeled as we are only taking non-tree edges as input)
// also note  that after_contraction_nodes map will be used here
vector<pair<int, int>> F_generator(vector<pair<int, int>> non_tree_edges, vector<pair<int, int>> IN_x, vector<pair<int, int>> OUT_x)
{
    vector<pair<int, int>> F;
    F.clear();

    for (int i = 0; i < non_tree_edges.size(); i++)
    {
        pair<int, int> edge;
        edge.first = non_tree_edges[i].second;
        edge.second = non_tree_edges[i].first;
        if (count(OUT_x.begin(), OUT_x.end(), non_tree_edges[i]) == 0 && count(OUT_x.begin(), OUT_x.end(), edge) == 0)
        {
            F.push_back(non_tree_edges[i]);
        }
    }

    return F;
}

// Given a spanning tree and the original graph and IN and OUT set of spanning tree it detects all valid non-tree edges
vector<pair<int, int>> non_tree_edges_set_gen(Graph G, Graph sp_tree, vector<pair<int, int>> IN_x, vector<pair<int, int>> OUT_x)
{

    int n = G.number_of_nodes;
    map<pair<int, int>, bool> included;
    vector<pair<int, int>> edges;

    included.clear();
    edges.clear();

    for (int i = 0; i < n; i++)
    {

        Node *iter = G.adj_list[i].head;

        while (iter != nullptr)
        {
            if (sp_tree.adj_list[i].search(iter->value) == NULL && !included[{i, iter->value}])
            {
                edges.push_back({i, iter->value});
                included[{i, iter->value}] = true;
                included[{iter->value, i}] = true;
            }
            iter = iter->next;
        }
    }

    return F_generator(edges, IN_x, OUT_x);
}

Graph copy_graph_object(Graph G)
{
    Graph temp(G.number_of_nodes);

    // making a deep copy of original Graph so as to not alter the original in this process
    for (int i = 0; i < G.number_of_nodes; i++)
    {
        Node *iter = G.adj_list[i].head;
        while (iter != nullptr)
        {
            temp.adj_list[i].addBack(iter->value);
            iter = iter->next;
        }
    }
    return temp;
}

// // generate the graph G_x for each node of the computation tree given the IN_x and OUT_x set  //aslo know that after_contraction_nodes map will be used here
// Graph G_x_generator(Graph G, vector<pair<int, int>> IN_x, vector<pair<int, int>> OUT_x)
// {
//     Graph temp = copy_graph_object(G);

//     for (int i = 0; i < OUT_x.size(); i++)
//     {

//         int vertex1 = finding_the_equivalent_vertex(temp, OUT_x[i].first);
//         int vertex2 = finding_the_equivalent_vertex(temp, OUT_x[i].second);

//         temp.deleting_edge({vertex1, vertex2});
//     }

//     for (int i = 0; i < IN_x.size(); i++)
//     {

//         temp.contract_edge({finding_the_equivalent_vertex(temp, IN_x[i].first), finding_the_equivalent_vertex(temp, IN_x[i].second)});
//     }

//     return temp;
// }

// Cycle_x set generator for a spanning tree at node x of the computation tree, these cycles should only containe dges which are also present in G_x i.e. they should not be in IN_x and OUT_x
map<pair<int, int>, Linked_list> Cycle_x_generator(Graph sp_tree, vector<pair<int, int>> F, vector<pair<int, int>> IN_x, vector<pair<int, int>> OUT_x)
{
    map<pair<int, int>, Linked_list> Cycle_x;

    Cycle_x.clear();

    for (int i = 0; i < F.size(); i++)
    {

        Linked_list samp = sp_tree.detect_cycle(F[i]);

        Node *iter = samp.head;

        while (iter != nullptr && iter->next != nullptr)
        {
            pair<int, int> edge;
            edge.first = iter->value;
            edge.second = iter->next->value;

            pair<int, int> edge2;
            edge2.first = iter->next->value;
            edge2.second = iter->value;
            if (count(IN_x.begin(), IN_x.end(), edge) != 0 || count(OUT_x.begin(), OUT_x.end(), edge) != 0 || count(IN_x.begin(), IN_x.end(), edge2) != 0 || count(OUT_x.begin(), OUT_x.end(), edge2) != 0)
            {
                samp.removeNode(iter->value);
            }
            iter = iter->next;
        }

        Cycle_x[F[i]] = samp;
    }

    return Cycle_x;
}

// to print the graph
void print_adjList_Graph(Graph sample)
{
    for (int i = 0; i < sample.number_of_nodes; i++)
    {
        cout << i << "=";
        sample.adj_list[i].printList();
    }
}

// function to perform swap edge operation in the spanning tree
Graph swap_edge(Graph sp_tree, pair<int, int> incoming_edge, pair<int, int> outgoing_edge)
{
    Graph temp = copy_graph_object(sp_tree);

    // removing the outgoing edge
    temp.adj_list[outgoing_edge.first].removeNode(outgoing_edge.second);
    temp.adj_list[outgoing_edge.second].removeNode(outgoing_edge.first);

    // adding the incoming edge
    temp.adj_list[incoming_edge.first].addBack(incoming_edge.second);
    temp.adj_list[incoming_edge.second].addBack(incoming_edge.first);

    return temp;
}

int counter = 0; // helps in generating new parent id
class Comp_Tree_Node
{
public:
    Graph sp_tree;
    int current_id;
    int parent_id;
    vector<pair<int, int>> INx;
    vector<pair<int, int>> OUTx;
    // Graph G_x;
    vector<pair<int, int>> F;
    map<pair<int, int>, Linked_list> Cycle_x;

    Comp_Tree_Node(Graph sp_tree_, vector<pair<int, int>> INx_, vector<pair<int, int>> OUTx_, vector<pair<int, int>> F_, map<pair<int, int>, Linked_list> Cycle_x_, int id_p, int id_c)
    {
        sp_tree = sp_tree_;
        INx = INx_;
        OUTx = OUTx_;
        // G_x = G_x_;
        F = F_;
        Cycle_x = Cycle_x_;

        parent_id = id_p;
        current_id = id_c;
    }
};

// to print the spanbning tree at eaxh node of the computation tree along with all of its associated property sets
void print_node_properties(Comp_Tree_Node *elem)
{
    cout << "The tree at the node of the computation tree: " << endl;
    print_adjList_Graph(elem->sp_tree);

    cout << endl;

    cout << "IN_x: ";
    for (int i = 0; i < elem->INx.size(); i++)
    {
        cout << "{" << elem->INx[i].first << "," << elem->INx[i].second << "}"
             << " ";
    }
    cout << endl;
    cout << endl;
    cout << "OUT_x: ";
    for (int i = 0; i < elem->OUTx.size(); i++)
    {
        cout << "{" << elem->OUTx[i].first << "," << elem->OUTx[i].second << "}"
             << " ";
    }
    cout << endl;
    cout << endl;
    // cout << "Graph G_x: " << endl;
    // print_adjList_Graph(elem->G_x);
    // cout << endl;
    cout << "F (set of non-tree edges wrt this spanning tree which are also present in G_x): ";
    for (int i = 0; i < elem->F.size(); i++)
    {
        cout << "{" << elem->F[i].first << "," << elem->F[i].second << "}"
             << " ";
    }
    cout << endl;
    cout << endl;
    cout << "Cycle_x: ";
    for (int i = 0; i < elem->F.size(); i++)
    {
        cout << "{" << elem->F[i].first << "," << elem->F[i].second << "}"
             << ": ";
        Node *iter = elem->Cycle_x[elem->F[i]].head;

        while (iter->next != NULL)
        {
            cout << "{" << iter->value << "," << iter->next->value << "}"
                 << " ";
            iter = iter->next;
        }
        cout << endl;
    }
    cout << endl;
}

// the routine to generate children of a node in computation tree
vector<pair<Comp_Tree_Node *, pair<pair<int, int>, pair<int, int>>>> generate_children_nodes(Comp_Tree_Node *elem, Graph G)
{
    vector<pair<Comp_Tree_Node *, pair<pair<int, int>, pair<int, int>>>> children;
    children.clear();

    int parent_id = elem->current_id;

    for (int i = 0; i < elem->F.size(); i++)
    {
        vector<pair<int, int>> prev_swaps;
        prev_swaps.clear();

        Graph child_sp = copy_graph_object(elem->sp_tree); // making a copy of parent tree to perform edge swap

        pair<int, int> incoming_edge = elem->F[i];

        Node *iter = elem->Cycle_x[incoming_edge].head;

        while (iter->next != nullptr)
        {
            pair<int, int> outgoing_edge = {iter->value, iter->next->value};
            Graph temp_sp = swap_edge(child_sp, incoming_edge, outgoing_edge);

            // Now we have a spanning tree ready now all that is left is to generate other sets

            // INx
            vector<pair<int, int>> child_INx(elem->INx);

            // Now we need to add the current incoming edge and all the previous swaps made to the child INx
            child_INx.push_back(incoming_edge);

            for (int j = 0; j < prev_swaps.size(); j++)
            {
                child_INx.push_back(prev_swaps[j]);
            }

            // OUTx
            vector<pair<int, int>> child_OUTx(elem->OUTx);

            // Now we have to add current outgoing edge to the OUT_x, we also have to push in all the non-tree edges which have been done for earlier
            child_OUTx.push_back(outgoing_edge);
            // cout << "i=" << i << " children_ID=" << counter << endl;

            for (int k = 0; k < i; k++)
            {
                child_OUTx.push_back(elem->F[k]);
            }

            // // generating G_x
            // Graph child_G_x = G_x_generator(G, child_INx, child_OUTx);

            // generate the set F
            vector<pair<int, int>> child_F = non_tree_edges_set_gen(G, temp_sp, child_INx, child_OUTx);

            // generating the Cycle_x set for the child
            map<pair<int, int>, Linked_list> child_Cycle_x = Cycle_x_generator(temp_sp, child_F, child_INx, child_OUTx);

            Comp_Tree_Node *samp = new Comp_Tree_Node(temp_sp, child_INx, child_OUTx, child_F, child_Cycle_x, parent_id, counter);
            counter++; // updating the id for making the next id

            // pushing the currently outgoing edge into the prev_swaps vector for the next iteration.
            prev_swaps.push_back(outgoing_edge);

            // pushing the current child into the children vector
            children.push_back({samp, {incoming_edge, outgoing_edge}});

            iter = iter->next;
        }
    }

    return children;
}

// the simultaion of algorithm
void enumerating_spanning_trees(Graph G)
{
    cout << "----------------------------Original Graph-------------------------------------" << endl;
    print_adjList_Graph(G);
    cout << "-------------------------------------------------------------------------------" << endl;

    // // first we need an arbitary spanning tree
    //     Graph sp_tree({{0, 1, 0, 0, 0, 1, 0},
    //                    {1, 0, 1, 0, 0, 0, 0},
    //                    {0, 1, 0, 0, 0, 0, 0},
    //                    {0, 0, 0, 0, 1, 0, 0},
    //                    {0, 0, 0, 1, 0, 1, 1},
    //                    {1, 0, 0, 0, 1, 0, 0},
    //                    {0, 0, 0, 0, 1, 0, 0}

    //     });
    Graph sp_tree = arbitary_spanning_tree(G);

    // Now to generate initial sets F,INx,OUTx,Cycle_x and they can be generated in the order mentioned below because the quantities mentioned further down below are dependent on the quantities mentioned up above.
    vector<pair<int, int>>
        INx = {};
    vector<pair<int, int>> OUTx = {};
    // Graph G_x = G_x_generator(G, INx, OUTx);
    vector<pair<int, int>> F = non_tree_edges_set_gen(G, sp_tree, INx, OUTx);
    map<pair<int, int>, Linked_list> Cycle_x = Cycle_x_generator(sp_tree, F, INx, OUTx);

    Comp_Tree_Node *elem = new Comp_Tree_Node(sp_tree, INx, OUTx, F, Cycle_x, -1, 0);
    counter++; // updating the counter so that next id can be created

    // Approaching it as bfs traversal of the computation tree

    queue<pair<Comp_Tree_Node *, pair<pair<int, int>, pair<int, int>>>> q;
    // contains the element and the swap of edges used to reach here

    q.push({elem, {{-1, -1}, {-1, -1}}});
    int x;

    while (!q.empty())
    {
        pair<Comp_Tree_Node *, pair<pair<int, int>, pair<int, int>>> curr = q.front();
        q.pop();

        // printing the current node element
        cout << "-----Generated by swapping {" << curr.second.first.first << "," << curr.second.first.second << "} with the edge {" << curr.second.second.first << "," << curr.second.second.second << "} in the tree id=" << curr.first->parent_id << "-----" << endl;
        cout << "Current ID=" << curr.first->current_id << endl;
        x = curr.first->current_id;
        print_adjList_Graph(curr.first->sp_tree);

        // //print_node_properties(curr.first); // for deubug purposes

        cout << "-------------------------------------------------------------------------------" << endl;

        // now we need a routine to generate all possible children
        vector<pair<Comp_Tree_Node *, pair<pair<int, int>, pair<int, int>>>> children = generate_children_nodes(curr.first, G);

        for (int i = 0; i < children.size(); i++)
        {
            q.push(children[i]);
        }
    }

    cout << "---------------------Total number of trees = " << x + 1 << "-----------------------------------" << endl;
    counter = 0;
}

int main()
{
    auto start = chrono::high_resolution_clock::now();

    // unsync the I/O of C and C++.
    ios_base::sync_with_stdio(false);

    Graph G({{0, 1, 1, 0, 0, 1, 0},
             {1, 0, 1, 0, 0, 0, 0},
             {1, 1, 0, 1, 0, 0, 0},
             {0, 0, 1, 0, 1, 0, 0},
             {0, 0, 0, 1, 0, 1, 1},
             {1, 0, 0, 0, 1, 0, 0},
             {0, 0, 0, 0, 1, 0, 0}

    });

    // Graph G({{0, 1, 1, 1},
    //          {1, 0, 1, 0},
    //          {1, 1, 0, 1},
    //          {1, 0, 1, 0}

    // });
    enumerating_spanning_trees(G);

    auto end = chrono::high_resolution_clock::now();

    // Calculating total time taken by the program.
    double time_taken =
        chrono::duration_cast<chrono::nanoseconds>(end - start).count();

    time_taken *= 1e-9;

    cout << "Time taken by program is : " << fixed
         << time_taken << setprecision(9);
    cout << " sec" << endl;
    return 0;
}