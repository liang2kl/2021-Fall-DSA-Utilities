import re, sys, os

"""
Match: #include "xxx.h"
Discard: #include "xxx.h" /* EXCLUDE */
"""
INCLUDE_PATTERN = re.compile(r'#include\s*"(.*?\.h)"(?!(\s*\/\*\s*EXCLUDE\s*\*\/))', flags=re.IGNORECASE)
"""
Match:
/**
 <any content>
 */
"""
DOC_COMMENT_PATTERN = re.compile(r'^\s*?\/\*\*(.|\n)*?\*\/', flags=re.MULTILINE)
"""
Match:
(#ifndef <any> _OJ_ | #if !defined(_OJ_) | #ifdef DEBUG)
<any>
#endif
"""
NON_OJ_DIRECTIVE_PATTERN = re.compile(r'(#ifndef.*?_OJ_|#if .*?\!defined\(_OJ_\)|#ifdef.*?DEBUG)(.|\n)*?#endif\s*?\n?')
EMPTY_LINE_PATTERN = re.compile(r'^\s*\n', flags=re.MULTILINE)

included_files = []

def dir_path(path):
    return os.path.dirname(os.path.abspath(path))

def file_content(match: re.Match, base_dir: str) -> str:
    hdr_path: str = base_dir + "/" + match.group(1)
    components = hdr_path.split("/")

    while True:
        flag = False
        for index, component in enumerate(components):
            if component == "..":
                components = components[:index - 1] + components[index + 1:]
                break
            if (index == len(components) - 1):
                flag = True
        if (flag):
            hdr_path = "/".join(components)
            break
    
    if hdr_path in included_files:
        return ""

    included_files.append(hdr_path)

    content = ""

    with open(hdr_path) as file:
        content = file.read()

    # Substitute files recursively
    content = INCLUDE_PATTERN.sub(lambda m: file_content(m, dir_path(hdr_path)), content)
        
    # Remove all docs
    content = DOC_COMMENT_PATTERN.sub("", content)

    # Remove empty lines
    content = EMPTY_LINE_PATTERN.sub("", content)
    
    return f"/* BEGIN AUTO INSERTED \"{match.group(1)}\" */\n" + \
        content + f"\n/* END AUTO INSERTED \"{match.group(1)}\" */\n"

if __name__ == "__main__":
    """
    Create a submit version of the source file.
    """

    if len(sys.argv) <= 1:
        print("No source file provided")
        exit(1)
    
    path = sys.argv[1]

    content = ""

    with open(path) as file:
        content = file.read()

    # Substitute header files
    sub = INCLUDE_PATTERN.subn(lambda m: file_content(m, dir_path(path)), content)

    if sub[1]:
        content = sub[0]

    # Remove non-OJ sections
    content = NON_OJ_DIRECTIVE_PATTERN.sub("", content)

    # Write to the file to be submitted
    with open(path[:-4] + "_submit.cpp", "w") as file:
        file.write(content)
