#include "route_planner.h"
#include "route_model.h"
#include <algorithm>

using namespace std;

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    // TODO 2: Use the m_Model.FindClosestNode method to find the closest nodes to the starting and ending coordinates.
    // Store the nodes you find in the RoutePlanner's start_node and end_node attributes.
    *start_node = m_Model.FindClosestNode(start_x, start_y);
    *end_node = m_Model.FindClosestNode(end_x, end_y);
}


// TODO 3: Implement the CalculateHValue method.
// Tips:
// - You can use the distance to the end_node for the h value.
// - Node objects have a distance method to determine the distance to another node.

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
    return abs(&start_node[0] - &end_node[0]) + abs(&start_node[1] - &end_node[1]);     // Manhattan Distance (h) = | x 1 − x 2 | + | y 1 − y 2 |
}


// TODO 4: Complete the AddNeighbors method to expand the current node by adding all unvisited neighbors to the open list.
// Tips:
// - Use the FindNeighbors() method of the current_node to populate current_node.neighbors vector with all the neighbors.
// - For each node in current_node.neighbors, set the parent, the h_value, the g_value. 
// - Use CalculateHValue below to implement the h-Value calculation.
// - For each node in current_node.neighbors, add the neighbor to open_list and set the node's visited attribute to true.

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
    // current node data    (parent nodes)
    auto x = &current_node->neighbors[0];
    auto y = &current_node->neighbors[1];
    auto g = &current_node->neighbors[2];   // g_value

    for (int i = 0; i < 4; i++)     // loops through current node's potential neighbors     goes up to 4 for north, south, east, and west sides
    {
        auto search_node = Model::Node FindNeighbors(current_node);

        auto g_2 = g + 1;
        auto h = CalculateHValue(current_node);

        open_list.push_back(search_node);
    }
    
}


// TODO 5: Complete the NextNode method to sort the open list and return the next node.
// Tips:
// - Sort the open_list according to the sum of the h value and g value.
// - Create a pointer to the node in the list with the lowest sum.
// - Remove that node from the open_list.
// - Return the pointer.

RouteModel::Node *RoutePlanner::NextNode() {
    bool Compare(const vector<int> node_1, const vector<int> node_2)    // compares f-values of two nodes
    {
        auto f_1 = node_1[2] + node_1[3];   // f_1 = g_1 + h_1
        auto f_2 = node_2[2] + node_2[3];   // f_1 = g_2 + h_2
        return f_1 > f_2;
    }
    sort(open_list->begin(), open_list->end(), Compare);    // sorts two-dimensional vector of ints in DESCENDING order useing Compare function to determine sorting order

    int* low_sum_node = &open_list->end();  // pointer to the node in the list with the lowest sum

    open_list.pop_back(*low_sum_node);

    return *low_sum_node;
}


// TODO 6: Complete the ConstructFinalPath method to return the final path found from your A* search.
// Tips:
// - This method should take the current (final) node as an argument and iteratively follow the 
//   chain of parents of nodes until the starting node is found.
// - For each node in the chain, add the distance from the node to its parent to the distance variable.
// - The returned vector should be in the correct order: the start node should be the first element
//   of the vector, the end node should be the last element.

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;

    // TODO: Implement your solution here.

    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    return path_found;

}


// TODO 7: Write the A* Search algorithm here.
// Tips:
// - Use the AddNeighbors method to add all of the neighbors of the current node to the open_list.
// - Use the NextNode() method to sort the open_list and return the next node.
// - When the search has reached the end_node, use the ConstructFinalPath method to return the final path that was found.
// - Store the final path in the m_Model.path attribute before the method exits. This path will then be displayed on the map tile.

void RoutePlanner::AStarSearch() {
    RouteModel::Node *current_node = nullptr;

    // TODO: Implement your solution here.

}