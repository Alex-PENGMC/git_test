#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main() {
    // 获取系统的时钟频率
    const long interval = sysconf(_SC_CLK_TCK);
    const long half_interval = interval / 2;

    // 无限循环
    while(1) {
        // 记录循环开始的时间
        clock_t start = clock();

        // 忙等待直到半个时间片
        while (clock() < start + half_interval);

        // 休眠半个时间片
        usleep(half_interval * 1000000 / interval);
    }

    return 0;
}
