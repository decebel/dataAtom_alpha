import springboard as sb
import os

path = os.getcwdu()
mod = path + os.path.sep + "testPlugin_DataDrivers.py"

if __name__ == "__main__":
    print mod
    sb.load_plugin(mod)
    plugins = sb.create_output_datadrivers()
    for p in plugins:
        p.onData("testStream", "data")
    


