#include <bits/stdc++.h>
#include "linked_list.h"
using namespace std;

class Graph
{
public:
    map<int, Linked_list> adj_list;
    int number_of_nodes;
    Graph();
    Graph(vector<vector<int>> adj_matrix);
    void deleting_edge(pair<int, int> edge);
    void adding_edge(pair<int, int> edge);
    void contract_edge(pair<int, int> edge);
    Linked_list detect_cycle(pair<int, int> edge); // only for detecting fundamental cycles in tree which is made by the edge, if it is used for graph other than tree, it will not be able to list the cycle and return something else.

    bool search_edge(pair<int, int> edge);
};

Graph::Graph()
{
    number_of_nodes = 0;
    adj_list.clear();
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
}

void Graph::deleting_edge(pair<int, int> edge) // O(n)
{
    Node *iter = adj_list[edge.first].head;
    while (iter != NULL)
    {

        if (iter->value == edge.second)
        {
            adj_list[edge.first].removeNode(iter->value);
        }
    }
}

void Graph::adding_edge(pair<int, int> edge) // O(n)
{
    adj_list[edge.first].addBack({edge.second});
}

void Graph::contract_edge(pair<int, int> edge)
{

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
                bool flag = (adj_list[edge.first].search(iter->value) == NULL);

                if (flag)
                {
                    adj_list[edge.first].addBack(iter->value);
                }
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
                if (adj_list[i].search(edge.second) != NULL && adj_list[i].search(edge.first) != NULL)
                {
                    adj_list[i].removeNode(edge.second);
                }
                else if (adj_list[i].search(edge.second) != NULL)
                {
                    adj_list[i].removeNode(edge.second);
                    adj_list[i].addBack(edge.first);
                }
            }
        }
    }
    else
    {
        cout << "No such edge exist" << endl;
    }

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
