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
#include <boost/noncopyable.hpp>
#include <rdma/rdma_cma.h>

namespace rdma {

class EventChannel : private boost::noncopyable {
 public:
  EventChannel() : channel_(nullptr), last_event_(nullptr) {}
  void Init();
  ~EventChannel();

  rdma_cm_event *GetEvent();
  void AckEvent();

  void set_channel(rdma_event_channel *channel) { channel_ = channel; }
  rdma_event_channel *channel() const { return channel_; }

 private:
  rdma_event_channel *channel_;
  rdma_cm_event *last_event_;
};

inline void EventChannel::Init() {
  channel_ = rdma_create_event_channel();
  if (!channel_) perror("[Error] rdma_create_event_channel");
}

inline EventChannel::~EventChannel() {
  if (!channel_) return;
  AckEvent();
  rdma_destroy_event_channel(channel_);
}

inline rdma_cm_event *EventChannel::GetEvent() {
  if (!channel_) return nullptr;
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

