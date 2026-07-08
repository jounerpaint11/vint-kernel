extern "C" {
    void kprint(const char* str);
    void get_cpu_vendor(char* buffer);

    void run_sysinfo() {
        kprint("OS Name: Vint System\nKernel: vintkrnl.bin\nCPU Vendor: ");
        char vendor[13];
        vendor[12] = '\0';
        get_cpu_vendor(vendor);
        kprint(vendor);
        kprint("\n");
    }
}