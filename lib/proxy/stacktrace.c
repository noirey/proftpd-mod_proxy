/*
 * ProFTPD - mod_proxy stacktrace logging
 * Copyright (c) 2012 TJ Saunders
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Suite 500, Boston, MA 02110-1335, USA.
 *
 * As a special exemption, TJ Saunders and other respective copyright holders
 * give permission to link this program with OpenSSL, and distribute the
 * resulting executable, without including the source code for OpenSSL in the
 * source distribution.
 */

#include "mod_proxy.h"
#include "proxy/stacktrace.h"

#ifdef HAVE_EXECINFO_H
# include <execinfo.h>
#endif

void proxy_log_stacktrace(void) {
#if defined(HAVE_EXECINFO_H) && \
    defined(HAVE_BACKTRACE) && \
    defined(HAVE_BACKTRACE_SYMBOLS)
  void *trace[PR_TUNABLE_CALLER_DEPTH];
  char **strings;
  size_t tracesz;

  (void) pr_log_writefile(proxy_logfd, MOD_PROXY_VERSION,
    "-----BEGIN STACK TRACE-----");

  tracesz = backtrace(trace, PR_TUNABLE_CALLER_DEPTH);
  strings = backtrace_symbols(trace, tracesz);
  if (strings != NULL) {
    register unsigned int i;

    for (i = 1; i < tracesz; i++) {
      (void) pr_log_writefile(proxy_logfd, MOD_PROXY_VERSION,
        "[%u] %s", i-1, strings[i]);
    }

    /* Prevent memory leaks. */
    free(strings);

  } else {
    (void) pr_log_writefile(proxy_logfd, MOD_PROXY_VERSION,
      "error obtaining stacktrace symbols: %s", strerror(errno));
  }

  (void) pr_log_writefile(proxy_logfd, MOD_PROXY_VERSION,
    "-----END STACK TRACE-----");
#endif
}
