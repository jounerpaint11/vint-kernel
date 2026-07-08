extern "C" {
    void clear_screen();

    void run_matrix() {
        clear_screen();
        volatile unsigned int* vmem32 = (volatile unsigned int*)0xB8000;
        unsigned int seed = 12345;
        
        for (int r = 0; r < 100; r++) {
            for (int i = 0; i < (80 * 25) / 2; i++) {
                seed = seed * 1103515245 + 12345;
                if ((seed % 15) == 0) {
                    unsigned char char1 = (seed % 90) + 33;
                    unsigned char char2 = ((seed >> 2) % 90) + 33;
                    vmem32[i] = (char1 | (0x02 << 8)) | ((char2 | (0x02 << 8)) << 16);
                } else if ((seed % 7) == 0) {
                    vmem32[i] = (' ' | (0x02 << 8)) | ((' ' | (0x02 << 8)) << 16);
                }
            }
            for (volatile int d = 0; d < 5000000; d++);
        }
        clear_screen();
    }
}