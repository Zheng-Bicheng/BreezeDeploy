// Copyright (c) 2024/1/31 Zheng-Bicheng. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef BREEZE_DEPLOY_INDEX_BREEZE_DEPLOY_INDEX_H_
#define BREEZE_DEPLOY_INDEX_BREEZE_DEPLOY_INDEX_H_
#include <faiss/index_factory.h>
#include <faiss/IndexIDMap.h>
namespace breeze_deploy {
namespace index {
enum class BreezeDeployIndexMetricType {
  METRIC_INNER_PRODUCT = faiss::METRIC_INNER_PRODUCT,  ///< 内积（Inner Product）距离度量
  METRIC_L2 = faiss::METRIC_L2,    ///< 欧几里德距离（L2距离）度量。
  METRIC_L1 = faiss::METRIC_L1,    ///< 曼哈顿距离（L1距离）度量。
  METRIC_Linf = faiss::METRIC_Linf,
  METRIC_Lp = faiss::METRIC_Lp,
  METRIC_Canberra = faiss::METRIC_Canberra,
  METRIC_BrayCurtis = faiss::METRIC_BrayCurtis,
  METRIC_JensenShannon = faiss::METRIC_JensenShannon,
  METRIC_Jaccard = faiss::METRIC_Jaccard,
};

class BreezeDeployIndex {
 public:
  BreezeDeployIndex(int feature_length,
					const std::string &description,
					BreezeDeployIndexMetricType metric = BreezeDeployIndexMetricType::METRIC_INNER_PRODUCT);
  bool AddFeature(const std::vector<float> &feature, int64_t label_id);
 private:
  int feature_length_ = 0;
  std::string description_;
  BreezeDeployIndexMetricType metric_;
  std::unique_ptr<faiss::Index> faiss_index_;
  faiss::IndexIDMap index_index_map_;
};
}
}
#endif //BREEZE_DEPLOY_INDEX_BREEZE_DEPLOY_INDEX_H_
