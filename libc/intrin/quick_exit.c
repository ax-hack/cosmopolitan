/*-*- mode:c;indent-tabs-mode:nil;c-basic-offset:2;tab-width:8;coding:utf-8 -*-│
│vi: set net ft=c ts=2 sts=2 sw=2 fenc=utf-8                                :vi│
╞══════════════════════════════════════════════════════════════════════════════╡
│ Copyright 2021 Justine Alexandra Roberts Tunney                              │
│                                                                              │
│ Permission to use, copy, modify, and/or distribute this software for         │
│ any purpose with or without fee is hereby granted, provided that the         │
│ above copyright notice and this permission notice appear in all copies.      │
│                                                                              │
│ THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL                │
│ WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED                │
│ WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE             │
│ AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL         │
│ DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR        │
│ PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER               │
│ TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR             │
│ PERFORMANCE OF THIS SOFTWARE.                                                │
╚─────────────────────────────────────────────────────────────────────────────*/
#include "libc/bits/weaken.h"
#include "libc/dce.h"
#include "libc/nt/console.h"
#include "libc/nt/runtime.h"
#include "libc/runtime/internal.h"
#include "libc/runtime/runtime.h"
#include "libc/stdio/stdio.h"

const char kConsoleHandles[2] = {kNtStdInputHandle, kNtStdOutputHandle};

/**
 * Exits process faster.
 *
 * @param exitcode is masked with 255
 * @noreturn
 */
wontreturn void quick_exit(int exitcode) {
  int i;
  const uintptr_t *p;
  if (weaken(fflush)) {
    weaken(fflush)(0);
  }
  if (SupportsWindows() && __ntconsolemode[0]) {
    for (i = 0; i < 2; ++i) {
      SetConsoleMode(GetStdHandle(kConsoleHandles[i]), __ntconsolemode[i]);
    }
  }
  for (p = __fini_array_end; p > __fini_array_start;) {
    ((void (*)(void))(*--p))();
  }
  _Exit(exitcode);
}