/******************************************************************************\
**
**  This file is part of the Hades GBA Emulator, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2020 - The Hades Authors
**
\******************************************************************************/

#include <endian.h>
#include <string.h>
#include "memory.h"
#include "hades.h"
#include "core.h"

void
mem_init(
    struct memory *memory
) {
    memset(memory, 0, sizeof(*memory));
}

/*
** Read the byte at the given address.
*/
uint8_t
mem_read8(
    struct memory const *memory,
    uint32_t addr
) {
    switch (addr >> 24) {
        case BIOS_REGION:
            return (memory->bios[addr & BIOS_MASK]);
        case EWRAM_REGION:
            return (memory->ewram[addr & EWRAM_MASK]);
        case IWRAM_REGION:
            return (memory->iwram[addr & IWRAM_MASK]);
        case IO_REGION:
            return (memory->io[addr & IO_MASK]);
        case PALRAM_REGION:
            return (memory->palram[addr & PALRAM_MASK]);
        case VRAM_REGION:
            return (memory->vram[addr & VRAM_MASK]);
        case OAM_REGION:
            return (memory->oam[addr & OAM_MASK]);
        case CART_0_REGION_1:
        case CART_0_REGION_2:
        case CART_1_REGION_1:
        case CART_1_REGION_2:
        case CART_2_REGION_1:
        case CART_2_REGION_2:
            return (memory->rom[addr & CART_MASK]);
        case CART_SRAM_REGION:
            return (memory->sram[addr & CART_SRAM_MASK]);
        default:
            panic(HS_CORE, "Invalid read at address %#08x", addr);
    }
}

/*
** Read the word at the given address, hiding all endianness conversions.
**
** This function returns an `uint32_t` instead of an `uint16_t` to account for
** some of the shenanigans the ARM7TDMI does when supplied an unligned address.
*/
uint32_t
mem_read16(
    struct memory const *memory,
    uint32_t addr
) {
    uint32_t rotate;
    uint32_t value;

    rotate = (addr % 2) << 3;
    addr &= 0xFFFFFFFE;

    value =
        (mem_read8(memory, addr + 0) << 0) |
        (mem_read8(memory, addr + 1) << 8)
    ;

    /* Unaligned 16-bits loads are supposed to be unpredictable, but in practise the GBA rotates them */
    return ((value >> rotate) | (value << (32 - rotate)));
}

/*
** Read the double-word at the given address, hiding all endianness conversions.
*/
uint32_t
mem_read32(
    struct memory const *memory,
    uint32_t addr
) {
    uint32_t rotate;
    uint32_t value;

    rotate = (addr % 4) << 3;
    addr &= 0xFFFFFFFE;

    value =
        (mem_read8(memory, addr + 0) << 0) |
        (mem_read8(memory, addr + 1) << 8) |
        (mem_read8(memory, addr + 2) << 16) |
        (mem_read8(memory, addr + 3) << 24)
    ;

    /* Unaligned 32-bits loads are rotated */
    return ((value >> rotate) | (value << (32 - rotate)));
}

/*
** Read the byte at the given address.
*/
void
mem_write8(
    struct memory *memory,
    uint32_t addr,
    uint8_t val
) {
    switch (addr >> 24) {
        case BIOS_REGION:
            memory->bios[addr & BIOS_MASK] = val;
            break;
        case EWRAM_REGION:
            memory->ewram[addr & EWRAM_MASK] = val;
            break;
        case IWRAM_REGION:
            memory->iwram[addr & IWRAM_MASK] = val;
            break;
        case IO_REGION:
            memory->io[addr & IO_MASK] = val;
            if (addr % 4 == 3) {
                mem_io_write(memory, addr - 3);
            }
            break;
        case PALRAM_REGION:
            memory->palram[addr & PALRAM_MASK] = val;
            break;
        case VRAM_REGION:
            memory->vram[addr & VRAM_MASK] = val;
            break;
        case OAM_REGION:
            memory->oam[addr & OAM_MASK] = val;
            break;
        case CART_0_REGION_1:
        case CART_0_REGION_2:
        case CART_1_REGION_1:
        case CART_1_REGION_2:
        case CART_2_REGION_1:
        case CART_2_REGION_2:
            memory->rom[addr & CART_MASK] = val;
            break;
        case CART_SRAM_REGION:
            memory->sram[addr & CART_SRAM_MASK] = val;
            break;
        default:
            panic(HS_CORE, "Invalid write at address %#08x", addr);
    }
}

/*
** Read the word at the given address, hiding all endianness conversions.
*/
void
mem_write16(
    struct memory *memory,
    uint32_t addr,
    uint16_t val
) {
    mem_write8(memory, addr + 0, (uint8_t)(val >> 0));
    mem_write8(memory, addr + 1, (uint8_t)(val >> 8));
}

/*
** Read the double-word at the given address, hiding all endianness conversions.
*/
void
mem_write32(
    struct memory *memory,
    uint32_t addr,
    uint32_t val
) {
    mem_write8(memory, addr + 0, (uint8_t)(val >>  0));
    mem_write8(memory, addr + 1, (uint8_t)(val >>  8));
    mem_write8(memory, addr + 2, (uint8_t)(val >> 16));
    mem_write8(memory, addr + 3, (uint8_t)(val >> 24));
}