//
//  server_id.h
//  RDMA++
//
//  Created by Jinglei Ren on May 2, 2015.
//  Copyright (c) 2015 Jinglei Ren <jinglei@ren.systems>.
//

#ifndef RDMA_SERVER_ID_H_
#define RDMA_SERVER_ID_H_

#include "communication_id.h"

namespace rdma {

// The RDMA socket for server.
class ServerId : public CommunicationId {
 public:
  ServerId(VpiDevice *dev = nullptr) : CommunicationId(dev) {}

  void Listen(uint16_t port);
  struct rdma_cm_id *Accept(MemInfo local_mem);
};

} // namespace rdma

#endif // RDMA_SERVER_ID_H_

