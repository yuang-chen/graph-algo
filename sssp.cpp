#include <iostream>
#include <queue>
#include <vector>

std::vector<int> bellmanFord(const int root, const std::vector<int>& rowPtr,
                             const std::vector<int>& colIdx, const std::vector<float>& weight)
{
  const auto         numVertices = rowPtr.size() - 1;
  std::vector<float> distance(numVertices, std::numeric_limits<float>::max());
  std::vector<int>   parent(numVertices, -1);

  distance[root] = 0.0f;
  // The shortest path between any two vertices in a graph can contain at most numVertices - 1 edges.
  // If there are more edges in the path, it means there must be a cycle in the path,
  //  hence, numVertices - 1 iterations of edge relaxation are required
  for(size_t i = 0; i < numVertices - 1; i++)
  {
    for(size_t curr = 0; curr < numVertices; ++curr)
    {
      for(auto j = rowPtr[curr]; j < rowPtr[curr + 1]; j++)
      {
        const auto next = colIdx[j];
        const auto wgt  = weight[j];
        if(distance[curr] != std::numeric_limits<float>::max()
           && (distance[curr] + wgt < distance[next]))    // check if curr is valid
        {
          distance[next] = distance[curr] + wgt;
          parent[next]   = curr;
        }
      }
    }
  }

  // check for negative cycles
  // The sum of the edge weights along the cycle is negative.
  // If you detect a change in the distance of any vertex during this additional iteration,
  // it implies the presence of a negative-weight cycle in the graph.
  // The reason is that the negative cycle allows you to keep going around it, reducing the distance with each iteration.
  // in the absence of negative-weight cycles, the distance of each vertex should have stabilized after V-1 iterations.
  for(size_t curr = 0; curr < numVertices; curr++)
  {
    for(auto j = rowPtr[curr]; j < rowPtr[curr + 1]; j++)
    {
      const auto next = colIdx[j];
      const auto wgt  = weight[j];
      if(distance[curr] != std::numeric_limits<float>::max()
         && (distance[curr] + wgt < distance[next]))
      {
        std::cout << "Negative Cycle Detected\n";
        return {};
      }
    }
  }
  // Print shortest distances from the curr
  //   std::cout << "Shortest distances from vertex " << root << ":" << std::endl;
  //   for(int i = 0; i < numVertices; i++)
  //   {
  //     std::cout << "Vertex " << i << ": " << distance[i] << std::endl;
  //   }
  return parent;
}

auto dijkstra(const int root, const std::vector<int>& rowPtr, const std::vector<int>& colIdx,
              const std::vector<float>& weight)
{
  const auto         numVertices = rowPtr.size() - 1;
  std::vector<float> distance(numVertices, std::numeric_limits<float>::max());
  std::vector<int>   parent(numVertices, -1);
  std::priority_queue<std::pair<float, int>, std::vector<std::pair<float, int>>,
                      std::greater<std::pair<float, int>>>
      min_heap;

  distance[root] = 0.0f;
  min_heap.push(std::make_pair(0.0f, root));

  while(!min_heap.empty())
  {
    const auto [dist, curr] = min_heap.top();
    min_heap.pop();
    // a shorter path to vertex curr has already been discovered and processed
    // further exploration of u is unnecessary.
    if(dist > distance[curr])
    {
      continue;
    }
    for(auto i = rowPtr[curr]; i < rowPtr[curr + 1]; i++)
    {
      const auto next = colIdx[i];
      const auto wgt  = weight[i];
      if(distance[curr] != std::numeric_limits<float>::max()
         && (distance[curr] + wgt < distance[next]))
      {
        distance[next] = distance[curr] + wgt;
        parent[next]   = curr;
        min_heap.emplace(distance[next], next);
      }
    }
  }
  // Print shortest distances from the curr
  //   std::cout << "Shortest distances from vertex " << root << ":" << std::endl;
  //   for(int i = 0; i < numVertices; i++)
  //   {
  //     std::cout << "Vertex " << i << ": " << distance[i] << std::endl;
  //   }
  return parent;
}

int main()
{
  std::vector<int>   rowPtr = { 0, 2, 4, 6, 8, 10, 12 };
  std::vector<int>   colIdx = { 1, 2, 0, 3, 0, 1, 4, 1, 2, 5, 3, 5 };
  std::vector<float> weight = { 1.2, 2.3, 0.5, 3.1, 4.4, 0.7, 2.8, 1.9, 0.8, 2.0, 1.5, 3.3 };

  auto path = bellmanFord(0, rowPtr, colIdx, weight);

  if(path.empty())
  {
    std::cout << "Path Not Found\n";
  }
  else
  {
    std::cout << "Path from Root to next: ";
    for(size_t i = 0; i < path.size(); i++)
    {
      std::cout << path[i] << " ";
    }
  }
  std::cout << "\n";

  path = dijkstra(0, rowPtr, colIdx, weight);

  if(path.empty())
  {
    std::cout << "Path Not Found\n";
  }
  else
  {
    std::cout << "Path from Root to next: ";
    for(size_t i = 0; i < path.size(); i++)
    {
      std::cout << path[i] << " ";
    }
  }

  return 0;
}