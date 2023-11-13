#include <stdio.h>
#include <ctype.h> // 用于 isdigit 函数

/* 回调函数类型定义 */
typedef void (*Callback)(void);

/* 正确的处理函数 */
void HandleCorrect() {
    printf("1111\n");
}

/* 错误的处理函数 */
void HandleError() {
    printf("0000\n");
}

/* 读取输入并使用回调函数处理 */
void ProcessInput(Callback correct_cb, Callback error_cb) {
    char input;
    printf("Enter a character: ");
    
    // 读取一个字符
    input = getchar();

    // 检查是否为数字
    if (isdigit(input)) {
        correct_cb(); // 调用正确处理的回调函数
    } else {
        error_cb(); // 调用错误处理的回调函数
    }
}

int main() {
    while (1) {
        // 读入字符并根据字符是否为数字调用相应的回调函数
        ProcessInput(HandleCorrect, HandleError);
        
        // 清除输入缓冲区中的剩余字符，包括换行符
        while (getchar() != '\n');
    }
    return 0;
}
