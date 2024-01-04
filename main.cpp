#include <SFML/Graphics.hpp> // for basic game functions
#include <time.h>		// Selecting a Random Cactus out of 4 every 5 seconds
#include <SFML/Audio.hpp>  // for crash and jump sounds 


using namespace sf;

// Constants for jump height, pixel count, frame count, and initial game speed

// jumpPixel represents the vertical distance (in pixels) that the dinosaur will move upwards
// and in game loop it will be used to check if the dino has reached its max height or not 
const int jumpPixel = 220;	

// pixel count, to move dino upwards we will change these pixels 
int pixels = 0;



//This creates a simple animation loop where the dinosaur alternates between two running frames, 
// giving the appearance of movement.
int frame = 0;

// initial game speed
float speedgame = 10;


// Flags/boolean variables to track game state and dinosaur movement
bool onGround = true;
bool jump = false;
bool game = false;
bool gameover = false;


// texture datatypes that initilizes variables to hold images 
Texture DinoT;
Texture texture;
Texture End;
Texture Cactus;
Texture Restart;


int main()
{
	// Create a game window and set frames to 60 frames 
	RenderWindow window(VideoMode(1200, 400), "DINO");
	window.setFramerateLimit(70);


	// Loading images from files to texture variables 
	DinoT.loadFromFile("resources/dinosaur.png");
	texture.loadFromFile("resources/DINO.png"); // background and floor 
	End.loadFromFile("resources/gameover.png");
	Cactus.loadFromFile("resources/cactus.png");
	Restart.loadFromFile("resources/restart.png");


	// Sprite class will contain the image in itself and make us able to edit 
	// and resize those images 
	Sprite dino(DinoT, { 0,0,44,47 });
	Sprite background(texture, { 2,58,600,10 });
	Sprite cactus(Cactus);
	Sprite end(End);
	Sprite restart(Restart);


	// Resizing the images using .setScale function and 
	// .setPosition to set initial positions of images or sprites in program
	dino.setScale(3, 3);
	dino.setPosition(50, 230);
	background.setScale(6, 6);
	background.setPosition(0, 340);
	cactus.setScale(2, 2);
	cactus.setPosition(-50, 250);
	end.setPosition(315, 150);
	end.setScale(3, 3);
	restart.setPosition(546, 220);
	restart.setScale(3, 3);


	// Loading Jump Sounds and Crash sound in memory 
	SoundBuffer jumpBuffer;
	if (!jumpBuffer.loadFromFile("resources/effect.mp3"))
	{
		// Handle loading error
		return -1;
	}

	// sound datatype will hold that loaded audio in memory 
	//  sound buffer holds the data of a sound, which is an array of audio samples.
	// .setBuffer make sure to play the audio when needed 
	Sound jumpSound;
	jumpSound.setBuffer(jumpBuffer);


	SoundBuffer crashBuffer;
	if (!crashBuffer.loadFromFile("resources/crash.mp3"))
	{
		// Handle loading error
		return -1;
	}


	// Crash Sound Effect Buffer 
	Sound crashSound;
	crashSound.setBuffer(crashBuffer);




	// game loop 
	while (window.isOpen())
	{
		// holds all the information about a system event that just happened like pressing a buttom
		Event event;


		// Checking Events 
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			if (event.type == Event::KeyPressed)
			{
				if ((event.key.code == Keyboard::Space) || (event.key.code == Keyboard::Up))
				{
					if (!game)
					{
						game = true;
						dino.setPosition(50, 230);
						cactus.setPosition(-50, 250);
						dino.setTextureRect({ 0,0,44,47 });
						pixels = 0;
						frame = 0;
						speedgame = 10;
						jump = false;
						onGround = true;
						
					}
					else if (onGround)
					{
						jump = true;
						onGround = false;
						pixels = 0;
						dino.setTextureRect({ 88,0,44,47 });

						// Play the jump sound
						jumpSound.play();
					}
				}
			}
		}

		// Press 'Enter to Exit'
		if (event.key.code == Keyboard::Enter)
		{

			return (0);
		}


		//  game is started 
		if (!game)
		{
			window.clear(Color::White);
			window.draw(background);
			
			// if game is over after running the game 
			if (gameover)
			{
				window.draw(cactus);
				window.draw(dino);
				window.draw(end);
				window.draw(restart);
			}
			else
			{
				window.draw(dino);
			}
			window.display();
			continue;
		}
		


		if (jump)
		{
			if (jumpPixel > pixels)
			{
				// to make dino jump upward 
				dino.move(0, (-jumpPixel / float(25)));
				pixels += jumpPixel / float(25); 
			}
			else // if dino is on the ground it must remain on the ground 
			{
				jump = false;
				pixels = 0;
			}
		}

		// To bring back the dino from air to ground again 
		else if (!onGround) 
		{
			if (jumpPixel > pixels)
			{
				dino.move(0, (jumpPixel / float(25)));
				pixels += jumpPixel / float(25);
			}
			else  // // if dino is on the ground it must remain on the ground 
					// and make dino ready to jump again
			{
				onGround = true;
			}
		}

		else // running animation: to make dino look like it's running 
		{
			if (frame <= 5)
			{
				dino.setTextureRect({ 88,0,44,47 });
				frame++;
			}
			else if (frame <= 10)
			{
				dino.setTextureRect({ 132,0,44,47 });
				frame++;
			}
			else frame = 0;
		}

		// Selecting a new random cactus out of five cactus 
		if (cactus.getPosition().x <= -25) 
		{
			srand(time(NULL));
			cactus.setPosition(1200, 250);
			int random = rand() % 5;
			switch (random)
			{
				// set position and scale the size of the images of cactuses 
			case 0: cactus.setTextureRect({ 0,0,25,53 }); break;
			case 1: cactus.setTextureRect({ 25,0,25,53 }); break;
			case 2: cactus.setTextureRect({ 50,0,25,53 }); break;
			case 3: cactus.setTextureRect({ 75,0,25,53 }); break;
			case 4: cactus.setTextureRect({ 100,0,28,53 }); break;
			}
		}

		// increase cactus motion speed every 5 frames 
		cactus.move(-speedgame, 0);
		speedgame += 0.005;
		

		// intersection of dino and cactus and game over 
		if (dino.getGlobalBounds().intersects(cactus.getGlobalBounds()))
		{
			gameover = true;
			game = false;
			// Play the crash sound
			crashSound.play();
		}




		// Draw
		window.clear(Color::White);
		window.draw(background);
		window.draw(dino);
		window.draw(cactus);
		window.display();

		

	}

	return 0;
}