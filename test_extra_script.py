import argparse
import ("env", "projenv")



def main():
    print(env.Dump())

    parser = argparse.ArgumentParser()
    parser.add_argument("--model", "-m", help="-m --model : Choose a model of scooter")
    parser.add_argument("--version", "-v", help="-v --version : Choose a firmware version")
    args = parser.parse_args()


    if args.model:
        print(args.model)
    else:
        print("[ERROR] Please choose a model ! With -m or --model xxxxxxx")
        exit(-1)

    if args.version:
        print(args.version)
    else:
        print("[ERROR] Please choose a version for the firmware ! With -v or --version xxxxxxx")
        exit(-1)




if __name__ == "__main__":
    # Execute only if run as a script

    print(env)
    # access to project construction environment
    print(projenv)
    main()
    exit(0)