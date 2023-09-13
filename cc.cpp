#include <iostream>
#include <numeric>
#include <unordered_set>
#include <vector>
/********************
* DFS CC
********************/
auto dfs_cc(const std::vector<int>& rowPtr, const std::vector<int>& colIdx)
{
  const auto                    numVertices = rowPtr.size() - 1;
  std::vector<bool>             visited(numVertices, false);
  std::vector<std::vector<int>> connectedComponents;

  std::function<void(int, std::vector<int>&)> dfs =
      [&dfs, &rowPtr, &colIdx, &visited](int root, std::vector<int>& component) {
        visited[root] = true;
        component.push_back(root);
        for(int i = rowPtr[root]; i < rowPtr[root + 1]; i++)
        {
          auto next = colIdx[i];
          if(visited[next] == false)
          {
            dfs(next, component);    // propagate the component vector
          }
        }
      };

  for(int i = 0; i < numVertices; i++)
  {
    if(visited[i] == false)
    {
      std::vector<int> component;
      dfs(i, component);
      connectedComponents.push_back(component);
    }
  }
  return connectedComponents;
}

/********************
* DFS + Label Propagation
********************/
auto afforest(const std::vector<int>& rowPtr, const std::vector<int>& colIdx)
{
  int              labelCount  = 0;
  const auto       numVertices = rowPtr.size() - 1;
  std::vector<int> label(numVertices, -1);

  std::function<void(int)> dfs = [&dfs, &rowPtr, &colIdx, &label](int curr) {
    auto currentLabel = label[curr];
    for(auto i = rowPtr[curr]; i < rowPtr[curr + 1]; i++)
    {
      auto next = colIdx[i];
      if(label[next] == -1)
      {
        label[next] = currentLabel;
        dfs(next);    // propagate the label
      }
    }
  };

  for(int i = 0; i < numVertices; i++)
  {
    if(label[i] == -1)
    {
      label[i] = labelCount++;
      dfs(i);
    }
  }
  return label;
}
/********************
* Union-Find CC
********************/
auto union_find(const std::vector<int>& rowPtr, const std::vector<int>& colIdx)
{
  const auto       numVertices = rowPtr.size() - 1;
  std::vector<int> parent(numVertices);
  std::vector<int> rank(numVertices);    // keep tree relatively balanced
  std::iota(parent.begin(), parent.end(), 0);
  //! Find
  // the resurive find function is not very clever
  // we can use `while(parent[i] != i) { i = parent[i]; }` instead
  std::function<int(int)> find = [&find, &parent](int i) {
    if(parent[i] == i)
    {
      return i;
    }
    // return find(parent[i]);
    // if without path compression, ends here
    //! Optimization: Path Compression
    parent[i] = find(parent[i]);
    return parent[i];
  };

  std::function<void(int, int)> unite = [&find, &parent, &rank](int i, int j) {
    i = find(i);
    j = find(j);
    // if(i != j)
    // {
    //   parent[j] = i;
    // }
    // if without rank, ends here
    //! Optimization 2: Union by Rank
    if(i == j)
    {
      return;
    }
    //Attach the smaller rank tree under root of the larger rank tree
    if(rank[i] > rank[j])
    {
      std::swap(i, j);
    }
    parent[i] = j;    // rank[i] < rank[j]

    if(rank[i] == rank[j])
    {
      rank[j]++;
    }
  };

  for(int i = 0; i < numVertices; i++)
  {
    for(auto j = rowPtr[i]; j < rowPtr[i + 1]; j++)
    {
      unite(i, colIdx[j]);
    }
  }

  for(int i = 0; i < numVertices; ++i)
  {
    parent[i] = find(i);
  }
  return parent;
}

/********************
* Shiloach-Vishkin CC
********************/
auto shiloach_vishkin(const std::vector<int>& rowPtr, const std::vector<int>& colIdx)
{
  const auto       numVertices = rowPtr.size() - 1;
  std::vector<int> parent(numVertices);    // same as parent
  std::iota(parent.begin(), parent.end(), 0);

  bool update = true;
  while(update)
  {
    update = false;
    //? Hooking Phase
    //! allowing for parallelism
    for(int i = 0; i < numVertices; i++)
    {
      auto curr_parent = parent[i];
      for(auto j = rowPtr[i]; j < rowPtr[i + 1]; j++)
      {
        auto next = colIdx[j];
        // lower component ID wins independent of direction
        if(parent[next] > curr_parent)
        {
          update       = true;
          parent[next] = curr_parent;
        }
      }
    }
    //? Shortcutting / Compressing / Jumping Phase
    //! allowing for parallelism
    for(int i = 0; i < numVertices; i++)
    {
      while(parent[i] != parent[parent[i]])
      {
        parent[i] = parent[parent[i]];
        update    = true;
      }
    }
  }
  return parent;
}

int main()
{

  std::vector<int> rowPtr = { 0, 1, 2, 3, 4, 5, 6 };
  std::vector<int> colIdx = { 1, 2, 0, 4, 5, 3 };
  //////////
  // dfs //
  /////////
  auto cc = dfs_cc(rowPtr, colIdx);

  std::cout << "DFS: ";
  int count = 0;
  for(auto& component : cc)
  {
    std::cout << "Group " << ++count << ": [ ";
    for(auto& vertex : component)
    {
      std::cout << vertex << ' ';
    }
    std::cout << "] ";
  }

  auto label = afforest(rowPtr, colIdx);
  std::cout << "\n\nLabels of Afforest: ";
  for(auto& l : label)
  {
    std::cout << l << ' ';
  }

  label = union_find(rowPtr, colIdx);
  std::cout << "\n\nLabels of Union Find: ";
  for(auto& l : label)
  {
    std::cout << l << ' ';
  }

  label = shiloach_vishkin(rowPtr, colIdx);
  std::cout << "\n\nLabels of Shiloach Vishkin: ";
  for(auto& l : label)
  {
    std::cout << l << ' ';
  }
  return 0;
}