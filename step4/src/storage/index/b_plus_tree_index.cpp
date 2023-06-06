//===----------------------------------------------------------------------===//
//
//                         CMU-DB Project (15-445/645)
//                         ***DO NO SHARE PUBLICLY***
//
// Identification: src/index/b_plus_tree_index.cpp
//
// Copyright (c) 2018, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "storage/index/b_plus_tree_index.h"

namespace bustub {
/*
 * Constructor
 */
INDEX_TEMPLATE_ARGUMENTS
BPLUSTREE_INDEX_TYPE::BPlusTreeIndex(std::unique_ptr<IndexMetadata> &&metadata, BufferPoolManager *buffer_pool_manager)
    : Index(std::move(metadata)),
      comparator_(GetMetadata()->GetKeySchema()),
      container_(GetMetadata()->GetName(), buffer_pool_manager, comparator_) {}

INDEX_TEMPLATE_ARGUMENTS
void BPLUSTREE_INDEX_TYPE::InsertEntry(const Tuple &key, RID rid, Transaction *transaction) {
  // construct insert index key
  KeyType index_key;
  index_key.SetFromKey(key);

  container_.Insert(index_key, rid);
}

INDEX_TEMPLATE_ARGUMENTS
void BPLUSTREE_INDEX_TYPE::DeleteEntry(const Tuple &key, RID rid, Transaction *transaction) {
}

INDEX_TEMPLATE_ARGUMENTS
void BPLUSTREE_INDEX_TYPE::ScanKey(const Tuple &key, std::vector<RID> *result, Transaction *transaction) {
  // construct scan index key
  KeyType index_key;
  index_key.SetFromKey(key);

  container_.GetValue(index_key, result);
}

INDEX_TEMPLATE_ARGUMENTS
INDEXITERATOR_TYPE BPLUSTREE_INDEX_TYPE::GetBeginIterator() { return container_.Begin(); }

INDEX_TEMPLATE_ARGUMENTS
INDEXITERATOR_TYPE BPLUSTREE_INDEX_TYPE::GetBeginIterator(const KeyType &key) { return container_.Begin(key); }

INDEX_TEMPLATE_ARGUMENTS
INDEXITERATOR_TYPE BPLUSTREE_INDEX_TYPE::GetEndIterator() { return container_.End(); }

template class BPlusTreeIndex<GenericKey<4>, RID, GenericComparator<4>>;
template class BPlusTreeIndex<GenericKey<8>, RID, GenericComparator<8>>;
template class BPlusTreeIndex<GenericKey<16>, RID, GenericComparator<16>>;
template class BPlusTreeIndex<GenericKey<32>, RID, GenericComparator<32>>;
template class BPlusTreeIndex<GenericKey<64>, RID, GenericComparator<64>>;

}  // namespace bustub