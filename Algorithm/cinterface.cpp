#include <pybind11/pybind11.h>
#include <pybind11/stl.h> 
namespace py = pybind11;
#include "graph.h"
#include<queue>
std::vector<std::string> show_types(graph &g)
{
    return g.node_type;
}
std::vector<std::string>show_all_edges(graph &g){
    std::vector<std::string> res;
    res.reserve(g.edge_num);
    for(auto  &edge:g.edges){
        if(g.has_edge(edge.u,edge.v)){
             res.push_back(edge.u+"--"+edge.v+"("+std::to_string(edge.w)+")");
        }
    }
     return res;
}

std::unordered_map<std::string,std::vector<std::string>>show_all_nodes(graph &g){
    std::vector<std::vector<std::string>> res;
    res=g.showAllNodes();
    std::unordered_map<std::string,std::vector<std::string>> result;
    for(int i=1;i<=6;i++){
        result[g.node_type[i]]=res[i-1];
    }
    return result;
}

Edge queryEdgeByIdx(graph &g,int idx){
    if(idx>=g.edges.size()||idx<0)
    return Edge("","",0);
    if(!g.is_exist_edge[idx])
    return Edge("","",0);
    return g.edges[idx];
}

std::vector<std::string>euler_path(graph &g){
    std::string start_node="" ,end_node="";
    g.hasEulerAccess(&start_node,&end_node,false);
    return g.showEulerPath(start_node,false);
}

std::pair<std::vector<Edge>,int>mst(graph &g){
    int sum=0;
    std::vector<Edge> mst=g.MST(g.edges,g.node_num,false,&sum);
    return std::make_pair(mst,sum);
}

std::pair<std::vector<std::string>, int> shortest_road(graph &g, std::string name1, std::string name2)
{
    if (!g.has_node(name1) || !g.has_node(name2) || name1 == name2)
    {
        return make_pair(std::vector<std::string>(), 0);
    }

    std::vector<std::pair<int, std::string>> vec;
    vec.reserve(2 * g.node_num);
    std::priority_queue<std::pair<int, std::string>, std::vector<std::pair<int, std::string>>, std::greater<std::pair<int, std::string>>> pq(std::greater<std::pair<int, std::string>>(), move(vec));
    std::unordered_map<std::string, ll> dist;
    dist.reserve(2 * g.node_num);
    std::unordered_map<std::string, std::string> prev;
    prev.reserve(2 * g.node_num);
    std::unordered_set<std::string> visited;
    visited.reserve(2 * g.node_num);

    for (auto &type_nodes : g.nodes)
    {

        for (auto &node : type_nodes)
        {
            dist[node.name] = INT_MAX;
        }
    }

    dist[name1] = 0;
    pq.push({0, name1});

    while (!pq.empty())
    {
        auto [current_dist, current] = pq.top();
        pq.pop();

        if (visited.count(current))
            continue;
        visited.insert(current);

        if (current == name2)
            break;

        auto idx = g.nodeName_to_idx[current];
        for (auto &edge : g.nodes[idx.first][idx.second].edges)
        {
            if (!g.is_exist_edge[edge.second])
                continue;

            std::string neighbor = edge.first;
            int edge_idx = edge.second;
            ll new_dist = current_dist + g.edges[edge_idx].w;

            if (new_dist < dist[neighbor])
            {
                dist[neighbor] = new_dist;
                prev[neighbor] = current;
                pq.push({new_dist, neighbor});
            }
        }
    }
    if (dist[name2] == INT_MAX)
    {
        return make_pair(std::vector<std::string>(), -1);
    }
    std::vector<std::string> path;
    for (std::string at = name2; at != ""; at = prev[at])
    {
        path.push_back(at);
    }
    std::reverse(path.begin(), path.end());

    return make_pair(path, dist[name2]);
}

PYBIND11_MODULE(graph, m)
{
    m.doc() = "c++topython"; // optional module docstring
    // 绑定 Node 结构体
    py::class_<Node>(m, "Node")
        .def(py::init<int, std::string, int>(),  // 绑定构造函数
             py::arg("Type"), py::arg("name"), py::arg("viewTime"))
        .def_readwrite("Type", &Node::Type)      // 绑定公共成员变量
        .def_readwrite("name", &Node::name)
        .def_readwrite("viewTime", &Node::viewTime)
        .def_readwrite("edges", &Node::edges)    // 绑定 edges 向量
        .def("__repr__", [](const Node &n) {     // 自定义打印格式
            return "Type:" + std::to_string(n.Type) + 
                   "   name:" + n.name + 
                   "   viewTime:" + std::to_string(n.viewTime);
        });

    // 绑定 Edge 结构体
    py::class_<Edge>(m, "Edge")
        .def(py::init<std::string, std::string, int>(),  // 绑定构造函数
             py::arg("u"), py::arg("v"), py::arg("w"))
        .def_readwrite("u", &Edge::u)      // 绑定公共成员变量
        .def_readwrite("v", &Edge::v)
        .def_readwrite("w", &Edge::w)
        .def("__repr__", [](const Edge &e) {  // 自定义打印格式
            return  e.u + " -- " + e.v + " :" + std::to_string(e.w) + "m";
        });

    py::class_<graph>(m, "graph")
    .def(py::init<>())
   .def("read_nodes", &graph::readNodes)
   .def("read_edges", &graph::readEdges)
   .def("queryNodeByName",&graph::queryNodeByName)
   .def("queryEdgeByName",&graph::queryEdgeByName)
   .def("has_node",&graph::has_node)
   .def("has_edge",&graph::has_edge)
   .def("isConnected",&graph::isConnected)
   .def("hasEulerAccess",&graph::hasEulerAccess,py::arg("start") = nullptr,
        py::arg("end") = nullptr,
        py::arg("is_res") = false)
    .def("updateEdge_weight", &graph::updateEdge_weight)
    .def("updateNode_viewTime", &graph::updateNode_viewTime)
    .def("deleteNode", &graph::deleteNode)
    .def("addNode", &graph::addNode)
    .def("deleteEdge", &graph::deleteEdge)
    .def("addEdge", &graph::addEdge)
    .def("init",&graph::init);

    m.def("show_types", &show_types);
    m.def("show_all_edges", &show_all_edges);
    m.def("show_all_nodes", &show_all_nodes);
    m.def("queryEdgeByIdx", &queryEdgeByIdx);
    m.def("euler_path",&euler_path);
    m.def("MST",&mst);
    m.def("shortestRoad",&shortest_road);
}