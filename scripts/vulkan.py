import os
import subprocess
import sys
from pathlib import Path
from io import BytesIO
from urllib.request import Request, urlopen
from zipfile import ZipFile

from fake_useragent import UserAgent

import Utils

VULKAN_SDK = os.environ.get("VULKAN_SDK")

VULKAN_SDK_INSTALLER_URL = {
    "win32": "https://sdk.lunarg.com/sdk/download/1.2.176.1/windows/vulkan_sdk.exe",
    "linux": "https://sdk.lunarg.com/sdk/download/1.2.176.1/linux/vulkansdk-linux-x86_64-1.2.176.1.tar.gz"
}
QUASAR_VULKAN_VERSION = "1.2.176.1"

VULKAN_SDK_EXE_PATH = "Quasar/vendor/VulkanSDK/VulkanSDK.exe"
VULKAN_SDK_TAR_FILE = "vulkan_sdk.tar.gz"

VULKAN_SDK_LINUX_PATH = "~/vulkan"
VULKAN_SDK_SETUP_SH_PATH = f"{VULKAN_SDK_LINUX_PATH}/{QUASAR_VULKAN_VERSION}/setup-env.sh"

VULKAN_SDK_DEBUG_LIBS_URL = "https://sdk.lunarg.com/sdk/download/1.2.176.1/windows/VulkanSDK-1.2.176.1-DebugLibs.zip"
VULKAN_SDK_DEBUG_LIBS_OUTPUT_DIR = "Quasar/vendor/VulkanSDK"


def install_vulkan_sdk_win32():
    print(f"Downloading {VULKAN_SDK_INSTALLER_URL['win32']} to {VULKAN_SDK_EXE_PATH}")
    Utils.download_file(VULKAN_SDK_INSTALLER_URL['win32'], VULKAN_SDK_EXE_PATH)
    print("Done!")
    print("Running Vulkan SDK installer...")
    os.startfile(os.path.abspath(VULKAN_SDK_EXE_PATH))
    print("Re-run this script after installation")


def install_vulkan_sdk_linux():
    original_cwd = os.getcwd()
    print(f"Downloading {VULKAN_SDK_INSTALLER_URL['linux']} to ~/vulkan")
    Utils.create_dir_if_not_exists(VULKAN_SDK_LINUX_PATH)
    os.chdir(VULKAN_SDK_LINUX_PATH)
    Utils.download_file(VULKAN_SDK_INSTALLER_URL['linux'], VULKAN_SDK_TAR_FILE)
    print("Done!")
    print("Extracting Vulkan SDK...")
    subprocess.check_call(["tar", "xf", VULKAN_SDK_TAR_FILE])
    print("Done!")
    print("Setting up runtime environment... (Modifying ~/.profile)")
    with open("../.profile", "w+") as profile_file:
        profile_file.write(f"source {VULKAN_SDK_SETUP_SH_PATH}")
    print("Done!")
    os.chdir(original_cwd)


def install_vulkan_prompt():
    print("Install the Vulkan SDK?")
    install = Utils.yes_or_no()
    if install:
        if sys.platform == "win32":
            install_vulkan_sdk_win32()
        elif sys.platform == "linux":
            install_vulkan_sdk_linux()
        else:
            print("Platform not supported!")
        quit()


def check_vulkan_sdk():
    if VULKAN_SDK is None:
        print("You don't have the Vulkan SDK installed!")
        install_vulkan_prompt()
        return False
    elif QUASAR_VULKAN_VERSION not in VULKAN_SDK:
        print(f"Located Vulkan SDK at {VULKAN_SDK}")
        print(f"Vulkan SDK is not the right version (Quasar requires version {QUASAR_VULKAN_VERSION})")
        install_vulkan_prompt()
        return False
    else:
        print(f"Located correct Vulkan SDK at {VULKAN_SDK}")
        return True


def check_vulkan_sdk_debug_libs():
    shaderc_lib = Path(f"{VULKAN_SDK_DEBUG_LIBS_OUTPUT_DIR}/Lib/shaderc_sharedd.lib")
    if not shaderc_lib.exists():
        print(f"No Vulkan SDK debug libs found. (Checked for existence of {shaderc_lib})")
        print(f"Downloading {VULKAN_SDK_DEBUG_LIBS_URL}")
        ua = UserAgent()
        headers = {'User-Agent': ua.chrome}
        req = Request(VULKAN_SDK_DEBUG_LIBS_URL, headers=headers)
        with urlopen(req) as zip_response:
            with ZipFile(BytesIO(zip_response.read())) as zip_file:
                zip_file.extractall(VULKAN_SDK_DEBUG_LIBS_OUTPUT_DIR)

    print(f"Vulkan SDK debug libs located at {VULKAN_SDK_DEBUG_LIBS_OUTPUT_DIR}")
    return True
