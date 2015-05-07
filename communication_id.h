//
//  communication_id.h
//  RDMA++
//
//  Created by Jinglei Ren on May 2, 2015.
//  Copyright (c) 2015 Jinglei Ren <jinglei@ren.systems>.
//

#ifndef RDMA_COMMUNICATION_ID_H_
#define RDMA_COMMUNICATION_ID_H_

#include <cstdio>
#include <boost/noncopyable.hpp>
#include <infiniband/verbs.h>
#include <rdma/rdma_cma.h>

namespace rdma {

struct MemInfo {
  uint64_t addr;
  uint32_t key;
};

// The RDMA socket (root class).
// Manages a connection and remote memory region info.
class CommunicationId : private boost::noncopyable {
 public:
  CommunicationId() : id_(nullptr), remote_mem_({}) {}
  void Init(rdma_event_channel *chnl = nullptr, ibv_context *ctx = nullptr);
  virtual ~CommunicationId();

  void set_id(rdma_cm_id *id) { id_ = id; }
  rdma_cm_id *id() const { return id_; }
  void set_remote_mem(MemInfo mem) { remote_mem_ = mem; }
  MemInfo remote_mem() const { return remote_mem_; }
  ibv_context *context() const { return id_->verbs; }

 private:
  rdma_cm_id *id_;
  MemInfo remote_mem_;
};

inline void CommunicationId::Init(rdma_event_channel *chnl, ibv_context *ctx) {
  if (rdma_create_id(chnl, &id_, ctx, RDMA_PS_TCP)) {
    perror("[Error] rdma_create_id");
  }
}

inline CommunicationId::~CommunicationId() {
  if (id_ && rdma_destroy_id(id_)) {
    perror("[Error] rdma_destroy_id");
  }
}

} // namespace rdma

#endif // RDMA_COMMUNICATION_ID_H_

