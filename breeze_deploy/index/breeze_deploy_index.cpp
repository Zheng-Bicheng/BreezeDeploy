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

#include "breeze_deploy/core/logger/breeze_deploy_logger.h"
#include "breeze_deploy/index/breeze_deploy_index.h"

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
bool BreezeDeployIndex::AddFeature(const std::vector<float> &feature, int64_t label_id) {
  if (feature.empty()) {
	BREEZE_DEPLOY_LOGGER_ERROR("The feature is empty.")
	return false;
  }

  if (feature.size() != feature_length_) {
	BREEZE_DEPLOY_LOGGER_ERROR("The size of input feature != feature_length. Input size is {},feature_length is {}",
							   feature.size(), feature_length_)
	return false;
  }
  index_index_map_.add_with_ids(1, feature.data(), &label_id);
  return true;
}
}
}
