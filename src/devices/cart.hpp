#pragma once

#include <unordered_map>
#include <cstdlib>
#include <fstream>
#include <array>

#include "../aliases.hpp"
#include "../log.hpp"

#include "mapper.hpp"

#define HDR_CART_TYPE 0x47
#define RVA_BEGIN 0x0
#define RVA_END 0xff
#define HDR_BEGIN 0x100
#define HDR_END 0x14f

namespace gameboy {
    namespace cart {
        typedef std::array<u8, 0x50> header_t;
        typedef std::array<u8, 0x100> rva_t;

        u8 dummy;

        rva_t rva;
        header_t header;

        std::unordered_map <u8, std::string> types {
            { 0x00, "ROM Only" },
            { 0x01, "ROM+MBC1" },
            { 0x02, "ROM+MBC1+RAM" },
            { 0x03, "ROM+MBC1+RAM+BATT" }
        };

        mapper* cartridge = nullptr;

        void insert_cartridge(std::string rom) {
            std::ifstream f(rom, std::ios::binary);

            if (f.is_open()) {
                f.read((char*)rva.data(), rva.size());
                f.read((char*)header.data(), header.size());
            }

            switch (header[HDR_CART_TYPE]) {
                case 0x00: { cartridge = new rom_only(); } break;
                case 0x01: { cartridge = new mbc1(); } break;

                default: {
                    _log(error, "Unimplemented cartridge type 0x%02x", header[HDR_CART_TYPE]);
                    std::exit(1);
                }
            }

            cartridge->init(&f);
        }

        u32 read(u16 addr, size_t size) {
            if (addr <= RVA_END) { return utility::default_mb_read(rva.data(), addr, size, RVA_BEGIN); }
            if (addr >= HDR_BEGIN && addr <= HDR_END) { return utility::default_mb_read(header.data(), addr, size, HDR_BEGIN); }
            if (addr >= CART_ROM_BEGIN && addr <= CART_ROM_END) { return cartridge->read(addr, size); }
            if (addr >= CART_RAM_BEGIN && addr <= CART_RAM_END) { return cartridge->read(addr, size); }
            return 0;
        }

        void write(u16 addr, u16 value, size_t size) {
            if (addr <= HDR_END                               ) { return; }
            if (addr >= CART_ROM_BEGIN && addr <= CART_ROM_END) { cartridge->write(addr, value, size); }
            if (addr >= CART_RAM_BEGIN && addr <= CART_RAM_END) { cartridge->write(addr, value, size); }
        }

        u8& ref(u16 addr) {
            if (addr >= CART_RAM_BEGIN && addr <= CART_RAM_END) { cartridge->ref(addr); }
            return dummy;
        }
    }
}