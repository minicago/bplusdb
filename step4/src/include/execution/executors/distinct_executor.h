//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// distinct_executor.h
//
// Identification: src/include/execution/executors/distinct_executor.h
//
// Copyright (c) 2015-2021, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include <memory>
#include <utility>
#include <set>
#include "common/util/hash_util.h"
#include "execution/executors/abstract_executor.h"
#include "execution/plans/distinct_plan.h"




namespace bustub {



struct DistinctKey {


	//这里发现distinct去重，没办法直接往set里放tuple来去重
	//故在这里模仿聚合函数中相同key值直接聚合的方法，使用key来去重
	//set去重的原理是排序，DistinctKey没有排序的逻辑，故采用map的hash去重
  std::vector<Value> distincts_;

  bool operator==(const DistinctKey &other) const {
    for (uint32_t i = 0; i < other.distincts_.size(); i++) {
      if (distincts_[i].CompareEquals(other.distincts_[i]) != CmpBool::CmpTrue) {
        return false;
      }
    }
    return true;
  }
};
}
namespace std {
//该函数用于map中DistinctKey的哈希计算
/** Implements std::hash on AggregateKey */
template <>
struct hash<bustub::DistinctKey> {
  std::size_t operator()(const bustub::DistinctKey &agg_key) const {
    size_t curr_hash = 0;
    for (const auto &key : agg_key.distincts_) {
      if (!key.IsNull()) {
        curr_hash = bustub::HashUtil::CombineHashes(curr_hash, bustub::HashUtil::HashValue(&key));
      }
    }
    return curr_hash;
  }
};
}  // namespace std

namespace bustub {
/**
 * DistinctExecutor removes duplicate rows from child ouput.
 */
class DistinctExecutor : public AbstractExecutor {
 public:
  /**
   * Construct a new DistinctExecutor instance.
   * @param exec_ctx The executor context
   * @param plan The limit plan to be executed
   * @param child_executor The child executor from which tuples are pulled
   */
  DistinctExecutor(ExecutorContext *exec_ctx, const DistinctPlanNode *plan,
                   std::unique_ptr<AbstractExecutor> &&child_executor);

  /** Initialize the distinct */
  void Init() override;

  /**
   * Yield the next tuple from the distinct.
   * @param[out] tuple The next tuple produced by the distinct
   * @param[out] rid The next tuple RID produced by the distinct
   * @return `true` if a tuple was produced, `false` if there are no more tuples
   */
  bool Next(Tuple *tuple, RID *rid) override;

  /** @return The output schema for the distinct */
  const Schema *GetOutputSchema() override { return plan_->OutputSchema(); };

 private:
  /** The distinct plan node to be executed */
  const DistinctPlanNode *plan_;
  /** The child executor from which tuples are obtained */
  std::unique_ptr<AbstractExecutor> child_executor_;
  std::unordered_map<DistinctKey, Tuple> res;
  std::unordered_map<DistinctKey, Tuple>::iterator iter_;
};
}  // namespace bustub

