#Savannah Kreider s5501023 CFGAA Assignment
Assignment Ideas/Work Doc
- ----------------------------------------------------------------------------------------------------------------------------------------------
**Video:
------------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------------
Annotated Bibliography:
- 
- Scoops (2008)by NimbleBit:
- MobyGames. (n.d.). Scoops (2008). [online] Available at: https://www.mobygames.com/game/49407/scoops/.
- 
- Originally, the game that I had in mind to expand upon was Scoops. You tilt your phone side to side to move the cone back and forth on the screen in doing so avoiding trash but collecting scoops to build the tower up. - - However, making it 3D means moving up the orthographic camera isnt as viable for a perspective as eventually it becomes too tiny to see everything properly either the board and/or the falling objects. So trying to come up with a more creative workaround to implement a game to this. While it does give me a basis as to powerups being special scoops, trash to avoid taking a life, and stackable scoops that earn point, it requires modification to be changed to 3D on a computer. Having controlls be on a common wsad movement, allowing to use the mouse to rotate around the enviroment, and trying to see how I can still collect without running into visual issues with the game is what I need to further explore.
-Overall, this game gives me the basic framework of my game, its general rules that I can then compare to similar games like this one , and add on my own solutions to the issues mentioned.
- 
- Stitch 625 Sandwitch Stacker (2006)by Disney:
- The Walt Disney Company (n.d.). 625 Sandwich Stacker. [online] Internet Archive. Available at: https://archive.org/details/flash_625-sandwich-stacker.
- 
- Sandwitch stacker is a 2D game that you can move side to side inorder to catch falling ingredents and avoid junk things that are rotten or non-food items. As the stack builds it starts to drag the stack, making it harder to avoid or catch items. Plus, if you catch another sandwitch bread it ends the sandwitch, catching junk then results in a lost life which you have initially 3 of. However, unlike catching junk, sandwitch bread means you move to a new stack so that you keep to screenspae and create a cumilative score. This more creative solution to keeping the tower if I want to stack will be benificial to think about when implementing my own version of this type of game.
- 
- Sky Burger (2009) by NimbelBit:
- Wikipedia. (2023). Sky Burger. [online] Available at: https://en.wikipedia.org/wiki/Sky_Burger.
- 
- Sky Burger's unique implementation of this style of game was that there were certian ingredents and certian ammounts of ingredents that you needed to collect and then top off with a bun in order to complete the order, the closer you were to the correct order,the larger the 'tip' was. Using the money you could unlock other items, themes, and levels. While in the scope of this project I won't be going as robust as that,again its a way if I have time to expand upon my project in a challenging way for the game play. I could implement ingredents/challeges of what to collect or a casual mode.
- 
- Tower Bloxx (2007) by Digital Chocolate:
-PeakD. (n.d.). Tower Bloxx Deluxe - Perfect for Stress. [online] Available at: https://peakd.com/hive-140217/@musicandreview/tower-bloxx-deluxe-perfect-for-stress.
- 
- Unlike the prior 3 game I've looked at, this one has the controlls above as you try to aim below to accurately build a tower as centered as you can. This one is also in 3D instead of 2D, so certian things in gameplay could be more useful as to how they solved certian issies. It adusts the camera height with each block placed so that when you're aiming it continues to swing side to side, adding to the difficulty of placing the block ontop. It is non rotatable cameraview, in more of a front view. Playing in this manner for a catching game instead of a aming game would be challenging. However things such as having a finite goal till it ends may be one way to stop a player going out of bounds of the game that it was reasonably built to. It also explores the effect that having aiming could affect the game play, perfect no swing and everything else would increase the swaying. I could implement a jitter as you're 'balancing' to create this effect if I dont visually see the stack  change past a certian number if its on the floor
- 
- Patterns in C- Part 2:State :
- Peatersen, A. (n.d.). Patterns In C - Part 2: STATE. [online] Available at: https://www.google.com/url?sa=t&source=web&rct=j&opi=89978449&url=https://www.adamtornhill.com/Patterns%2520in%2520C%25202,%2520STATE.pdf&ved=2ahUKEwjilNbAveCFAxVgXEEAHXbIAYUQFnoECBIQAQ&usg=AOvVaw214ZWxK1HMAcGCyseQ0b7i.
- 
- Exploring more on pattern in C, STATE could be helpful in regards to menues such as pause/play and start/end game where the input would be handled diffrent based on the behavors these states would result in such that game sets the state of menue and while the game is running it handles the input, updates based on it, and renders. This could also be applied to player events, game actions that would need to be performed later on. In the example Peatersen explores is a stopwatch where there goes into more explanation of the benifits and effects of using the pattern would have on the program to reduce complexity while allowing for flexibility to extend the code as you build the program. Being able to have flexibility is key with how I approach this project with working to first implement the basics then to expand.
- 
- Patterns in C- Part 4 :Observer:
- Peatersen, A. (n.d.). Patterns In C - Part 4: OBSERVER. [online] Available at:https://www.google.com/url?sa=t&source=web&rct=j&opi=89978449&url=https://www.adamtornhill.com/Patterns%2520in%2520C%25204,%2520OBSERVER.pdf&ved=2ahUKEwjpkcDnveCFAxVSS0EAHYC9AcAQFnoECBYQAQ&usg=AOvVaw2TBLtvxukUpHV0Czg4XXXw
- 
- From my understanding of observer from reading Patterns in C, it could be benificial for inputs to help manage these two things in my game. Things such as player stats like points, scoring overall, lives , all displayed as player stats visually too would use more of an Observer pattern. Observer pattter could handle specific events such as the player moving around for my game. However, I could expand it to collision detection that triggers an event from the wall or to the blocks falling, UI elements clicking , power ups, and enviroment changes potentially. I could split up inputs into player inputs and menu input handeler as well, however, I'm still very unsure until I start handelling these things if it would be worth putting in at an early stage.
- 
- Game Programming Patterns:
- Gameprogrammingpatterns.com. (2014). Game Loop · Sequencing Patterns · Game Programming Patterns. [online] Available at: https://gameprogrammingpatterns.com/game-loop.html.
- 
- Having the stucture of:
- while (true)
{
  processInput();//all user inputs from last update
  update();//updating the game 
  render();//renders to screen as frame
  sleep(start + MS_PER_FRAME - getCurrentTime());//delay render
}
- will allow me to have user inputs handled since the last call, updates it visuall, then renders it on the screen. The game loop will run continuously and can be used to control the rate of gameplay. However, Robert Nystrom mentions two things is to control  that th game dosen't run too fast if it processes fames too quicky and to not force patterns. While a game loop will be benificial, fps also needs to account for thus a sleep function ise useful. However, its fails to consider what happens if the computer can't handle the calculations thus revising to account for lag as well to catch up with the player:
- double previous = getCurrentTime();
double lag = 0.0;//ensures catch up of multipe updates occures gradually over time making for a smoother gameplay
//MS_PER_UPDATE-consistent rate indepentent of system performance
- while (true)
{
  double current = getCurrentTime();
  double elapsed = current - previous;
  previous = current;
  lag += elapsed;

  processInput();

  while (lag >= MS_PER_UPDATE)
  {
    update();
    lag -= MS_PER_UPDATE;
  }

  render(lag / MS_PER_UPDATE);
}
-
- Jon's Teapot: Getting Started with the Programming Assignment The SpaceShip
- Jon Macey (2012). Jon’s Teapot: Getting Started with the Programming Assignment Pt 4 The SpaceShip. [online] Jon’s Teapot. Available at: http://jonmacey.blogspot.com/2012/02/getting-started-with-programming_8527.html
- 
- Taking a look at how someone that has handle keys in NGL and overall game loops, there's a few things that I need to implement such as processing holding a key down consistently to continually move  by processing the key pressed in the object you want to move and draw it in the paintGL.
- REMINDER: copy ngl shaders edit config build
- ----------------------------------------------------------------
-A common rounding error for calculating where the boundaries of the block (if an actual block is used) is a rounding error on the cone opsition and the block distance, where the calculated error is clipped as the distance is in a sphere shape for catchDistance and distance = (cone position - block position ) length  it would calc in a sphere shape instead of the actual geometry of a block
-anything outide of the sphere would read as if it didnt collide with the block unless further calculations are made
!["block with distance from center, collision in shape of sphere"](images/IllustrateCatchError.png)

