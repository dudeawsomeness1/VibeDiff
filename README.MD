# VibeDiff [WIP]

VibeDiff helps you compare and contrast things to find the best among them (whatever "best" means to you). And by "vibes", I mean human vibes; no AI was used in the making or conception of this project.
VibeDiff is built with ImGui, GLFW, GLEW, and OpenGL3 for cross-platform operation.

## How VibeDiff helps you compare and contrast

VibeDiff provides the following tools:

### Comparison Structures

* **Decision Matrices**: Compare things by scoring each thing based on a set of criteria. Define arbitrary weights for each criteria to define their relative impact on the final score. One score for each criterion, which are automatically weighted and summed for a final score on each thing.
* **Tournament Brackets**
  * *Single Elimination*: Your things are paired up and you decide which of each pair wins (manually or by scoring), and repeat until one remains.
  * *Double Elimination*: Same as single elimination, but with a second bracket for the losers of the first bracket. The ultimate winners of each bracket then face off to decide the overall winner.
  * *N-Elimination*: The same concept as double elimination, but extended to `N` brackets (`N-1` loser-brackets).

### Comparison functionality

* Common & scientific mathematical functions
  * Stuff you would find on a scientific calculator
* Variables and custom scoring-functions
* Graphing of quantitative and categorical data
* Ability to modify parameters and functions using a graph
* Score Balancer: Attempts to balance selected scores by adjusting given parameters

## Exports & Imports

* Import data from text or binary files
* Export data to text, binary, QR code, or image files
* Duplication of comparison structures and associated variables/functions

## Source Code & Permissions

Please consult the `license.txt` included in this project.

## Compile & Run

### Windows (Visual Studio 2022 cl.exe)

Replace `C:/VibeDiffProjectDir` with actual location of project files, and `C:/YourLibDir` with the actual location of you libraries.
Other paths like `C:/Windows Kits/10/Include/10.0.22621.0` and `C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.43.34808` may need to be changed for your system.

`& 'C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.43.34808/bin/Hostx64/x64/cl.exe' /O2 /GS /GL /W3 /Gy /Zc:wchar_t /sdl /Zc:inline /MD /EHsc /nologo /D _UNICODE /D UNICODE /std:c++20 /Fe: 'C:/VibeDiffProjectDir/VIBEDIFF.EXE' 'C:/VibeDiffProjectDir/WhatVibesWithU.cpp' 'C:/VibeDiffProjectDir/ImGui/imgui_demo.cpp' 'C:/VibeDiffProjectDir/ImGui/imgui_draw.cpp' 'C:/VibeDiffProjectDir/ImGui/imgui_impl_glfw.cpp' 'C:/VibeDiffProjectDir/ImGui/imgui_impl_opengl3.cpp' 'C:/VibeDiffProjectDir/ImGui/imgui_stdlib.cpp' 'C:/VibeDiffProjectDir/ImGui/imgui_tables.cpp' 'C:/VibeDiffProjectDir/ImGui/imgui_widgets.cpp' 'C:/VibeDiffProjectDir/ImGui/imgui.cpp' /I 'C:/Windows Kits/10/Include/10.0.22621.0/um' /I 'C:/Windows Kits/10/Include/10.0.22621.0/ucrt' /I 'C:/Windows Kits/10/Include/10.0.22621.0/shared' /I 'C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.43.34808/include' /I 'C:/VibeDiffProjectDir/ImGui' /I 'C:/YourLibDir/glfw-3.4.bin.WIN64/include' /I 'C:/YourLibDir/glew-2.1.0/include/GL' /link /NXCOMPAT /DYNAMICBASE /MACHINE:X64 /OPT:REF /OPT:ICF /NOLOGO /LIBPATH 'C:/YourLibDir/glew-2.1.0/lib/Release/x64/glew32s.lib' /LIBPATH 'C:/YourLibDir/glfw-3.4.bin.WIN64/lib-vc2022/glfw3dll.lib' /LIBPATH 'C:/Windows Kits/10/Lib/10.0.22621.0/um/x64/OpenGL32.Lib' /LIBPATH 'C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.43.34808/lib/x64/OLDNAMES.lib' /LIBPATH 'C:/Windows Kits/10/Lib/10.0.22621.0/um/x64/kernel32.lib' /LIBPATH 'C:/Windows Kits/10/Lib/10.0.22621.0/um/x64/shell32.lib' /LIBPATH 'C:/Windows Kits/10/Lib/10.0.22621.0/um/x64/Imm32.lib' /LIBPATH 'C:/Windows Kits/10/Lib/10.0.22621.0/um/x64/user32.lib' /LIBPATH 'C:/Windows Kits/10/Lib/10.0.22621.0/um/x64/uuid.lib' /LIBPATH 'C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.43.34808/lib/x64/msvcprt.lib' /LIBPATH 'C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.43.34808/lib/x64/MSVCRT.lib' /LIBPATH 'C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.43.34808/lib/x64/vcruntime.lib' /LIBPATH 'C:/Windows Kits/10/Lib/10.0.22621.0/ucrt/x64/ucrt.lib'; & 'C:/VibeDiffProjectDir/VIBEDIFF.EXE'`

### Linux (g++) [untested]

Note that the GLFW doesn't provide precompiled binaries for Linux.

`cd VibeDiffProjectDir`
`g++ -o vibediff.app WhatVibesWithU.cpp imgui_demo.cpp imgui_draw.cpp imgui_impl_glfw.cpp imgui_impl_opengl3.cpp imgui_stdlib.cpp imgui_tables.cpp imgui_widgets.cpp imgui.cpp -std=c++20 -O2 -Wall -Wextra -Wpedantic -pipe -g -I ImGui -I 'YourLibDir/glew-2.1.0/include/GL' -I 'YourLibDir/glfw-3.4-Linux64/include'`
