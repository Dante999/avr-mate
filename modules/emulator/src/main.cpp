#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>

#include <array>


#include "handheld_emulator.hpp"

#include <spdlog/spdlog.h>

int main()
{
	spdlog::set_level(spdlog::level::info);
	// graphx_init();

	// create the window
	sf::RenderWindow window(sf::VideoMode(128, 64), "My window");
	window.setSize(sf::Vector2u(1280, 640));
	// window.setFramerateLimit(60);

	sf::Image   img;
	sf::Sprite  sprite;
	sf::Texture texture;

	img.create(128, 64, sf::Color::Blue);
	texture.loadFromImage(
	    img, sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(128, 64)));
	sprite.setTexture(texture);

	auto emulator = handheld_emulator_c(img);

	// run the program as long as the window is open
	while (window.isOpen()) {
		// check all the window's events that were triggered since the
		// last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event)) {
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// clear the window with black color
		window.clear(sf::Color::Green);

		// draw everything here...
		// draw_graphx_buffer(img, graphx_loop());
		emulator.do_work();

		texture.update(img);
		window.draw(sprite);

		// end the current frame
		window.display();
	}

	return 0;
}
