import requests, sys, json, os
from os.path import basename
from shutil import copyfile

Import("env")

try:
    import configparser
except ImportError:
    import ConfigParser as configparser
project_config = configparser.ConfigParser()
project_config.read("platformio.ini")
version = project_config.get("common", "JsonVersion")
host = project_config.get("common", "JsonHost")
port = project_config.get("common", "JsonPort")
bin = project_config.get("common", "JsonBin")


def after_build(source, target, env):
    print("----------------------------------------------------------------------------------------------------------")
    firmware_path = str(source[0])
    envName = env["PIOENV"]
    dstMainPath = env["PROJECT_DIR"] + "\\ota_updates\\" + envName + "\\"
    dstFilename = "firmware_v" + version + ".bin"
    finalDstFullPath = dstMainPath + dstFilename
    
    if os.path.isfile(finalDstFullPath):
        print ("This firmware version already exists ... increase the firmware_version or delete the files if you want to generate a new version.")
    else:
        print("Copying bin file...")
        print(finalDstFullPath)

        os.makedirs(os.path.dirname(dstMainPath), exist_ok=True)
        copyfile(firmware_path , finalDstFullPath)

        # Generating JSON version file
        print("Generating JSON version file...")
        payload = {}
        payload['type'] = envName
        payload['version'] = version
        payload['host'] = host
        payload['port'] = port
        payload['bin'] = bin + envName + "/" + dstFilename

        try:
            with open(dstMainPath + "firmware.json", 'w') as outfile:
                json.dump(payload, outfile)
        except:
            print("[ERROR] Error when generating the JSON version file...")

    print("----------------------------------------------------------------------------------------------------------")


env.AddPostAction("buildprog", after_build)
env.AddPreAction("upload", after_build)
#env.Replace(PROGNAME="firmware_v%s" % version)