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
    //return abs(&start_node[0] - &end_node[0]) + abs(&start_node[1] - &end_node[1]);     // Manhattan Distance (h) = | x 1 − x 2 | + | y 1 − y 2 |
    //return sqrt(pow((&node[0] - &end_node[0]), 2) + pow((&node[1] - &end_node[1]), 2));     // Distance Formula (h) = sqrt( ( x_2 − x_1 )^2 + ( y_2 − y_1 )^2 )

    return (*node).distance(*end_node);     // ACCESSES Distance Formula (h) = sqrt( ( x_2 − x_1 )^2 + ( y_2 − y_1 )^2 )
    //return node->distance(*end_node);     // ACCESSES Distance Formula (h) = sqrt( ( x_2 − x_1 )^2 + ( y_2 − y_1 )^2 )
    //return node->distance(end_node);     // ACCESSES Distance Formula (h) = sqrt( ( x_2 − x_1 )^2 + ( y_2 − y_1 )^2 )
}


// TODO 4: Complete the AddNeighbors method to expand the current node by adding all unvisited neighbors to the open list.
// Tips:
// - Use the FindNeighbors() method of the current_node to populate current_node.neighbors vector with all the neighbors.
// - For each node in current_node.neighbors, set the parent, the h_value, the g_value. 
// - Use CalculateHValue below to implement the h-Value calculation.
// - For each node in current_node.neighbors, add the neighbor to open_list and set the node's visited attribute to true.

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
    const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}}    // directional deltas
    
    // current node data    (parent nodes)
    RouteModel::Node** x = &current_node->neighbors[0];
    RouteModel::Node** y = &current_node->neighbors[1];
    RouteModel::Node** g = &current_node->neighbors[2];   // g_value

    for (auto neighbor : current_node->neighbors)     // loops through current node's potential neighbors     goes up to 4 for north, south, east, and west sides
    {
        current_node->FindNeighbors();     // (pointer_name)->(variable_name)        fills "current_node.neighbors" vector with neighbors       DON'T KNOW HOW MANY NEIGHBORS EACH NODE HAS
        
        RouteModel::Node** g_2 = g + 1;
        float h = CalculateHValue(current_node);

        open_list.push_back(current_node);
    }
    
}


// TODO 5: Complete the NextNode method to sort the open list and return the next node.
// Tips:
// - Sort the open_list according to the sum of the h value and g value.
// - Create a pointer to the node in the list with the lowest sum.
// - Remove that node from the open_list.
// - Return the pointer.

bool Compare(RouteModel::Node* node_1, RouteModel::Node* node_2)    // compares f-values of two nodes
{
    return (node_1->g_value + node_1->h_value) > (node_2->g_value + node_2->h_value);   // return f_1 > f_2             f_1 = g_1 + h_1             f_2 = g_2 + h_2
}

RouteModel::Node *RoutePlanner::NextNode() {
    sort(open_list->begin(), open_list->end(), Compare);    // sorts two-dimensional vector of ints in DESCENDING order useing Compare function to determine sorting order

    RouteModel::Node* low_sum_node = &open_list.back();  // WANT LAST ELEMENT (pointer to node in list with LOWEST SUM)          'end()' - returns POINTER          'back()' - returns LAST ELEMENT

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
    //auto final_node = *current_node + i;     // NOTE (delete later)
    while (current_node != start_node)  // iterate through nodes until start_node
    {
        path_found.push_back(*current_node);
        current_node = (*current_node).parent;  // changes "current_node" to "parent_node"
        //current_node->distance();

        distance += (*current_node).distance(*((*current_node).parent))  // adds distance from node to parent to "distance"
    }
    
    reverse(open_list.begin(), open_list.end());    // reverse vector

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
    RouteModel::Node *current_node = nullptr;   // initializes *current_node

    // TODO: Implement your solution here.
    open_list{};    // HOLDS ALL NODES (ALREADY HAS NODES IN IT) intialize?
    //vector<vector<int>> open_nodes_list{};  // JUST IN CASE

    // starting node    JUST IN CASE (maybe)
    /*int x = start_x;
    int y = start_y;
    int g = 0;
    int h = CalculateHValue(*current_node);   // JUST IN CASE (maybe)*/
    
    AddNeighbors(current_node);    // CHECKS IF GOAL WAS REACHED (maybe?)

    while (open_list.size() > 0)   // checks if open_list vector is NOT EMPTY
    {
        RouteModel::Node* next_node = NextNode();     // sorts "open_list{}" & returns next node

        RouteModel::Node* x = &next_node[0];    // obtains x value from "next_node"
        RouteModel::Node* y = &next_node[1];    // obtains y value from "next_node"

        if (x == &end_node[0] && y == &end_node[1])  // CHECKS IF GOAL WAS REACHED
        {
            m_Model.path = ConstructFinalPath(current_node);    // stores final path in m_Model.path
            return ConstructFinalPath(end_node);   // returns FINAL PATH & Exits while loop
        }
        
    }
    
    AddNeighbors(current_node);    // ALSO checks other nodes (maybe?)
}