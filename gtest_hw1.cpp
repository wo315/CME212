#include <gtest/gtest.h>
#include <fstream>

#include "CME212/Util.hpp"
#include "Graph.hpp"
#include "shortest_path.hpp"
#include "subgraph.hpp"


class GraphPointFixture : public ::testing::Test {
 protected:
   //Define types
  using GraphType = Graph<int>;
  using NodeType  = typename GraphType::node_type;
  using NodeIter = typename GraphType::node_iterator;
  using EdgeType  = typename GraphType::edge_type;

  //Set up Graph and Points
  GraphType graph;
  std::vector<Point> points;
  virtual void SetUp() {
    for(int i = 0; i < 10; i++)
      points.push_back(Point(i));
  }
  
};

// Test adding node with default value
TEST_F(GraphPointFixture, DefaultNodeVal){
  graph.add_node(points[0]);
  EXPECT_EQ( graph.node(0).value(), 0 ) << "add_node does not intalize node vale with a default 0 value";
}

// Test degree function
TEST_F(GraphPointFixture, Degree){
  NodeType n0 = graph.add_node(points[0]);
  NodeType n1 = graph.add_node(points[1]);
  NodeType n2 = graph.add_node(points[2]);
  graph.add_edge(n0, n1);

  EXPECT_EQ(n2.degree(),0)  << "n2 degree is 0";
  EXPECT_EQ(n1.degree(), 1) << "n1 degree is 1";
}

// Test node iterator
TEST_F(GraphPointFixture, NodeIter){
  graph.add_node(points[0]);
  graph.add_node(points[1]);
  graph.add_node(points[2]);
  
  int iter = 0;
  for(auto ni = graph.node_begin(); ni != graph.node_end(); ++ni){
    ++iter;
  }
  EXPECT_EQ(iter, 3) << " error in node iteration " ;
}

//Test nearest node
TEST_F(GraphPointFixture, ShortestPath){
  graph.add_node(points[0]);
  graph.add_node(points[1]);
  graph.add_node(points[2]);
  
  NodeIter nearest = nearest_node(graph, Point(0));
  EXPECT_EQ( *nearest, graph.node(0)) << " error finding nearest node " ;
}
