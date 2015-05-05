//
//  server_id.cc
//  RDMA++
//
//  Created by Jinglei Ren on May 2, 2015.
//  Copyright (c) 2015 Jinglei Ren <jinglei@ren.systems>.
//

#include "server_id.h"

#include <cstring>
#include <cassert>

namespace rdma {

void ServerId::Listen(uint16_t port) {
  if (!id()) return;

  struct sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  sin.sin_addr.s_addr = INADDR_ANY;
  if (rdma_bind_addr(id(), (struct sockaddr *)&sin)) {
    perror("[Error] rdma_bind_addr");
    return;
  }

  if (rdma_listen(id(), 1)) {
    perror("[Error] rdma_listen");
    return;
  }
}

struct rdma_cm_id *ServerId::Accept(MemInfo local_mem) {
  struct rdma_cm_event *event = channel_.GetEvent();
  if (!event || event->event != RDMA_CM_EVENT_CONNECT_REQUEST) {
    fprintf(stderr, "[Error] Failed to detect connection request!\n");
  }
  struct rdma_cm_id *cm_id = event->id;
  assert(cm_id);

  struct rdma_conn_param param = {};
  param.responder_resources = 1;
  param.private_data = &local_mem;
  param.private_data_len = sizeof(MemInfo);

  if (rdma_accept(cm_id, &param)) {
    perror("[Error] rdma_accept");
    return nullptr;
  }
  event = channel_.GetEvent();
  if (!event || event->event != RDMA_CM_EVENT_ESTABLISHED) {
    fprintf(stderr, "[Error] Failed to establish connection!\n");
    return nullptr;
  }

  memcpy(&remote_mem_, event->param.conn.private_data, sizeof(MemInfo));
  return cm_id;
}

} // namespace rdma

