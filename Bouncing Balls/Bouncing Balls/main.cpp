#include <iostream>
#include <SFML/Graphics.hpp>

//Main method
int main() {
	//Initalise window
	sf::RenderWindow window(sf::VideoMode(720, 480), "Bouncing Balls");
	//Define window event handle
	sf::Event windowEvent;

	//While window is open, check for user input, update game logic and draw
	while (window.isOpen()) {
		//Check for user input & events
		while (window.pollEvent(windowEvent)) {
			if (windowEvent.type == sf::Event::Closed) {
				window.close();
			}
		}

		//Clear the window
		window.clear();

		//Display what has been draw for this frame
		window.display();
	}

	//Close the window & release resources
	window.close();

	//Close as expected
	return 0;
}