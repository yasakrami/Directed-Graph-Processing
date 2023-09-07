# Directed Graph Processing Project

## Project Overview

This final project focuses on working with directed graphs, where nodes are connected by multiple directed edges, each with attributes such as time, distance, and cost. The goal is to modify existing code for Breadth-First Search (BFS) or Depth-First Search (DFS) to incorporate these edge attributes and then perform several operations on the graph.

### Graph Structure

The graph represents trips between cities, with various types of connections (e.g., air, train, bus) associated with time (minutes), distance (kilometers), and cost (Turkish Lira). The graph is directed and may have multiple connections between nodes.

### Project Tasks

#### 1. Modify Graph Structure 

- Modify the BFS or DFS code to include edge attributes for time, distance, and cost in the struct.
- Implement a function called `AddEdges(Graph *g)` to add edges to the graph, considering the Adjacency List approach.
- Display the graph with edge attributes in the Adjacency List format.

#### 2. Path Selection 

- Prompt the user for:
  - A starting node.
  - An ending node.
  - The criteria for the path (time, distance, or cost).
- Traverse the graph from the starting node to the ending node, eliminating redundant nodes based on the selected criteria.
- Remove self-loop edges during this process.
- Print the updated graph to show that extra edges are removed.

#### 3. Cycle Elimination 

- Use a Breadth-First Search (BFS) approach to eliminate cycles in the graph.
- Start from the starting node and traverse the graph:
  - If a visited or being processed node is encountered, eliminate the edge to that node.
  - If the final node is encountered, delete all edges back to other nodes.
- Continue BFS to remove all edges that would create cycles.
- Print the graph to show that it now resembles a Directed Acyclic Graph (DAG).

