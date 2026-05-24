# io.sage — SageOS I/O bridge (non-class version)

proc readfile(path):
    return _io_readfile(path)

proc writefile(path, content):
    return io_writefile(path, content)

proc exists(path):
    return io_exists(path)

proc appendfile(path, content):
    let old = ""
    if exists(path):
        old = readfile(path)
    return writefile(path, old + content)

proc isdir(path):
    # Dummy implementation if not supported by native
    return false

proc mkdir(path):
    # Dummy implementation if not supported by native
    return false
