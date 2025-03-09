#include "Delay.h"

void Delay_us(uint32_t us) {
    uint32_t count = us * 168 / 5; // 估算循环次数
    while (count--) {
        __NOP();  // 空操作，防止优化
    }
}

void Delay_ms(uint32_t ms) {
    while (ms--) {
        Delay_us(1000);  // 调用微秒延时
    }
}
