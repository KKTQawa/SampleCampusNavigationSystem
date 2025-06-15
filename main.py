import tkinter as tk
from logging import exception
from tkinter import ttk
from PIL import Image, ImageTk  #  pip install Pillow
import logging
import os
import sys
import graph
from logger import logger
from tkinter import messagebox
def show_main_page():
    # 清除当前页面
    for widget in root.winfo_children():
        widget.destroy()

    # 创建主容器
    main_container = tk.Frame(root)
    main_container.pack(fill="both", expand=True)

    # 加载背景图片
    try:
        bg_image = Image.open("assets/mainView.png")
        bg_image = bg_image.resize((width//2, height), Image.LANCZOS)  # 固定尺寸
        bg_photo = ImageTk.PhotoImage(bg_image)

        bg_canvas = tk.Canvas(main_container, width=600, height=800)
        bg_canvas.pack(fill="both", expand=True)
        bg_canvas.create_image(0, 0, image=bg_photo, anchor="nw")
        bg_canvas.bg_image = bg_photo  # 保存引用
    except Exception as e:
        print(f"加载背景图片失败: {e}")
        bg_canvas = tk.Canvas(main_container, bg="white")
        bg_canvas.pack(fill="both", expand=True)

    def update_nodes(event):
        level1 = level1_combo.get()
        if level1=="All":
            level2_combo['values']=G["all_nodes"]
        else:
            level2_combo['values'] = G["nodes"].get(level1, [])
        level2_combo.set('')

    def select_node_name(event, combobox, values):
        typed = combobox.get()  # 获取用户输入并转换为小写
        filtered_values = [v for v in G["all_nodes"] if typed == v]  # 根据输入过滤
        if len(filtered_values)>0:
            combobox['values'] = filtered_values
            combobox.event_generate('<Down>')  # 打开下拉列表
        elif typed:
            node_name=level2_combo.get()
            type_id = node_types.index(level1_combo.get())
            new_vtime=G["tk_viewtime"].get()
            n_type = new_vtime
            if not isinstance(new_vtime,int):
                n_type=int(new_vtime)
            answer = messagebox.askyesno("提示", f"确认添加:{node_name},  类型:{level1_combo.get()},  参观时长:{type_id} ?")
            if answer:
                try:
                    g.addNode(node_name,type_id,n_type)
                    messagebox.showinfo("提示",f"成功添加节点{level1_combo.get()}" )
                    G_init()
                except Exception as e:
                    messagebox.showwarning("失败", f"添加节点失败:{str(e)}")

    def update_tree():
        # 清空现有数据
        for item in tree.get_children():
            tree.delete(item)
        # 插入新数据
        for i, item in enumerate(G["node_neighbors"], start=1):
            tree.insert("", tk.END, values=("-", i, item[0], item[1]))

    def update_nodes_info(event):
        level2 = level2_combo.get()
        if not level2:
            return
        t_node=g.queryNodeByName(level2)
        #print("t_node",t_node)
        if not t_node or t_node=="":
            return
        view_time=t_node.viewTime
        #print("参观点时长",view_time,"参观点类型:",node_types[t_node.Type])
        G["node_neighbors"]=[]
        for node in t_node.edges:
            #print("node",node)
            edge = graph.queryEdgeByIdx(g,node[1])
            if not edge.u and not edge.v:  # 更健壮的空值检查
                continue
            G["node_neighbors"].append((node[0], edge.w))  # 直接使用 append
        G["tk_viewtime"].delete(0, "end")
        G["tk_viewtime"].insert(0, f"{view_time}")
        update_tree()
        level1_combo.set(node_types[t_node.Type])
        #level2_combo.set('')

    def query_euler():
        is_connected=g.isConnected()
        G["is_connected"].config(text=f"是否连通: {is_connected}")
        a=""
        b=""
        c=False
        has_euler=g.hasEulerAccess(a,b,c)

        if not has_euler:
            G["euler_dis"].config(text=f"欧拉路径:  不存在！")
            G["euler_path"].config(state="normal")  # 先解除禁用状态（如果之前禁用了）
            G["euler_path"].delete(1.0, tk.END)  # 清空所有内容（从第1行第0字符到末尾）
            G["euler_path"].config(state="disabled")
        else:
            euler_paths=graph.euler_path(g)
            contents = "-→".join(euler_paths)
            G["euler_dis"].config(text=f"欧拉路径: 经过{len(euler_paths)}个参观点")
            G["euler_path"].config(state="normal")  # 先解除禁用状态（如果之前禁用了）
            G["euler_path"].delete(1.0, tk.END)  # 清空所有内容（从第1行第0字符到末尾）
            G["euler_path"].insert(tk.END, contents)  # 插入新内容
            G["euler_path"].config(state="disabled")

    def query_MST():
        paths=graph.MST(g)
        #print(paths)
        #print(paths[1])
        if not paths[0] or paths[1]==0:
            G["MST_dis"].config(text=f"没有最小生成树！")
            G["MST"].config(state="normal")  # 先解除禁用状态（如果之前禁用了）
            G["MST"].delete(1.0, tk.END)  # 清空所有内容（从第1行第0字符到末尾）
            G["MST"].config(state="disabled")
            return
        content=" ".join(f"{edge.u}--{edge.v}({edge.w}m)" for edge in paths[0])
        G["MST_dis"].config(text=f"最小生成树: {paths[1]} m")
        G["MST"].config(state="normal")  # 先解除禁用状态（如果之前禁用了）
        G["MST"].delete(1.0, tk.END)  # 清空所有内容（从第1行第0字符到末尾）
        G["MST"].insert(tk.END, content)  # 插入新内容
        G["MST"].config(state="disabled")

    def shortd(event,u,v):
        s=u.get()
        t=v.get()
        if not s or not t:
            G["shortest_dis"].config(text=f"的最短距离为:  m")
            G["shortest_path"].config(state="normal")
            G["shortest_path"].delete(1.0, tk.END)
            G["shortest_path"].config(state="disabled")
            return
        #print("s:",s,"t:",t)
        pathss=graph.shortestRoad(g,s,t)
        if not pathss[0] or pathss[1]==0:
            #print("pathss",pathss[0]," ",pathss[1])
            G["shortest_dis"].config(text=f"的最短距离为: -1 m")
            G["shortest_path"].config(state="normal")
            G["shortest_path"].delete(1.0, tk.END)
            G["shortest_path"].config(state="disabled")
            return
        contentss = "-→".join(pathss[0])
        G["shortest_dis"].config(text=f"的最短距离为: {pathss[1]} m")
        G["shortest_path"].config(state="normal")  # 先解除禁用状态（如果之前禁用了）
        G["shortest_path"].delete(1.0, tk.END)  # 清空所有内容（从第1行第0字符到末尾）
        G["shortest_path"].insert(tk.END, contentss)  # 插入新内容
        G["shortest_path"].config(state="disabled")

    def update_edge_w(event):
        region = tree.identify_region(event.x, event.y)

        if region != "cell":  # 只允许单元格编辑
            return
        column = tree.identify_column(event.x)
        row=tree.identify_row(event.y)
        if  column == "#4"  :
            row = tree.identify_row(event.y)
            x, y, width, height = tree.bbox(row, column)
            value = tree.set(row, column)

            # 创建编辑框
            entry = ttk.Entry(tree_frame)
            entry.place(x=x, y=y, width=width, height=height)
            entry.insert(0, value)
            entry.focus()

            def get_previous_column(column):
                if column.startswith("#"):
                    current_index = int(column[1:])  # 转换为整数
                    previous_index = current_index - 1
                    if previous_index > 0:  # 确保列索引有效
                        return f"#{previous_index}"
                return None  # 没有前一列
            def re_edge_w():
                #print(level2_combo.get(),"-",tree.set(row,column),":",entry.get())
                flag=False
                try:
                    flag=g.updateEdge_weight(level2_combo.get(),tree.set(row,get_previous_column(column)),int(entry.get()))
                except Exception as e:
                    messagebox.showwarning("失败", f"更新失败:{str(e)}")
                if flag:
                    tree.set(row, column, entry.get())
                    messagebox.showinfo("提示", "更新成功")
                else:
                    messagebox.showwarning("失败", "更新失败")
                entry.destroy()

            entry.bind("<Return>", lambda e: re_edge_w())
            entry.bind("<KP_Enter>", lambda e: re_edge_w())
            entry.bind("<FocusOut>", lambda e: entry.destroy())

    def updateViewTime(event,level2_combo,new_time):
        flag=False
        try:
            flag=g.updateNode_viewTime(level2_combo.get(),int(new_time))
        except Exception as e:
            messagebox.showwarning("失败", f"更新失败:{str(e)}")
        if not flag:
            messagebox.showwarning("失败", "更新失败")
        else:
            messagebox.showinfo("提示", "更新成功")
            G["tk_viewtime"].delete(0, "end")
            G["tk_viewtime"].insert(0, f"{new_time}")

    def deletnode():
        node_name=level2_combo.get()
        if not node_name:
            return
        answer = messagebox.askyesno("提示", f"确认删除:{node_name} ?")
        if answer:
            try:
                g.deleteNode(node_name, False)
                messagebox.showinfo("提示", f"删除节点成功")
                G_init()
                level1_combo.set('')
                level2_combo.set('')
                G["tk_viewtime"].delete(0, "end")
                G["tk_viewtime"].insert(0, f" 0 ")
                for item in tree.get_children():
                    tree.delete(item)
            except Exception as e:
                messagebox.showwarning("失败", f"删除节点失败:{str(e)}")

    def edge_action(event):
        # 获取点击位置的列和行
        column = tree.identify_column(event.x)  # 格式 "#1", "#2" 等
        row_id = tree.identify_row(event.y)  # 返回行 ID，如 "I001"
        #print("row_id:",row_id)
        if not row_id:  # 如果点击的是 heading（无行）
            if column == "#1":  # 判断是否是第 1 列
                edge_add()
            return

        if column == "#1":  # 判断是否是第 1 列
            edge_delete(row_id)

    def edge_add():
        # 插入新行
        entry_row = tree.insert("", "end", values=("", " ", " ", " "), tags=("new",))
        tree.set(entry_row,column="#2",value=entry_row[3:])

        def cancel_entry(entry1,entry2, row_id):
            if entry1:
                entry1.destroy()
            if entry2:
                entry2.destroy()
            if cancel_btn:
                cancel_btn.destroy()
            tree.delete(row_id)

        def save_entry(entry1,entry2, row_id, button):
            new_value1 = entry1.get()
            new_value2 = entry2.get()
            if not isinstance(new_value2,int):
                new_value2=int(new_value2)
            try:
                g.addEdge(level2_combo.get(), new_value1,new_value2)
                tree.item(row_id, values=("-", entry_row[3:], new_value1, new_value2))  # 更新输入值到表格
                messagebox.showinfo("提示", "添加成功！")
            except Exception as e:
                messagebox.showwarning("失败", f"添加失败:{str(e)}")
                tree.delete(row_id)
            if entry1:
                entry1.destroy()
            if entry2:
                entry2.destroy()
            button.destroy()

        tree.update_idletasks()#刷新界面获取准确值
        bbox0=tree.bbox(entry_row, column="#1")
        bbox1 = tree.bbox(entry_row, column="#3")
        bbox2 = tree.bbox(entry_row, column="#4")

        if bbox0:
            x, y, width, height =bbox0
            cancel_btn = tk.Button(tree_frame, text="取消",  bg="red",command=lambda: cancel_entry(entry1,entry2, entry_row))
            cancel_btn.place(x=x+5, y=y, height=height)

        if bbox1:
            x, y, width, height = bbox1
            entry1 = tk.Entry(tree_frame, bg="lightyellow")
            entry1.place(x=x+5, y=y, width=width-10, height=height)
            entry1.bind("<Return>", lambda e: save_entry(entry1,entry2, entry_row, cancel_btn))
            entry1.bind("<KP_Enter>", lambda e: save_entry(entry1, entry2, entry_row, cancel_btn))
            entry1.focus()

        if bbox2:
            x, y, width, height = bbox2
            entry2 = tk.Entry(tree_frame, bg="lightyellow")
            entry2.place(x=x+5, y=y, width=width-10, height=height)
            entry2.bind("<Return>", lambda e: save_entry(entry1,entry2, entry_row, cancel_btn))
            entry2.bind("<KP_Enter>", lambda e: save_entry(entry1, entry2, entry_row, cancel_btn))

    def edge_delete(row_id):
        edge_u=level2_combo.get()
        edge_v=tree.set(row_id,"#3")
        edge_w= tree.set(row_id, "#4")
        answer = messagebox.askyesno("提示", f"确认删除:{edge_u}-{edge_v}:{edge_w} ?")
        if answer:
            try:
                ee=g.deleteEdge(edge_u,edge_v)
                if (not ee.u and not ee.v) or ee.w==0:
                    messagebox.showwarning("失败", f"删除道路失败")
                    return
                messagebox.showinfo("提示", f"删除道路成功")
                tree.delete(row_id)
            except Exception as e:
                messagebox.showwarning("失败", f"删除道路失败:{str(e)}")

    def renew():
        answer = messagebox.askyesno("提示", f"确认刷新磁盘数据吗？?")
        if not answer:
            return
        try:
            g.init()
            g.read_nodes("nodes.csv")
            g.read_edges("edges.csv")
            G["all_nodes"]=[]
            G["node_neighbors"]=[]
            G["nodes"]=graph.show_all_nodes(g)
            all_values = G["nodes"].values()
            for node_list in all_values:
                G["all_nodes"] += node_list
            level1_combo.set('')
            level2_combo.set('')
            G["tk_viewtime"].delete(0, "end")
            G["tk_viewtime"].insert(0, f" 0 ")
            for item in tree.get_children():
                tree.delete(item)
            G["euler_dis"].config(text="欧拉路径:")
            G["euler_path"].config(state="normal")
            G["euler_path"].delete("1.0", tk.END)
            G["euler_path"].config(state="disabled")
            G["is_connected"].config(text=f"是否连通:   ")
            G["MST_dis"].config(text="最小生成树: 0 m")
            G["MST"].config(state="normal")
            G["MST"].delete("1.0", tk.END)
            G["MST"].config(state="disabled")
            G["s1"].delete(0, "end")
            G["s1"].insert(0, "")
            G["s2"].delete(0, "end")
            G["s2"].insert(0, "")
            G["shortest_dis"].config(text="的最短距离为: 0 m")
            G["shortest_path"].config(state="normal")
            G["shortest_path"].delete("1.0", tk.END)
            G["shortest_path"].config(state="disabled")
            messagebox.showinfo("提示", "已重载数据！")
        except Exception as e:
            messagebox.showwarning("失败", f"刷新失败:{str(e)}")

    #节点选择区
    node_frame = tk.Frame(bg_canvas, bg="lightblue")
    bg_canvas.create_window(750, 0, window=node_frame, anchor="n",height=270,width=500)

    tk.Label(node_frame, text="参观点类型:").grid(row=0, column=0, padx=5, pady=5, sticky="w")
    level1_combo = ttk.Combobox(node_frame, values=node_types, state="readonly")
    level1_combo.grid(row=1, column=0, padx=5, pady=5, sticky="ew")
    level1_combo.bind("<<ComboboxSelected>>", update_nodes)

    tk.Label(node_frame, text="参观点名称:").grid(row=0, column=1, padx=5, pady=5, sticky="w")
    level2_combo = ttk.Combobox(node_frame, values=[])
    level2_combo.grid(row=1, column=1,columnspan=2, padx=5, pady=5, sticky="ew")
    level2_combo.bind("<<ComboboxSelected>>", update_nodes_info)
    level2_combo.bind("<KP_Enter>", lambda e: select_node_name(e, level2_combo, level2_combo['values']))
    level2_combo.bind("<Return>", lambda e: select_node_name(e, level2_combo, level2_combo['values']))
    #level2_combo.bind("<FocusOut>", lambda e: select_node_name(e, level2_combo, level2_combo['values']))
    del_node_button = tk.Button(node_frame, text="删除", command=deletnode,
                            width=12, height=1, bg="lightgray", font=("Times New Roman", 10))
    del_node_button.grid(row=0, column=2, padx=5, pady=5, sticky="w")
    #参观时间
    tkt= tk.Label(node_frame, text=f"参观时长:")
    tkt.grid(row=0, column=3, padx=5, pady=5, sticky="w")
    tktt = tk.Entry(node_frame)
    tktt.insert(0, "0")  # 设置默认文本
    tktt.grid(row=1, column=3, padx=5, pady=5, sticky="w")
    tktt.bind("<KP_Enter>", lambda e: updateViewTime(e, level2_combo, tktt.get()))
    tktt.bind("<Return>", lambda e: updateViewTime(e, level2_combo, tktt.get()))
    G["tk_viewtime"] = tktt
    #相邻节点
    tree_frame = tk.Frame(node_frame)
    tree_frame.grid(row=2, column=0, columnspan=4,sticky="nsew", padx=50, pady=10)
    tree = ttk.Treeview(tree_frame, columns=("action","id","name", "distance"), show="headings", height=5)
    tree.heading("action", text="+", anchor="center")  # 添加按钮列
    tree.heading("id", text="编号")
    tree.heading("name", text="相邻参观点")
    tree.heading("distance", text="距离")
    tree["selectmode"] = "none"  # 完全禁用选
    tree.bind('<B1-Motion>',  'break')  # 禁止列宽调整
    # 绑定双击编辑事件
    tree.bind("<Double-1>", update_edge_w)
    tree.bind("<Button-1>", edge_action)

    scrollbar3 = tk.Scrollbar(tree_frame, orient="vertical", command=tree.yview)
    tree.configure(yscrollcommand=scrollbar3.set)
    tree.pack(side="left", fill="both", expand=True)
    scrollbar3.pack(side="right", fill="y")
    # 添加数据
    for i, item in enumerate(G["node_neighbors"], start=1):
        tree.insert("", tk.END, values=("-", i, item[0], item[1]))
    # 设置列的对齐方式
    tree.column("action", anchor="center", width=100)
    tree.column("distance", anchor="center")
    tree.column("name", anchor="center")
    tree.column("id", anchor="center",width=100)
    G["tree"]=tree

    node_frame.grid_columnconfigure(0, weight=1)
    node_frame.grid_columnconfigure(1, weight=1)
    node_frame.grid_columnconfigure(2, weight=1)
    node_frame.grid_columnconfigure(3, weight=1)
    node_frame.grid_rowconfigure(2, weight=1)  # 允许 tree 所在行扩展
    #欧拉路径
    euler_frame = tk.Frame(bg_canvas, bg="lightgreen")
    bg_canvas.create_window(625, 270, window=euler_frame, anchor="n",height=150,width=250)

    G["euler_dis"] = tk.Label(euler_frame, text="欧拉路径:",wraplength=55)
    G["euler_dis"].grid(row=0, column=0, padx=5, pady=5, sticky="w")
    G["is_connected"]= tk.Label(euler_frame, text=f"是否连通:   ",wraplength=55)
    G["is_connected"].grid(row=0, column=1, padx=5, pady=5, sticky="w")
    euler_button = tk.Button(euler_frame, text="查询欧拉路径", command=query_euler,
                            width=10, height=1, bg="lightgray", font=("Times New Roman", 10))
    euler_button.grid(row=0, column=2, padx=5, pady=5, sticky="w")

    text_frame = tk.Frame(euler_frame)
    text_frame.grid(row=1, column=0, columnspan=3, sticky="nsew", padx=5, pady=5)
    G["euler_path"] = tk.Text(text_frame, wrap=tk.WORD, height=5)
    G["euler_path"].config(state="disabled")
    scrollbar1 = tk.Scrollbar(text_frame, orient="vertical", command=G["euler_path"].yview)
    G["euler_path"].configure(yscrollcommand=scrollbar1.set)
    G["euler_path"].pack(side="left", fill="both", expand=True)
    scrollbar1.pack(side="right", fill="y")

    euler_frame.grid_rowconfigure(1, weight=1)  # 让文本区可扩展
    euler_frame.grid_columnconfigure(0, weight=1)
    euler_frame.grid_columnconfigure(1, weight=1)
    euler_frame.grid_columnconfigure(2, weight=1)
    #MST
    MST_frame=tk.Frame(bg_canvas, bg="orange")
    bg_canvas.create_window(875, 270, window=MST_frame, anchor="n", height=150, width=250)
    G["MST_dis"] = tk.Label(MST_frame, text="最小生成树: 0 m")
    G["MST_dis"].grid(row=0, column=0, padx=5, pady=5, sticky="w")
    MST_button = tk.Button(MST_frame, text="查询最小生成树", command=query_MST,
                            width=12, height=1, bg="lightgray", font=("Times New Roman", 10))
    MST_button.grid(row=0, column=1, padx=5, pady=5, sticky="w")
    Mtext_frame = tk.Frame(MST_frame)
    Mtext_frame.grid(row=1, column=0, columnspan=2, sticky="nsew", padx=5, pady=5)
    G["MST"] = tk.Text(Mtext_frame, wrap=tk.WORD, height=5, width=40)
    G["MST"].config(state="disabled")
    scrollbar2 = tk.Scrollbar(Mtext_frame, orient="vertical", command=G["MST"].yview)
    G["MST"].configure(yscrollcommand=scrollbar2.set)
    G["MST"].pack(side="left", fill="both", expand=True)
    scrollbar2.pack(side="right", fill="y")

    MST_frame.grid_rowconfigure(1, weight=1)
    MST_frame.grid_columnconfigure(0, weight=1)
    MST_frame.grid_columnconfigure(1, weight=1)
    #最短路径
    shortest_frame=tk.Frame(bg_canvas, bg="lightblue")
    bg_canvas.create_window(750, 420, window=shortest_frame, anchor="n", height=130, width=500)

    G["s1"]= tk.Entry(shortest_frame, font=("Times New Roman", 10), width=15)
    G["s1"].bind("<KP_Enter>", lambda e: shortd(e,G["s1"],G["s2"]))
    G["s1"].bind("<Return>", lambda e: shortd(e,G["s1"],G["s2"]))
    G["s1"].grid(row=0, column=0, padx=5, pady=5)
    tk.Label(shortest_frame, text="到").grid(row=0, column=1, padx=5, pady=5)
    G["s2"]=tk.Entry(shortest_frame, font=("Times New Roman", 10), width=15)
    G["s2"].bind("<KP_Enter>", lambda e: shortd(e,G["s1"],G["s2"]))
    G["s2"].bind("<Return>", lambda e: shortd(e,G["s1"],G["s2"]))
    G["s2"].grid(row=0, column=2, padx=5, pady=5)
    G["shortest_dis"]= tk.Label(shortest_frame, text="的最短距离为: 0 m")
    G["shortest_dis"].grid(row=0, column=3, padx=5, pady=5, sticky="w")

    texts_frame = tk.Frame(shortest_frame)
    texts_frame.grid(row=1, column=0, columnspan=4, sticky="nsew", padx=5, pady=5)
    G["shortest_path"] = tk.Text(texts_frame, wrap=tk.WORD, height=5, width=40)
    G["shortest_path"].config(state="disabled")
    scrollbar = tk.Scrollbar(texts_frame, orient="vertical", command=G["shortest_path"].yview)
    G["shortest_path"].configure(yscrollcommand=scrollbar.set)
    G["shortest_path"].pack(side="left", fill="both", expand=True)
    scrollbar.pack(side="right", fill="y")

    shortest_frame.grid_rowconfigure(1, weight=3)  # 让文本区可扩展
    shortest_frame.grid_columnconfigure(0, weight=3)
    shortest_frame.grid_columnconfigure(1, weight=1)
    shortest_frame.grid_columnconfigure(2, weight=3)
    shortest_frame.grid_columnconfigure(3, weight=3)
    # 返回按钮
    exit_button = tk.Button(bg_canvas, text="返回首页", command=show_start_page,
                            width=10, height=2, bg="red", font=("微软雅黑", 12))
    bg_canvas.create_window(950, 580, window=exit_button)
    #刷新数据
    renew_button = tk.Button(bg_canvas, text="刷新磁盘数据", command=renew,
                            width=10, height=2, bg="#FFD700", font=("微软雅黑", 12))
    bg_canvas.create_window(555, 580, window=renew_button)
def show_start_page():
    # 清除当前页面
    for widget in root.winfo_children():
        widget.destroy()

    # 加载背景图片
    try:
        bg_image = Image.open("assets/startView.png")
        bg_image = bg_image.resize((width,height), Image.LANCZOS)
        bg_photo = ImageTk.PhotoImage(bg_image)

        bg_canvas = tk.Canvas(root, width=1000, height=600)
        bg_canvas.pack(fill="both", expand=True)
        bg_canvas.create_image(0, 0, image=bg_photo, anchor="nw")
        bg_canvas.bg_image = bg_photo
    except Exception as e:
        print(f"加载背景图片失败: {e}")
        bg_canvas = tk.Canvas(root, bg="white")
        bg_canvas.pack(fill="both", expand=True)

    #标题和按钮
    title_label = tk.Label(bg_canvas, text="华东师范大学校园导航系统",
                           font=("微软雅黑", 24, "bold"), bg="white", fg="navy")
    bg_canvas.create_window(480, 150, window=title_label)

    experience_button = tk.Button(bg_canvas, text="开始游览", command=show_main_page,
                                  width=15, height=2, bg="lightblue",
                                  font=("微软雅黑", 14), fg="white")
    bg_canvas.create_window(480, 300, window=experience_button)

    exit_button = tk.Button(bg_canvas, text="退出系统", command=root.quit,
                            width=15, height=2, bg="lightcoral",
                            font=("微软雅黑", 14), fg="white")
    bg_canvas.create_window(480, 380, window=exit_button)

def set_window_icon(root, icon_path):
    try:
        # 对于PNG/JPG等图片格式
        if os.path.exists(icon_path):
            img = Image.open(icon_path)
            temp_ico = "temp_icon.ico"
            img.save(temp_ico, format='ICO', sizes=[(32,32), (16,16), (48,48)])
            root.iconbitmap(temp_ico)
            os.remove(temp_ico)  # 删除临时文件
            return True
    except Exception as e:
        logger.warning(f"设置窗口图标失败: {e}")
    return False

def G_init():
    G["nodes"]=graph.show_all_nodes(g)
    G["all_nodes"]=[]
    all_values = G["nodes"].values()
    for node_list in all_values:
        G["all_nodes"] += node_list

width, height = 1000, 600  # 设定基准尺寸

# 创建图实例
g = graph.graph()
g.init()
# 调用方法
g.read_nodes("nodes.csv")
g.read_edges("edges.csv")

G={
    "nodes":graph.show_all_nodes(g),
    "node_neighbors":[],
    "all_nodes":[],
    "tk_viewtime":None,
    "tree":None,
    "euler_path":None,
    "euler_dis":None,
    "is_connected":None,
    "MST":None,
    "MST_dis":None,
    "shortest_dis":None,
    "shortest_path":None,
    "s1":None,
    "s2":None,
    "visitpath":None
}
node_types=graph.show_types(g)
#print("node_types",node_types)

all_values=G["nodes"].values()
for node_list in all_values:
    G["all_nodes"]+=node_list

# 创建主窗口
root = tk.Tk()
root.title("华东师范大学校园导航系统")
root.geometry("1000x600")
root.resizable(False, False)

# 设置窗口图标（支持PNG/JPG/ICO）
icon_path = "assets/flag.ico"  # 你的PNG图标路径
if not set_window_icon(root, icon_path):
    logger.warning("将使用默认图标")

# 初始显示开始页面
show_start_page()

root.mainloop()