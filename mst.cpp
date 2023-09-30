#include <iostream>
#include <queue>
#include <vector>
#include <numeric>
//------------//
// Priority Queue
// Prim's algorithm to find the minimum spanning tree
//------------//
auto Prim(const std::vector<int>& row_pointer, const std::vector<int>& column_index, const std::vector<float>& values)
{
  // the same setting as Dijkstra's SSSP
  const auto         num_nodes = row_pointer.size() - 1;
  std::vector<float> distance(num_nodes, std::numeric_limits<float>::max());
  std::vector<int>   parent(num_nodes, -1);
  std::vector<bool>  visited(num_nodes, false);
  std::priority_queue<
      std::pair<float, int>,
      std::vector<std::pair<float, int>>,
      std::greater<std::pair<float, int>>>
      min_heap;
  // start from vertex 0
  min_heap.push({ 0.0, 0 });
  distance[0] = 0.0;

  while(!min_heap.empty())
  {
    auto [dist, source] = min_heap.top();
    min_heap.pop();
    visited[source] = true;

    // iterate over all outgoing edges
    for(auto i = row_pointer[source]; i < row_pointer[source + 1]; i++)
    {
      auto target = column_index[i];
      auto weight = values[i];

      if(!visited[target] && weight < distance[target])
      {
        distance[target] = weight;
        parent[target]   = source;
        min_heap.push({ weight, target });
      }
    }
  }
  return parent;
}
//------------//
// Union Find //
//------------//
auto Kruskal(const std::vector<int>& row_pointer, const std::vector<int>& column_index, const std::vector<float>& values){
  auto num_nodes = row_pointer.size() - 1;
  auto num_edges = column_index.size();
  std::vector<std::tuple<int, int, float>> edges;
  std::vector<std::tuple<int, int, float>> tree;

  for(int i = 0; i < num_nodes; i++) {
    for(auto j = row_pointer[i]; j < row_pointer[i + 1]; j++){
      edges.emplace_back(i, column_index[j], values[j]);
    }
  }
  // O(ElogE) -- the most expensive part
  std::sort(edges.begin(), edges.end(), [](auto& a, auto& b){
    return std::get<2>(a) < std::get<2>(b);
  });
  // Code below here are similar to Union Find in cc.cpp
  std::vector<int> parent(num_nodes);
  std::vector<int> rank(num_nodes);    // keep tree relatively balanced
  std::iota(parent.begin(), parent.end(), 0);
  // find 
  auto find = [&parent](int i) {
    while(parent[i] != i){
      i = parent[i];
    }
    return i;
  };

  auto unite = [&find, &rank, &parent](int i, int j) {
    i = find(i);
    j = find(j);
    if(rank[i] > rank[j]) {
      std::swap(i, j);
    }
    parent[i] = j;
    if(rank[i] == rank[j]) {
      rank[j]++;
    }
  };
  // O(ElogV) 
  for(const auto [source, target, weight] : edges) {
    if(find(source) != find(target)) {
      unite(source, target);
      tree.emplace_back(source, target, weight);
    }
  }
  return tree;
}

int main()
{
  std::vector<int>   row_pointer  = { 0, 3, 5, 7, 10, 12, 14 };
  std::vector<int>   column_index = { 1, 2, 3, 0, 2, 0, 1, 0, 4, 5, 3, 5, 3, 4 };
  std::vector<float> values       = { 1.2, 3.4, 0.5, 1.2, 4.1, 3.4, 4.1, 0.5, 2.8, 1.9, 2.8, 4.7, 1.9, 4.7};

  auto parent = Prim(row_pointer, column_index, values);
  for(size_t i = 0; i < parent.size(); i++)
  {
    std::cout << parent[i] << " ";
  }
  std::cout << std::endl;

  auto mst = Kruskal(row_pointer, column_index, values);
  for (const auto& [u, v, w] : mst) {
    std::cout << u << " - " << v << " (Weight: " << w << ") , ";
  }
  std::cout << std::endl;

}