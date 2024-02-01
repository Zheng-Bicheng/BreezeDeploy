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

#include <faiss/utils/distances.h>
#include <faiss/IndexFlat.h>
#include "breeze_deploy/core/logger/breeze_deploy_logger.h"
#include "breeze_deploy/index/breeze_deploy_index.h"
#include "breeze_deploy/utils/data_process/normalize_l2/normalize_l2.h"

namespace breeze_deploy {
namespace index {
BreezeDeployIndex::BreezeDeployIndex(int feature_length,
									 const std::string &description,
									 BreezeDeployIndexMetricType metric)
	: feature_length_{feature_length}, description_{description}, metric_{metric} {
  auto index_description = description.c_str();
  auto index_metric = static_cast<faiss::MetricType>(metric);
  auto faiss_index_pointer_ = faiss::index_factory(feature_length, index_description, index_metric);
  BREEZE_DEPLOY_LOGGER_ASSERT(faiss_index_pointer_ != nullptr, "Failed to create BreezeDeployIndex. "
															   "Please check if the description and BreezeDeployIndexMetricType are set correctly.")
  faiss_index_ = std::unique_ptr<faiss::Index>(faiss_index_pointer_);
  index_index_map_ = faiss::IndexIDMap(faiss_index_.get());
}
bool BreezeDeployIndex::AddFeature(const std::vector<float> &feature, const std::vector<int64_t> &label_id) {
  if (feature.empty()) {
	BREEZE_DEPLOY_LOGGER_ERROR("The feature is empty.")
	return false;
  }

  auto feature_size = feature.size();
  if (feature_size % feature_length_ != 0) {
	BREEZE_DEPLOY_LOGGER_ERROR("The size of input feature is error.")
	return false;
  }

  auto n = static_cast<int64_t>(feature_size / feature_length_);
  if (n != label_id.size()) {
	BREEZE_DEPLOY_LOGGER_ERROR("The size of input label_id != n.")
	return false;
  }

  if (use_normalize_) {
	std::vector<float> normalize_feature = feature;
	utils::data_process::Normalize<float>(normalize_feature);
	index_index_map_.add_with_ids(n, normalize_feature.data(), label_id.data());
  } else {
	index_index_map_.add_with_ids(n, feature.data(), label_id.data());
  }
  return true;
}
bool BreezeDeployIndex::SearchIndex(const std::vector<float> &feature,
									int64_t k,
									std::vector<float> &distance,
									std::vector<int64_t> &label_id) {
  auto feature_size = feature.size();
  if (feature_size % feature_length_ != 0) {
	BREEZE_DEPLOY_LOGGER_ERROR("The size of input feature is error.")
	return false;
  }

  auto n = static_cast<int64_t>(feature_size / feature_length_);
  label_id.resize(n);
  distance.resize(n);
  if (use_normalize_) {
	std::vector<float> normalize_feature = feature;
	utils::data_process::Normalize<float>(normalize_feature);
	index_index_map_.search(n, normalize_feature.data(), k, distance.data(), label_id.data());
  } else {
	index_index_map_.search(n, feature.data(), k, distance.data(), label_id.data());
  }
  return true;
}
}
}