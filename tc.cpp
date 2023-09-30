#include <iostream>
#include <queue>
#include <stack>
#include <vector>

// Reference: https://github.com/georgegito/vertexwise-triangle-counting/blob/master/src/v3/v3_seq.cpp
// allow for parallelism
auto bfs_tc(const std::vector<int>& row_pointer, const std::vector<int>& column_index)
{
  int        numTriangles = 0;
  const auto num_nodes  = row_pointer.size() - 1;
  // check if two nodes have an edge between them with binary search (require sorted column_index)
  auto intersect = [&](int first, int second) -> bool {
    // std::find is O(N), assuming the iterator is a forward iterator
    // auto first_begin = column_index.begin() + row_pointer[first];
    // auto first_end   = column_index.begin() + row_pointer[first + 1];
    // std::find(first_begin, first_end, second) != first_end;

    auto first_begin = row_pointer[first];
    auto first_end   = row_pointer[first + 1] - 1;
    auto first_mid   = (first_begin + first_end) / 2;
    while(first_begin <= first_end)
    {
      if(column_index[first_mid] == second)
      {
        return true;
      }
      else if(column_index[first_mid] < second)
      {
        first_begin = first_mid + 1;
      }
      else
      {
        first_end = first_mid - 1;
      }
      first_mid = (first_begin + first_end) / 2;
    }
    return false;
  };

  for(int first = 0; first < num_nodes; first++)
  {
    for(int i = row_pointer[first]; i < row_pointer[first + 1]; i++)
    {
      for(int j = i + 1; j < row_pointer[first + 1]; j++)
      {
        const auto second = column_index[i];
        const auto third  = column_index[j];
        if(intersect(second, third))
        {
          numTriangles++;
        }
      }
    }
  }
  return numTriangles / 3;
}

int main()
{
  std::vector<int> row_pointer      = { 0, 3, 5, 7, 10, 12, 14 };
  std::vector<int> column_index      = { 1, 2, 3, 0, 2, 0, 1, 0, 4, 5, 3, 5, 3, 4 };
  const auto       num_nodes = row_pointer.size() - 1;
  for(int i = 0; i < num_nodes; i++)
  {
    std::sort(column_index.begin() + row_pointer[i], column_index.begin() + row_pointer[i + 1]);
  }
  // unweighted graph for simplicity
  auto triangles = bfs_tc(row_pointer, column_index);
  std::cout << "number of trianlges: " << triangles << std::endl;
}