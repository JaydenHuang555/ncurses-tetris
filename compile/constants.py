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
