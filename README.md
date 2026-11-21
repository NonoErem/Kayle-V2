## Kayle V2

A C++ 2D RPG and map editor built with SFML.

## Features

- Tile-based 2D maps
- Player and entity management
- Map loading and saving
- Editable tiles, entities, and effects
- Sprite and texture management
- Custom editor interface
- Menu and game states
- Keyboard and mouse input handling
- Windows-specific integration

## Project Structure

```text
Kayle/
├── main.cpp                 # Application entry point
├── game.cpp/hpp             # Main game logic
├── map.cpp/hpp              # Map loading and rendering
├── player.cpp/hpp           # Player behavior
├── entity.cpp/hpp           # Entity definitions
├── editor_interface.cpp/hpp # Map editor interface
├── sprites_manager.cpp/hpp  # Sprite management
├── textures.cpp/hpp         # Texture definitions
├── inputHandler.cpp/hpp     # Input handling
├── Textures/                # Image resources
├── *.txt                    # Maps and object sets
└── *.dll                    # Windows runtime dependencies
External/SFML/               # SFML dependency files
Images/                      # Project images
Kayle V2.sln                 # Visual Studio solution
```

## Requirements

- Windows
- Visual Studio with C++ support
- SFML 2
- OpenAL, depending on your configuration

The required SFML DLL files are included in the `Kayle` directory.

## Building

1. Clone the repository:

```bash
git clone https://github.com/NonoErem/Kayle-V2.git
cd Kayle-V2
```

2. Open `Kayle V2.sln` in Visual Studio.

3. Select the desired build configuration, such as **Debug** or **Release**.

4. Build the solution with **Build → Build Solution**.

5. Run the project from Visual Studio or execute the generated executable.

## Editing Maps

Map and object data are stored in text files inside `Kayle`, including:

- `map.txt`
- `map 2.txt`
- `tileSet.txt`
- `entitySet.txt`
- `effectSet.txt`

The included DOCX files contain additional notes and documentation about the editor interface and available features.

## Language

- C++: 100%

## Repository

[NonoErem/Kayle-V2](https://github.com/NonoErem/Kayle-V2)
