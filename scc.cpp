#include <iostream>
#include <stack>
#include <vector>

auto Kosaraju(const std::vector<int> &csr_pointer,
              const std::vector<int> &csr_index,
              const std::vector<int> &csc_pointer,
              const std::vector<int> &csc_index) {
    const auto num_nodes = csr_pointer.size() - 1;
    std::vector<bool> visited(num_nodes, false);
    std::stack<int> post_order_stack;
    //! First DFS
    // traverse CSR graph
    std::function<void(int)> dfs_1st = [&](int source) {
        visited[source] = true;
        for (auto i = csr_pointer[source]; i < csr_pointer[source + 1]; i++) {
            auto target = csr_index[i];
            if (!visited[target]) {
                dfs_1st(target);
            }
        }
        post_order_stack.push(source);
    };

    for (int i = 0; i < num_nodes; i++) {
        if (!visited[i]) {
            dfs_1st(i);
        }
    }
    //! Second DFS
    std::function<void(int, std::vector<int> &)> dfs_2nd =
        [&](int target, std::vector<int> &SCC) {
            visited[target] = false;
            SCC.push_back(target);
            for (auto i = csc_pointer[target]; i < csc_pointer[target + 1];
                 i++) {
                auto source = csc_index[i];
                if (visited[source]) {
                    dfs_2nd(source, SCC);
                }
            }
        };
    // traverse CSC graph -- backtracking
    std::vector<std::vector<int>> all_SCCs;
    while (!post_order_stack.empty()) {
        auto node = post_order_stack.top();
        post_order_stack.pop();
        if (visited[node])  // go through the visited nodes
        {
            std::vector<int> SCC;
            dfs_2nd(node, SCC);
            all_SCCs.push_back(SCC);
        }
    }
    return all_SCCs;
}

auto Tarjan(const std::vector<int> &csr_pointer,
            const std::vector<int> &csr_index) {
    auto num_nodes = csr_pointer.size() - 1;
    // The depth-first search order in which the vertices are discovered
    std::vector<int> pre_order(num_nodes, -1);
    //  This represents the smallest pre_order value reachable from vertex [i]
    std::vector<int> low_link(num_nodes, 0);
    std::vector<bool> in_stack(num_nodes, false);
    std::stack<int> dfs_tree_stack;
    std::vector<std::vector<int>> all_SCCs;

    std::function<void(int, int, std::vector<int> &, std::vector<int> &,
                       std::vector<bool> &, std::stack<int> &,
                       std::vector<std::vector<int>> &)>
        dfs = [&csr_pointer, &csr_index,
               &dfs](int source, int count, std::vector<int> &pre_order,
                     std::vector<int> &low_link, std::vector<bool> &in_stack,
                     std::stack<int> &dfs_tree_stack,
                     std::vector<std::vector<int>> &all_SCCs) {
            pre_order[source] = count++;
            low_link[source] = pre_order[source];
            in_stack[source] = true;
            dfs_tree_stack.push(source);
            // Back Edge: A back edge is an edge that connects a vertex to one
            // of its ancestors in the DFS tree, creating a cycle.
            for (auto i = csr_pointer[source]; i < csr_pointer[source + 1];
                 i++) {
                auto target = csr_index[i];
                // if target is unvisited, perform DFS
                if (pre_order[target] == -1) {
                    dfs(target, count, pre_order, low_link, in_stack,
                        dfs_tree_stack, all_SCCs);
                }
                // When a back edge from "source" to "target" is encountered and
                // "target" is still in the stack, low_link[source] is updated
                // to min(low_link[source], dfn[target]). This is because
                // "target" is an ancestor of "source" in the DFS tree. The
                // update indicates that there is a way to reach back to
                // "target" from "source," thereby forming a cycle.
                if (in_stack[target]) {
                    low_link[source] =
                        std::min(low_link[source], low_link[target]);
                }
            }
            // >> Backtracking, after visiting a root vertex and all its
            // descendants, check if current vertex is the root vertex if true,
            // the DFS tree is a SCC
            if (pre_order[source] == low_link[source]) {
                std::vector<int> scc;
                while (true) {
                    auto node = dfs_tree_stack.top();
                    dfs_tree_stack.pop();
                    in_stack[node] = false;
                    scc.push_back(node);

                    if (node == source) {
                        break;
                    }
                }
                all_SCCs.push_back(scc);
            }
        };

    for (int i = 0; i < num_nodes; i++) {
        if (pre_order[i] == -1) {
            dfs(i, 0, pre_order, low_link, in_stack, dfs_tree_stack, all_SCCs);
        }
    }

    return all_SCCs;
}

int main() {
    // CSR representation for the graph
    std::vector<int> csr_pointer = {0, 2, 3, 4, 5, 6};
    std::vector<int> csr_index = {1, 3, 2, 0, 4, 3};

    // CSC representation for the transpose of the graph
    std::vector<int> csc_pointer = {0, 1, 2, 3, 5, 6};
    std::vector<int> csc_index = {2, 0, 1, 0, 4, 3};

    auto all_SCCs = Kosaraju2(csr_pointer, csr_index, csc_pointer, csc_index);

    for (const auto &scc : all_SCCs) {
        std::cout << "SCC: ";
        for (int vertex : scc) {
            std::cout << vertex << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "------------------" << std::endl;
    all_SCCs = Tarjan(csr_pointer, csr_index);
    for (const auto &scc : all_SCCs) {
        std::cout << "SCC: ";
        for (int vertex : scc) {
            std::cout << vertex << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}