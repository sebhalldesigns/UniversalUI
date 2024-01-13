
import sys
import os

print(os.getcwd())
sys.path.append('universalui/universalui_api/bindings/python/')




from uuiPy.UniversalUI import UniversalUI

def main():
    uui = UniversalUI('universalui/target/debug/universalui.dll')
    uui.CreateWindow("my window", 800, 600)



if __name__ == "__main__":
    main()