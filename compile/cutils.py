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
