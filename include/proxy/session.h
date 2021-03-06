/*
 * ProFTPD - mod_proxy sessions
 * Copyright (c) 2012-2013 TJ Saunders
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

#ifndef MOD_PROXY_SESSION_H
#define MOD_PROXY_SESSION_H

struct proxy_conn;

struct proxy_session {
  struct pool_rec *pool;

  int connect_timeout;
  int connect_timerno;

  /* Frontend connection */
  conn_t *frontend_ctrl_conn;
  conn_t *frontend_data_conn;
  volatile int frontend_sess_flags;
  pr_netaddr_t *frontend_data_addr;

  /* Backend connection */
  conn_t *backend_ctrl_conn;
  conn_t *backend_data_conn;
  volatile int backend_sess_flags;
  pr_netaddr_t *backend_data_addr;

  /* Address for connections to/from destination server.  May be null. */
  pr_netaddr_t *src_addr;

  /* Address of the destination server.  May be null. */
  pr_netaddr_t *dst_addr;
  struct proxy_conn *dst_pconn;

  /* Features supported by backend server */
  pr_table_t *backend_features;

  /* Data transfer policy: PASV, EPSV, PORT, EPRT, or client. */
  int dataxfer_policy;
};

/* Zero indicates "do what the client does". */
#define PROXY_SESS_DATA_TRANSFER_POLICY_DEFAULT		0

struct proxy_session *proxy_session_alloc(pool *p);

#endif /* MOD_PROXY_SESSION_H */
