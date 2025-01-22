代码规范

1. 命名规范
    1.1 变量命名
        小写字母+下划线：
            sample：
                int buffer_size;
                char input_char;
        局部变量应简短清晰，除i；j；k外避免使用单字母变量名；
        全局变量需加上模块或功能前缀，避免命名冲突；
            sample：
                int network_buffer_size;
    1.2 函数命名
        小写+下划线：
            sample：
                void calcute_sum();
                int get_user_input();
        如果是库函数或模块内函数，加上模块前缀：
            sample：
                void string_copy();
                int network_initialize();
    1.3 常量命名
        使用全大写+下划线，表示不可修改的值：
        sample：
            #define MAX_BUFFER_SIZE 1024
            const int DEFAULT_TIMEOUT = 30;
    1.4 枚举命名
        枚举名和枚举值采用全大写，加上前缀：
        sample：
            enum ERROR_CODE 
            {
                ERR_SUCCESS,
                ERR_INVALID_INPUT,
                ERR_TIMEOUT
            };

2. 代码结构规范
    2.1 缩进与空格
        缩进通常为4个空格或1个Tab；
        操作符、关键字和括号之间留空格：
            sample：
                if (a == b) 
                {
                    total += a + b;
                }
    2.2 函数长度
        单个函数尽量控制在50行以内，超过100行需要拆分或详细注释
    2.3 头文件
        每个模块须有对应的.h文件，提供接口声明，避免直接应用.c文件；
        头文件需加宏保护，避免重复包含：
            sample：
                #ifndef MY_HEADER_H
                #define MY_HEADER_H

                // Declarations

                #endif
 
3. 注释规范
    3.1 文件注释
        在每个源文件或头文件加上文件级注释，说明文件作用和作者信息：
            sample：
              /**
                * File: network.c
                * Description: Implements network communication logic.
                * Author: John Doe
                * Date: 2025-01-22
                */
    3.2 函数注释
        函数前加注释，说明函数用途、参数、返回值：
        sample：
            /**
            * @brief Adds two numbers.
            * @param a First integer.
            * @param b Second integer.
            * @return Sum of a and b.
            */
            int add(int a, int b);
    3.3 代码注释
        使用单行注释说明逻辑，注释应简洁明了：
            sample：
                // Initialize buffer to zero
                memset(buffer, 0, sizeof(buffer));

4. 错误处理
    检查所有函数返回值，避免忽略错误：
        sample：
            if (fopen("data.txt", "r") == NULL) 
            {
                perror("Failed to open file");
                return -1;
            }
    定义统一的错误码：
        sample：
            #define ERR_SUCCESS 0
            #define ERR_FILE_NOT_FOUND -1

5. 可读性与模块化
    5.1 单一职责
        每个函数只做一件事，模块内职责清晰：
            sample：
                void read_file();
                void process_data();
                void write_output();
    5.2 避免魔法数字：
        使用宏定义或常量代替：
            sample：
                #define MAX_USERS 100
    5.3 拆分文件
        大型项目按功能模块拆分.c和.h文件：
            main.c：程序入口
            network.c，network.h：网络模块
            utils.c，util.h：工具模块

6. 内存管理
    所有动态分配的内存必须配对释放：
        sample：
            char *buffer = malloc(1024);
            if (buffer == NULL) 
            {
                return -1;
            }
            free(buffer);

7. 编译与警告处理
    启用所有警告（如 -Wall -Wextra），并修正代码中的警告。
    避免使用未定义行为或不安全函数（如 gets）。

8. 日志与调试
    使用同一的日志模块记录关键信息：
        sample：
            #define LOG_INFO(msg) printf("[INFO]: %s\n", msg)
            #define LOG_ERROR(msg) printf("[ERROR]: %s\n", msg)
   






