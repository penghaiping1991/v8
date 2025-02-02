// Copyright 2019 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_HEAP_READ_ONLY_HEAP_INL_H_
#define V8_HEAP_READ_ONLY_HEAP_INL_H_

#include "src/execution/isolate-utils-inl.h"
#include "src/heap/read-only-heap.h"
#include "src/roots/roots-inl.h"

namespace v8 {
namespace internal {

// static
ReadOnlyRoots ReadOnlyHeap::GetReadOnlyRoots(HeapObject object) {
#ifdef V8_COMPRESS_POINTERS_IN_ISOLATE_CAGE
  return ReadOnlyRoots(
      Isolate::FromRootAddress(GetIsolateRootAddress(object.ptr())));
#else
#ifdef V8_SHARED_RO_HEAP
  auto* shared_ro_heap = SoleReadOnlyHeap::shared_ro_heap_;
  // If this check fails in code that runs during initialization make sure to
  // load the ReadOnlyRoots from an isolate instead.
  // TODO(olivf, v8:13466): Relax this to a DCHECK once we are sure we got it
  // right everywhere.
  CHECK(shared_ro_heap && shared_ro_heap->roots_init_complete());
  return ReadOnlyRoots(shared_ro_heap->read_only_roots_);
#else
  return ReadOnlyRoots(GetHeapFromWritableObject(object));
#endif  // V8_SHARED_RO_HEAP
#endif  // V8_COMPRESS_POINTERS
}

}  // namespace internal
}  // namespace v8

#endif  // V8_HEAP_READ_ONLY_HEAP_INL_H_
