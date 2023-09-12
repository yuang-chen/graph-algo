#include <iostream>
#include <unordered_set>
#include <vector>

/********************
* Connected components using DFS
********************/
auto DFS_CC(const std::vector<int>& rowPtr, const std::vector<int>& colIdx)
{
  const auto numVertices = rowPtr.size() - 1;
  std::vector<bool> visited(numVertices, false);
  std::vector<std::vector<int>> connectedComponents;

  std::function<void(int, std::vector<bool>&,
                          std::vector<int>&)>
      DFS = [&DFS, &rowPtr, &colIdx](int root, 
                                    std::vector<bool>& visited,
                                    std::vector<int>& component) {
        visited[root] = true;
        component.push_back(root);
        for(int i = rowPtr[root]; i < rowPtr[root + 1]; i++)
        {
          auto next = colIdx[i];
          if(visited[next] == false)
          {
            DFS(next, visited, component);
          }
        }
      };

  for(int i = 0; i < numVertices; i++)
  {
    if(visited[i] == false)
    {
      std::vector<int> component;
      DFS(i, visited, component);
      connectedComponents.push_back(component);
    }
  }
  return connectedComponents;
}

int main()
{

  std::vector<int> rowPtr = {0, 1, 2, 3, 4, 5, 6};
  std::vector<int> colIdx = {1, 2, 0, 4, 5, 3};
  //////////
  // DFS //
  /////////
  auto cc = DFS_CC(rowPtr, colIdx);

  if(cc.empty())
  {
    std::cout << "Component Not Found\n";
  }
  else
  {
    std::cout << "Connected Components: ";
    int count = 0;
    for(auto& component : cc)
    {
      std::cout << "\nComponent " << ++count << ": ";
      for(auto& vertex : component)
      {
        std::cout << vertex << ' ';
      }
    }
  }

  return 0;
}