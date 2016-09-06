# ShareModule
共享模块，方便模块的复用和模块的替换而不改变其它模块的逻辑
已有模块
模块编号          模块名称                                         模块介绍
   1.     NetService  网络服务模块    从NoahGameFrame剥离出来的，支持创建客户端和服务器的网络模块
   2.     DBService 数据模块          用于加载csv表格的模块。配合DatasetTool生成csv和DBDefine表格数据表(无需重新编译模块)
   3.     SQLService 数据库访问模块   有三个模块任选一个 sqlite mysql odbc
   
0-128的编号是用于编写通用模块
129-255的编号用于编写特定逻辑的模块

Kernel是模块管理器 加载Module.xml来初始化所有模块。此配置作为exe的第一个参数，传入

SDK 是编译好的通用模块
SDK/Example 范例目录
目前有 推箱子范例 PushBox
