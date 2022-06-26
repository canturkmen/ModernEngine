import os
from re import sub
import subprocess
import platform

from SetupPython import PythonConfiguration as PythonRequirements

PythonRequirements.Validate()

from SetupPremake import PremakeConfiguration as PremakeRequirements
from SetupVulkan import VulkanConfiguration as VulkanRequirements
os.chdir('./../')

premakeInstalled = PremakeRequirements.Validate()
VulkanRequirements.Validate()

print("\nUpdating submodules...")
subprocess.call(["git", "submodule", "update", "--init", "--recursive"])

if (premakeInstalled):
    if platform.system() == "Windows":
        print("\nRunning Premake...")
        subprocess.call([os.path.abspath("./scripts/Win-ProjectGenerator.bat"), "nopause"])
        
    print("\nSetup completed!")
else:
    print("You need to install the premake to use Modern Engine")