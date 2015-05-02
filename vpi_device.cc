//
//  vpi_device.cc
//  RDMA++
//
//  Created by Jinglei Ren on May 1, 2015.
//  Copyright (c) 2015 Jinglei Ren <jinglei@ren.systems>.
//

#include "vpi_device.h"

#include <cstdio>
#include <cstring>

namespace rdma {

VpiDevice::VpiDevice(const char *name) : device_(nullptr), context_(nullptr) {
  VpiDeviceList local_devices;
  if (!local_devices.count()) {
    fprintf(stderr, "[Error] Unavailable device list for %s.\n", name);
    return;
  }

  device_ = local_devices.Find(name);
  if (!device_) {
    fprintf(stderr, "[Info] Device %s is not found!\n", name);
    return;
  }

  context_ = ibv_open_device(device_);
  if (!context_) {
    fprintf(stderr, "[Error] Failed to open device %s!\n", name);
    return;
  }
}

VpiDevice::~VpiDevice() {
  if (context_) {
    ibv_close_device(context_);
  }
}

struct ibv_device *VpiDeviceList::Find(const char *dev_name) const {
  for (int i = 0; i < count(); ++i) {
    if (!dev_name || strcmp(dev_name, ibv_get_device_name(list()[i])) == 0) {
      return list()[i];
    }
  }
  return nullptr;
}

} // namespace rdma

