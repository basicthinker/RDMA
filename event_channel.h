//
//  event_channel.h
//  RDMA++
//
//  Created by Jinglei Ren on May 1, 2015.
//  Copyright (c) 2015 Jinglei Ren <jinglei@ren.systems>.
//

#ifndef RDMA_EVENT_CHANNEL_H_
#define RDMA_EVENT_CHANNEL_H_

#include <cstdio>
#include <rdma/rdma_cma.h>

namespace rdma {

class EventChannel {
 public:
  EventChannel();
  ~EventChannel();

  struct rdma_cm_event *GetEvent();
  void AckEvent();

  struct rdma_event_channel *channel() const { return channel_; }

 private:
  struct rdma_event_channel *channel_;
  struct rdma_cm_event *last_event_;
};

inline EventChannel::EventChannel() : last_event_(nullptr) {
  channel_ = rdma_create_event_channel();
  if (!channel_) perror("[Error] rdma_create_event_channel");
}

inline EventChannel::~EventChannel() {
  if (channel_) {
    AckEvent();
    rdma_destroy_event_channel(channel_);
  }
}

inline struct rdma_cm_event *EventChannel::GetEvent() {
  AckEvent();
  if (rdma_get_cm_event(channel_, &last_event_)) {
    perror("[Error] rdma_get_cm_event");
  }
  return last_event_;
}

inline void EventChannel::AckEvent() {
  if (!last_event_) return;
  if (rdma_ack_cm_event(last_event_)) {
    perror("[Error] rdma_ack_cm_event");
  }
  last_event_ = nullptr;
}


} // namespace rdma

#endif // RDMA_EVENT_CHANNEL_H_

