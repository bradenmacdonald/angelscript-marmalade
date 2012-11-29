import os
import sys
import shutil

INCLUDES_BASE = "include/angelscript"
INCLUDES_ADDONS="include/angelscript/add_on" # We copy all the .h files for addons to this folder
ADDONS_SRC = "upstream/add_on"

if not os.path.exists("./angelscript.mkf"):
    sys.exit("Invalid working directory. Run from the project root.")

# Check that folders exist:
for folder in (INCLUDES_BASE, INCLUDES_ADDONS):
    if not os.path.exists(folder):
        os.mkdir(folder)

# Flat list of all addon header files:
header_files = [(path,file) for (path,_,files) in os.walk(ADDONS_SRC) for file in files if file[-2:]==".h"]
for (path,h) in header_files:
    shutil.copyfile(path+'/'+h, INCLUDES_ADDONS+'/'+h)
    #print(h)

shutil.copyfile("upstream/angelscript/include/angelscript.h", "include/angelscript.h")

print("Include files updated.")
