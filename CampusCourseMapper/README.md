# Campus Navigation & Course Planner

A C program that provides:
- 🏛️ **Campus Navigation**: Dijkstra's algorithm for shortest paths between buildings
- 📚 **Course Planner**: Topological sort for prerequisite-based course scheduling

## Features
- **Building Network**:
  - Loads building connections from file
  - Calculates shortest paths using Dijkstra's algorithm
  - Displays routes with intermediate stops

- **Course Scheduling**:
  - Parses course prerequisites
  - Detects cycles in prerequisites
  - Generates valid course sequences via topological sort
