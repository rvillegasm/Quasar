import os

from setup_python import PythonSetup
from setup_vulkan import VulkanSetup


# verify that the python environment is setup correctly
PythonSetup.validate()

# change active dir to root
os.chdir("../")

VulkanSetup.validate()

print("\nSetup Complete!")
