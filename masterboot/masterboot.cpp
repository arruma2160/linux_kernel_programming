#include <fstream>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <utility>

class mbr {
    public:
        mbr(const unsigned char * const addr)
            : boot_type_{addr+boot_type_offset}
            , system_code_{addr+system_code_offset}
            , num_sectors_{addr+num_sectors_offset} { }

        friend std::ostream& operator<<(std::ostream& os, const mbr& record) {
            const auto boot_type = *reinterpret_cast<const uint8_t*>(record.boot_type_);
            const auto system_code = *reinterpret_cast<const uint8_t*>(record.system_code_);
            const auto num_sectors = *reinterpret_cast<const uint32_t*>(record.num_sectors_);
            os << "{" << "\n";
            os << "\t - boot type: 0x" << std::hex << (boot_type  & 0x0ff) << ",\n";
            os << "\t - system code: 0x" << std::hex << (system_code & 0x0ff) << ",\n";
            os << "\t - num sectors: " << std::dec << num_sectors << "\n";
            os << "}";
            return os;
        }
    private:
        const unsigned char * const boot_type_;
        const unsigned char * const system_code_;
        const unsigned char * const num_sectors_;
        static const unsigned boot_type_offset = 0x00;
        static const unsigned system_code_offset = 0x04;
        static const unsigned num_sectors_offset = 0x0c;

};
static void terminate(std::string_view msg, int code=-1) {
    std::cerr << msg;
    exit(code);
}

int main(void) {
    std::string dev = "/dev/sda";
    std::ifstream ifs{dev, std::fstream::binary};
    if (!ifs) {
        terminate("Error opening device: " + dev + "\n");
    }

    const size_t size = 512;
    unsigned char buff[size]{};
    ifs.read((char*)buff, size);
    if (!ifs) {
        terminate("Error reading bytes from: " + dev + "\n");
    }

    const unsigned first = 0x1be;
    const unsigned second = 0x1ce;
    const unsigned third = 0x1de;
    const unsigned fourth = 0X1ee;

    mbr partition_one{std::as_const(buff)+first};
    mbr partition_two{std::as_const(buff)+second};
    mbr partition_three{std::as_const(buff)+third};
    mbr partition_four{std::as_const(buff)+fourth};
    std::cout << partition_one << ",\n";
    std::cout << partition_two << ",\n";
    std::cout << partition_three << ",\n";
    std::cout << partition_four << "\n";

    return 0;
}
