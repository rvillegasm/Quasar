import sys
import subprocess
import importlib.util as importlib_util

import misc


class PythonSetup:
    required_packages = ["requests"]

    @classmethod
    def validate(cls):
        if not cls._validate_python():
            return

        for package_name in cls.required_packages:
            if not cls._validate_package(package_name):
                return

    @classmethod
    def _validate_python(cls, version_mayor=3, version_minor=3):
        if sys.version is not None:
            print(f"Python version {sys.version_info.major:d}.{sys.version_info.minor:d}.{sys.version_info.micro:d}")
            if sys.version_info.major < version_mayor or (sys.version_info.major == version_mayor and sys.version_info.minor < version_minor):
                print(f"Python version does not meet requirements. Expected version {version_mayor}.{version_minor} or higher")
                return False
            return True

    @classmethod
    def _validate_package(cls, package_name):
        if importlib_util.find_spec(package_name) is None:
            return cls._install_package(package_name)
        else:
            return True

    @classmethod
    def _install_package(cls, package_name):
        permission_granted = misc.yes_or_no(f"Would you like to install Python package '{package_name:s}'?")
        if permission_granted:
            print(f"Installing {package_name} module...")
            subprocess.check_call(["python", "-m", "pip", "install", package_name])
            return cls._validate_package(package_name)


if __name__ == "__main__":
    PythonSetup.validate()
