/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2020-2025 Eric Herman <eric@freesa.org> */

#ifndef EEMBED_ARDUINO_H
#define EEMBED_ARDUINO_H 1

#include <Arduino.h>
#include <HardwareSerial.h>

#ifndef ARDUINO
#error "Expected an ARDUINO build"
#endif

#ifndef EEMBED_HOSTED
#define EEMBED_HOSTED 0
#endif

/* If using a _VARIANT_ARDUINO_DUE_X_ there is both the Serial and SerialUSB
 * but either one might be correct for this, so let the caller define it,
 * or adjust it here */

#ifndef eembed_out
#define eembed_out Serial
#endif

#ifndef eembed_err
#define eembed_err eembed_out
#endif

#ifndef print_s
#define print_s(s) eembed_out.print(s)
#endif

#ifndef print_err_s
#define print_err_s(s) eembed_err.print(s)
#endif

#ifndef print_i
#define print_i(i) eembed_out.print(i)
#endif

#ifndef print_err_i
#define print_err_i(i) eembed_err.print(i)
#endif

#ifndef print_l
#define print_l(l) eembed_out.print(l)
#endif

#ifndef print_err_l
#define print_err_l(l) eembed_err.print(l)
#endif

#ifndef print_ll
#define print_ll(ll) eembed_out.print(ll)
#endif

#ifndef print_err_ll
#define print_err_ll(ll) eembed_err.print(ll)
#endif

#ifndef print_u
#define print_u(u) eembed_out.print(u)
#endif

#ifndef print_err_u
#define print_err_u(u) eembed_err.print(u)
#endif

#ifndef print_ul
#define print_ul(ul) eembed_out.print(ul)
#endif

#ifndef print_err_ul
#define print_err_ul(ul) eembed_err.print(ul)
#endif

#ifndef print_ull
#define print_ull(ull) eembed_out.print(ull)
#endif

#ifndef print_err_ull
#define print_err_ull(ull) eembed_err.print(ull)
#endif

#ifndef print_z
#define print_z(z) eembed_out.print(z)
#endif

#ifndef print_err_z
#define print_err_z(z) eembed_err.print(z)
#endif

#ifndef print_c
#define print_c(c) eembed_out.print(c)
#endif

#ifndef print_err_c
#define print_err_c(c) eembed_err.print(c)
#endif

#ifndef print_f
#define print_f(f) eembed_out.print(f)
#endif

#ifndef print_err_f
#define print_err_f(f) eembed_err.print(f)
#endif

#ifndef print_lf
#define print_lf(lf) eembed_out.print(lf)
#endif

#ifndef print_err_lf
#define print_err_lf(lf) eembed_err.print(lf)
#endif

#ifndef print_fd
#define print_fd(f,d) eembed_out.print(f, d)
#endif

#ifndef print_err_fd
#define print_err_fd(f,d) eembed_err.print(f, d)
#endif

#ifndef print_lfd
#define print_lfd(lf,d) eembed_out.print(lf, d)
#endif

#ifndef print_err_lfd
#define print_err_lfd(lf,d) eembed_err.print(lf, d)
#endif

#ifndef print_x
#define print_x(x) eembed_out.print(x, HEX)
#endif

#ifndef print_err_x
#define print_err_x(x) eembed_err.print(x, HEX)
#endif

#ifndef print_lx
#define print_lx(lx) eembed_out.print(lx, HEX)
#endif

#ifndef print_err_lx
#define print_err_lx(lx) eembed_err.print(lx, HEX)
#endif

#ifndef print_u8_bin
#define print_u8_bin(u8) eembed_out.print(u8, BIN)
#endif

#ifndef print_err_u8_bin
#define print_err_u8_bin(u8) eembed_err.print(u8, BIN)
#endif

#ifndef print_u16_bin
#define print_u16_bin(u16) eembed_out.print(u16, BIN)
#endif

#ifndef print_err_u16_bin
#define print_err_u16_bin(u16) eembed_err.print(u16, BIN)
#endif

#ifndef print_u32_bin
#define print_u32_bin(u32) eembed_out.print(u32, BIN)
#endif

#ifndef print_err_u32_bin
#define print_err_u32_bin(u32) eembed_err.print(u32, BIN)
#endif

#ifndef print_u64_bin
#define print_u64_bin(u64) eembed_out.print(u64, BIN)
#endif

#ifndef print_err_u64_bin
#define print_err_u64_bin(u64) eembed_err.print(u64, BIN)
#endif

#ifndef print_u8_hex
#define print_u8_hex(u8) eembed_out.print(u8, HEX)
#endif

#ifndef print_err_u8_hex
#define print_err_u8_hex(u8) eembed_err.print(u8, HEX)
#endif

#ifndef print_vp_hex
#define print_vp_hex(p) eembed_out.print((uintptr_t)p, HEX)
#endif

#ifndef print_err_vp_hex
#define print_err_vp_hex(p) eembed_err.print((uintptr_t)p, HEX)
#endif

#ifndef print_eol
#define print_eol(void) eembed_out.println()
#endif

#ifndef print_err_eol
#define print_err_eol(void) eembed_err.println()
#endif

#define delay_ms_u16(milliseconds) delay(milliseconds)
#define uptime_ms() millis()

extern unsigned long eembed_die_delay;

void eembed_arduino_serial_log_init(void);

#endif /* EEMBED_ARDUINO_H */
