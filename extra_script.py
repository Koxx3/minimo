import requests, sys, json, os
from os.path import basename
from shutil import copyfile
import re
import time, datetime

Import("env")

try:
    import configparser
except ImportError:
    import ConfigParser as configparser
project_config = configparser.ConfigParser()
project_config.read("platformio.ini")
version = project_config.get("ota", "JsonVersion")
host = project_config.get("ota", "JsonHost")
port = project_config.get("ota", "JsonPort")
bin = project_config.get("ota", "JsonBin")


def myconverter(o):
    if isinstance(o, datetime.datetime):
        return o.__str__() # return "{}-{}-{}".format(o.year, o.month, o.day)

def after_build(source, target, env):
    print("----------------------------------------------------------------------------------------------------------")
    firmware_path = str(source[0])
    envName = env["PIOENV"]
    dstMainPath = env["PROJECT_DIR"] + "\\ota_updates\\" + envName + "\\"
    dstFilename = "firmware_v" + version + ".bin"
    finalDstFullPath = dstMainPath + dstFilename
    
    if False: #os.path.isfile(finalDstFullPath):
        print ("This firmware version already exists ... increase the firmware_version or delete the files if you want to generate a new version.")
    else:
        print("Copying bin file...")
        print(finalDstFullPath)

        os.makedirs(os.path.dirname(dstMainPath), exist_ok=True)
        copyfile(firmware_path , finalDstFullPath)

        modified = os.path.getmtime(finalDstFullPath)
        dateFile = datetime.datetime.fromtimestamp(modified)

        # Generating JSON version file
        print("Generating JSON version file...")
        payload = {}
        payload['type'] = envName
        payload['version'] = version
        payload['host'] = host
        payload['port'] = port
        payload['bin'] = bin + envName + "/" + dstFilename

        try:
            with open(dstMainPath + "firmware_v" + version + ".json", 'w') as outfile:
                json.dump(payload, outfile, default = myconverter)
        except:
            print("[ERROR] Error when generating the JSON version file...")

        fullPayload = {}
        fullPayload['versions'] = []
        # r=root, d=directories, f = files
        for r, d, f in os.walk(dstMainPath):
            for file in f:
                if file.endswith(".bin"):
                    filePath = os.path.join(r, file)
                    print(filePath)
                    fileName = os.path.basename(filePath)
                    #modificationDate = os.path.getmtime(filePath)
                    # remove file extension
                    versionNumber = os.path.splitext(fileName)[0]
                    # split with 'v"
                    versionNumber = re.split('v', versionNumber)[1]
                    
                    versionElement = {}
                    versionElement['version'] = versionNumber
                    #modificationDateStr = datetime.datetime.fromtimestamp(modificationDate)
                    #year,month,day,hour,minute,second=time.localtime(modificationDate)[:-3]
                    versionElement['date'] = os.stat(filePath)[-2]
                    fullPayload['versions'].append(versionElement)

        try:
            with open(dstMainPath + "firmware.json", 'w') as outfile:
                json.dump(fullPayload, outfile, default = myconverter)
        except:
            print("[ERROR] Error when generating the JSON version file...")

    print("----------------------------------------------------------------------------------------------------------")


env.AddPostAction("buildprog", after_build)
env.AddPreAction("upload", after_build)
#env.Replace(PROGNAME="firmware_v%s" % version)