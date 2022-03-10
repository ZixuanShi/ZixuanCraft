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
- Android

## Input
- Keyboard & Mouse
- XBox Controller
- Mobile Touches

## Controls
#### Keyboard & Mouse
| Action        | Keyboard & Mouse   | Status             |
|:-------------:| :-----------------:|:------------------:|
| Move          | WASD               | Hold               |
| Jump          | Space bar          | Press              |
| Attack        | Left Mouse Button  | Press              |
| Sprint        | Left Shift Key     | Hold               |
| Destroy Cube  | F Key              | Press              |
| Place Cube    | Right Mouse Button | Press              |
| Quit Game     | Escape             | Press              |

#### Controller
| Action        | Controller         | Status             |
|:-------------:|:------------------:|:------------------:|
| Move          | Left Thumb Stick   | Hold               |
| Jump          | Face Pad Bottom    | Press              |
| Attack        | Right Trigger      | Press              |
| Sprint        | Left Thumb Stick   | Hold               |
| Destroy Cube  | Right Trigger      | Press              |
| Place Cube    | Left Trigger       | Press              |
| Quit Game     | Menu Button        | Press              |

#### Mobile Input
To be added. Right now there are two joysticks, the left one controls the movement and the right one controls camera, and single press the screen for shooting.

## Planned Feature Implementations
- Finish Inventory System
- Add more mobile input & controls

## Known Issues
- There are some tree leaves in the air which don't attach to any tree trunks.
- The NavMesh is limited, if the player walk towards one direction for ~15 mins, the NPC won't be able to find a path.
- The player falls into meshes due to faulty collision with new placed cube