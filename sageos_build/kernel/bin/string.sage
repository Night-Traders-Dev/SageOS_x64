# string.sage — SageOS string bridge (non-class version)
import strings

proc substr(s, start, length):
    return strings.string_substr(s, start, length)

proc contains(s, pattern):
    return strings.str_contains(s, pattern)

proc find(s, pattern):
    # Minimal implementation of find
    for i in range(len(s) - len(pattern) + 1):
        if strings.string_substr(s, i, len(pattern)) == pattern:
            return i
    return -1
