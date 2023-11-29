
#include <iostream>
#include <vector>

std::vector<int> greedy_coloring(const std::vector<int> &row_ptr,
                                 const std::vector<int> &col_idx) {
    int num_nodes = row_ptr.size() - 1;
    std::vector<int> colors(num_nodes,
                            -1);  // one element is modified by every node
    std::vector<bool> used(num_nodes, false);  // reset by every node
    colors[0] = 0;

    for (int i = 1; i < num_nodes; i++) {
        for (int j = row_ptr[i]; j < row_ptr[i + 1]; j++) {
            auto neighbor = col_idx[j];
            // mark the neighbor's color as used
            if (colors[neighbor] != -1) {
                used[colors[neighbor]] = true;
            }
        }
        int color = 0;
        // find the first unused color
        for (; color < num_nodes; color++) {
            if (!used[color]) {
                break;
            }
        }
        colors[i] = color;

        for (int j = row_ptr[i]; j < row_ptr[i + 1]; j++) {
            auto neighbor = col_idx[j];
            // reset the used array
            if (colors[neighbor] != -1) {
                used[colors[neighbor]] = false;
            }
        }
    }
    return colors;
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

    std::vector<int> colors = greedy_coloring(rowPtr, colIndex);

    // Print the result
    for (int i = 0; i < colors.size(); ++i) {
        std::cout << "Vertex " << i << " ---> Color " << colors[i] << std::endl;
    }

    return 0;
}