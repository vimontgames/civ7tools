civ7map
-----

An unofficial map editor for Civilization VII.

![Screenshot](doc/img/v0.42.png)

# How to use?

## Mouse

| Key                | Description |
|--------------------|-------------|
| Left mouse button  | Paint       |
| Right mouse button | Pan camera  |
| Mouse wheel        | Zoom        |

## Keyboard

| Key                | Description |
|--------------------|-------------|
| Shift (hold)       | Select area |
| Space              | Select plot |
| Ctrl-Z             | Undo        |
| Ctrl-Y             | Redo        |

# Build
If you want to compile the tool yourself rather than using a released version, open civ7tools.sln using Visual Studio 2022 (free version can be downloaded [here](https://visualstudio.microsoft.com/downloads/)), selection $(SolutionDir) as the working directory, then build solution.

# Dependencies


* [SFML](https://github.com/SFML/SFML) >= 2.5.1 (*)
* [Dear ImGui](https://github.com/ocornut/imgui) >= 1.80
* [Font-Awesome]( https://github.com/FortAwesome/Font-Awesome/tree/6.x)
* [ImGui-SFML](https://github.com/eliasdaler/imgui-sfml) >= 2.3+
* [ImGui-Addons](https://github.com/wflohry/imgui-addons)
* [IconFont_CppHeaders](https://github.com/juliettef/IconFontCppHeaders)
* [miniz-cpp](https://github.com/tfussell/miniz-cpp)
* [TinyXML-2](https://github.com/leethomason/tinyxml2)
* [Base64](https://github.com/terrakuh/base64)
* [stb](https://github.com/nothings/stb)
* [magic__enum](https://github.com/Neargye/magic_enum)
* [fmt](https://github.com/fmtlib/fmt/)

(*) All dependencies but the SFML have been copied to this repo, you will need to set an environment variable named $(SFML) pointing to your SFML install in order to compile.

e.g. **SFML=G:\SFML\SFML-2.5.1**
