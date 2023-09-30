#include <iostream>
#include <numeric>
#include <unordered_set>
#include <vector>

/********************
* DFS + Label Propagation
********************/
auto dfs_cc(const std::vector<int>& row_pointer, const std::vector<int>& column_index)
{
  int              labelCount  = 0;
  const auto       num_nodes = row_pointer.size() - 1;
  std::vector<int> label(num_nodes, -1);

  std::function<void(int)> dfs = [&dfs, &row_pointer, &column_index, &label](int curr) {
    auto currentLabel = label[curr];
    for(auto i = row_pointer[curr]; i < row_pointer[curr + 1]; i++)
    {
      auto next = column_index[i];
      if(label[next] == -1)
      {
        label[next] = currentLabel;
        dfs(next);    // propagate the label
      }
    }
  };

  for(int i = 0; i < num_nodes; i++)
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
auto union_find(const std::vector<int>& row_pointer, const std::vector<int>& column_index)
{
  const auto       num_nodes = row_pointer.size() - 1;
  std::vector<int> parent(num_nodes);
  std::vector<int> rank(num_nodes);    // keep tree relatively balanced
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

  for(int i = 0; i < num_nodes; i++)
  {
    for(auto j = row_pointer[i]; j < row_pointer[i + 1]; j++)
    {
      unite(i, column_index[j]);
    }
  }
  return parent;
}

/********************
* Shiloach-Vishkin CC
********************/
auto shiloach_vishkin(const std::vector<int>& row_pointer, const std::vector<int>& column_index)
{
  const auto       num_nodes = row_pointer.size() - 1;
  std::vector<int> parent(num_nodes);    // same as parent
  std::iota(parent.begin(), parent.end(), 0);

  bool update = true;
  while(update)
  {
    update = false;
    //? Hooking Phase
    //! allowing for parallelism
    for(int i = 0; i < num_nodes; i++)
    {
      auto curr_parent = parent[i];
      for(auto j = row_pointer[i]; j < row_pointer[i + 1]; j++)
      {
        auto next = column_index[j];
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
    for(int i = 0; i < num_nodes; i++)
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

  std::vector<int> row_pointer = { 0, 1, 2, 3, 4, 5, 6 };
  std::vector<int> column_index = { 1, 2, 0, 4, 5, 3 };
  //////////
  // dfs //
  /////////

  auto label = dfs_cc(row_pointer, column_index);
  std::cout << "Labels of dfs_cc: ";
  for(auto& l : label)
  {
    std::cout << l << ' ';
  }

  label = union_find(row_pointer, column_index);
  std::cout << "\n\nLabels of Union Find: ";
  for(auto& l : label)
  {
    std::cout << l << ' ';
  }

  label = shiloach_vishkin(row_pointer, column_index);
  std::cout << "\n\nLabels of Shiloach Vishkin: ";
  for(auto& l : label)
  {
    std::cout << l << ' ';
  }
  return 0;
}