#include <vector>
#include <fstream>

#include "CME212/SFML_Viewer.hpp"
#include "CME212/Util.hpp"
#include "CME212/Color.hpp"
#include "CME212/Point.hpp"

#include "Graph.hpp"
#include "SpaceSearcher.hpp"

int main()

{

    
    // Construct an empty graph
    using GraphType = Graph<int,int>;
    
    using Node = typename GraphType::node_type;
    
    GraphType graph;
    {
        // Create a nodes_file from the first input argument
        std::ifstream nodes_file("data/large.nodes");
        // Interpret each line of the nodes_file as a 3D Point and add to the Graph
        Point p;
        std::vector<Node> nodes;
        while (CME212::getline_parsed(nodes_file, p))
            nodes.push_back(graph.add_node(p));
        
        // Create a tets_file from the second input argument
        std::ifstream tets_file("data/large.tets");
        // Interpret each line of the tets_file as four ints which refer to nodes
        std::array<int,4> t;
        while (CME212::getline_parsed(tets_file, t))
            for (unsigned i = 1; i < t.size(); ++i)
                for (unsigned j = 0; j < i; ++j)
                    graph.add_edge(nodes[t[i]], nodes[t[j]]);
    }
    
    std::cout << graph.num_nodes() << "  " << graph.num_edges() << std::endl;
    
    // Launch a viewer
    CME212::SFML_Viewer viewer;
    auto node_map = viewer.empty_node_map(graph);
    
    // All of the Graph nodes
    //viewer.add_nodes(graph.node_begin(), graph.node_end(), node_map);
    
    // Bounding box of all the nodes
    Box3D bigbb(Point(-2,-2,-2), Point(2,2,2));
    
    // Construct the Searcher
    CME212::Clock clock;
    auto n2p = [](const Node& n) { return n.position(); };
    SpaceSearcher<Node> searcher(bigbb, graph.node_begin(), graph.node_end(), n2p);
    double time = clock.seconds();
    std::cout << "SpaceSearcher Construction Time: " << time << std::endl;
    
    // Spacesearcher subset of the nodes
    Box3D bb(Point(-0.5,-0.5,-0.5), Point(0.5,-0.25,0.3));
    viewer.add_nodes(searcher.begin(bb), searcher.end(bb), node_map);
    //viewer.add_edges(graph.edge_begin(), graph.edge_end(), node_map);
    
    // Create another graph to plot the bb (hypergraph bounding box)
    GraphType bb_graph;
    auto hypr = [&](int a, int b) { return a&(1<<b) ? bb.min()[b] : bb.max()[b]; };
    for (int a = 0; a < 8; ++a) {
        // Create and add the eight points defined by the bounding box
        bb_graph.add_node(Point(hypr(a,0), hypr(a,1), hypr(a,2)));
        // Create and add all edges that have a Hamming distance of two or less
        for (int b = 0; b < a; ++b) {
            int c = b^a;        // Hamming distance mask
            c &= c-1; c &= c-1; // Clear the two least sig bits
            if (c == 0)         // Hamming dist <= 2
                bb_graph.add_edge(bb_graph.node(a), bb_graph.node(b));
        }
    }
    
    viewer.add_nodes(bb_graph.node_begin(), bb_graph.node_end(), node_map);
    viewer.add_edges(bb_graph.edge_begin(), bb_graph.edge_end(), node_map);
    
    viewer.center_view();
    
    viewer.event_loop();

    return 0;
}

