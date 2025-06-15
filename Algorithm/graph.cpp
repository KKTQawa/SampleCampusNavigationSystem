#include "graph.h"
#include <iostream>
#include <fstream>
#include<sstream>
#include<stack>
#include<unordered_set>
#include <queue>
#include <algorithm>
#include<functional>
void graph::readNodes(std::string FileName)
{
    // if(FileName.size()<4 || FileName.substr(FileName.size()-4, 4)!=".csv")
    if (FileName != "nodes.csv")
        throw std::runtime_error("Error: Could not found file nodes.csv");
    std::ifstream file(FileName);
    if (!file.is_open())
        throw std::runtime_error("Error: Could not open file " + FileName);

    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line))
    {
        lineNumber++;
        std::vector<std::string> columns;
        std::stringstream ss(line);
        std::string column;

        while (std::getline(ss, column, ','))
        {
            columns.push_back(column);
        }

        // 检查.csv文件是否有3列
        if (columns.size() != 3)
        {
            std::cerr << "Error in line " << lineNumber
                 << ": Expected 3 columns, found " << columns.size() << std::endl;
            continue;
        }

        if(columns[2]=="weight")continue;//跳过第一行

        addNode(columns[0], stoi(columns[1]), stoi(columns[2]));
    }

    file.close();
}
void graph::readEdges(std::string FileName)
{
    // if(FileName.size()<4 || FileName.substr(FileName.size()-4, 4)!=".csv")
    if (FileName != "edges.csv")
        throw std::runtime_error("Error:  Could not found file edges.csv");
    // FileName = "../" + FileName;
    std::ifstream file(FileName);
    if (!file.is_open())
        throw std::runtime_error("Error: Could not open file " + FileName);

    std::string line;
    int lineNumber = 0;

    while (getline(file, line))
    {
        lineNumber++;
        std::vector<std::string> columns;
        std::stringstream ss(line);
        std::string column;

        while (getline(ss, column, ','))
        {
            columns.push_back(column);
        }

        // 检查.csv文件是否有3列
        if (columns.size() != 3)
        {
            std::cerr << "Error in line " << lineNumber
                 << ": Expected 3 columns, found " << columns.size() << std::endl;
            continue;
        }
        
        if(columns[2]=="weight")continue;//跳过第一行
        if (!has_node(columns[0]) || !has_node(columns[1]))
        {
            std::cerr << "节点" << columns[0] << "或" << columns[1] << "不存在！" << std::endl;
            continue;
        }
        addEdge(columns[0], columns[1], stoi(columns[2]));
    }

    file.close();
}
bool graph::has_edge(std::string name1,std::string name2){
    int u1=nodeName_to_idx[name1].first,u2=nodeName_to_idx[name1].second;
    Node& tmp =nodes[u1][u2];
    for(auto it:tmp.edges){
        if(it.first==name2 && is_exist_edge[it.second])return true;
    }
    return false;
}
bool graph::has_node(std::string name){
    if(!nodeName_to_idx.count(name)||nodeName_to_idx[name]==std::make_pair(0,0))
        return false;
    return true;
}
Node& graph::queryNodeByName(std::string name){
    if(!has_node(name))
        throw std::runtime_error("节点不存在！");
    int id1=nodeName_to_idx[name].first,id2=nodeName_to_idx[name].second;
    return nodes[id1][id2];
}
std::vector<Node> graph::queryNodeByType(int type)
{
    if (type < 1 || type > 6)
        throw std::runtime_error("没有该类型的节点！");
    std::vector<Node>tmp;
    for(auto it:nodes[type]){
        if(has_node(it.name)){
            tmp.push_back(it);
        }
    }
    return tmp;
}
std::vector<std::vector<std::string>> graph::showAllNodes(){
    std::vector<std::vector<std::string>>ans;
    for(int i=1;i<=6;i++){
        std::vector<Node>tmp=queryNodeByType(i);
        std::vector<std::string>t;
        for(auto it:tmp){
            t.push_back(it.name);
        }
        ans.push_back(t);
    }
    return ans;
}

Edge graph::queryEdgeByName(std::string name1, std::string name2)
{
    std::vector<std::pair<std::string, int>> &edges1 = nodes[nodeName_to_idx[name1].first][nodeName_to_idx[name1].second].edges;
    std::vector<std::pair<std::string, int>> &edges2 = nodes[nodeName_to_idx[name2].first][nodeName_to_idx[name2].second].edges;
    if (edges1.size() < edges2.size())
    {
        for (auto &edge : edges1)
        {
            if (edge.first == name2 && is_exist_edge[edge.second])
            {
                return edges[edge.second];
            }
        }
    }
    else
    {
        for (auto &edge : edges2)
        {
            if (edge.first == name1 && is_exist_edge[edge.second])
            {
                return edges[edge.second];
            }
        }
    }
    return Edge("", "", 0);
}
void graph::addNode(std::string name, int type, int viewTime)
{
    if (has_node(name)){
        throw std::runtime_error("节点已存在");
        return;
    }
    //std::cout<<"添加节点:"<<name<<" "<<type<<" "<<viewTime<<std::endl;
    nodes[type].push_back(Node(type, name, viewTime));
    nodeName_to_idx[name] = {type, nodes[type].size() - 1};
    node_num++;
    return ;
}
void graph::addEdge(std::string name1, std::string name2, int weight)
{
    if (has_edge(name1, name2))
    {
        throw std::runtime_error(name1+"-"+name2+" 已存在");
        return;
    }
    if(name1 == name2){
        throw std::runtime_error("不能自环！");
        return;
    }
    edges.push_back(Edge(name1, name2, weight));
    int k = edges.size() - 1;
    int u1 = nodeName_to_idx[name1].first, u2 = nodeName_to_idx[name1].second;
    int v1 = nodeName_to_idx[name2].first, v2 = nodeName_to_idx[name2].second;
    nodes[u1][u2].edges.push_back({name2, k});
    nodes[v1][v2].edges.push_back({name1, k});
    is_exist_edge[k] = 1;
    edge_num++;
}
bool graph::updateNode_viewTime(std::string name, int viewTime)
{
    if (!has_node(name))
    {
        throw std::runtime_error("节点不存在！");
        return false;
    }
    int id1 = nodeName_to_idx[name].first, id2 = nodeName_to_idx[name].second;
    nodes[id1][id2].viewTime = viewTime;
    return true;
}
bool graph::updateEdge_weight(std::string name1, std::string name2, int weight){
    if(!has_edge(name1,name2)){
        throw std::runtime_error("边不存在！");
        return  false; 
    }
    std::vector<std::pair<std::string,int>>&edges1=nodes[nodeName_to_idx[name1].first][nodeName_to_idx[name1].second].edges;
    std::vector<std::pair<std::string,int>>&edges2=nodes[nodeName_to_idx[name2].first][nodeName_to_idx[name2].second].edges;
    if(edges1.size()<edges2.size()){
        for(auto&edge:edges1){
            if(edge.first==name2&&is_exist_edge[edge.second]){
                edges[edge.second].w=weight;
                return true;
            }
        }
    }else{
        for(auto&edge:edges2){
            if(edge.first==name1&&is_exist_edge[edge.second]){
                edges[edge.second].w=weight;
                return true;
            }
        }
    }
}
void graph::deleteNode(std::string name,bool is_show){
    if(!has_node(name)){
        throw std::runtime_error("节点不存在！");
       return; 
    }
    std::pair<int,int>idx=nodeName_to_idx[name];
    std::vector<std::pair<std::string, int>>& tmp = nodes[idx.first][idx.second].edges;
    int count=0;
    for(auto it:tmp){
       Edge ee= deleteEdge(name,it.first);
       if(is_show&&ee.u!=""){
        count++;
          std::cout<<"已删除边："<<ee.u<<"--"<<ee.v<<" 长度："<<ee.w<<std::endl; 
       }
    }
    nodeName_to_idx.erase(name);
    node_num--;
    if(is_show)
    std::cout<<"附带删除了"<<count<<"条边"<<std::endl;
    return;
}
Edge graph::deleteEdge(std::string name1, std::string name2)
{
    if (!has_edge(name1, name2))
        return Edge("", "", 0);
    std::vector<std::pair<std::string, int>> &edges1 = nodes[nodeName_to_idx[name1].first][nodeName_to_idx[name1].second].edges;
    std::vector<std::pair<std::string, int>> &edges2 = nodes[nodeName_to_idx[name2].first][nodeName_to_idx[name2].second].edges;
    if (edges1.size() < edges2.size())
    {
        for (auto &edge : edges1)
        {
            if (edge.first == name2 && is_exist_edge[edge.second])
            {
                is_exist_edge[edge.second] = 0;
                edge_num--;
                return edges[edge.second];
            }
        }
    }
    else
    {
        for (auto &edge : edges2)
        {
            if (edge.first == name1 && is_exist_edge[edge.second])
            {
                is_exist_edge[edge.second] = 0;
                edge_num--;
                return edges[edge.second];
            }
        }
    }
    return Edge("", "", 0);
}
bool graph::isConnected() {
    if (node_num == 0)
        return true; // 空图视为连通

    std::unordered_set<std::string> visited;
    visited.reserve(2*node_num);//减少哈希冲突
    std::queue<std::string> q;

    // 从第一个存在的节点开始BFS
    for (auto &type_nodes : nodes)
    {
        int flag = 0;
        for (int i = 0; i < type_nodes.size(); i++)
        {
            if (has_node(type_nodes[i].name))
            {
                flag = 1;
                q.push(type_nodes[i].name);
                visited.insert(type_nodes[i].name);
                break;
            }
        }
        if (flag)
            break;
    }

    while (!q.empty()) {
        std::string current = q.front();
        q.pop();
        
        auto idx = nodeName_to_idx[current];
        for (auto& edge : nodes[idx.first][idx.second].edges) {
            if (is_exist_edge[edge.second] && !visited.count(edge.first)) {
                visited.insert(edge.first);
                q.push(edge.first);
            }
        }
    }
    
    // 检查是否所有节点都被访问过
    return visited.size()==node_num;
}
bool graph::hasEulerAccess(std::string* start,std::string*end,bool is_res){
    if (!isConnected()) return false;
    
    std::unordered_map<std::string, int> degrees;
    degrees.reserve(2*node_num);
    
    // 计算所有节点的度数
    for (auto& edge : edges) {
        if (is_exist_edge[&edge - &edges[0]]) {
            degrees[edge.u]++;
            degrees[edge.v]++;
        }
    }
    std::string start_node;
    std::string end_node;
    std::string tmp_name;
    int flag=0;
    int odd_count = 0;
    for (auto& [name, degree] : degrees) {
        if (degree % 2 != 0) {
            odd_count++;
           if(start_node=="") start_node = name;
           else if(end_node=="") end_node=name;
        }
        else if(degree==0)flag=1;
        tmp_name=name;
    }

    // 欧拉通路条件：0或2个奇数度节点
    bool has_euler = (odd_count == 0 || odd_count == 2||flag);
    if (has_euler&&start != NULL && end != NULL)
    {
        if (odd_count == 0)
        {
            start_node  = tmp_name;
            *start = start_node;
            *end = start_node;
            if(is_res)std::cout<<"存在欧拉回路！"<<std::endl;
        }
        else if (odd_count == 2)
        {
            *start = start_node;
            *end = end_node;
            if(is_res)std::cout<<"存在欧拉通路！"<<std::endl;
        }
    }
    return has_euler;
}
std::vector<std::string> graph::showEulerPath(const std::string& start_node,bool is_show) {
    if(!has_node(start_node)){
        throw std::runtime_error("起始点不存在！");
        return {};
    }
    
    // 创建邻接表并记录未访问的边
    std::unordered_map<std::string, std::vector<std::string>> adj;
    adj.reserve(2*node_num);
    for (const auto& edge : edges) {
        if (is_exist_edge[&edge - &edges[0]]) {
            adj[edge.u].push_back(edge.v);
            adj[edge.v].push_back(edge.u);
        }
    }

    // // 确保按节点名称排序（输出有序结果）
    // for (auto& [node, neighbors] : adj) {
    //     std::sort(neighbors.begin(), neighbors.end());
    // }

    std::stack<std::string> stack;
    std::vector<std::string> path;
    path.reserve(edge_num+10);
    std::string current = start_node;
    stack.push(current);

    while (!stack.empty()) {
        current = stack.top();
        
        if (!adj[current].empty()) {
            std::string next = adj[current].back();
            adj[current].pop_back();
            
            // 删除反向边
            auto& neighbors = adj[next];
            neighbors.erase(std::find(neighbors.begin(), neighbors.end(), current));
            
            stack.push(next);
        } else {
            path.push_back(current);
            stack.pop();
        }
    }

    if (is_show)
    {
        // 输出路径
        std::cout << "Euler Path: ";
        int count=1;
        for (int i = path.size() - 1; i >= 0; i--)
        {
            if(count>1000){
                std::cout<<"仅展示前1000条数据..."<<std::endl;
                break;
            }
            if (i != path.size() - 1)
                std::cout << " -> ";
            std::cout << path[i];
            count++;
        }
        std::cout << std::endl;
    }
    return path;
}
void graph::showAllEdges(bool isSortedByWeight) {
    if(edge_num==0){
       std::cout<<"当前图没有边!"<<std::endl;
       return; 
    }
    std::vector<Edge> active_edges;
    active_edges.reserve(edge_num+10);
    
    for (int i = 0; i < edges.size(); i++) {
        if (is_exist_edge[i]) {
            active_edges.push_back(edges[i]);
        }
    }
    
    if (isSortedByWeight) {
        sort(active_edges.begin(), active_edges.end());
    }
    
    std::cout << "所有存在的边：" << std::endl;
    int total_length=0;
    int count=1;
    for (auto& edge : active_edges) {
        if(count==1001){
            std::cout<<"仅展示前1000条数据..."<<std::endl;
        }
        total_length+=edge.w;
        if(count>1000)continue;
        std::cout << edge.u << " -- " <<  edge.v<<":"<<edge.w << std::endl;
        count++;
    }
    std::cout<<"总长度:"<<total_length<<std::endl;
}
void graph::autoAddEdgesToConnected(bool showInfo,int defaultWeight) {
    if (isConnected()) return;
    
    std::vector<std::vector<std::string>> components;
    std::unordered_set<std::string> visited;
    visited.reserve(node_num+5);
    
    // 找到所有连通分量
    for (auto& type_nodes : nodes) {
        for (auto& node : type_nodes) {
            if (has_node(node.name)&&!visited.count(node.name)) {
                std::vector<std::string> component;
                component.reserve(node_num);
                std::queue<std::string> q;
                q.push(node.name);
                visited.insert(node.name);
                
                while (!q.empty()) {
                    std::string current = q.front();
                    q.pop();
                    component.push_back(current);
                    
                    auto idx = nodeName_to_idx[current];
                    for (auto& edge : nodes[idx.first][idx.second].edges) {
                        if (is_exist_edge[edge.second] && !visited.count(edge.first)) {
                            visited.insert(edge.first);
                            q.push(edge.first);
                        }
                    }
                }
                components.push_back(component);
            }
        }
    }
    if(showInfo)std::cout<<"共有"<<components.size()<<"个连通分量"<<std::endl;
    // 连接连通分量
    for (int i = 1; i < components.size(); i++) {
        std::string u = components[i-1][0];
        std::string v = components[i][0];
        addEdge(u, v, defaultWeight); // 添加权重为1的边
        if(showInfo)std::cout << "添加边：" << u << " -- " << v <<":"<<defaultWeight<< std::endl;
    }
    if(showInfo)
    std::cout << "已添加 " << components.size()-1 << " 条边使图连通" << std::endl;
}
int graph::shortestRoad(std::string name1,std::string name2,bool is_show){
    if (!has_node(name1) ){
        std::cout << "节点 " << name1 << " 不存在！" << std::endl;
        return -1;
    }
    if (!has_node(name2)) {
        std::cout << "节点 " << name2 << " 不存在！" << std::endl;
        return -1;
    }
    
    // Dijkstra算法
    std::vector<std::pair<int, std::string>>vec;
    vec.reserve(2*node_num);
    std::priority_queue<std::pair<int, std::string>, std::vector<std::pair<int, std::string>>, std::greater<std::pair<int, std::string>>> pq(std::greater<std::pair<int, std::string>>(),move(vec));
    std::unordered_map<std::string, ll> dist;
    dist.reserve(2*node_num);
    std::unordered_map<std::string, std::string> prev;
    prev.reserve(2*node_num);
    std::unordered_set<std::string> visited;
    visited.reserve(2*node_num);
    
    for (auto& type_nodes : nodes) {
        for (auto& node : type_nodes) {
            dist[node.name] = INT_MAX;
        }
    }
    
    dist[name1] = 0;
    pq.push({0, name1});
    
    while (!pq.empty()) {
        auto [current_dist, current] = pq.top();
        pq.pop();
        
        if (visited.count(current)) continue;
        visited.insert(current);
        
        if (current == name2) break;
        
        auto idx = nodeName_to_idx[current];
        for (auto& edge : nodes[idx.first][idx.second].edges) {
            if (!is_exist_edge[edge.second]) continue;
            
            std::string neighbor = edge.first;
            int edge_idx = edge.second;
            ll new_dist = current_dist + edges[edge_idx].w;
            
            if (new_dist < dist[neighbor]) {
                dist[neighbor] = new_dist;
                prev[neighbor] = current;
                pq.push({new_dist, neighbor});
            }
        }
    }
    
    if (dist[name2] == INT_MAX) {
        std::cout << name1 << " 到 " << name2 << " 没有路径！" << std::endl;
        return -1;
    }

    // 打印路径
    if (is_show)
    {
        std::vector<std::string> path;
        for (std::string at = name2; at != ""; at = prev[at])
        {
            path.push_back(at);
        }

        std::cout << "最短路径：" << std::endl;
        for (int i = path.size() - 1; i >= 0; i--)
        {
            if (i != path.size() - 1)
                std::cout << " -> ";
            std::cout << path[i];
        }
        std::cout << std::endl
                  << "总长度：" << dist[name2] << std::endl;
    }
    return dist[name2];
}
std::vector<Edge> graph::MST(const std::vector<Edge> &edges, int num_vertices, bool is_show,int* total)
{
    if (!isConnected())
    {
        std::cout << "图不连通，无法生成最小生成树！" << std::endl;
        return{};
    }
    // Kruskal算法
    std::vector<Edge> mst_edges;
    mst_edges.reserve(num_vertices);
    std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> active_edges;

    for (int i = 0; i < edges.size(); i++)
    {
        if (is_exist_edge[i])
        {
            active_edges.push(edges[i]);
        }
    }
    std::unordered_map<std::string, std::string> parent;
    parent.reserve(3 * num_vertices);
    for (auto &type_nodes : nodes)
    {
        for (auto &node : type_nodes)
        {
            parent[node.name] = node.name;
        }
    }
    std::function<std::string(std::string)> find = [&](std::string u) { // ✅ 显式声明类型
        return parent[u] == u ? u : parent[u]=find(parent[u]);
    };

    auto unite = [&](std::string u, std::string v)
    {
        std::string root_u = find(u);
        std::string root_v = find(v);
        if (root_u != root_v)
        {
            parent[root_v] = root_u;
            return true;
        }
        return false;
    };

    int total_weight = 0;
    while(!active_edges.empty())
    {
        Edge edge = active_edges.top();
        active_edges.pop();
        if (unite(edge.u, edge.v))
        {
            mst_edges.push_back(edge);
            total_weight += edge.w;
        }
    }
    if (total != NULL)
        *total = total_weight;
        
    if(!is_show)return mst_edges;

    //std::cout << "最小生成树("<<mst_edges.size()<<"条边)：" << std::endl;

    // 输出对齐的边信息
    if(mst_edges.size()>1000){
        std::cout<<"道路过多，仅输出前1000条"<<std::endl;
    }
    std::cout << "总权重：" << total_weight << std::endl;
    int count=1;
    for (const auto &edge : mst_edges)
    {
        if (count<=1000){
             std::cout << edge.u<<"--"<<edge.v<<":"<<edge.w << std::endl;
        }
        count++;
    }

    return mst_edges;
}
void graph::showVisitPath(const std::string& start_node, int type) {
    if(!has_node(start_node)){
        throw  std::runtime_error("起点不存在！");
        return;
    }
    // 1. 确定要访问的节点集合
    std::vector<std::string> target_nodes;
    if (type == 0) {
        // 访问所有节点
        for (const auto& [name, idx] : nodeName_to_idx) {
            if(has_node(name))
            target_nodes.push_back(name);
        }
    } else if (type >= 1 && type <= 6) {
        // 访问特定类型的节点
        for (const Node& node : nodes[type]) {
            if(has_node(node.name))
            target_nodes.push_back(node.name);
        }
    } else {
        std::cerr << "Error: Invalid node type specified.\n";
        return;
    }

    if (std::find(target_nodes.begin(), target_nodes.end(), start_node) == target_nodes.end())
    {
        target_nodes.push_back(start_node);
    }

    // 2. 根据问题规模选择算法
    if (target_nodes.size() <=1000)
    {
        // 中小规模：Christofides算法
        christofidesTSP(start_node, target_nodes);
    }
    else
    {
        // 大规模：2-近似算法
        mstApproxTSP(start_node, target_nodes);
    }
}
//辅助函数类
std::vector<Edge> graph::greedyMatching(const std::vector<std::string>& odd_vertices,const std::vector<Edge>& edges) {
    std::vector<Edge> matching;
    std::unordered_set<std::string> matched;
    
    // 按权重排序所有可能的边
    std::vector<Edge> candidate_edges;
    for (size_t i = 0; i < odd_vertices.size(); ++i) {
        for (size_t j = i+1; j < odd_vertices.size(); ++j) {
            for (const auto& edge : edges) {
                if ((edge.u == odd_vertices[i] && edge.v == odd_vertices[j]) ||
                    (edge.u == odd_vertices[j] && edge.v == odd_vertices[i])) {
                    candidate_edges.push_back(edge);
                    break;
                }
            }
        }
    }
    std::sort(candidate_edges.begin(), candidate_edges.end());
    
    // 贪心选择
    for (const auto& edge : candidate_edges) {
        if (matched.count(edge.u) || matched.count(edge.v)) continue;
        
        matching.push_back(edge);
        matched.insert(edge.u);
        matched.insert(edge.v);
        
        if (matched.size() == odd_vertices.size()) break;
    }
    
    return matching;
}

std::vector<std::string> graph::generateEulerTour(const std::vector<Edge>& edges,const std::string& start) {
    // 构建邻接表
    std::unordered_map<std::string, std::vector<std::string>> adj;
    for (const auto& edge : edges) {
        adj[edge.u].push_back(edge.v);
        adj[edge.v].push_back(edge.u);
    }
    
    // Hierholzer算法
    std::stack<std::string> stack;
    std::vector<std::string> path;
    stack.push(start);
    
    while (!stack.empty()) {
        std::string current = stack.top();
        
        if (!adj[current].empty()) {
            std::string next = adj[current].back();
            adj[current].pop_back();
            // 删除反向边
            auto& neighbors = adj[next];
            neighbors.erase(std::find(neighbors.begin(), neighbors.end(), current));
            stack.push(next);
        } else {
            path.push_back(current);
            stack.pop();
        }
    }
    
    // 短路处理（去除重复访问的节点）
    std::vector<std::string> short_path;
    std::unordered_set<std::string> visited;
    
    for (const auto& node : path) {
        if (visited.count(node)) continue;
        short_path.push_back(node);
        visited.insert(node);
    }
    short_path.push_back(start); // 闭合路径
    
    return short_path;
}

std::vector<std::string> graph::preorderTraversal(const std::vector<Edge>& mst,const std::string& start) {
    // 构建邻接表
    std::unordered_map<std::string, std::vector<std::string>> adj;
    for (const auto& edge : mst) {
        adj[edge.u].push_back(edge.v);
        adj[edge.v].push_back(edge.u);
    }
    
    // DFS前序遍历
    std::vector<std::string> path;
    std::stack<std::string> stack;
    std::unordered_set<std::string> visited;
    
    stack.push(start);
    visited.insert(start);
    
    while (!stack.empty()) {
        std::string current = stack.top();
        stack.pop();
        path.push_back(current);
        
        for (const auto& neighbor : adj[current]) {
            if (!visited.count(neighbor)) {
                visited.insert(neighbor);
                stack.push(neighbor);
            }
        }
    }
    
    return path;
}
int graph::calculatePathLength(const std::vector<std::string>& path) {
    int total = 0;
    for (size_t i = 0; i < path.size()-1; ++i) {
        total += shortestRoad(path[i], path[i+1],false);
    }
    return total;
}

int graph::calculateViewTime(const std::vector<std::string>& path) {
    int total = 0;
    std::unordered_set<std::string> counted;
    
    for (const auto& node : path) {
        if (counted.count(node)) continue;
        total += queryNodeByName(node).viewTime;
        counted.insert(node);
    }
    
    return total;
}

void graph::printPath(const std::vector<std::string>& path) {
    if (path.empty()) return;
    
    std::cout << "Optimal Tour Path (" << path.size()-1 << " steps):\n";
    for (size_t i = 0; i < path.size(); ++i) {
        if (i != 0) std::cout << " -> ";
        std::cout << path[i];
        if (i == 0) std::cout << " (Start)";
        if (i == path.size()-1 && path.size() > 1) std::cout << " (End)";
    }
    
    int length = calculatePathLength(path);
    int view_time = calculateViewTime(path);
    
    std::cout<< "\n (路程: " << length << "m, 参观时长: " << view_time << ")\n";
}
// Christofides算法（中小规模）
void graph::christofidesTSP(const std::string& start_node, const std::vector<std::string>& targets) {
    if (targets.empty()) return;
    
    // 1. 构建完全子图（使用节点间最短路径作为边权）
    std::vector<Edge> subgraph_edges;
    for (size_t i = 0; i < targets.size(); ++i) {
        for (size_t j = i+1; j < targets.size(); ++j) {
            int weight = shortestRoad(targets[i], targets[j],false);
            subgraph_edges.emplace_back(targets[i], targets[j], weight);
        }
    }

    // 2. 构建最小生成树
    std::vector<Edge> mst = MST(subgraph_edges, targets.size(),false);
    if (mst.empty() && targets.size() > 1) {
        std::cerr << "Error: Failed to build MST\n";
        return;
    }

    // 3. 找到奇数度顶点
    std::unordered_map<std::string, int> degrees;
    for (const auto& edge : mst) {
        degrees[edge.u]++;
        degrees[edge.v]++;
    }

    std::vector<std::string> odd_vertices;
    for (const auto& [v, deg] : degrees) {
        if (deg % 2 == 1) odd_vertices.push_back(v);
    }

    // 4. 最小权完美匹配（简化实现）
    std::vector<Edge> matching = greedyMatching(odd_vertices, subgraph_edges);

    // 5. 合并MST和匹配
    std::vector<Edge> combined = mst;
    combined.insert(combined.end(), matching.begin(), matching.end());

    // 6. 生成欧拉回路并短路
    std::vector<std::string> path = generateEulerTour(combined, start_node);
    
    // 7. 输出结果
    printPath(path);
}
// MST 2-近似算法（大规模）
void graph::mstApproxTSP(const std::string& start_node, const std::vector<std::string>& targets) {
    if (targets.empty()) return;
    
    // 1. 构建完全子图
    std::vector<Edge> subgraph_edges;
    for (size_t i = 0; i < targets.size(); ++i) {
        for (size_t j = i+1; j < targets.size(); ++j) {
            int weight = shortestRoad(targets[i], targets[j],false);
            subgraph_edges.emplace_back(targets[i], targets[j], weight);
        }
    }

    // 2. 构建最小生成树
    std::vector<Edge> mst = MST(subgraph_edges, targets.size(),false);
    if (mst.empty() && targets.size() > 1) {
        std::cerr << "Error: Failed to build MST\n";
        return;
    }

    // 3. 前序遍历生成路径
    std::vector<std::string> path = preorderTraversal(mst, start_node);
    path.push_back(start_node); // 闭合路径

    // 4. 输出结果
    printPath(path);
}
