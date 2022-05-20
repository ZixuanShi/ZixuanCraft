# ZixuanCraft
Minecraft clone using Unreal Engine.

## Implemented Features
- Endless Terrain Generation
- AI can react to the player
    - Zombie
    - Chicken
- Adding Cube
- Removing Cube
- Inventory System
- Crafting System
 
## Target Platfroms
- Windows
- Android

## Input
- Keyboard & Mouse
- Controller
- Mobile Touches

## Controls
#### Keyboard & Mouse
| Action           | Keyboard & Mouse    | Status             |
|:----------------:|:-------------------:|:------------------:|
| Move             | WASD                | Hold               |
| Rotate Camera    | Mouse X & Y axis    | Hold               |
| Jump             | Space bar           | Press              |
| Attack           | F Key               | Press              |
| Sprint           | Left Shift Key      | Hold               |
| Destroy Cube     | F Key               | Hold               |
| Place Cube       | Right mouse button  | Press              |
| Quit Game        | Escape              | Press              |
| Toggle Inventory | E Key               | Press              |
| Scroll Inventory | Mouse Wheel         | Scroll             |

#### Controller
| Action           | Controller          | Status             |
|:---------------: |:-------------------:|:------------------:|
| Move             | Left Thumb Stick    | Hold               |
| Rotate Camera    | Right Thumb Stick   | Hold               |
| Jump             | Face Pad Bottom     | Press              |
| Attack           | Face Pad Left       | Press              |
| Sprint           | Left Thumb Stick    | Hold               |
| Destroy Cube     | Right Trigger       | Hold               |
| Place Cube       | Left Trigger        | Press              |
| Quit Game        | Menu Button         | Press              |
| Toggle Inventory | Face Pad Top        | Press              |
| Scroll Inventory | Left/Right Shoulder | Press              |
 
#### Mobile Input
| Action           | Mobile              | Status             |
|:---------------: |:-------------------:|:------------------:|
| Move             | Virtual Joystick    | Hold               |
| Rotate Camera    | Scroll on Screen    | Hold               |
| Jump             | Jump Button         | Press              |
| Attack           | A Button            | Press              |
| Sprint           | TBD                 | Hold               |
| Destroy Cube     | A Button            | Hold               |
| Place Cube       | B Button            | Press              |
| Quit Game        | TBD                 | Press              |
| Toggle Inventory | TBD                 | Press              |
| Scroll Inventory | Touch               | Press              |

## Future Plans
- Add village
- More terrains like sand, river, mountain

## Known Issues
- Controller can't manipulate all inventory UI mouse
- The NavMesh is limited, if the player walk towards one direction for ~15 mins, the NPC won't be able to find a path.