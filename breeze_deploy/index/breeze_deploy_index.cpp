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

#include "breeze_deploy/index/breeze_deploy_index.h"
#include "faiss/utils/distances.h"
#include "faiss/IndexFlat.h"
#include "breeze_deploy/core/logger/breeze_deploy_logger.h"

namespace breeze_deploy {
namespace index {
BreezeDeployIndex::BreezeDeployIndex(int feature_length,
									 const std::string &description,
									 BreezeDeployIndexMetricType metric)
	: feature_length_{feature_length}, description_{description}, metric_{metric} {
  auto index_description = description.c_str();
  auto index_metric = static_cast<faiss::MetricType>(metric);
  auto faiss_index_pointer_ = faiss::index_factory(feature_length, index_description, index_metric);
  BDLOGGER_ASSERT(faiss_index_pointer_ != nullptr, "Failed to create BreezeDeployIndex. "
															   "Please check if the description and BreezeDeployIndexMetricType are set correctly.")
  faiss_index_ = std::unique_ptr<faiss::Index>(faiss_index_pointer_);
  index_index_map_ = faiss::IndexIDMap(faiss_index_.get());
}
bool BreezeDeployIndex::AddFeature(const std::vector<float> &feature, const std::vector<int64_t> &label_id) {
  if (faiss_index_ == nullptr){
    BDLOGGER_ERROR("faiss_index_ == nullptr.")
    return false;
  }

  if (feature.empty()) {
	BDLOGGER_ERROR("The feature is empty.")
	return false;
  }

  auto feature_size = feature.size();
  if (feature_size % feature_length_ != 0) {
	BDLOGGER_ERROR("The size of input feature is error.")
	return false;
  }

  auto n = static_cast<int64_t>(feature_size / feature_length_);
  if (n != label_id.size()) {
	BDLOGGER_ERROR("The size of input label_id != n.")
	return false;
  }

  index_index_map_.add_with_ids(n, feature.data(), label_id.data());
  return true;
}
bool BreezeDeployIndex::SearchIndex(const std::vector<float> &feature,
									size_t k,
									std::vector<float> &topk_distance,
									std::vector<int64_t> &topk_label_id) {
  auto feature_size = feature.size();
  if (feature_size % feature_length_ != 0) {
	BDLOGGER_ERROR("The size of input feature is error.")
	return false;
  }

  auto temp_n = static_cast<int64_t>(feature_size / feature_length_);
  topk_label_id.resize(temp_n);
  topk_distance.resize(temp_n);
  auto temp_k = static_cast<faiss::idx_t>(k);
  index_index_map_.search(temp_n, feature.data(), temp_k, topk_distance.data(), topk_label_id.data());
  return true;
}
}
}
