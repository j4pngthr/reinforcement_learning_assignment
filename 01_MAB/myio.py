
# Write the data in a list to a file
def write_data(param, seed, data, file_name):
    # Open a file
    # fout = open(file_name, "a") # 追記
    fout = open(file_name, "w") # 上書き

    # Write data
    s = "\n"
    # fout.write(param + " " + str(seed) + s) # 1つ目の前
    fout.write(s.join(map(str, data)))
    fout.write("\n")
    fout.close()
