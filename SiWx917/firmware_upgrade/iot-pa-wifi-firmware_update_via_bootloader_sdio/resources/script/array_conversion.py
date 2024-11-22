#Update the .rps format firmware file name here.
fileName = "out-of-box-demo-soc.rps"

with open(fileName, mode='rb') as file:
    fileContent = file.read()
    hex_data = [f"0x{byte:02x}," for byte in fileContent]

print('{')
print(" ".join(hex_data))
print('};')
