extern "C" {
    unsigned char inb(unsigned short port);
    void put_char(char c);
    void kprint(const char* str);
    void kprint_int(int num);

    int read_int_from_keyboard() {
        char buf[16];
        int idx = 0;
        const char kbd_nums[128] = { 0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };

        while (1) {
            if (inb(0x64) & 1) {
                unsigned char scancode = inb(0x60);
                if (!(scancode & 0x80)) {
                    if (scancode >= 0x02 && scancode <= 0x0B) {
                        if (idx < 15) {
                            char c = kbd_nums[scancode];
                            put_char(c);
                            buf[idx++] = c;
                        }
                    } else if (scancode == 0x0E) {
                        if (idx > 0) {
                            idx--;
                            buf[idx] = '\0';
                            put_char('\b');
                        }
                    } else if (scancode == 0x1C) {
                        put_char('\n');
                        buf[idx] = '\0';
                        break;
                    }
                }
            }
        }
        int res = 0;
        for (int i = 0; buf[i] != '\0'; i++) res = res * 10 + (buf[i] - '0');
        return res;
    }

    char read_op_from_keyboard() {
        while (1) {
            if (inb(0x64) & 1) {
                unsigned char scancode = inb(0x60);
                if (!(scancode & 0x80)) {
                    char c = 0;
                    if (scancode == 0x0C) c = '-';
                    else if (scancode == 0x35) c = '/';
                    else if (scancode == 0x0D) c = '+';
                    else if (scancode == 0x2D) c = '*';

                    if (c != 0) {
                        put_char(c);
                        put_char('\n');
                        return c;
                    }
                }
            }
        }
    }

    void run_calculator_app() {
        kprint("Enter first number: ");
        int a = read_int_from_keyboard();
        kprint("Enter operator (+, -, *, / [use X for *]): ");
        char op = read_op_from_keyboard();
        kprint("Enter second number: ");
        int b = read_int_from_keyboard();

        kprint("Result: ");
        if (op == '+') kprint_int(a + b);
        else if (op == '-') kprint_int(a - b);
        else if (op == '*') kprint_int(a * b);
        else if (op == '/') {
            if (b == 0) kprint("Error: Div by 0");
            else kprint_int(a / b);
        }
        kprint("\n");
    }
}