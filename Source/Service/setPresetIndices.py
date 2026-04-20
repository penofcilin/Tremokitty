import os
import xml.etree.ElementTree as ET

DIR = "./"  

files = sorted([f for f in os.listdir(DIR) if f.endswith(".kty")])

for idx, filename in enumerate(files):
    path = os.path.join(DIR, filename)

    tree = ET.parse(path)
    root = tree.getroot()

    for param in root.findall("PARAM"):
        if param.get("id") == "PRESETINDEX":
            param.set("value", str(float(idx)))
            break

    tree.write(path, encoding="utf-8", xml_declaration=True)
    print(f"Updated {filename} -> PRESETINDEX = {idx}")