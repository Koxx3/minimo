# Copyright (c) 2014-present PlatformIO <contact@platformio.org>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import requests
import sys
from os.path import basename

Import('env')

# from platformio import util
# project_config = util.load_project_config()
# bintray_config = {k: v for k, v in project_config.items("bintray")}
# version = project_config.get("common", "release_version")

try:
    import configparser
except ImportError:
    import ConfigParser as configparser
project_config = configparser.ConfigParser()
project_config.read("platformio.ini")
version = project_config.get("common", "release_version")
#bintray_config = {k: v for k, v in project_config.items("bintray")}

#
# Push new firmware to the Bintray storage using API
#


def publish_firmware(source, target, env):
    firmware_path = str(source[0])
    firmware_name = basename(firmware_path)

    print("Uploading {0} to Bintray. Version: {1}".format(
        firmware_name, version))


# Custom upload command and program name
env.Replace(PROGNAME="firmware_v_%s" % version, UPLOADCMD=publish_firmware)