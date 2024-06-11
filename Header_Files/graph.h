// #include <bits/stdc++.h>
#include <iostream>
#include <map>
#include <vector>
#include <utility>
#include <stack>
#include "linked_list.h"
using namespace std;

class Graph
{
public:
    map<int, Linked_list> adj_list;
    int number_of_nodes;
    map<int, int> after_contraction_nodes; // initially eadch node will be mapped to itself, but once contraction of edges are done it will keep track of which edge if contracted on which;
    Graph();
    Graph(int n);
    Graph(vector<vector<int>> adj_matrix);
    void deleting_edge(pair<int, int> edge);
    void adding_edge(pair<int, int> edge);
    void contract_edge(pair<int, int> edge);
    int self_finding_the_equivalent_vertex(int vertex);
    Linked_list detect_cycle(pair<int, int> edge); // only for detecting fundamental cycles in tree which is made by the edge, if it is used for graph other than tree, it will not be able to list the cycle and return something else.

    bool search_edge(pair<int, int> edge);
};

Graph::Graph()
{
    number_of_nodes = 0;
    adj_list.clear();
}

Graph::Graph(int n)
{
    number_of_nodes = n;
    adj_list.clear();

    for (int i = 0; i < n; i++)
    {
        after_contraction_nodes[i] = i;
    }
}

Graph::Graph(vector<vector<int>> adj_matrix) // O(n^2)
{

    number_of_nodes = adj_matrix[0].size();

    for (int i = 0; i < number_of_nodes; i++)
    {

        for (int j = 0; j < number_of_nodes; j++)
        {
            if (adj_matrix[i][j] != 0 && i != j)
            {
                adj_list[i].addBack(j);
            }
        }
    }
    for (int i = 0; i < number_of_nodes; i++)
    {
        after_contraction_nodes[i] = i;
    }
}

void Graph::deleting_edge(pair<int, int> edge) // O(n)
{
    if (edge.first == edge.second)
    {
        return;
    } // it might happen that this edge was already contracted
    Node *iter = adj_list[edge.first].search(edge.second);
    if (iter == NULL)
    {
        cout << "Delete Edge Error: "
             << "{" << edge.first << "," << edge.second << "} do not exist." << endl;
        return;
    }

    adj_list[edge.first].removeNode(edge.second);

    adj_list[edge.second].removeNode(edge.first);
}

void Graph::adding_edge(pair<int, int> edge) // O(n)
{
    adj_list[edge.first].addBack({edge.second});
}

int finding_the_equivalent_vertex(Graph G_x, int vertex) // it might happen that multiple edge contractions have happened, say like {0,2} and {1,0}. So 2 is equivalent to 0 but 0 itself isn't the base, because 0 is further contracted on 1. Therefore we will keep going on until we reach the base vertex which isn't contracted on anyone else.
{
    int x = vertex;
    while (G_x.after_contraction_nodes[x] != x)
    {
        x = G_x.after_contraction_nodes[x];
    }

    return x;
}

int Graph::self_finding_the_equivalent_vertex(int vertex)
{
    int x = vertex;
    while (after_contraction_nodes[x] != x)
    {
        x = after_contraction_nodes[x];
    }

    return x;
}

void Graph::contract_edge(pair<int, int> edge)
// so if the edge exist we contract the edge.second onto the edge.first, but and we do this by adjusting neighbours of all nodes and simply empty the list of edge.second, that mis not the same as contracting, because contracting means that now edge.first and edge.second are the same vertex. Therefore we update in after_contraction_nodes map (which contains which veretex is equivalent to which vertex) that edge.second is now same as edge.first
{

    if (edge.first == edge.second)
    {
        return;
    } // it might happen that this edge was already contracted

    if (search_edge(edge))
    {
        Node *iter = adj_list[edge.second].head;
        Node *iter1 = adj_list[edge.first].head;

        adj_list[edge.first].removeNode(edge.second);

        while (iter != NULL)

        // adding all the neighbours of vanishing node to the node it is merging on, if the merging node doesn't already have the, as neighbours

        {

            if (iter->value != edge.first)
            {

                adj_list[edge.first].addBack(iter->value);
            }
            Node *iter2 = iter;
            iter = iter->next;
            adj_list[edge.second].removeNode(iter2->value);
        }

        // Removing the vanishing node from the adjacency list of all its neighbours and adding the merging node as its neighbour, if it isn't already

        // since we are checking if the merging node is already a neighbour of the neighbours of the vanishing node, and if this is the case then we don't add the merging node to the adjacency list of neighbours again. This eliminates the multi edge that might be forming! Keep this in mind!!!!!!!!!!!!!!!!!

        for (int i = 0; i < number_of_nodes; i++)
        {
            if (i != edge.first && i != edge.second)
            {

                if (adj_list[i].search(edge.second) != NULL)
                {
                    Node *iter = adj_list[i].search(edge.second);
                    iter->value = edge.first;
                }
            }
        }
        cout << endl;
        after_contraction_nodes[edge.second] = edge.first; // since edge.second is combined with edge.first
    }
    else
    {
        cout << "Contract Edge Error: "
             << "{" << edge.first << "," << edge.second << "} do not exist." << endl;
    }

    // // by this time all the contractioons have happened and nothing more can be changed so all equivalent vertex have been concluded therefore let's check if all equivalent vertex have takent heir place

    // for (int i = 0; i < number_of_nodes; i++)
    // {
    //     Node *iter = adj_list[i].head;

    //     while (iter != nullptr)
    //     {
    //         int x = self_finding_the_equivalent_vertex(iter->value);
    //         if (x == i)
    //         {
    //             Node *iter2 = iter;
    //             iter = iter->next;
    //             adj_list[i].removeNode(iter2->value);
    //         }
    //         else
    //         {
    //             iter->value = x;
    //             iter = iter->next;
    //         }
    //     }
    // }

    return;
}

Linked_list Graph::detect_cycle(pair<int, int> edge)
{

    int start_vert = edge.first;

    stack<pair<int, vector<int>>> s;
    s = stack<pair<int, vector<int>>>();

    Linked_list cycle;

    map<int, int> visited;
    visited.clear();

    s.push({start_vert, {}});

    while (!s.empty())
    {
        pair<int, vector<int>> curr = s.top();
        s.pop();

        if (curr.first == edge.second)
        {
            vector<int> temp = curr.second;
            temp.push_back(curr.first);

            cycle.addFront(temp[0]);
            for (int i = 1; i < temp.size(); i++)
            {
                cycle.addBack(temp[i]);
            }

            break;
        }

        if (visited[curr.first] == 0)
        {
            visited[curr.first] = 1;

            Node *iter = adj_list[curr.first].head;

            while (iter != NULL)
            {
                if (visited[iter->value] == 0)
                {
                    vector<int> temp = curr.second;
                    temp.push_back(curr.first);
                    s.push({iter->value, temp});
                }
                iter = iter->next;
            }
        }
    }

    return cycle;
}

bool Graph::search_edge(pair<int, int> edge)
{
    return adj_list[edge.first].search(edge.second) != NULL;
}
