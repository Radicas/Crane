import os
import subprocess

# Function to find all .h and .cpp files in the given directory and its subdirectories
def find_source_files(directory):
    source_files = []
    for root, _, files in os.walk(directory):
        for file in files:
            if file.endswith(".h") or file.endswith(".cpp") or file.endswith(".hpp"):
                source_files.append(os.path.join(root, file))
    return source_files

# Format each file using clang-format
def format_files(files):
    for file in files:
        subprocess.run(["clang-format", "-i", file])
        print(f"Formatted: {file}")

if __name__ == "__main__":
    current_directory = os.getcwd()
    source_files = find_source_files(current_directory)
    
    if source_files:
        format_files(source_files)
        print("Formatting complete.")
    else:
        print("No .h or .cpp files found.")
