//
//  client_id.cc
//  RDMA++
//
//  Created by Jinglei Ren on May 2, 2015.
//  Copyright (c) 2015 Jinglei Ren <jinglei@ren.systems>.
//

#include "client_id.h"

#include <cstdio>
#include <cstring>
#include <netdb.h>

namespace rdma {

void ClientId::Resolve(const char *host, const char *port) {
  struct addrinfo hints = {};
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  struct addrinfo *res;
  int err = getaddrinfo(host, port, &hints, &res);
  if (err) {
    fprintf(stderr, "[Error] getaddrinfo: %s\n", gai_strerror(err));
  }

  if (!id()) return;

  for (struct addrinfo *i = res; i; i = i->ai_next) {
    err = rdma_resolve_addr(id(), nullptr, i->ai_addr, kResolveTimeout);
    if (!err) break;
  }
  freeaddrinfo(res);
  if (err || channel_.GetEvent()->event != RDMA_CM_EVENT_ADDR_RESOLVED) {
    fprintf(stderr, "[Error] Failed to resolve address %s\n", host);
    return;
  }

  if (rdma_resolve_route(id(), kResolveTimeout)) {
    perror("[Error] rdma_resolve_route");
    return;
  }
  if (channel_.GetEvent()->event != RDMA_CM_EVENT_ROUTE_RESOLVED) {
    fprintf(stderr, "[Error] Failed to resolve route to %s\n", host);
    return;
  }
}

void ClientId::Connect(MemInfo local_mem) {
  if (!id()) return;

  struct rdma_conn_param param = {};
  param.initiator_depth = 1;
  param.retry_count = 10;
  param.private_data = &local_mem;
  param.private_data_len = sizeof(MemInfo);

  if (rdma_connect(id(), &param)) {
    perror("[Error] rdma_connect");
    return;
  }
  struct rdma_cm_event *event = channel_.GetEvent(); 
  if (event->event != RDMA_CM_EVENT_ESTABLISHED) {
    fprintf(stderr, "[Error] Failed to establish connection!\n");
    return;
  }

  memcpy(&remote_mem_, event->param.conn.private_data, sizeof(MemInfo));
}

} // namespace rdma

