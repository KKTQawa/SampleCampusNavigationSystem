#ifndef GRAPH_H
#define GRAPH_H
#include<vector>
#include<string>
#include<unordered_map>

typedef long long int ll;
struct Node
{
    int Type;
    std::string name;
    int viewTime;
    std::vector<std::pair<std::string, int>> edges;

    Node(int Type, std::string name, int viewTime) : Type(Type), name(name), viewTime(viewTime) {};
};
struct Edge
{
    std::string u, v;
    int w;
    Edge(std::string u, std::string v, int w) : u(u), v(v), w(w) {};
    bool operator<(const Edge &e) const
    {
        return w < e.w;
    }
    bool operator>(const Edge &e) const
    {
        return w > e.w;
    }
};

struct graph
{
    std::vector<std::vector<Node>> nodes;//二维数组，第一维是节点类型，第二维是节点下标
    std::vector<Edge> edges;

    std::vector<std::string> node_type = {"All", "Dining_room","Medical",  "Sports_and_Entertainment", "Teaching_Research_and_Administration", "dormitory", "other"};

    std::unordered_map<std::string, std::pair<int, int>> nodeName_to_idx; // 节点名称映射索引
    std::vector<int> is_exist_edge;//直接使用1和0判断边是否存在。  

    long long node_num=0;
    long long edge_num=0;
    void init()
    {
        nodes.assign(7, std::vector<Node>());
        for (int i = 1; i <= 6; i++)
            nodes[i].reserve(1000000);
        edges.clear();
        nodeName_to_idx.clear();
        nodeName_to_idx.reserve(10000000);
        is_exist_edge.assign(10000000, 0);
        edges.reserve(10000000);
        node_num = 0, edge_num = 0;
    }
    graph(){
        init();
    }
    //基础功能
    void readNodes(std::string FileName) ;
    void readEdges(std::string FileName) ;
    void addNode(std::string name, int type, int viewTime) ;
    void addEdge(std::string name1, std::string name2, int weight) ;
    bool updateNode_viewTime(std::string name, int viewTime) ;
    bool updateEdge_weight(std::string name1, std::string name2, int weight) ;
    void deleteNode(std::string name,bool is_show=true) ;
    Edge deleteEdge(std::string name1, std::string name2) ;
    Node& queryNodeByName(std::string name) ;
    std::vector<Node> queryNodeByType(int type) ;
    std::vector<std::vector<std::string>> showAllNodes();
    Edge queryEdgeByName(std::string name1, std::string name2) ;

    bool has_edge(std::string name1,std::string name2);
    bool has_node(std::string name);
    //指定功能
    bool isConnected();//判断图是否连通
    bool hasEulerAccess(std::string* start=NULL,std::string*end=NULL,bool is_res=false);//判断是否存在欧拉通路
    void showAllEdges(bool isSortedByWeight=false);//打印所有存在的边
    void autoAddEdgesToConnected(bool showInfo=false,int defaultWeight=1);//自动添加边使得图连通
    int shortestRoad(std::string name1,std::string name2,bool is_show=true);//求最短路径，打印路径和路径长度
    std::vector<Edge> MST(const std::vector<Edge> &edges, int num_vertices, bool is_show = true,int* total=NULL);//输出最小生成树
    std::vector<std::string> showEulerPath(const std::string& start_node,bool is_show=true) ;//输出欧拉路径
    //扩展功能
    void showVisitPath(const std::string& start_node,int type=0);//给出游览一圈的最优规划路径
    void christofidesTSP(const std::string& start_node, const std::vector<std::string>& targets) ;//中小规模
    void mstApproxTSP(const std::string& start_node, const std::vector<std::string>& targets);//大规模
    //辅助函数
    std::vector<Edge> greedyMatching(const std::vector<std::string>& odd_vertices,  const std::vector<Edge>& edges);// 贪心最小权匹配
    std::vector<std::string> generateEulerTour(const std::vector<Edge>& edges, const std::string& start);// 生成欧拉回路
    std::vector<std::string> preorderTraversal(const std::vector<Edge>& mst,const std::string& start);// 前序遍历MST
    int calculatePathLength(const std::vector<std::string>& path);// 计算路径长度
    int calculateViewTime(const std::vector<std::string>& path);// 计算总参观时
    void printPath(const std::vector<std::string>& path);
};

#endif 

