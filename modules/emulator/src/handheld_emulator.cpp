#include "handheld_emulator.hpp"
#include "spdlog/spdlog.h"

#include "avr-mate-core/graphx.h"

#include <thread>

#include <SFML/Window/Keyboard.hpp>

namespace {
handheld_emulator_c *g_emulator;

void emulator_draw_buffer(const uint8_t *buffer)
{
	(void)buffer;
	g_emulator->draw_buffer();
}

void emulator_get_buttons(struct cp_buttons *buttons)
{
	g_emulator->get_buttons(buttons);
}

void emulator_send_byte(uint8_t byte)
{
	g_emulator->send_byte(byte);
}

uint8_t emulator_receive_byte()
{
	return g_emulator->receive_byte();
}
} // namespace

handheld_emulator_c::handheld_emulator_c(sf::Image &img) : m_image(img)
{
	g_emulator = this;

	m_handheld_dev.draw_buffer                    = emulator_draw_buffer;
	m_handheld_dev.get_buttons                    = emulator_get_buttons;
	m_handheld_dev.publish_busy                   = [] {};
	m_handheld_dev.publish_ready                  = [] {};
	m_handheld_dev.protocol.send_byte    = emulator_send_byte;
	m_handheld_dev.protocol.receive_byte = emulator_receive_byte;

	handheld_init(&m_handheld_dev);
}

void handheld_emulator_c::draw_buffer()
{
	for (uint8_t x = 0; x < GRAPHX_WIDTH; ++x) {

		for (uint8_t y = 0; y < GRAPHX_HEIGHT; ++y) {
			m_image.setPixel(x, y,
			                 graphx_get_pixel(x, y) == PIXEL_ON
			                     ? sf::Color::Black
			                     : sf::Color::White);
		}
	}
}

void handheld_emulator_c::get_buttons(struct cp_buttons *buttons)
{

	buttons->states = 0;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		buttons->states |= (1 << cp_button_msk::BUTTON_UP);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		buttons->states |= (1 << cp_button_msk::BUTTON_DOWN);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		buttons->states |= (1 << cp_button_msk::BUTTON_LEFT);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		buttons->states |= (1 << cp_button_msk::BUTTON_RIGHT);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
		buttons->states |= (1 << cp_button_msk::BUTTON_B);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
		buttons->states |= (1 << cp_button_msk::BUTTON_A);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
		buttons->states |= (1 << cp_button_msk::BUTTON_START);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace)) {
		buttons->states |= (1 << cp_button_msk::BUTTON_SELECT);
	}
}

void handheld_emulator_c::do_work()
{
	handheld_waitfor_instructions();
}
