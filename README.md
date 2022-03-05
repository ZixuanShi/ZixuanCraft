# ZixuanCraft
Minecraft clone using Unreal Engine.

## Implemented Features
- Endless Terrain Generation
- AI can react to the player
    - Zombie
    - Chicken
- Adding Cube
- Removing Cube
- Inventory (In Progress)

## Target Platfroms
- Windows

## Input
- Keyboard & Mouse
- XBox Controller

## Controls
#### Keyboard & Mouse
| Action        | Keyboard & Mouse   | Status             |
|:-------------:| :-----------------:|:------------------:|
| Move          | WASD               | Key Press          |
| Jump          | Space bar          | Key Down           |
| Attack        | Left Mouse Button  | Button Down        |
| Sprint        | Left Shift Key     | Key Press          |
| Destroy Cube  | F Key              | Key Down           |
| Place Cube    | Right Mouse Button | Button Down        |
| Quit Game     | Escape             | Key Down           |

#### Controller
| Action        | Controller         | Status             |
|:-------------:|:------------------:|:------------------:|
| Move          | Left Thumb Stick   | Hold               |
| Jump          | Face Pad Bottom    | Button Down        |
| Attack        | Right Trigger      | Trigger Down       |
| Sprint        | Left Thumb Stick   | Thumb Stick Press  |
| Destroy Cube  | Right Trigger      | Trigger Down       |
| Place Cube    | Left Trigger       | Trigger Down       |
| Quit Game     | Menu Button        | Button Down        |

## Planned Feature Implementations
- Finish Inventory System
- Add mobile input & controls
- Add Android as target platform

## Known Issues
- There are some tree leaves in the air which don't attach to any tree trunks.
- The NavMesh is limited, if the player walk towards one direction for ~15 mins, the NPC won't be able to find a path.
- The player falls into meshes due to faulty collision with new placed cube
