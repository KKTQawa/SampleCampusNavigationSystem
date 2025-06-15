
#include "graph.h"
#include "menu.h"
#include <iostream>
void node_operation(graph &g){
    std::vector<std::string> options = {
        "返回上一级菜单",
        "添加参观点",
        "更新参观点时间",
        "删除参观点",
        "查询参观点信息",
        "按类别查找参观点",
        "显示参观点数量",
        "显示所有参观点",
        };
    while (true)
    {
        system("cls"); // 清屏
        SetColor(BrightYellow);
        std::cout << ">>>>>>>>>>>>>>>>>>>校园建筑<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
        ResetColor();
        std::cout << std::endl;
        int op = showMenu(options);
        std::string name;
        int type;
        int viewTime = 0;
        switch (op)
        {
        case 0:
            return;
        case 1:
        {
            SetColor(BrightYellow);
            std::cout << "请输入节点名称：";
            std::cin >> name;
            for (int j = 1; j <= 6; j++)
            {
                std::cout << "类型" << j << ":" << g.node_type[j] << std::endl;
            }
            std::cout << "请输入节点类型编号：";

            try
            {
                type=getValidInt();
                if(type<1||type>6)throw std::runtime_error("输入类型必须为1-6！");
            }
            catch (std::runtime_error &e)
            {
                SetColor(BrightRed);
                std::cout << e.what() << std::endl;
                ResetColor();
                iscom();
                break;
            }
            std::cout << "请输入节点访问时间：";
            try
            {
                viewTime = getValidInt();
            }
            catch (std::invalid_argument &e)
            {
                SetColor(BrightRed);
                std::cout << e.what() << std::endl;
                ResetColor();
                iscom();
                break;
            }
            try
            {
                g.addNode(name, type, viewTime);
                SetColor(BrightGreen);
                std::cout << "添加节点成功！" << std::endl;
                ResetColor();
            }
            catch (std::exception &e)
            {
                SetColor(BrightRed);
                std::cout << e.what() << std::endl;
                std::cout << "添加节点失败！" << std::endl;
                ResetColor();
            }
            iscom();
            break;
        }
        case 2:
        {
            SetColor(BrightYellow);
            std::cout << "请输入节点名称：";
            std::cin >> name;
            std::cout << "请输入节点访问时间：";
            try
            {
                viewTime = getValidInt();
            }
            catch (std::invalid_argument &e)
            {
                SetColor(BrightRed);
                std::cout << e.what() << std::endl;
                ResetColor();
                iscom();
                break;
            }
            try
            {
                g.updateNode_viewTime(name, viewTime);
                SetColor(BrightGreen);
                std::cout << "更新节点成功！" << std::endl;
                ResetColor();
            }
            catch (...)
            {
                SetColor(BrightRed);
                std::cout << "更新节点失败！" << std::endl;
                ResetColor();
            }
            iscom();
            break;
        }
        case 3:
        {
            SetColor(BrightYellow);
            std::cout << "请输入节点名称：";
            std::cin >> name;
            try
            {
                g.deleteNode(name);
                SetColor(BrightGreen);
                std::cout << "删除节点成功！" << std::endl;
                ResetColor();
            }
            catch (...)
            {
                SetColor(BrightRed);
                std::cout << "删除节点失败！" << std::endl;
                ResetColor();
            }
            iscom();
            break;
        }
        case 4:
        {
            SetColor(BrightYellow);
            std::cout << "请输入节点名称：";
            std::cin >> name;
            try
            {
                Node node = g.queryNodeByName(name);
                SetColor(BrightGreen);
                std::cout << node.name << "的信息如下：" << std::endl;
                std::cout << "类型：" << g.node_type[node.Type] << std::endl;
                std::cout << "建议参观时长：" << node.viewTime << std::endl;
                std::cout << "相邻建筑：" << std::endl;
                int count = 0;
                for (auto &it : node.edges)
                {

                    if (!g.is_exist_edge[it.second])
                        continue;
                    if (count == 1001)
                        std::cout << "仅展示1000条数据..."<<std::endl;
                    if(count<=1000)
                    std::cout << it.first << "\t距离" << g.edges[it.second].w << "米" << std::endl;
                    count++;
                }
                std::cout << "共" << count << "个相邻建筑" << std::endl;
                ResetColor();
            }
            catch (...)
            {
                SetColor(BrightRed);
                std::cout << "查询节点失败！请检查输入节点名称是否正确！" << std::endl;
                ResetColor();
            }
            iscom();
            break;
        }
        case 5:
        {
            SetColor(BrightYellow);
            for (int j = 1; j <= 6; j++)
            {
                std::cout << "类型" << j << ":" << g.node_type[j] << std::endl;
            }
            std::cout << "请输入查找的类型编号：";
            try
            {
                type=getValidInt();
                if(type<1||type>6)throw std::runtime_error("输入类型必须为1-6！");
            }
            catch (std::runtime_error &e)
            {
                SetColor(BrightRed);
                std::cout << e.what() << std::endl;
                ResetColor();
                iscom();
                break;
            }
            try{
                std::vector<Node> nodes = g.queryNodeByType(type);
                SetColor(BrightGreen);
                std::cout << "查询成功！共找到" << nodes.size() << "个节点" << std::endl;
                int count = 1;
                for (auto &it : nodes)
                {
                    if (count > 1000)
                    {
                        std::cout << "仅展示1000条数据..." << std::endl;
                        break;
                    }
                    std::cout << it.name << " " << g.node_type[it.Type] << " 建议参观时长：" << it.viewTime << std::endl;
                    count++;
                }
            }
            catch (std::runtime_error &e)
            {
                SetColor(BrightRed);
                std::cout << e.what() << std::endl;
                ResetColor();
            }
            iscom();
            break;
        }
        case 6:
        {
            SetColor(BrightGreen);
            for (int i = 1; i <= 6; i++)
            {
                int count = 0;
                for (auto &it : g.nodes[i])
                {
                    if (!g.has_node(it.name))
                        continue;
                    count++;
                }
                std::cout << g.node_type[i] << "有" << count << "个参观点" << std::endl;
            }
            SetColor(BrightYellow);
            std::cout << "共有" << g.node_num << "个节点" << std::endl;
            ResetColor();
            iscom();
            break;
        }
        case 7:
        {
            SetColor(BrightGreen);
            std::cout << "共有" << g.node_num << "个节点" << std::endl;
            int count = 0;
            int total_viewTime = 0;
            for (int i = 1; i <= 6; i++)
            {
                if (count < 1001)
                    std::cout << "类型" << i << "：" << g.node_type[i] << std::endl;
                for (auto &it : g.nodes[i])
                {
                    if (!g.has_node(it.name))
                        continue;
                    if (count == 1001)
                        std::cout << "仅展示1000条数据..." << std::endl;
                    if (count <= 1000)
                        std::cout << it.name << " " << g.node_type[it.Type] << "建议参观时长:" << it.viewTime << std::endl;
                    count++;
                    total_viewTime += it.viewTime;
                };
            }
            std::cout << "总参观时长：" << total_viewTime << std::endl;
            ResetColor();
            iscom(1);
            break;
        }
        case -1:
            return;
        default:
            break;
        }
    }
}

void edge_operation(graph &g){
    std::vector<std::string> options = {
        "返回上一级菜单",
        "添加道路",
        "删除道路",
        "修改道路长度",
        "查询道路信息",
        "显示道路数量",
        "显示所有道路"
        };
    while (true)
    {
        system("cls"); // 清屏
        SetColor(BrightYellow);
        std::cout << ">>>>>>>>>>>>>>>>>>校园道路<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
        ResetColor();
        int op = showMenu(options);
        std::string name1, name2;
        int weight = 0;
        switch (op)
        {
        case 0:
            return;
        case 1:
        {
            SetColor(Cyan);
            std::cout << "请输入道路起点和终点：";
            std::cin >> name1>>name2;
            std::cout << "请输入距离：";
            try
            {
                weight = getValidInt();
            }
            catch (std::invalid_argument &e)
            {
                SetColor(BrightRed);
                std::cout << e.what() << std::endl;
                ResetColor();
                iscom();
                break;
            }
            try
            {
                if(!g.has_node(name1)||!g.has_node(name2)){
                    throw std::invalid_argument("节点不存在");
                }
                g.addEdge(name1, name2, weight);
                SetColor(BrightGreen);
                std::cout << "添加道路成功！" << std::endl;
                ResetColor();
            }
            catch (std::exception &e)
            {
                SetColor(BrightRed);
                std::cout << "添加道路失败！" << std::endl;
                std::cout<<e.what()<<std::endl;
                ResetColor();
            }
            iscom();
            break;
        }
        case 2:
        {
            SetColor(Cyan);
            std::cout << "请输入道路起点和终点：";
            std::cin >> name1>>name2;
            try{
                if(!g.has_node(name1)||!g.has_node(name2)){
                    throw std::invalid_argument("节点不存在"); 
                }
               Edge edge= g.deleteEdge(name1, name2);
               if(edge.u==""&&edge.v==""){
                    SetColor(BrightRed);
                    std::cout << "没有这条路！" << std::endl;
                    ResetColor();
                    iscom();
                    break;
               }
               SetColor(BrightGreen);
               std::cout << "已删除道路:" << std::endl;
               std::cout << edge.u << "--" << edge.v << ",长度:" << edge.w << std::endl;
               ResetColor();
            }catch(std::runtime_error &e){
                SetColor(BrightRed);
                std::cout << e.what() << std::endl;
                ResetColor();
            }catch(...){
                SetColor(BrightRed);
                std::cout << "删除道路失败！" << std::endl;
                ResetColor(); 
            }
            iscom();
            break;
        }
        case 3:
        {
            SetColor(Cyan);
            std::cout << "请输入道路起点和终点：";
            std::cin >> name1>>name2;
            std::cout << "请输入距离：";
            int weight = 0;
            try
            {
                weight = getValidInt();
            }
            catch (std::invalid_argument &e)
            {
                SetColor(BrightRed);
                std::cout << e.what() << std::endl;
                ResetColor();
                iscom();
                break;
            }
            try{
                if(!g.has_node(name1)||!g.has_node(name2)){
                    throw std::runtime_error("节点不存在");
                }
                g.updateEdge_weight(name1, name2, weight);
                SetColor(BrightGreen);
                std::cout << "更新长度成功！" << std::endl;
                ResetColor();
            }catch(std::runtime_error &e){
                SetColor(BrightRed);
                std::cout << e.what() << std::endl;
                ResetColor(); 
            }catch(...){
                SetColor(BrightRed);
                std::cout << "更新长度失败！" << std::endl;
                ResetColor(); 
            }
            iscom();
            break;
        }
        case 4:{
            SetColor(Cyan);
            std::cout << "请输入道路起点和终点：";
            std::cin >> name1>>name2;
            try
            {
                if(!g.has_node(name1)||!g.has_node(name2)){
                    throw std::runtime_error("节点不存在"); 
                }
                Edge edge = g.queryEdgeByName(name1, name2);
                if(edge.u==""&&edge.v==""){
                     SetColor(BrightRed);
                    std::cout << "没有这条路！" << std::endl;
                    ResetColor();
                    iscom();
                    break;
                }
                SetColor(BrightGreen);
                std::cout << "道路信息如下：" << std::endl;
                std::cout << edge.u << "--" << edge.v << ",长度:" << edge.w << std::endl;
                ResetColor();
            }
            catch (std::runtime_error &e)
            {
                SetColor(BrightRed);
                std::cout << e.what() << std::endl;
                ResetColor();
            }
            catch (...)
            {
                SetColor(BrightRed);
                std::cout << "查询道路失败！" << std::endl;
                ResetColor();
            }
            iscom();
            break;
        }
        case 5:{
            SetColor(Cyan);
            std::cout << "共有" << g.edge_num << "条道路" << std::endl;
            ResetColor();
            iscom();
            break;
        }
        case 6:{
            SetColor(Cyan);
            int co=0;
            std::cout<<"是否按长度排序？(1/0)"<<std::endl;
            int op=getValidInt();
            std::cout << "共有" << g.edge_num << "条道路" << std::endl;
            if(op!=0&&op!=1){
                SetColor(BrightRed);
                std::cout<<"输入错误！"<<std::endl;
                ResetColor();
                iscom();
                break;
            }
            if(op==1){
                SetColor(BrightYellow);
                g.showAllEdges(true);
                ResetColor();
            }else{
                SetColor(BrightYellow);
                g.showAllEdges(false);
                ResetColor();
            }
            iscom(1);
            break;
        }
        case -1:
            return;
        default:
            break;
        }
    }
}

void begin_visit(graph &g)
{
    std::vector<std::string> options = {
        "返回上一级菜单",
        "查找最短路径",
        "当前图是否连通",
        "添加道路补充图连通",
        "显示最小生成树",
        "显示欧拉路径",
        "游览所有建筑"
        };
    while (true)
    {
        system("cls"); // 清屏
        SetColor(BrightYellow);
        std::cout << ">>>>>>>>>>>>>>>>>>校园游览<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
        ResetColor();
        int op = showMenu(options);
        std::string name1, name2;
        int weight = 0;
        switch (op)
        {
        case 0:
            return;
        case 1:
        {
            SetColor(BrightMagenta);
            std::cout << "请输入道路起点和终点：";
            std::cin >> name1>>name2;
            int co;
            std::cout << "是否显示路径？(1/0)" << std::endl;
            co = getValidInt();
            if (co != 0 && co != 1)
            {
                SetColor(BrightRed);
                std::cout << "输入错误！" << std::endl;
                ResetColor();
                iscom();
                break;
            }
            try
            {
                if(!g.has_node(name1) || !g.has_node(name2)){
                    throw std::runtime_error("节点不存在！");
                }
                SetColor(BrightGreen);
                g.shortestRoad(name1, name2, co);
                ResetColor();
            }
            catch (std::runtime_error &e)
            {
                SetColor(BrightRed);
                std::cout << e.what() << std::endl;
                ResetColor();
            }
            catch (...)
            {
                SetColor(BrightRed);
                std::cout << "查找最短路径失败！" << std::endl;
                ResetColor();
            }
            iscom();
            break;
        }
        case 2:
        {
            try
            {
                if (g.isConnected())
                {
                    SetColor(BrightGreen);
                    std::cout << "当前图连通！" << std::endl;
                    ResetColor();
                }
                else
                {
                    SetColor(BrightRed);
                    std::cout << "当前图不连通！" << std::endl;
                    ResetColor();
                }
            }
            catch (...)
            {
                SetColor(BrightRed);
                std::cout << "错误！" << std::endl;
                ResetColor();
            }
            iscom();
            break;
        }
        case 3:
        {
            int co, length;
            try
            {
                SetColor(BrightGreen);
                std::cout << "是否显示添加过程？(1/0)" << std::endl;
                co = getValidInt();
                if (co!= 0 && co!= 1){
                    throw std::invalid_argument("输入错误");
                }
                std::cout << "请输入添加道路的长度(默认为1)：" << std::endl;
                length = getValidInt();
                ResetColor();
            }
            catch (std::invalid_argument &e)
            {
                SetColor(BrightRed);
                std::cout << e.what() << std::endl;
                ResetColor();
                iscom();
                break;
            }
            try{
                SetColor(BrightGreen);
                if(g.isConnected()){
                    std::cout<<"当前图已连通!"<<std::endl;
                    ResetColor();
                    iscom();
                    break;
                }
                if(co)
                g.autoAddEdgesToConnected(true, length);
                else
                g.autoAddEdgesToConnected(false, length);
                std::cout << "添加完毕..." << std::endl;
                ResetColor();
            }catch(std::runtime_error &e){
                SetColor(BrightRed);
                std::cout << e.what() << std::endl;
                ResetColor();
            }catch(...){
                SetColor(BrightRed);
                std::cout << "出现错误！" << std::endl;
                ResetColor();
            }
            iscom();    
            break;
        }
        case 4:{
            try{
                SetColor(BrightGreen);
                g.MST(g.edges, g.node_num);
                ResetColor();
            }catch(std::runtime_error &e){
                SetColor(BrightRed);
                std::cout << e.what() << std::endl;
                ResetColor(); 
            }catch(...){
                SetColor(BrightRed);
                std::cout << "出现错误！" << std::endl;
                ResetColor(); 
            }
            iscom();
            break;
        }
        case 5:{
            try
            {
                std::string start_node,end_node;
                SetColor(BrightGreen);
                bool flag=g.hasEulerAccess(&start_node,&end_node,true);
                if(!flag){
                    SetColor(BrightRed);
                    std::cout << "不存在欧拉路径!" << std::endl;
                    ResetColor();
                    iscom();
                    break;
                }
                g.showEulerPath(start_node);
                ResetColor();
            }
            catch (std::runtime_error &e)
            {
                SetColor(BrightRed);
                std::cout << e.what() << std::endl;
                ResetColor();
            }
            catch (std::exception &e)
            {
                SetColor(BrightRed);
                std::cout << "出现错误！" <<e.what()<< std::endl;
                ResetColor();
            }
            iscom();
            break;
        }
        case 6:{
            std::string start_node;
            int type=0;
            SetColor(BrightMagenta);
            std::cout << "请输入您当前所在地点：" << std::endl;
            std::cin >> start_node;
            if(!g.has_node(start_node)){
                SetColor(BrightRed);
                std::cout << "您输入的起点不存在！" << std::endl;
                ResetColor();
                iscom();
                break;
            }
            std::cout << "请输入您的游览类型(0-6)：" << std::endl;
            SetColor(BrightGreen);
            std::cout << "0:所有参观点" << std::endl;
            for (int j = 1; j <= 6; j++)
            {
                std::cout << "类型" << j << ":" << g.node_type[j] << std::endl;
            }
            SetColor(BrightMagenta);
            try{
                type=getValidInt();
            }catch(std::invalid_argument &e){
                SetColor(BrightRed);
                std::cout << e.what() << std::endl;
                ResetColor(); 
                iscom();
                break;
            }
            if(type>6){
                SetColor(BrightRed);
                std::cout << "输入错误！" << std::endl;
                ResetColor();
                iscom();
                break;
            }
            try{
                if(!g.has_node(start_node)){
                    throw std::runtime_error("您输入的起点不存在！");
                }
                SetColor(BrightGreen);
                g.showVisitPath(start_node,type);
                ResetColor();
            }catch(std::runtime_error &e){
                SetColor(BrightRed);
                std::cout << e.what() << std::endl;
                ResetColor(); 
            }catch(...){
                SetColor(BrightRed);
                std::cout << "出现错误！" << std::endl;
                ResetColor(); 
            }
            iscom(1);
            break;
        }
        case -1:
            return;
        default:
            break;
        }
    }
}
int main()
{
    // 设置控制台标题
    SetConsoleTitle(TEXT("ECNU_Navigation - 华东师范大学校园导航系统"));
    graph g;
    g.init();
    bool isinit_node = false, isinit_edge = false;
    std::string csvFilePath;

    while (true)
    {
        system("cls"); // 清屏
        SetColor(Cyan);
        std::cout << ">>>>>>>>>>>>>>>>>>>欢迎使用华东师范大学校园导航系统！<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
        ResetColor();
        std::cout << std::endl;

        if (!isinit_node || !isinit_edge)
        {
            SetColor(BrightRed);
            std::string text;
            if (!isinit_node)
                text = "节点";
            else if (!isinit_edge)
                text = "道路";

            std::cout << "未录入";
            SetColor(BrightGreen);
            std::cout << text;
            SetColor(BrightRed);
            std::cout << "信息！" << std::endl;

            std::cout << "请选择目录文件录入：" << std::endl;
            ResetColor(); 
            std::vector<std::string> csvFiles = getCSVFiles(".");
            if (csvFiles.empty())
            {
                SetColor(BrightRed); // 设置为亮红色（错误提示）
                std::cout << "当前目录下未找到任何.csv文件！";
                std::cout << "请将文件放入程序所在目录。" << std::endl;
                ResetColor(); // 恢复默认颜色

                iscom();
                continue;
            }

            // 添加退出选项
            csvFiles.push_back("退出程序");

            // 显示选择菜单
            int choice = showMenu(csvFiles);
            int tmp = csvFiles.size() - 1;
            if (choice == -1 || choice == tmp)
            {
                return 0; // 退出程序
            }

            csvFilePath = csvFiles[choice];
            SetColor(BrightGreen);
            std::cout << "\n已选择文件: " << csvFilePath << std::endl;
            ResetColor();
            // 模拟读取文件
            try
            {
                std::string ttt="";
                int count=0;
                if (!isinit_node)
                    g.readNodes(csvFilePath), isinit_node = true,ttt+="个节点",count=g.node_num;
                else if (!isinit_edge)
                    g.readEdges(csvFilePath), isinit_edge = true,ttt+="条道路",count=g.edge_num;
                SetColor(BrightGreen);
                std::cout << "成功加载" <<count<<ttt<< std::endl;
                ResetColor();
            }catch(std::logic_error& e){
                SetColor(BrightRed);
                std::cout <<"加载失败："<< e.what() << std::endl;
                ResetColor();
                g.init();
            }
            catch (std::runtime_error& e)
            {
                SetColor(BrightRed);
                std::cout <<e.what()<< std::endl;
                ResetColor();
            }catch(...){
                SetColor(BrightRed);
                std::cout <<  "数据加载失败,请重试！"<< std::endl;
                ResetColor();
            }
            iscom();
            continue;
        }
        // 主菜单
        std::vector<std::string> mainMenu = {
            "节点信息",
            "道路信息",
            "游览",
            "重新加载数据",
            "退出系统"};
        int choice = showMenu(mainMenu);

        switch (choice)
        {
        case 0:
            node_operation(g);
            break;
        case 1:
            edge_operation(g);
            break;
        case 2:
            begin_visit(g);
            break;
        case 3:
            g.init();
            isinit_edge = false, isinit_node = false;
            SetColor(BrightRed);
            std::cout << "数据已重置！" << std::endl;
            ResetColor();
            break;
        case 4:
            SetColor(BrightGreen);
            std::cout << "欢迎下次光临！" << std::endl;
            ResetColor();
            iscom();
            return 0;
        case -1:
            return 0;
        default:
            break;
        }
        iscom();
    }
    return 0;
}


