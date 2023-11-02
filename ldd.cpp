
#include <iostream>
#include <queue>
#include <vector>

/*
The Low-Diameter Decomposition (LDD) algorithm is a graph partitioning algorithm
that decomposes a graph into several connected subgraphs (or components) such
that each subgraph has a low diameter. The diameter of a subgraph is defined as
the maximum shortest path distance between any two nodes within the subgraph.

The LDD algorithm works as follows:

1. Start with an empty decomposition and an empty queue.
2. Pick an unvisited node u and create a new set containing only u.
3. Perform a breadth-first search (BFS) starting from u, adding nodes to u's set
until the diameter of the set reaches a specified threshold beta.
4. Mark all nodes in u's set as visited.
5. Repeat steps 2-4 until all nodes have
been visited and assigned to a set.
 */
std::vector<int> lowDiameterDecomposition(const std::vector<int> row_ptr,
                                          const std::vector<int> col_idx,
                                          const int beta) {
    int num_nodes = row_ptr.size() - 1;
    std::vector<int> components(num_nodes, -1);
    std::vector<bool> visited(num_nodes, false);
    std::queue<int> bfs_queue;

    int set_id = 0;
    for (int source = 0; source < num_nodes; source++) {
        if (!visited[source]) {
            int set_size = 0;

            bfs_queue.push(source);
            visited[source] = true;
            components[source] = set_id;

            while (!bfs_queue.empty()) {
                auto node = bfs_queue.front();
                bfs_queue.pop();

                for (auto i = row_ptr[node]; i < row_ptr[node + 1]; i++) {
                    auto target = col_idx[i];
                    if (visited[target]) {
                        continue;
                    }
                    bfs_queue.push(target);
                    visited[target] = true;
                    components[target] = set_id;
                    set_size++;

                    if (++set_size >= beta) {
                        // goto to the breakout of the nested loop
                        goto break_out;
                        // break;
                    }
                }
                // if (set_size >= beta) {
                //     break;
                // }
            }
        break_out:
            set_id++;
        }
    }
    return components;
}

int main() {
    // A cycle of 3 nodes: 0 -> 1 -> 2 -> 0
    // A cycle of 3 nodes: 3 -> 4 -> 5 -> 3
    std::vector<int> row_ptr = {0, 1, 2, 3, 4, 5, 6};
    std::vector<int> col_idx = {1, 2, 0, 4, 5, 3};
    int beta = 2;

    auto components = lowDiameterDecomposition(row_ptr, col_idx, beta);

    // Print the decomposition
    for (int i = 0; i < components.size(); ++i) {
        std::cout << "Node " << i << " is in set " << components[i]
                  << std::endl;
    }

    return 0;
}