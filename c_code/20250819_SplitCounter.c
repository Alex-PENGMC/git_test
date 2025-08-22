#include <stdint.h>
#include <stdio.h>
#include <unistd.h>  // 用于usleep模拟时间流逝

// 扩展计数器结构体
typedef struct {
    uint32_t high;         // high32bit计数
    uint32_t low;          // low 32bit计数
    uint32_t last_hw_val;  // 上一次硬件PTS值
} ExtendedCounter;

// 硬件PTS模拟器：1MHz计数(每微秒+1)
static uint32_t simulated_hw_pts = 0;

// 模拟读取硬件PTS寄存器(1MHz递增)
static uint32_t HW_PTS_Read(void)
{
    return simulated_hw_pts;
}

// 模拟硬件PTS递增(每微秒+1，模拟1MHz频率)
static void Simulate_HW_PTS_Increment(void) 
{
    simulated_hw_pts++;  // 每微秒递增1，1MHz频率
}

// 初始化扩展计数器
void ExtendedCounter_Init(ExtendedCounter *counter) 
{
    if (counter == NULL) return;

    counter->last_hw_val = HW_PTS_Read();
    counter->low         = counter->last_hw_val;
    counter->high        = 0;
}

// 更新扩展计数器
void ExtendedCounter_Update(ExtendedCounter *counter) 
{
    if (counter == NULL) return;

    uint32_t current_hw_val = HW_PTS_Read();
    uint32_t diff;

    if (current_hw_val >= counter->last_hw_val)
        diff = current_hw_val - counter->last_hw_val;
    else
        diff = (0xFFFFFFFF - counter->last_hw_val) + current_hw_val + 1;

    // 更新low 32bit并处理溢出进bit
    counter->low += diff;
    if (counter->low < diff) 
        counter->high++;

    counter->last_hw_val = current_hw_val;
}

// 获取high 32bit计数
uint32_t ExtendedCounter_GetHigh(ExtendedCounter *counter)
{
    return (counter != NULL) ? counter->high : 0;
}

// 获取low 32bit计数
uint32_t ExtendedCounter_GetLow(ExtendedCounter *counter)
{
    return (counter != NULL) ? counter->low : 0;
}

// 5. 打印当前计数状态(直观展示硬件和扩展计数)
void Print_Counter_Status (ExtendedCounter *counter, const char *desc) 
{
    printf ("[% s]\n", desc);
    printf ("HW PTS(32 bit): 0x%08X\n", simulated_hw_pts);
    printf ("Extended to (64 bit): 0x%08X%08X (high 32 bit: 0x%08X, low 32 bit: 0x%08X)\n\n",
    counter->high, counter->low, counter->high, counter->low);
}

uint32_t print_interval = 10000000;
uint32_t print_counter  = 0;

// 主函数：演示计数器工作
int main(void) 
{
    ExtendedCounter counter;
    ExtendedCounter_Init(&counter);
    printf("Init finished - high 32 bit: 0x%08X, low 32bit: 0x%08X\n",
           counter.high, counter.low);

    while (1)
    {
        Simulate_HW_PTS_Increment();

        ExtendedCounter_Update(&counter);

        print_counter++;
        if (print_counter > print_interval)
        {
            printf("HW PTS: 0x%08X, extend data: 0x%08X%08X\n",
                simulated_hw_pts,
                ExtendedCounter_GetHigh(&counter),
                ExtendedCounter_GetLow(&counter));
            
            print_counter = 0;
        }

        // usleep(1);
    }

    // // 模拟硬件溢出场景(手动设置到接近溢出值)
    // simulated_hw_pts = 0xFFFFFFFE;
    // ExtendedCounter_Update(&counter);
    // printf("\n接近溢出 - 硬件PTS: 0x%08X, 扩展计数: 0x%08X%08X\n",
    //        simulated_hw_pts, counter.high, counter.low);

    // // 触发硬件溢出(0xFFFFFFFE -> 0xFFFFFFFF -> 0x00000000)
    // Simulate_HW_PTS_Increment();  // 0xFFFFFFFF
    // Simulate_HW_PTS_Increment();  // 0x00000000
    // ExtendedCounter_Update(&counter);
    // printf("溢出后   - 硬件PTS: 0x%08X, 扩展计数: 0x%08X%08X\n",
    //        simulated_hw_pts, counter.high, counter.low);

    return 0;
}
    
