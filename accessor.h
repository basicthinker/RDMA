//
//  accessor.h
//  RDMA++
//
//  Created by Jinglei Ren on May 3, 2015.
//  Copyright (c) 2015 Jinglei Ren <jinglei@ren.systems>.
//

#ifndef RDMA_ACCESSOR_H_
#define RDMA_ACCESSOR_H_

#include <cstdio>
#include <rdma/rdma_cma.h>
#include "communication_id.h"
#include "protection_domain.h"
#include "fast_completion_queue.h"

namespace rdma {

// Provides RDMA access.
class Accessor {
 public:
  Accessor() : communication_(nullptr) {}
  void Init(CommunicationId *id);

  ~Accessor();

 private:
  CommunicationId *communication_;

  ProtectionDomain domain_;
  FastCompletionQueue send_completion_;
  FastCompletionQueue recv_completion_;
};

inline void Accessor::Init(CommunicationId *id) {
  if (id->id() && id->context()) {
    communication_ = id;
  } else {
    fprintf(stderr, "[Error] Invalid communication id in Accessor init.\n");
    return;
  }

  domain_.Init(communication_->context());
  send_completion_.Init(communication_->context());
  recv_completion_.Init(communication_->context());

  struct ibv_qp_init_attr attr = {};
  attr.cap.max_send_wr = 1;
  attr.cap.max_send_sge = 1;
  attr.cap.max_recv_wr = 1;
  attr.cap.max_recv_sge = 1;
  attr.send_cq = send_completion_.queue();
  attr.recv_cq = recv_completion_.queue();

  attr.qp_type = IBV_QPT_RC;
  if (rdma_create_qp(communication_->id(), domain_.domain(), &attr)) {
    perror("[Error] rdma_create_qp");
  }
}

inline Accessor::~Accessor() {
  if (communication_) rdma_destroy_qp(communication_->id());
}

} // namespace rdma

#endif // RDMA_ACCESSOR_H_

