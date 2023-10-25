#include <cmath>
#include <iostream>
#include <unordered_set>
#include <vector>

auto SCAN(const std::vector<int> row_ptr, const std::vector<int> col_idx,
          double eps, int mu) {
    // << First lambda
    // << Compute the structural similarity between two nodes
    auto structure_similarity = [&](int source, int target) {
        auto source_begin = row_ptr[source];
        auto target_begin = row_ptr[target];

        const auto source_end = row_ptr[source + 1];
        const auto target_end = row_ptr[target + 1];
        const auto source_degree = source_end - source_begin;
        const auto target_degree = target_end - target_begin;

        if (source_degree == 0 || target_degree == 0) {
            return 0.0;
        }

        int common_neighbors = 0;
        while (source_begin < source_end && target_begin < target_end) {
            if (col_idx[source_begin] == col_idx[target_begin]) {
                ++common_neighbors;
                ++source_begin;
                ++target_begin;
            } else if (col_idx[source_begin] < col_idx[target_begin]) {
                ++source_begin;
            } else {
                ++target_begin;
            }
        }
        return static_cast<double>(common_neighbors) /
               std::sqrt(source_degree * target_degree);
    };
    // << Second Functor, DFS
    // << Construct a new cluster from the core node
    std::function<void(int, std::unordered_set<int> &,
                       const std::vector<std::unordered_set<int>> &)>
        dfs =
            [&dfs](
                int source, std::unordered_set<int> &new_cluster,
                const std::vector<std::unordered_set<int>> &strong_neighbors) {
                new_cluster.insert(source);
                for (const auto target : strong_neighbors[source]) {
                    if (new_cluster.find(target) == new_cluster.end()) {
                        dfs(target, new_cluster, strong_neighbors);
                    }
                }
            };

    //>> Find Core Nodes
    const auto num_nodes = row_ptr.size() - 1;
    std::vector<std::unordered_set<int>> strong_neighbors(num_nodes);
    std::unordered_set<int> core_nodes;
    for (int source = 0; source < num_nodes; source++) {
        for (int j = row_ptr[source]; j < row_ptr[source + 1]; j++) {
            auto target = col_idx[j];
            // 1. For each vertex, compute its structural similarity with its
            // neighbors.
            if (structure_similarity(source, target) > eps) {
                // 2. If the structural similarity is above a certain threshold
                // (eps), mark the edge as 'strong'.
                strong_neighbors[source].insert(target);
            }
        }
        // 3. For each vertex, if the number of strong neighbors is above a
        // certain threshold (mu), mark it as a 'core' vertex.
        if (strong_neighbors[source].size() >= mu) {
            core_nodes.insert(source);
        }
    }
    // std::cout << "Strong Neighbors: " << std::endl;
    // for (int i = 0; i < strong_neighbors.size(); ++i) {
    //     std::cout << "Node " << i << ": ";
    //     for (const auto &neighbor : strong_neighbors[i]) {
    //         std::cout << neighbor << " ";
    //     }
    //     std::cout << std::endl;
    // }
    // std::cout << "Core Nodes: " << std::endl;
    // for (const auto &node : core_nodes) {
    //     std::cout << node << " ";
    // }
    // std::cout << std::endl;

    // >> Find Clusters
    std::vector<std::unordered_set<int>> clusters;
    std::unordered_set<int> visited;
    //    4. Perform a DFS to find clusters
    //    starting from each core node
    for (const auto core : core_nodes) {
        if (visited.find(core) == visited.end()) {
            std::unordered_set<int> new_cluster;
            dfs(core, new_cluster, strong_neighbors);
            clusters.push_back(new_cluster);
            visited.insert(new_cluster.begin(), new_cluster.end());
        }
    }
    return clusters;
}

int main() {
    std::vector<int> row_ptr = {0, 4, 8, 12, 16, 17, 21, 25, 29, 33, 34};
    std::vector<int> col_idx = {1, 2, 3, 0, 0, 2, 3, 1, 0, 1, 3, 2,
                                0, 1, 2, 3, 4, 5, 6, 7, 8, 5, 6, 7,
                                8, 5, 6, 7, 8, 5, 6, 7, 8, 9};

    // Parameters for SCAN
    double eps = 0.7;
    int mu = 2;

    auto clusters = SCAN(row_ptr, col_idx, eps, mu);

    for (const auto &cluster : clusters) {
        for (const auto node : cluster) {
            std::cout << node << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}
