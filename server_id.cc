//
//  server_id.cc
//  RDMA++
//
//  Created by Jinglei Ren on May 2, 2015.
//  Copyright (c) 2015 Jinglei Ren <jinglei@ren.systems>.
//

#include "server_id.h"

#include <cstring>

namespace rdma {

void ServerId::Listen(uint16_t port) {
  if (!listener_.id()) return;

  struct sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  sin.sin_addr.s_addr = INADDR_ANY;
  if (rdma_bind_addr(listener_.id(), (struct sockaddr *)&sin)) {
    perror("[Error] rdma_bind_addr");
    return;
  }

  if (rdma_listen(listener_.id(), 1)) {
    perror("[Error] rdma_listen");
    return;
  }
}

void ServerId::Accept(MemInfo local_mem) {
  struct rdma_cm_event *event = channel_.GetEvent();
  if (!event || event->event != RDMA_CM_EVENT_CONNECT_REQUEST) {
    fprintf(stderr, "[Error] Failed to detect connection request!\n");
    return;
  }

  communication_.set_id(event->id);
  accessor_.Init(&communication_);

  struct rdma_conn_param param = {};
  param.responder_resources = 1;
  param.private_data = &local_mem;
  param.private_data_len = sizeof(MemInfo);

  if (rdma_accept(communication_.id(), &param)) {
    perror("[Error] rdma_accept");
    return;
  }
  event = channel_.GetEvent();
  if (!event || event->event != RDMA_CM_EVENT_ESTABLISHED) {
    fprintf(stderr, "[Error] Failed to establish connection!\n");
    return;
  }

  MemInfo mem;
  memcpy(&mem, event->param.conn.private_data, sizeof(MemInfo));
  communication_.set_remote_mem(mem);
}

} // namespace rdma

