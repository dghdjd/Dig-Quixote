# Team-16-Dig-Quixote

This is our game

## Milestone 1

### Implemented features
Player movement (left & right) (world_system.cpp/on_key)

Player jump (world_system.cpp/on_key)

Gravity (physics_system.cpp/step)

Create a block (world_init.cpp/createBlock)

Object collision (world_system.cpp/handle_collisions; physics_system.cpp/step)

Simple background (render_system.cpp/draw)

BGM (world_system.cpp/create_window)

### Sum up
We have followed our plan for the milestone 1. Even thought we started a little bit late, we do finish this part of our game, such as player movement, object collision, basic rendering, etc. The only thing that we do not have time to work on is to let player break a block (an optional feature). We all agree that we should start our work earlier for the next milestone.

## Milestone 2

### Implemented features
User can dig blocks with 3 hits (world_system.cpp/step)

Sprite sheet animations for player on idle state (render_system.cpp/updatePlayerAnimation; render_system_init.cpp/setupPlayerAnimation)

Sprite sheet animations for player movement (render_system.cpp/updatePlayerAnimation; render_system_init.cpp/setupPlayerAnimation)

Character's animation while player is digging (render_system.cpp/updatePlayerAnimation; render_system_init.cpp/setupPlayerAnimation)

New sprite for blocks, stone, dirt, grass (render_system.hpp; world_init.cpp)

Upgraded collision (world_system.cpp/handle_collisions; physics_system.cpp/collides; physics_system.cpp/step)

New mesh block (iron), collision, gradient colour change of mesh block. (render_system.hpp; world_system.cpp)

### Sum up
We accomplished the basic game play, animations and level design. We have also created a basic random-map-generating system(will make it more playable in the next milestones).


## Milestone 3
   
### Implemented features
New gravity system (world_system.cpp/step; physics_system.cpp/step)
   
Digging particals with blocks(dirt) (world_system.cpp)
   
Death timer for dig count (world_system.cpp)
   
Mesh ball with collision and gravity
   
Crystal explosion (world_system.cpp)
   
A fully playable game level map
   
New game logic
   
### Sum up
We completed most of the features for mileston 3. We find out that we need more time for this milestone since we only have 4 people. We will bring more interesting features in the next milestone.


## Milestone 4
   
### Implemented features
New collide system, Prevented the unexpected fall of the stone.(world_system.cpp/step; physics_system.cpp/step)
   
Lava particals (world_system.cpp)

New blocks: lava & transferGate and their collide logic (world_system.cpp)

Tutorial map(world_system.cpp)

New map levels & transfer between levels(world_system.cpp)
   
New game logic
   
### Sum up
We completed our game. We bring 2 more different blocks and 2 different level of map, connect them smoothly. We changed the way handling collisions and fixed all the bugs.
 At this moment, I am sincerely proud of our team for successfully completing a game with such limited manpower.

## Attributes

Character: https://craftpix.net/freebies/free-raider-sprite-sheets-pixel-art/

Explosion effect: https://craftpix.net/freebies/11-free-pixel-art-explosion-sprites/?num=1&count=17&sq=explode&pos=5

Portal effect: https://tenor.com/en-CA/view/sonic-mania-portal-sticker-gif-26289678

Arrow icons: https://www.iconarchive.com/show/flat-cute-arrows-icons-by-custom-icon-design.html

Reset button designed by Freepik: https://www.freepik.com/icon/reset_5486166

Background clouds designed by Freepik: https://www.freepik.com/free-vector/watercolor-clouds-background_5476614.htm

Cloud blocks designed by vector_corp: https://www.freepik.com/free-vector/white-clouds-collection_52768330.htm

Music track: Flourish by Pufino Source: https://freetouse.com/music No Copyright Music for Videos (Free)

Music track: Cinematic by Aylex Source: https://freetouse.com/music Music for Video (Free Download)

Music track: Worlds by Aylex Source: https://freetouse.com/music Free Vlog Music Without Copyright

### More about
More thanks to Professor Helge Rhodin for his guidance and assistance in this semester!!!

More thanks to Luis Bolanos for the assistance provided during our game development and for helping us overcome some issues.

Thanks ChatGPT4.0 for generating the block and background textures.

Thanks Nora for drawing the first daft of the tutorial image.
