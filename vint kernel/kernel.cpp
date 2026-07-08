extern "C" {
    unsigned char inb(unsigned short port);
    void outb(unsigned short port, unsigned char data);
    void run_calculator_app();
    void run_sysinfo();
    void run_matrix();
    void run_reboot();

    const int VGA_WIDTH = 80;
    const int VGA_HEIGHT = 25;
    volatile char* video_memory = (volatile char*)0xB8000;
    
    int cursor_x = 0;
    int cursor_y = 0;
    const char DEFAULT_COLOR = 0x0F;

    char input_buffer[256];
    int input_index = 0;

    const char kbd_us[128] = {
        0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
        '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
        0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
        '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
    };

    inline void update_cursor(int x, int y) {
        unsigned short position = y * VGA_WIDTH + x;
        outb(0x3D4, 0x0F);
        outb(0x3D5, (unsigned char)(position & 0xFF));
        outb(0x3D4, 0x0E);
        outb(0x3D5, (unsigned char)((position >> 8) & 0xFF));
    }

    void clear_screen() {
        volatile unsigned int* vmem32 = (volatile unsigned int*)video_memory;
        unsigned int blank = (' ' | (DEFAULT_COLOR << 8)) | ((' ' | (DEFAULT_COLOR << 8)) << 16);
        for (int i = 0; i < (VGA_WIDTH * VGA_HEIGHT) / 2; i++) {
            vmem32[i] = blank;
        }
        cursor_x = 0; cursor_y = 0;
        update_cursor(cursor_x, cursor_y);
    }

    void put_char(char c) {
        if (c == '\n') {
            cursor_x = 0;
            cursor_y++;
        } else if (c == '\b') {
            if (cursor_x > 2) {
                cursor_x--;
                int index = (cursor_y * VGA_WIDTH + cursor_x) * 2;
                video_memory[index] = ' ';
            }
        } else {
            int index = (cursor_y * VGA_WIDTH + cursor_x) * 2;
            video_memory[index] = c;
            video_memory[index + 1] = DEFAULT_COLOR;
            cursor_x++;
        }

        if (cursor_x >= VGA_WIDTH) {
            cursor_x = 0;
            cursor_y++;
        }
        update_cursor(cursor_x, cursor_y);
    }

    void kprint(const char* str) {
        for (int i = 0; str[i] != '\0'; i++) put_char(str[i]);
    }

    void kprint_int(int num) {
        if (num == 0) { put_char('0'); return; }
        if (num < 0) { put_char('-'); num = -num; }
        char str[12]; int i = 0;
        while (num > 0) { str[i++] = (num % 10) + '0'; num /= 10; }
        for (int j = i - 1; j >= 0; j--) put_char(str[j]);
    }

    inline bool str_compare(const char* str1, const char* str2) {
        int i = 0;
        while (str1[i] != '\0' && str2[i] != '\0') {
            if (str1[i] != str2[i]) return false;
            i++;
        }
        return str1[i] == str2[i];
    }

    void execute_command() {
        put_char('\n');
        if (input_index == 0) { kprint("> "); return; }

        if (str_compare(input_buffer, "calculator.cpp")) run_calculator_app();
        else if (str_compare(input_buffer, "sysinfo.cpp")) run_sysinfo();
        else if (str_compare(input_buffer, "matrix.cpp")) run_matrix();
        else if (str_compare(input_buffer, "reboot.cpp")) run_reboot();
        else if (str_compare(input_buffer, "clear")) clear_screen();
        else if (str_compare(input_buffer, "help")) {
            kprint("Commands: help, clear, calculator.cpp, sysinfo.cpp, matrix.cpp, reboot.cpp\n");
        } else {
            kprint("Unknown command. Type 'help'\n");
        }

        input_index = 0;
        input_buffer[0] = '\0';
        kprint("> ");
    }

    void check_keyboard() {
        if (inb(0x64) & 1) {
            unsigned char scancode = inb(0x60);
            if (!(scancode & 0x80)) {
                char c = kbd_us[scancode];
                if (scancode == 0x1C) execute_command();
                else if (scancode == 0x0E) {
                    if (input_index > 0) {
                        input_index--;
                        input_buffer[input_index] = '\0';
                        put_char('\b');
                    }
                } else if (c > 0 && input_index < 255) {
                    put_char(c);
                    input_buffer[input_index++] = c;
                    input_buffer[input_index] = '\0';
                }
            }
        }
    }

    void kernel_main() {
        clear_screen();
        kprint("Welcome to Vint System!\nType 'help' for command list.\n\n> ");

        while (1) {
            check_keyboard();
        }
    }
}