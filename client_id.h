//
//  client_id.h
//  RDMA++
//
//  Created by Jinglei Ren on May 2, 2015.
//  Copyright (c) 2015 Jinglei Ren <jinglei@ren.systems>.
//

#ifndef RDMA_CLIENT_ID_H_
#define RDMA_CLIENT_ID_H_

#include "communication_id.h"

namespace rdma {

// The RDMA socket for client.
class ClientId : public CommunicationId {
 public:
  ClientId(VpiDevice *dev = nullptr) : CommunicationId(dev) {}

  void Resolve(const char *host, const char *port);
  void Connect(MemInfo local_mem);
};

} // namespace rdma

#endif // RDMA_CLIENT_ID_H_

