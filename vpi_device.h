//
//  vpi_device.h
//  RDMA++
//
//  Created by Jinglei Ren on May 1, 2015.
//  Copyright (c) 2015 Jinglei Ren <jinglei@ren.systems>.
//

#ifndef RDMA_VPI_DEVICE_H_
#define RDMA_VPI_DEVICE_H_

#include <infiniband/verbs.h>

namespace rdma {

// A VPI device for RDMA
class VpiDevice {
 public:
  VpiDevice(const char *name = nullptr);
  ~VpiDevice();

  struct ibv_device *device() const { return device_; }
  struct ibv_context *context() const { return context_; }

 private:
  struct ibv_device *device_;
  struct ibv_context *context_;
};

// A wrapper for local device list
class VpiDeviceList {
 public:
  VpiDeviceList() : count_(0), list_(ibv_get_device_list(&count_)) {}
  ~VpiDeviceList() { if (list_) ibv_free_device_list(list_); }

  struct ibv_device *Find(const char *dev_name) const;

  int count() const { return count_; }
  struct ibv_device **list() const { return list_; }

 private:
  int count_;
  struct ibv_device **list_;
};

} // namespace rdma

#endif // RDMA_VPI_DEVICE_H_

