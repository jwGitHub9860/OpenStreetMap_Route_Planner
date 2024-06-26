#include "route_planner.h"
#include "route_model.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <cstddef>

using namespace std;

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    // TODO 2: Use the m_Model.FindClosestNode method to find the closest nodes to the starting and ending coordinates.
    // Store the nodes you find in the RoutePlanner's start_node and end_node attributes.
    start_node = &m_Model.FindClosestNode(start_x, start_y);
    end_node = &m_Model.FindClosestNode(end_x, end_y);
}


// TODO 3: Implement the CalculateHValue method.
// Tips:
// - You can use the distance to the end_node for the h value.
// - Node objects have a distance method to determine the distance to another node.

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
    return node->distance(*end_node);     // ACCESSES Distance Formula (h) = sqrt( ( x_2 − x_1 )^2 + ( y_2 − y_1 )^2 )           -> - FOR POINTERS       . - FOR ACTUAL VALUE ITSELF     --->    Do NOT use "node->distance(*end_node)"
}


// TODO 4: Complete the AddNeighbors method to expand the current node by adding all unvisited neighbors to the open list.
// Tips:
// - Use the FindNeighbors() method of the current_node to populate current_node.neighbors vector with all the neighbors.
// - For each node in current_node.neighbors, set the parent, the h_value, the g_value. 
// - Use CalculateHValue below to implement the h-Value calculation.
// - For each node in current_node.neighbors, add the neighbor to open_list and set the node's visited attribute to true.

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
    current_node->FindNeighbors();     // (pointer_name)->(variable_name)        fills "current_node.neighbors" vector with neighbors       DON'T KNOW HOW MANY NEIGHBORS EACH NODE HAS

    for (RouteModel::Node* neighbor : current_node->neighbors)     // loops through current node's potential neighbors     Either: NEVER USE "auto" ---> (Must Be Specific) or Put Type BEFORE "auto"     goes up to 4 for north, south, east, and west sides
    {
        neighbor->parent = current_node;    // sets parent node
        neighbor->h_value = CalculateHValue(neighbor);      // sets h_value
        neighbor->g_value = current_node->g_value + current_node->distance(*neighbor);      // sets g_value         g_2 = g + 1

        open_list.push_back(neighbor);      // ADDS "current_node" to "open_list"        'push_back()' adds EXISTING node into container        'emplace_back()' CREATES NEW node & adds Directly into container
        neighbor->visited = true;      // MARKS node as visited (true)
    }
}


// TODO 5: Complete the NextNode method to sort the open list and return the next node.
// Tips:
// - Sort the open_list according to the sum of the h value and g value.
// - Create a pointer to the node in the list with the lowest sum.
// - Remove that node from the open_list.
// - Return the pointer.

RouteModel::Node *RoutePlanner::NextNode() {
    sort(open_list.begin(), open_list.end(), [](RouteModel::Node* node_1, RouteModel::Node* node_2){return (node_1->g_value + node_1->h_value) > (node_2->g_value + node_2->h_value);});    // sorts two-dimensional vector of ints in DESCENDING order using Compare function to determine sorting order           -> - FOR POINTERS       . - FOR ACTUAL VALUE ITSELF

    RouteModel::Node* low_sum_node = open_list.back();  // WANT LAST ELEMENT (pointer to node in list with LOWEST SUM)          'end()' - returns POINTER          'back()' - returns LAST ELEMENT

    open_list.pop_back();      // DON'T PUT ANYTHING IN () OF pop_back() since it ALWAYS deletes the LAST element of the list

    return low_sum_node;
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
    RouteModel::Node *c_n = current_node;

    while (c_n != start_node)  // iterate through nodes (current_node) until start_node        start_node is a POINTER        MUST COMPARE "POINTER" WITH "POINTER"
    {
        path_found.emplace_back(*c_n);        // adds "current_node" ITSELF to "path_found"        'emplace_back()' adds NEW node DIRECTLY into "path_found" vector        'push_back()' adds EXISTING node into container        line meaning ---> path_found.push_back(current_node);
        RouteModel::Node *par_c_n = c_n->parent;    // puts 'c->parent' into POINTER        current_node LOCATION -> parent

        distance += c_n->distance(*par_c_n);  // adds distance from node to parent to "distance"        'distance()' will ONLY take Pointer, NOT take 'c->parent'
        c_n = c_n->parent;  // changes "current_node" LOCATION to "parent_node" LOCATION
    }

    path_found.push_back(*c_n);        // adds EXISTING "current_nodes" to "path_found"        'push_back()' adds EXISTING node into container        'emplace_back()' adds NEW node DIRECTLY into "path_found" vector        line meaning ---> path_found.push_back(current_node);

    reverse(path_found.begin(), path_found.end());    // reverse "PATH_FOUND" vector

    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.      FIND DISTANCE FIRST BEFORE REVERSING VECTOR
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

    // Add "start_node" to "open_list" & Mark as visited ONLY		BECAUSE		"start_node" does NOT have a "parent node", "h_value", or "g_value"
    open_list.push_back(start_node);
    start_node->visited = true;

    current_node = start_node;   // initializes current_node		'open_list' vector ALREADY INTIALIZED

    while (open_list.size() > 0)   // checks if open_list vector is NOT EMPTY
    {
        AddNeighbors(current_node);    // MARKS current node (visited) & ADDS "current_node" to "open_list"        CHECKS IF GOAL WAS REACHED        if goal NOT REACHED, checks next node
        current_node = NextNode();     // sorts "open_list{}" & returns next node

        if (current_node->distance(*end_node) == 0)  // CHECKS IF GOAL WAS REACHED        using distance (equation) between current_node and end_node
        {
            m_Model.path = ConstructFinalPath(end_node);    // stores final path in m_Model.path & returns FINAL PATH
            break;      // Exits while loop
        }
    }
}

// help was recieved by users Thuong-ironman, Paulo, Kathy L, Sriteja M, Dohyeong, and Nayir