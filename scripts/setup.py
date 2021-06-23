import os
import sys

import check_python


# verify that every package is installed
check_python.validate_packages()

import vulkan

# change active dir to root
os.chdir("../")

if not vulkan.check_vulkan_sdk():
    print("Vulkan sdk not installed")

if sys.platform == "win32":
    if not vulkan.check_vulkan_sdk_debug_libs():
        print("Vulkan sdk debug libs not found")
