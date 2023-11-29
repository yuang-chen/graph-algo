#include <iostream>
#include <stack>
#include <vector>
// https://github.com/WarriorsSami/Tarjan/blob/master/tarjan.cpp
std::vector<std::vector<int>> tarjan(const std::vector<int> &row_ptr,
                                     const std::vector<int> &col_idx) {
    const auto num_vertices = row_ptr.size() - 1;
    std::vector<int> discovery_time(num_vertices, -1);
    std::vector<int> low_time(num_vertices, -1);
    std::vector<int> parent(num_vertices, -1);
    std::vector<bool> visited(num_vertices, false);
    std::stack<int> node_stack;
    std::vector<std::vector<int>> bcc;
    int depth = 0;

    auto dfs = [&](int source, auto &&dfs_ref) -> void {
        low_time[source] = discovery_time[source] = depth++;
        visited[source] = true;
        node_stack.push(source);
        int num_children = 0;

        for (int i = row_ptr[source]; i < row_ptr[source + 1]; i++) {
            int target = col_idx[i];
            if (visited[target]) {
                low_time[source] =
                    std::min(low_time[source], discovery_time[target]);
            } else {
                num_children++;
                parent[target] = source;
                dfs_ref(target, dfs_ref);
                low_time[source] = std::min(low_time[source], low_time[target]);
                // a source node is an articulation point if
                // 1. it is root && has more than 1 child
                // 2. not root && low_time of any of its children is
                // greater than or equal to discovery_time of the source node
                if ((parent[source] == -1 && num_children > 1) ||
                    (parent[source] &&
                     low_time[target] >= discovery_time[source])) {
                    std::vector<int> component;
                    while (node_stack.top() != target) {
                        component.push_back(node_stack.top());
                        node_stack.pop();
                    }
                    // component.push_back(node_stack.top());  // get the target
                    node_stack.pop();
                    component.push_back(source);  // get the source
                    bcc.push_back(component);
                }
            }
        }
    };
    for (int i = 0; i < num_vertices; ++i) {
        dfs(i, dfs);
    }
    return bcc;
}
int main() {
    /**
     * @brief Graph Visualization:
        0 --- 1
        |   /
        | /
        2 --- 3 --- 4
              \   /
              \ /
               5
     */
    std::vector<int> row_ptr = {0, 2, 4, 7, 10, 12, 14};
    std::vector<int> col_idx = {1, 2, 0, 2, 0, 1, 3, 2, 4, 5, 3, 5, 3, 4};

    auto BCCs = tarjan(row_ptr, col_idx);
    std::cout << "--------------------------\n";

    for (auto &component : BCCs) {
        for (auto &vertex : component) {
            std::cout << vertex << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}