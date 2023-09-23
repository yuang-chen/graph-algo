#include <iostream>
#include <queue>
#include <stack>
#include <vector>

auto brandes(const std::vector<int>& rowPtr, const std::vector<int>& colIdx)
{
  const auto         numVertices = rowPtr.size() - 1;
  std::vector<float> betweenness(numVertices, 0.0f);
  //For each vertex s, perform a BFS to establish levels and parents
  //! The time complexity is O(V(V+E) + V*V) = O(V*E)
  for(int i = 0; i < numVertices; i++)
  {
    std::vector<int>   distance(numVertices, -1);      // Initialize distance from s
    std::vector<int>   pathCount(numVertices, 0);      // Initialize path count
    std::vector<float> DepScore(numVertices, 0.0f);    // Dependency score for each vertex
    std::queue<int>    pathQueue;                      // BFS order
    std::stack<int>    pathStack;                      // reverse BFS order
    std::vector<std::vector<int>> parent(numVertices);

    distance[i]  = 0;
    pathCount[i] = 1;
    pathQueue.push(i);
    // BFS traversal
    // for a single source vertex, the time complexity is O(V+E),
    //! so the overall time complexity is O(V+E) for each vertex i
    while(!pathQueue.empty())
    {
      const auto source = pathQueue.front();
      pathQueue.pop();
      pathStack.push(source);
      for(auto i = rowPtr[source]; i < rowPtr[source + 1]; i++)
      {
        const auto target = colIdx[i];
        if(distance[target] < 0)
        {
          pathQueue.push(target);
          distance[target] = distance[source] + 1;
        }
        // shortest path to target via source?
        if(distance[target] == distance[source] + 1)
        {
          pathCount[target] += pathCount[source];
          parent[target].push_back(source);
        }
      }
    }
    // traverse the graph in reverse BFS order
    // backtrack to propagate the dependency scores back through the graph,
    // to calculate the BC value for each node.
    //! The time complexity is O(V) for each vertex i
    while(!pathStack.empty())
    {
      const auto curr = pathStack.top();
      pathStack.pop();

      const auto prev = parent[curr];

      for(const auto& prev : parent[curr])    // Iterate through all parents
      {
        DepScore[prev] +=
            (static_cast<float>(pathCount[prev]) / pathCount[curr]) * (1 + DepScore[curr]);
      }
      if(curr != i)    // if curr is not the source vertex
      {
        betweenness[curr] += DepScore[curr];
      }
    }
  }
  return betweenness;
}

int main()
{
  std::vector<int> rowPtr = { 0, 1, 3, 6, 9, 11, 12 };
  std::vector<int> colIdx = { 1, 0, 2, 3, 1, 2, 4, 2, 3, 4, 5, 4 };
  // Compute betweenness centrality using Brandes's algorithm
  // unweighted graph for simplicity
  auto betweenness = brandes(rowPtr, colIdx);
  // Print betweenness centrality
  std::cout << "Betweenness centrality:" << std::endl;
  for(int i = 0; i < betweenness.size(); i++)
  {
    std::cout << "Vertex " << i << ": " << betweenness[i] << std::endl;
  }
}