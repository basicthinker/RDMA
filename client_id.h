//
//  client_id.h
//  RDMA++
//
//  Created by Jinglei Ren on May 2, 2015.
//  Copyright (c) 2015 Jinglei Ren <jinglei@ren.systems>.
//

#ifndef RDMA_CLIENT_ID_H_
#define RDMA_CLIENT_ID_H_

#include "accessor.h"
#include "communication_id.h"
#include "event_channel.h"

namespace rdma {

// The RDMA socket for client.
class ClientId {
 public:
  ClientId();

  void Resolve(const char *host, const char *port);
  void Connect(MemInfo local_mem);

  static const int kResolveTimeout = 5000; // ms

 private:
  EventChannel channel_;
  CommunicationId communication_;
  Accessor accessor_;
};

inline ClientId::ClientId() {
  channel_.Init();
  communication_.Init(channel_.channel());
}

} // namespace rdma

#endif // RDMA_CLIENT_ID_H_

