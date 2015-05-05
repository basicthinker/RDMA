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
#include "event_channel.h"
#include "vpi_device.h"

namespace rdma {

struct MemInfo {
  uint64_t addr;
  uint32_t key;
};

// The RDMA socket (root class).
// Manages a connection and remote memory regions info.
class CommunicationId {
 public:
  CommunicationId(VpiDevice *dev = nullptr);
  virtual ~CommunicationId();

  struct rdma_cm_id *id() const { return id_; }
  MemInfo remote_mem() const { return remote_mem_; }

  static const int kResolveTimeout = 5000; // ms

 protected:
  EventChannel channel_;
  MemInfo remote_mem_;

 private:
  struct rdma_cm_id *id_;
};

inline CommunicationId::CommunicationId(VpiDevice *dev) {
  int err = rdma_create_id(channel_.channel(),
      &id_, (dev ? dev->context() : nullptr), RDMA_PS_TCP);
  if (err) {
    perror("[Error] rdma_create_id");
    id_ = nullptr;
  }
  remote_mem_ = {};
}

inline CommunicationId::~CommunicationId() {
  channel_.AckEvent();
  if (id_ && rdma_destroy_id(id_)) {
    perror("[Error] rdma_destroy_id");
  }
}

} // namespace rdma

#endif // RDMA_COMMUNICATION_ID_H_

