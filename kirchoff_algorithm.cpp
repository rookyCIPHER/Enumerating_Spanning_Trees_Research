#include <bits/stdc++.h>
#include "Header_files/graph.h"
using namespace std;

// code for determinant extracted from internet
int determinant(vector<vector<int>> &matrix);

// Helper function to get the submatrix excluding row i and column j
vector<vector<int>> getSubMatrix(vector<vector<int>> &matrix, int i, int j)
{
    vector<vector<int>> subMatrix;
    int n = matrix.size();

    for (int row = 0; row < n; ++row)
    {
        if (row != i)
        {
            vector<int> temp;
            for (int col = 0; col < n; ++col)
            {
                if (col != j)
                {
                    temp.push_back(matrix[row][col]);
                }
            }
            subMatrix.push_back(temp);
        }
    }
    return subMatrix;
}

// Recursive function to calculate determinant using Laplace expansion
int determinant(vector<vector<int>> &matrix)
{
    int n = matrix.size();
    if (n == 1)
    {
        return matrix[0][0];
    }

    int det = 0;
    int sign = 1;

    for (int j = 0; j < n; ++j)
    {
        vector<vector<int>> subMatrix = getSubMatrix(matrix, 0, j);
        det += sign * matrix[0][j] * determinant(subMatrix);
        sign = -sign;
    }

    return det;
}

// Algorithm Code
vector<vector<int>> diff(vector<vector<int>> matrix1, vector<vector<int>> matrix2)
{
    int n = matrix1.size();
    vector<vector<int>> ans(n, vector<int>(n, 0));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            ans[i][j] = matrix1[i][j] - matrix2[i][j];
        }
    }

    return ans;
}

int Kirchoff_Algo(Graph G)
{
    int count = 0;
    int n = G.number_of_nodes;
    vector<vector<int>> degree_matrix(n, vector<int>(n, 0));
    vector<vector<int>> adj_matrix(n, vector<int>(n, 0));

    for (int i = 0; i < n; i++)
    {

        degree_matrix[i][i] = G.adj_list[i].length();
        Node *iter = G.adj_list[i].head;

        while (iter != NULL)
        {
            adj_matrix[i][iter->value] = 1;
            iter = iter->next;
        }
    }

    // finding the difference
    vector<vector<int>> ans = diff(degree_matrix, adj_matrix);

    // leaving out the last row and column
    vector<vector<int>> samp(n - 1, vector<int>(n - 1, 0));

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - 1; j++)
        {
            samp[i][j] = ans[i][j];
        }
    }

    return determinant(samp);
}

int main()
{

    // Graph G({{0, 1, 1, 0, 0, 1, 0},
    //          {1, 0, 1, 0, 0, 0, 0},
    //          {1, 1, 0, 1, 0, 0, 0},
    //          {0, 0, 1, 0, 1, 0, 0},
    //          {0, 0, 0, 1, 0, 1, 1},
    //          {1, 0, 0, 0, 1, 0, 0},
    //          {0, 0, 0, 0, 1, 0, 0}

    // });

    Graph G({{0, 0, 0, 0, 0, 1, 1, 1, 0, 0}, {0, 0, 0, 1, 0, 0, 1, 1, 1, 0}, {0, 0, 0, 0, 0, 1, 0, 1, 1, 0}, {0, 1, 0, 0, 0, 0, 1, 0, 0, 1}, {0, 0, 0, 0, 0, 0, 1, 1, 0, 0}, {1, 0, 1, 0, 0, 0, 0, 0, 1, 0}, {1, 1, 0, 1, 1, 0, 0, 0, 1, 0}, {1, 1, 1, 0, 1, 0, 0, 0, 1, 0}, {0, 1, 1, 0, 0, 1, 1, 1, 0, 1}, {0, 0, 0, 1, 0, 0, 0, 0, 1, 0}});

    // Graph G({{0, 1, 1, 1},
    //          {1, 0, 1, 0},
    //          {1, 1, 0, 1},
    //          {1, 0, 1, 0}

    // });

    cout << Kirchoff_Algo(G) << endl;
    return 0;
}