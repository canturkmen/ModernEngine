import os
import sys
import subprocess

from pathlib import Path
from io import BytesIO
from urllib.request import urlopen

import Utils

class VulkanConfiguration: 
    RequiredVulkanVersion = "1.3."
    InstallVulkanVersion = "1.3.216.0"
    VulkanDirectory = "./ModernEngine/vendor/VulkanSDK"

    @classmethod
    def Validate(cls):
        if (not cls.CheckVulkanSDK()):
            print("Vulkan SDK is not insallted")
            return

        if (not cls.CheckVulkanSDKDebugLibs()):
            print("\nNo Vulkan SDK Debug Libs were found. Install Vulkan SDK with Debug Libs.")

    @classmethod
    def CheckVulkanSDK(cls):
        vulkanSDK = os.environ.get("VULKAN_SDK")
        if(vulkanSDK is None):
            print("\nYou don't have the Vulkan SDK installed!")
            cls.__InstallVulkanSDK()
            return False
        else:
            print(f"\nLocated Vulkan SDK at {vulkanSDK}")

        if (cls.RequiredVulkanVersion not in vulkanSDK):
            print(f"You don't have the correct Vulkan SDK version installed! Modern Engine requires {cls.RequiredVulkanVersion}")
            cls.__InstallVulkanSDK()
            return False

        print(f"Correct Vulkan SDK located at {vulkanSDK}")
        return True

    @classmethod
    def __InstallVulkanSDK(cls):
        PermissionGranted = False
        while not PermissionGranted:
            reply = str(input("Would you like to install VulkanSDK {0:s}? [Y/N]: ".format(cls.installVulkanVersion))).lower().strip()[:1]
            if reply == 'n':
                return
            PermissionGranted = (reply == 'y')

        VulkanInstallURL = f"https://sdk.lunarg.com/sdk/download/{cls.InstallVulkanVersion}/windows/VulkanSDK-{cls.InstallVulkanVersion}-Installer.exe"
        VulkanPath = f"{cls.VulkanDirectory}/VulkanSDK-{cls.InstallVulkanVersion}-Installer.exe"
        print("Downloading {0:s} to {1:s}".format(VulkanInstallURL, VulkanPath))
        Utils.DownloadFile(VulkanInstallURL, VulkanPath)
        print("Running Vulkan SDK installer...")
        os.startfile(os.path.abspath(VulkanPath))
        print("Re-run this script after installation!")
        quit()

    @classmethod
    def CheckVulkanSDKDebugLibs(cls):
        VulkanSDK = os.environ.get("VULKAN_SDK")
        shadercdLib = Path(f"{VulkanSDK}/Lib/shaderc_sharedd.lib")

        return shadercdLib.exists()

if __name__ == "__main__":
    VulkanConfiguration.Validate()
