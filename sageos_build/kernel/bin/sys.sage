# sys.sage — SageOS system bridge (non-class version)

proc args():
    return sys_args_builtin()

proc getenv(name):
    return _sys_getenv(name)

proc exec(cmd):
    return _sys_exec(cmd)
