//
//  fast_completion_queue.h
//  RDMA++
//
//  Created by Jinglei Ren on May 1, 2015.
//  Copyright (c) 2015 Jinglei Ren <jinglei@ren.systems>.
//

#ifndef RDMA_FAST_COMPLETION_QUEUE_H_
#define RDMA_FAST_COMPLETION_QUEUE_H_

#include <cstdio>
#include <boost/noncopyable.hpp>
#include <infiniband/verbs.h>

namespace rdma {

// A fast single-entry completion queue,
// without a completion channel or event notification.
// It should be polled after each read/write request.
// Simple inlined implementation wrapping ibv_cq functions.
class FastCompletionQueue : private boost::noncopyable {
 public:
  FastCompletionQueue() : queue_(nullptr) {}
  FastCompletionQueue(struct ibv_context *context) { Init(context); }
  void Init(struct ibv_context *context);

  ~FastCompletionQueue();

  bool PollCompletion();

  struct ibv_cq *queue() { return queue_; }

 private:
  struct ibv_cq *queue_;
};

inline void FastCompletionQueue::Init(struct ibv_context *context) {
  queue_ = ibv_create_cq(context, 1, nullptr, nullptr, 0);
}

inline FastCompletionQueue::~FastCompletionQueue() {
  if (queue_ && ibv_destroy_cq(queue_)) {
    perror("[Error] ibv_destroy_cq");
  }
}

inline bool FastCompletionQueue::PollCompletion() {
  if (!queue_) {
    fprintf(stderr, "[Warning] Polling a null completion queue.\n");
    return false;
  }
  struct ibv_wc wc;
  for (int i = 0; i < 1000000000; ++i) {
    if (ibv_poll_cq(queue_, 1, &wc) > 0 && wc.status == IBV_WC_SUCCESS) {
      return true;
    }
  }
}

} // namespace rdma

#endif // RDMA_FAST_COMPLETION_QUEUE_H_

