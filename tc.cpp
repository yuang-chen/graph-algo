#include <iostream>
#include <queue>
#include <stack>
#include <vector>

// Reference: https://github.com/georgegito/vertexwise-triangle-counting/blob/master/src/v3/v3_seq.cpp
// allow for parallelism
auto bfs_tc(const std::vector<int>& rowPtr, const std::vector<int>& colIdx)
{
  int        numTriangles = 0;
  const auto numVertices  = rowPtr.size() - 1;
  // check if two nodes have an edge between them with binary search (require sorted colIdx)
  auto intersect = [&](int first, int second) -> bool {
    // std::find is O(N), assuming the iterator is a forward iterator
    // auto first_begin = colIdx.begin() + rowPtr[first];
    // auto first_end   = colIdx.begin() + rowPtr[first + 1];
    // std::find(first_begin, first_end, second) != first_end;

    auto first_begin = rowPtr[first];
    auto first_end   = rowPtr[first + 1] - 1;
    auto first_mid   = (first_begin + first_end) / 2;
    while(first_begin <= first_end)
    {
      if(colIdx[first_mid] == second)
      {
        return true;
      }
      else if(colIdx[first_mid] < second)
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

  for(int first = 0; first < numVertices; first++)
  {
    for(int i = rowPtr[first]; i < rowPtr[first + 1]; i++)
    {
      for(int j = i + 1; j < rowPtr[first + 1]; j++)
      {
        const auto second = colIdx[i];
        const auto third  = colIdx[j];
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
  std::vector<int> rowPtr      = { 0, 3, 5, 7, 10, 12, 14 };
  std::vector<int> colIdx      = { 1, 2, 3, 0, 2, 0, 1, 0, 4, 5, 3, 5, 3, 4 };
  const auto       numVertices = rowPtr.size() - 1;
  for(int i = 0; i < numVertices; i++)
  {
    std::sort(colIdx.begin() + rowPtr[i], colIdx.begin() + rowPtr[i + 1]);
  }
  // unweighted graph for simplicity
  auto triangles = bfs_tc(rowPtr, colIdx);
  std::cout << "number of trianlges: " << triangles << std::endl;
}