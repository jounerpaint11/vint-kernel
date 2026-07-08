extern "C" {
    void outb(unsigned short port, unsigned char data);

    void run_reboot() {
        unsigned char good = 0x02;
        while (good & 0x02) {
            __asm__ volatile("inb $0x64, %0" : "=a"(good) :: "memory");
        }
        outb(0x64, 0xFE);
    }
}