#include "Delay.h"

void Delay_us(uint32_t us) {
    uint32_t count = us * 168 / 5; // ����ѭ������
    while (count--) {
        __NOP();  // �ղ�������ֹ�Ż�
    }
}

void Delay_ms(uint32_t ms) {
    while (ms--) {
        Delay_us(1000);  // ����΢����ʱ
    }
}
