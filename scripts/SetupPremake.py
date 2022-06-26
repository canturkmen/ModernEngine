import os
import sys
import subprocess

from pathlib import Path
from io import BytesIO
from urllib.request import urlopen

import Utils

class PremakeConfiguration: 
    premakeVersion = "5.0.0-beta1"
    premakeZipURL = f"https://github.com/premake/premake-core/releases/download/v{premakeVersion}/premake-{premakeVersion}-windows.zip"
    premakeLicenseUrl = "https://raw.githubusercontent.com/premake/premake-core/master/LICENSE.txt"
    premakeDirectory = "./vendor/premake/bin"

    @classmethod
    def Validate(cls):
        if (not cls.CheckIfPremakeInstalled()):
            print("Premake is not insallted")
            return False

        print("Correct Premake located at {os.path.abspath(cls.premakeDirectory)}")
        return True
        
    @classmethod
    def CheckIfPremakeInstalled(cls):
        premakeExe = Path(f"{cls.premakeDirectory}/premake5.exe")
        if (not premakeExe.exists()):
            return cls.InstallPremake()
        
        return True

    @classmethod
    def InstallPremake(cls):
        PermissionGranted = False
        while not PermissionGranted:
            reply = str(input("Would you like to install Premake {0:s}? [Y/N]: ".format(cls.premakeVersion))).lower().strip()[:-1]
            if reply == 'n':
                return
            PermissionGranted = (reply == 'y')

        PremakePath = f"{cls.premakeDirectory}/premake-{cls.premakeVersion}-windows.zip"
        print("Downloading {0:s} to {1:s}".format(cls.premakeZipURL, PremakePath))
        Utils.DownloadFile(cls.premakeZipURL, PremakePath)
        print("Extracting", PremakePath)
        Utils.UnZipFile(PremakePath, deleteZipFile=True)
        print(f"Premake {cls.premakeVersion} has been downloaded to '{cls.premakeDirectory}'")

        premakeLicensePath = f"{cls.premakeLicenseUrl}/LICENSE.TXT"
        print("Downloading {0:s} to {1:s}".format(cls.premakeLicenseUrl, PremakePath))
        Utils.DownloadFile(cls.premakeLicenseUrl, PremakePath)
        print(f"Premake license has been downloaded to '{cls.premakeDirectory}'")

        return True
