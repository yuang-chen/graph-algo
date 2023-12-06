
#include <algorithm>
#include <iostream>
#include <vector>
/*
The Maximal Matching algorithm is a graph algorithm that finds a matching in a
graph, where a matching is a set of edges without common vertices.

In other words, a subset of the edges is a matching if each vertex appears in at
most one edge of that matching.

A Maximal matching is a matching that cannot have any more edges added to it
without violating the matching property.

A maximum matching is a matching that contains the largest possible number of
edges.

A maximal matching is not necessarily the maximum matching,

More on Perfect Matching and Near-Perfect
Matching: https://en.wikipedia.org/wiki/Matching_(graph_theory)

require:
Undirected Unweighted Graph
 */

auto maximal_matching(const std::vector<int> &row_ptr,
                      const std::vector<int> &col_idx) {
    int num_nodes = row_ptr.size() - 1;
    std::vector<bool> matched(num_nodes, false);
    std::vector<std::pair<int, int>> edges;

    for (int source = 0; source < num_nodes; source++) {
        if (matched[source])
            continue;
        for (int j = row_ptr[source]; j < row_ptr[source + 1]; j++) {
            auto target = col_idx[j];
            if (matched[target])
                continue;
            matched[source] = true;
            matched[target] = true;
            edges.emplace_back(source, target);
            break;
        }
    }
    return edges;
}
int main() {
    /* Graph:
    0 -- 1
    |    |
    3 -- 2
    \  /
     4
    */
    std::vector<int> rowPtr = {0, 2, 4, 6, 8, 10};
    std::vector<int> colIndex = {1, 3, 0, 2, 1, 4, 0, 4, 2, 3};

    auto edges = maximal_matching(rowPtr, colIndex);

    // Print the result
    for (const auto &edge : edges) {
        std::cout << edge.first << " " << edge.second << std::endl;
    }

    return 0;
}
