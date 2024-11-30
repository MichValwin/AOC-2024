# AOC2024

# Setup
## Libraries for Raylib linux

## Setup for vscode/codium
- Install cmake & cmaketools extensions
- Install clangD extension
    - This extensions needs a server file (you can download it by doing `ClangD: Download language server`)
    - It also needs to know where to search for sources. For a cmake project, with CMAKE_EXPORT_COMPILE_COMMANDS ON it will create a file named "compile_commands.json". You need to put this file into the main folder or tell clangD where it is.
