#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>

std::stack<int> BFS(const int root, const int target, const std::vector<int>& row_pointer, const std::vector<int>& column_index) {
   
    std::unordered_map<int, int> parent;
    std::unordered_set<int> visited;
    std::queue<int> nodeQue;  // std::stack<int> nodeStk for DFS
    std::stack<int> path;
    bool hasFound = false;

    nodeQue.push(root);
    visited.insert(root);

    while(!nodeQue.empty()) {
        auto curr = nodeQue.front();  // nodeStk.top() for DFS
        nodeQue.pop();

        if(curr == target) {
            hasFound = true;
            break;
        }

        for(int i = row_pointer[curr]; i < row_pointer[curr+1]; i++) {
            auto next = column_index[i];
            if(visited.count(next) == 0)
            {   
                nodeQue.push(next);
                visited.insert(next);
                parent[next] = curr;
            }
        }
    }

    if(hasFound) {
        auto curr = target;
        path.push(curr);
        if(curr != root) {
            curr = parent[curr];
            path.push(curr);
        }
        path.push(root);
    } 
    return path;
}

void DFS(const int root, const int target, std::vector<int>& path, std::vector<bool>& visited, const std::vector<int>& row_pointer, const std::vector<int>& column_index) {
   visited[root] = true;
   path.push_back(root);

   if(root == target) {
    std::cout << "DFS path from root to target: ";
    for(int i = 0; i < path.size(); i++) {
        std::cout << path[i] << " ";
    }
    std::cout << '\n';
   }    
   for(int i = row_pointer[root]; i < row_pointer[root+1]; i++) {
      int next = column_index[i];
      if( visited[next] == false ) {
        DFS(next, target, path, visited, row_pointer, column_index);
      }
    }
}


int main() {
    
    std::vector<int> rowPointer = {0, 2, 4, 6, 8, 10, 12};
    std::vector<int> colIndices = {1, 2, 0, 3, 0, 1, 4, 1, 2, 5, 3, 5};
    //////////
    // BFS //
    /////////
    auto bfs_path = BFS(0, 5, rowPointer, colIndices);

    if(bfs_path.empty()) {
        std::cout << "Path Not Found\n";
    } else {
        std::cout << "Path from Root to Target: ";
        while(!bfs_path.empty())  {
            std::cout << bfs_path.top() << ' ';
            bfs_path.pop();
        }
    }
    //////////
    // DFS //
    /////////
    std::vector<int> dfs_path;
    std::vector<bool> visited( rowPointer.size()-1, false );  //used as unordered_set
    DFS(0, 5, dfs_path, visited, rowPointer, colIndices);
    
    if(dfs_path.empty()) {
        std::cout << "Path Not Found\n";
    } else {
        std::cout << "Path from Root to Target: ";
        while(!dfs_path.empty())  {
            std::cout << dfs_path.top() << ' ';
            dfs_path.pop();
        }
    }

    return 0;
}