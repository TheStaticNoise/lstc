#pragma once

#include <cstdint>
#include <vector>

namespace lstc::arch /* ? */ {
    int arch_ver();
    enum class variant {B8, B8T, B16, B32, B64};
    enum class x86_64 {
        RAX, RBX, RCX, RDX,
        RSI, RDI, RBP, RSP,
        R8, R9, R10, R11, R12, R13, R14, R15,
        RIP
    };
    enum class arm64 {
        USE_DIRECT_FUCKING_INTEGERS = 65535
    };
    enum class reg_type {VIRTUAL, REG, SYS, CALL, DIRECT};
    class arch_client {
        private:
            struct register_data {
                reg_type type;
                enum {MEM, REGREF} whence;
                uint64_t value;
                uint8_t variant;
            };
            struct _register {
                // base + (indx * scale) + disp
                register_data data;
                register_data indx; // needs restriction to not use r12/rsp
                uint64_t scale;
                uint32_t disp;
            };
            int arch = arch_ver();
            struct _operand {

            };
            struct {
                std::vector<_register> reg_vec;
            } unit_data;
        public:
            bool arch_expect_min(int arch_ver) {return arch >= arch_ver;}
            bool arch_expect(int arch_ver) {return arch == arch_ver;}
            uint8_t add_reg(reg_type type, uint64_t reg_val, uint8_t variant);

    };
};
