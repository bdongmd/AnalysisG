from pwinput import pwinput
import subprocess

def _getcmd(cmd):
    return subprocess.check_output(cmd, stderr=subprocess.STDOUT, shell=True).decode("UTF-8")

try: from .core.analysis import Analysis
except Exception as error: print("failed to load Analysis!: " + str(error))

