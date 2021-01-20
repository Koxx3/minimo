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
    print("after_build !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
    firmware_path = str(source[0])
    print(firmware_path)
    firmware_name = basename(firmware_path)
    #print(env.Dump())
    
    
    envName = env["PIOENV"]
    print(envName)
    dstMainPath = env["PROJECT_DIR"] + "\\ota_updates\\" + envName + "\\"
    dstFilename = "firmware_v" + version + ".bin"
    print(dstMainPath)

    os.makedirs(os.path.dirname(dstMainPath), exist_ok=True)
    copyfile(firmware_path , dstMainPath + dstFilename)

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


env.AddPostAction("buildprog", after_build)
env.AddPreAction("upload", after_build)
#env.Replace(PROGNAME="firmware_v%s" % version)