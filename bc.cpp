#include <iostream>
#include <queue>
#include <stack>
#include <vector>

auto Brandes(const std::vector<int>& row_pointer, const std::vector<int>& column_index)
{
  const auto         num_nodes = row_pointer.size() - 1;
  std::vector<float> betweenness(num_nodes, 0.0f);
  //For each vertex s, perform a BFS to establish levels and parents
  //! The time complexity is O(V(V+E) + V*V) = O(V*E)
  for(int i = 0; i < num_nodes; i++)
  {
    std::vector<int>   distance(num_nodes, -1);      // Initialize distance from s
    std::vector<int>   path_count(num_nodes, 0);      // Initialize path count
    std::vector<float> score(num_nodes, 0.0f);    // Dependency score for each vertex
    std::queue<int>    path_queue;                      // BFS order
    std::stack<int>    path_stack;                      // reverse BFS order
    std::vector<std::vector<int>> parent(num_nodes);

    distance[i]  = 0;
    path_count[i] = 1;
    path_queue.push(i);
    // BFS traversal
    // for a single source vertex, the time complexity is O(V+E),
    //! so the overall time complexity is O(V+E) for each vertex i
    while(!path_queue.empty())
    {
      const auto source = path_queue.front();
      path_queue.pop();
      path_stack.push(source);
      for(auto i = row_pointer[source]; i < row_pointer[source + 1]; i++)
      {
        const auto target = column_index[i];
        if(distance[target] < 0)
        {
          path_queue.push(target);
          distance[target] = distance[source] + 1;
        }
        // shortest path to target via source?
        if(distance[target] == distance[source] + 1)
        {
          path_count[target] += path_count[source];
          parent[target].push_back(source);
        }
      }
    }
    // traverse the graph in reverse BFS order
    // backtrack to propagate the dependency scores back through the graph,
    // to calculate the BC value for each node.
    //! The time complexity is O(V) for each vertex i
    while(!path_stack.empty())
    {
      const auto curr = path_stack.top();
      path_stack.pop();

      const auto prev = parent[curr];

      for(const auto& prev : parent[curr])    // Iterate through all parents
      {
        score[prev] +=
            (static_cast<float>(path_count[prev]) / path_count[curr]) * (1 + score[curr]);
      }
      if(curr != i)    // if curr is not the source vertex
      {
        betweenness[curr] += score[curr];
      }
    }
  }
  return betweenness;
}

int main()
{
  std::vector<int> row_pointer = { 0, 1, 3, 6, 9, 11, 12 };
  std::vector<int> column_index = { 1, 0, 2, 3, 1, 2, 4, 2, 3, 4, 5, 4 };
  // Compute betweenness centrality using Brandes's algorithm
  // unweighted graph for simplicity
  auto betweenness = Brandes(row_pointer, column_index);
  // Print betweenness centrality
  std::cout << "Betweenness centrality:" << std::endl;
  for(int i = 0; i < betweenness.size(); i++)
  {
    std::cout << "Vertex " << i << ": " << betweenness[i] << std::endl;
  }
}