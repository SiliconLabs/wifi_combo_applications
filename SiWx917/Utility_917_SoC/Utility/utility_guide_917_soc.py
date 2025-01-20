import os
import sys
import time

def secure_device(device):
    print("You chose to SECURE the DEVICE!") 
    print("")
    # Check if commanderkeys.json exists and delete it
    if os.path.exists("commanderkeys.json"):
        os.remove("commanderkeys.json")
        print("Deleted existing commanderkeys.json")

    # Define the commands to be executed
    commands = [
        f"commander manufacturing provision --mbr default -d {device}",
        f"commander manufacturing init --mbr default -d {device}",
        f"commander util genkeyconfig --outfile commanderkeys.json -d {device}",
        f"commander manufacturing provision --keys commanderkeys.json -d {device}",
        f"commander manufacturing provision --mbr default --data updatedmbrfields.json -d {device}"
    ]

    # Execute each command and check the return code
    for command in commands:
        result = os.system(command)
        if result != 0:
            print(f"Command failed: {command}")
            sys.exit(1)
        print(f"Output of {command}:\n{result}")
        
        # Add sleep and wait for user input after specific commands
        if command == f"commander manufacturing init --mbr default -d {device}":
            time.sleep(10)
            print(f" ")
            print(f" --------------------- POWER CYCLE the DEVICE --------------------- ")
            print(f" ")
            print(f" Make sure the device is completely booted after the Power Cycle ")
            print(f" ")
            print(f" ------------------------------------------------------------------ ")
            print(f" ")
            input_string = input("Please type 'Yes' (case sensitive) if Power cycle of the device is done:- ")

            # Compare the input string to the hard-coded string
            if input_string != "Yes":
                sys.exit(1)
            else:
                print(f"Power cycle of the device DONE")
                
        if command == f"commander util genkeyconfig --outfile commanderkeys.json -d {device}":
            time.sleep(10)
        if command == f"commander manufacturing provision --keys commanderkeys.json -d {device}":
            time.sleep(10)
        if command == f"commander manufacturing provision --mbr default --data updatedmbrfields.json -d {device}":
            time.sleep(10)         
  
def load_secured_NWP_firmware(device):
    print("You chose to LOAD SECURED NWP FIRMWARE!")
    print("")
    # Check if commanderkeys.json exists and delete it
    if os.path.exists("secured_nwp.rps"):
        os.remove("secured_nwp.rps")
        print("Deleted existing secured_nwp.rps")
    time.sleep(5)
   
    NWP = input(" Enter NWP rps file name: ")
    print("")   

    # Define the commands to be executed
    commands = [
       # f"commander manufacturing read taipmu --out filename.bin -d {device}",
        f"commander rps convert secured_nwp.rps --taapp {NWP} --mic commanderkeys.json --encrypt commanderkeys.json --sign commanderkeys.json",
        f"commander rps load secured_nwp.rps -d {device}"
    ]
    
    # Execute each command and check the return code
    for command in commands:
        result = os.system(command)
        if result != 0:
            print(f"Command failed: {command}")
            sys.exit(1)
        print(f"Output of {command}:\n{result}")

        if command == f"commander rps convert secured_nwp.rps --taapp {NWP} --mic commanderkeys.json --encrypt commanderkeys.json --sign commanderkeys.json":
            time.sleep(10)
        if command == f"commander rps load secured_nwp.rps -d {device}":
            time.sleep(10)
      

def load_secured_M4_firmware(device):
    print("You chose to LOAD SECURED M4 FIRMWARE!")
    print("")
    # Check if secured_M4.rps exists and delete it
    if os.path.exists("secured_M4.rps"):
        os.remove("secured_M4.rps")
        print("Deleted existing secured_M4.rps")
    time.sleep(5)
 
    M4 = input(" Enter M4 rps file name: ")
    print("") 
    # Define the commands to be executed
    commands = [
       # f"commander manufacturing read taipmu --out filename.bin -d {device}",
        f"commander rps convert secured_M4.rps --app {M4} --mic commanderkeys.json --encrypt commanderkeys.json --sign commanderkeys.json",
        f"commander rps load secured_M4.rps -d {device}",
        f"commander device reset -d {device}"
    ]
    
    # Execute each command and check the return code
    for command in commands:
        result = os.system(command)
        if result != 0:
            print(f"Command failed: {command}")
            sys.exit(1)
        print(f"Output of {command}:\n{result}")

        if command == f"commander rps convert secured_M4.rps --app {M4} --mic commanderkeys.json --encrypt commanderkeys.json --sign commanderkeys.json":
            time.sleep(10)
        if command == f"commander rps load secured_M4.rps -d {device}":
            time.sleep(10)         
      

def non_secure_device(device):
    print("You chose to NON SECURE the DEVICE")
    print("")
    # Define the commands to be executed
    commands = [
    f"commander manufacturing provision --mbr default -d {device}"
    ]
    # Execute each command and check the return code
    for command in commands:
        result = os.system(command)
        if result != 0:
            print(f"Command failed: {command}")
            sys.exit(1)
        print(f"Output of {command}:\n{result}")
     
def load_non_secured_NWP_firmware(device):
    print("You chose to LOAD NON-SECURED M4 FIRMWARE!")
    print("")

    NWP = input(" Enter NWP rps file name: ")
    print("") 
    # Define the commands to be executed
    commands = [
       # f"commander manufacturing read taipmu --out filename.bin -d {device}",
        f"commander rps load {NWP} -d {device}",
    ]
    # Execute each command and check the return code
    for command in commands:
        result = os.system(command)
        if result != 0:
            print(f"Command failed: {command}")
            sys.exit(1)
        print(f"Output of {command}:\n{result}")

def load_non_secured_M4_firmware(device):
    print("You chose to LOAD NON-SECURED M4 FIRMWARE!")
    print("")

    M4 = input(" Enter M4 rps file name: ")
    print("") 
    # Define the commands to be executed
    commands = [
       # f"commander manufacturing read taipmu --out filename.bin -d {device}",
        f"commander rps load {M4} -d {device}",
    ]
    # Execute each command and check the return code
    for command in commands:
        result = os.system(command)
        if result != 0:
            print(f"Command failed: {command}")
            sys.exit(1)
        print(f"Output of {command}:\n{result}")


def erase_flash(device):
    print("You chose to Erase the flash")
    print("")
    commands = [ f"commander device masserase -d {device}"]
    # Execute each command and check the return code
    for commands in commands:
        result = os.system(commands)
        if result != 0:
            print(f"Command failed: {commands}")
            sys.exit(1)
        print(f"Output of {commands}:\n{result}")
       
def main(device):
    while True:
        # Hard-coded strings
        print("Select the below options:")
        print("1. Secure the device")
        print("2. Load the NWP Secured Firmware")
        print("3. Load the M4 Secured Firmware")
        print("4. Non-Secure the device")
        print("5. Load the NWP Non-Secured Firmware")
        print("6. Load the M4 Non-Secured Firmware")
        print("7. Erase the Flash: (Switch to ISP mode if any powersave example is running)")
        print("8. Exit ")
        print("")

        # Get input from the user
        input_string = input("Enter the option: ")
        print("")

        # Call the appropriate section based on the input
        if input_string == "1":
            print("1. Make sure to *erase the flash* before Securing the device.") 
            print(" ")
            print("2. Check in the *Device Info* of Simplicity Commander Tool, if the device is *connected*.")
            print(" ")
            input_string = input("Enter 'y' if you want to continue:  ")
            # Compare the input string to the hard-coded string
            if input_string != "y":
                sys.exit(1)
            else:            
                secure_device(device)
        elif input_string == "2":
            load_secured_NWP_firmware(device)
        elif input_string == "3":
            load_secured_M4_firmware(device)
        elif input_string == "4":
            print("1. Make sure to *erase the flash* before Securing the device.") 
            print(" ")
            print("2. Check in the *Device Info* of Simplicity Commander Tool, if the device is *connected*.")
            print(" ")
            input_string = input("Enter 'y' if you want to continue:  ")
            # Compare the input string to the hard-coded string
            if input_string != "y":
                sys.exit(1)
            else:
                non_secure_device(device)
        elif input_string == "5":
            load_non_secured_NWP_firmware(device)
        elif input_string == "6":
            load_non_secured_M4_firmware(device)
        elif input_string == "7":
            erase_flash(device)
        elif input_string == "8":
            print(f" Exiting ...... ")
            sys.exit(1)
        else:
            print("Invalid input. Please select one number from the given options")
            print("")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print(" ")
        print("Usage: python utility_guide_917_soc.py <OPN>")
        print(" ")
        print("Example: python utility_guide_917_soc.py SiWG917M111MGTBA")
        print(" ")
        sys.exit(1)
    
    device = sys.argv[1]
    main(device)