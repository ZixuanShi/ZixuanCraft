# ZixuanCraft
Minecraft clone using Unreal Engine.
Demo: https://www.youtube.com/watch?v=VBDN45e8FBQ

## Implemented Features
- Endless Terrain Generation
- AI can react to the player
    - Zombie (Normal, Skeleton, Creeper)
    - Livestocks (Chicken, Cow)
    - Pets (Dog)
- Adding Cube
- Removing Cube
- Inventory System
- Crafting System
- GooglePlay Services for Android
 
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
- Convert to UE5
- More terrains like sand, river, mountain
- Swimming
- Fix away compiling warnings for Android
- Minecraft Character instead of Unreal default Gun
- Saving and Loading progress
- Networking
- Refactor UI code

## Known Issues
- Controller can't manipulate all inventory UI mouse
- The NavMesh is limited, if the player walk towards one direction for ~15 mins, the NPC won't be able to find a path.
