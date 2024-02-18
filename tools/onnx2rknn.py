# Copyright (c) 2022 PaddlePaddle Authors. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
import os
import yaml
import argparse
from rknn.api import RKNN


def get_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('--onnx_model_path', type=str, help='The path of onnx model for loading')
    parser.add_argument('--config_file_path', type=str, help='Path to the configuration file')
    parser.add_argument('--rknn_model_path', type=str, help='The path of rknn model for saving')
    parser.add_argument('--verbose', type=bool, default=True, help='Whether to output rknntoolkit logs')
    temp_args = parser.parse_args()
    return temp_args


class RKNNConverter:
    def __init__(self, temp_args):
        self.args = temp_args
        self.verbose = self.args.verbose
        self.onnx_model_path = self.args.onnx_model_path
        self.config_file_path = self.args.config_file_path
        self.rknn_model_path = self.args.rknn_model_path

        self.yaml_config = None
        self.rknn_export = None

        self.rknn = RKNN(self.verbose)

    def read_config(self):
        try:
            with open(self.config_file_path) as config_file:
                config_file_args = config_file.read()
                self.yaml_config = yaml.safe_load(config_file_args)
        except Exception as e:
            assert False, (f"An error{e} occurred while reading the file. Please check if the configuration file is "
                           f"correct.")

        self.rknn_export = self.yaml_config["rknn_export"]

    def config_model(self):
        Config_config = None
        for node in self.rknn_export:
            if "Config" in node:
                Config_config = node["Config"]
        assert Config_config is not None, "rknn_export must have a node name Config"
        mean_values = [mean * 255 for mean in Config_config["mean_values"]]
        std_values = [std * 255 for std in Config_config["std_values"]]
        target_platform = Config_config["target_platform"]
        ret = self.rknn.config(mean_values=mean_values, std_values=std_values, target_platform=target_platform)
        assert ret == 0, "Config model failed!"

    def load_model(self):
        LoadONNX_config = None
        for node in self.rknn_export:
            if "LoadONNX" in node:
                LoadONNX_config = node["LoadONNX"]
        assert LoadONNX_config is not None, "rknn_export must have a node name LoadONNX"
        inputs = LoadONNX_config["inputs"]
        input_size_list = LoadONNX_config["input_size_list"]
        input_initial_val = LoadONNX_config["input_initial_val"]
        outputs = LoadONNX_config["outputs"]
        ret = self.rknn.load_onnx(model=self.onnx_model_path, inputs=inputs, input_size_list=input_size_list,
                                  input_initial_val=input_initial_val, outputs=outputs)
        assert ret == 0, "Load model failed!"

    def build_model(self):
        Build_config = None
        for node in self.rknn_export:
            if "Build" in node:
                Build_config = node["Build"]
        assert Build_config is not None, "rknn_export must have a node name Build"
        do_quantization = Build_config["do_quantization"]
        dataset = Build_config["dataset"]
        rknn_batch_size = Build_config["rknn_batch_size"]
        ret = self.rknn.build(do_quantization=do_quantization, dataset=dataset, rknn_batch_size=rknn_batch_size)
        assert ret == 0, "Build model failed!"

    def init_runtime(self):
        ret = self.rknn.init_runtime()
        assert ret == 0, "Init runtime environment failed!"

    def export_rknn(self):
        ret = self.rknn.export_rknn(self.rknn_model_path)
        assert ret == 0, "Export rknn model failed!"


if __name__ == "__main__":
    args = get_args()
    rknn_converter = RKNNConverter(args)
    rknn_converter.read_config()
    rknn_converter.config_model()
    rknn_converter.load_model()
    rknn_converter.build_model()
    rknn_converter.init_runtime()
    rknn_converter.export_rknn()
