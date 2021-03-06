/*
 * blockstore-inl.hpp
 *
 *  Created on: Feb 9, 2016
 *      Author: moro
 */

#ifndef INCLUDE_MEMORY_BLOCKSTORE_INL_HPP_
#define INCLUDE_MEMORY_BLOCKSTORE_INL_HPP_

namespace afl {
namespace mem {
constexpr std::size_t tag_size(std::size_t capacity) {
  std::size_t bytes(0);
  while (capacity > 0) {
    capacity = capacity >> sizeof(char);
    bytes++;
  }
  return bytes;
}

template <std::size_t BlockSizeHint, std::size_t Capacity, typename AlignType>
void *BlockStore<BlockSizeHint, Capacity, AlignType>::malloc(void) noexcept {
  if (this->used == BlockStore::MAX_BLOCKS) {
    return nullptr;

  } else {
    void *retval;
    // if available, store pointer from top and return top.
    if (this->available > 0) {
      void *nxt = *reinterpret_cast<void **>(this->top);
      retval = this->top;
      this->top = nxt;
      this->available--;

      // if not available, take from back of free store. (all blocks are
      // packed).
    } else {
      retval = this->data + (this->used * BlockStore::BLOCK_SIZE);
    }

    this->used++;
    return retval;
  }
}

template <std::size_t BlockSizeHint, std::size_t Capacity, typename AlignType>
void BlockStore<BlockSizeHint, Capacity, AlignType>::free(void *vp) noexcept {
  if (this->available > 0) {
    // top is a valid reference
    *(reinterpret_cast<void **>(vp)) = top;
    top = vp;

  } else {
    *(reinterpret_cast<void **>(vp)) =
        (static_cast<char *>(vp) + BlockStore::BLOCK_SIZE);
    top = vp;
  }
  this->used--;
  this->available++;
}
} // namespace mem
} // namespace afl

#endif /* INCLUDE_MEMORY_BLOCKSTORE_INL_HPP_ */
