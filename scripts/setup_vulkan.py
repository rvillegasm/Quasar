import os
import sys
import subprocess
from pathlib import Path

import utils
import misc


class VulkanSetup:
    required_vulkan_version = "1.2.176.1"

    vulkan_sdk_install_url = {
        "win32": f"https://sdk.lunarg.com/sdk/download/{required_vulkan_version}/windows/vulkan_sdk.exe",
        "linux": f"https://sdk.lunarg.com/sdk/download/{required_vulkan_version}/linux/vulkansdk-linux-x86_64-{required_vulkan_version}.tar.gz"
    }
    vulkan_sdk_exe_path = "./Quasar/vendor/VulkanSDK/VulkanSDK.exe"
    vulkan_sdk_tar_file = "vulkan_sdk.tar.gz"
    vulkan_sdk_linux_path = "~/vulkan"
    vulkan_sdk_setup_sh_path = f"{vulkan_sdk_linux_path}/{required_vulkan_version}/setup-env.sh"

    vulkan_sdk_debug_libs_url = f"https://sdk.lunarg.com/sdk/download/{required_vulkan_version}/windows/VulkanSDK-{required_vulkan_version}-DebugLibs.zip"
    vulkan_sdk_debug_libs_dir = "./Quasar/vendor/VulkanSDK"
    vulkan_sdk_debug_libs_zip = f"{vulkan_sdk_debug_libs_dir}/VulkanSDK-{required_vulkan_version}-DebugLibs.zip"

    @classmethod
    def validate(cls):
        if not cls._check_vulkan_sdk():
            print("Vulkan SDK not found!")
            return

        if sys.platform == "win32" and not cls._check_vulkan_sdk_debug_libs():
            print("Vulkan SDK debug libs not found!")
            return

    @classmethod
    def _check_vulkan_sdk(cls):
        vulkan_sdk = os.environ.get("VULKAN_SDK")
        if vulkan_sdk is None:
            print("Vulkan SDK is not currently installed!")
            cls._install_vulkan_sdk()
            return False
        else:
            print(f"Found vulkan sdk located at {vulkan_sdk}")

        if cls.required_vulkan_version not in vulkan_sdk:
            print(f"Vulkan SDK is not the right version! (Quasar requires version {cls.required_vulkan_version})")
            cls._install_vulkan_sdk()
            return False
        else:
            print(f"Vulkan sdk meets the required version. ({cls.required_vulkan_version})")
            return True

    @classmethod
    def _install_vulkan_sdk(cls):
        permission_granted = misc.yes_or_no(f"Would you like to install the vulkan sdk?")
        if permission_granted:
            if sys.platform == "win32":
                cls._install_vulkan_sdk_win32()
            elif sys.platform == "linux":
                cls._install_vulkan_sdk_linux()
            else:
                print("Dev Platform not supported!")
                quit()

    @classmethod
    def _install_vulkan_sdk_win32(cls):
        print(f"Downloading '{cls.vulkan_sdk_install_url['win32']}' to '{cls.vulkan_sdk_exe_path}'...")
        utils.download_file(cls.vulkan_sdk_install_url['win32'], cls.vulkan_sdk_exe_path)
        print("Running vulkan sdk installer...")
        os.startfile(os.path.abspath(cls.vulkan_sdk_exe_path))
        print("Re-run this script after installation!")
        quit()

    @classmethod
    def _install_vulkan_sdk_linux(cls):
        original_cwd = os.getcwd()
        print(f"Downloading '{cls.vulkan_sdk_install_url['linux']}' to '{cls.vulkan_sdk_linux_path}'")
        misc.create_dir_if_not_exists(cls.vulkan_sdk_linux_path)
        os.chdir(cls.vulkan_sdk_linux_path)
        utils.download_file(cls.vulkan_sdk_install_url['linux'], cls.vulkan_sdk_tar_file)
        print("Done!")
        print("Extracting Vulkan SDK...")
        subprocess.check_call(["tar", "xf", cls.vulkan_sdk_tar_file])
        print("Done!")
        print("Setting up runtime environment... (Modifying ~/.profile)")
        with open("../.profile", "w+") as profile_file:
            profile_file.write(f"source {cls.vulkan_sdk_setup_sh_path}")
        print("Done!")
        os.chdir(original_cwd)

    @classmethod
    def _check_vulkan_sdk_debug_libs(cls):
        shaderc_lib = Path(f"{cls.vulkan_sdk_debug_libs_dir}/Lib/shaderc_sharedd.lib")
        if not shaderc_lib.exists():
            print(f"No Vulkan SDK debug libs found. (Checked for existence of {shaderc_lib})")
            permission_granted = misc.yes_or_no(f"Would you like to install the vulkan sdk debug libs?")
            if permission_granted:
                print(f"Downloading '{cls.vulkan_sdk_debug_libs_url}' to '{cls.vulkan_sdk_debug_libs_zip}'...")
                utils.download_file(cls.vulkan_sdk_debug_libs_url, cls.vulkan_sdk_debug_libs_zip)
                print(f"Extracting '{cls.vulkan_sdk_debug_libs_zip}'...")
                utils.unzip_file(cls.vulkan_sdk_debug_libs_zip, delete_zip=True)
                print(f"Done! Vulkan sdk debug libs installed at '{os.path.abspath(cls.vulkan_sdk_debug_libs_dir)}'")
        else:
            print(f"Vulkan sdk debug libs located at '{os.path.abspath(cls.vulkan_sdk_debug_libs_dir)}'")

        return True


if __name__ == "__main__":
    VulkanSetup.validate()
