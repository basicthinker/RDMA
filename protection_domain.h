//
//  protection_domain.h
//  RDMA++
//
//  Created by Jinglei Ren on May 3, 2015.
//  Copyright (c) 2015 Jinglei Ren <jinglei@ren.systems>.
//

#ifndef RDMA_PROTECTION_DOMAIN_H_
#define RDMA_PROTECTION_DOMAIN_H_

#include <cstdio>
#include <vector>
#include <boost/noncopyable.hpp>
#include <rdma/rdma_cma.h>

namespace rdma {

class ProtectionDomain : boost::noncopyable {
 public:
  ProtectionDomain() : domain_(nullptr) {}
  ProtectionDomain(struct ibv_context *context) { Init(context); }
  void Init(struct ibv_context *context);

  ~ProtectionDomain();

  struct ibv_mr *Register(void *addr, size_t len, enum ibv_access_flags access);

  struct ibv_pd *domain() const { return domain_; }

 private:
  struct ibv_pd *domain_;
  std::vector<struct ibv_mr *> mem_regions_;
};

inline void ProtectionDomain::Init(struct ibv_context *context) {
  domain_ = ibv_alloc_pd(context);
}

inline ProtectionDomain::~ProtectionDomain() {
  for (struct ibv_mr *mr : mem_regions_) {
    if (ibv_dereg_mr(mr)) perror("[Error] ibv_dereg_mr");
  }

  if (domain_ && ibv_dealloc_pd(domain_)) {
    perror("[Error] ibv_dealloc_pd");
  }
}

inline struct ibv_mr *ProtectionDomain::Register(
    void *addr, size_t len, enum ibv_access_flags access) {
  struct ibv_mr *mem_region = ibv_reg_mr(domain(), addr, len, access);
  if (mem_region) {
    mem_regions_.push_back(mem_region);
  } else {
    fprintf(stderr, "[Error] Failed to register memory region %p.\n", addr);
  }
  return mem_region;
}

} // namespace rdma

#endif // RDMA_PROTECTION_DOMAIN_H_

