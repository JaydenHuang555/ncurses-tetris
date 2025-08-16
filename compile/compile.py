import subprocess
import sys
import threading


def _reader(stream, prefix):
    for line in iter(stream.readline, ""):
        if line:
            print(f"{prefix}: {line.strip()}")
            sys.stdout.flush()
    stream.close()


def OpenProcess(pname):
    with subprocess.Popen(
        pname, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, bufsize=1
    ) as process:
        # Start threads to read stdout and stderr
        threads = [
            threading.Thread(target=_reader, args=(process.stdout, "STDOUT")),
            threading.Thread(target=_reader, args=(process.stderr, "STDERR")),
        ]
        for t in threads:
            t.daemon = True
            t.start()

        # Wait for process to complete
        process.wait()

        # Ensure all threads finish
        for t in threads:
            t.join()


def GetStdout(command):
    try:
        return subprocess.run(
            command, capture_output=True, text=True, check=True
        ).stdout
    except subprocess.CalledProcessError:
        raise ValueError("unable to run command", command)


def ParseElements(fname):
    try:
        with open(fname) as stream:
            return [
                element.strip()
                for element in stream.read().split("\n")
                if element != "" and element[0] != "#"
            ]
    except FileNotFoundError:
        raise ValueError("unable to open file", fname)


class Constants:
    def __init__(self):
        self.c_files = GetStdout(["find", ".", "-type", "f", "-name", "*.c"]).split(
            "\n"
        )
        self.c_files.pop(len(self.c_files) - 1)
        self.cc = "cc"
        self.pwd = GetStdout(["pwd"]).removesuffix("\n")
        self.includes = ParseElements("includes")
        self.includes.append(self.pwd)
        self.args = ParseElements("args")
        self.name_arg = "-o"
        self.name = "tetris"
        self.libs = ParseElements("libs")


def PrefixStr(pre, buffer):
    return pre + buffer


def CompileRelease(constants: Constants):
    print("COMPILING RELEASE BUILD")
    compile_command = []
    compile_command.append(constants.cc)
    [compile_command.append(arg) for arg in constants.args]
    [compile_command.append("-I" + dir) for dir in constants.includes]
    [compile_command.append(file) for file in constants.c_files]
    [compile_command.append("-l" + lib) for lib in constants.libs]
    compile_command.append(constants.name_arg)
    compile_command.append(constants.name)
    OpenProcess(compile_command)
    print("FINISHED COMPILING RELEASE BUILD")


def Compile(constants: Constants):
    print("STARTING COMPILING")
    CompileRelease(constants)


if __name__ == "__main__":
    constants = Constants()
    print("c files:", constants.c_files)
    print("pwd:", constants.pwd)
    print("includes:", constants.includes)
    print("args:", constants.args)
    print("libs:", constants.libs)
    Compile(constants)
