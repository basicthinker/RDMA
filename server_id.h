//
//  server_id.h
//  RDMA++
//
//  Created by Jinglei Ren on May 2, 2015.
//  Copyright (c) 2015 Jinglei Ren <jinglei@ren.systems>.
//

#ifndef RDMA_SERVER_ID_H_
#define RDMA_SERVER_ID_H_

#include "accessor.h"
#include "communication_id.h"
#include "event_channel.h"

namespace rdma {

// The RDMA socket for server.
class ServerId {
 public:
  ServerId();

  void Listen(uint16_t port);
  void Accept(MemInfo local_mem);

 private:
  EventChannel channel_;
  CommunicationId listener_;
  //TODO: put the following into threads
  CommunicationId communication_;
  Accessor accessor_;
};

inline ServerId::ServerId() {
  channel_.Init();
  listener_.Init(channel_.channel());
}

} // namespace rdma

#endif // RDMA_SERVER_ID_H_

