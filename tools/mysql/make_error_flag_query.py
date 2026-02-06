import re
import pathlib
import typing
def find_enum_name(file: typing.TextIO):
    pattern = re.compile(r"enum\s+class\s+ErrorType")
    while True:
        line = file.readline()
        if line == "":
            raise EOFError()
        if re.search(pattern, line):
            break

def make_query(enum_text):
    rows = []
    for line in enum_text:
        m = re.search(r'^\s*([A-Z0-9_]+)\s*=\s*(\d+)\s*,\s*$', line)
        if m:
            name, v = m.group(1), int(m.group(2))
            rows.append((v, name, v//64, v%64))

    print("INSERT INTO error_map(error_id,name,word_idx,bit_idx) VALUES")
    print(",\n".join([f"({v},'{n}',{w},{b})" for v,n,w,b in rows]) + ";")

def main():
    filename = pathlib.Path(__file__).parent/"../../onboard/source/core/include/ErrorManager.hh"
    print(filename)
    with open(filename) as f:
        find_enum_name(f)
        make_query(f.readlines())
    


if __name__ == "__main__":
    main()