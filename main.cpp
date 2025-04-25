#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <fmt/core.h>
#include <iostream>

#include "fractal_serial.h"
#include "fractal_simd.h"

const double x_min = -1.5;
const double x_max = 1.2;
const double y_min = -1.0;
const double y_max = 1.0;

#define WIDTH 1600
#define HEIGHT 900

int max_iteraciones = 10;
static uint32_t* pixel_buffer = nullptr;

enum class runtime_type { CPU_1, CPU_2, SIMD };

int main() {
    pixel_buffer = new uint32_t[WIDTH * HEIGHT];

    runtime_type r_type = runtime_type::CPU_1;

    // Create window
    sf::RenderWindow window(sf::VideoMode({ WIDTH, HEIGHT }), "Fractal Julia");

    sf::Texture texture({ WIDTH, HEIGHT });
    sf::Sprite sprite(texture);

    const sf::Font font("Arial.ttf");
    sf::Text text(font, "Julia Set", 25);
    text.setFillColor(sf::Color::White);
    text.setPosition({ 10, 10 });
    text.setStyle(sf::Text::Bold);

    sf::Text textOptions(font, "OPTIONS: [1] CPU_1 [2] CPU_2", 24);
    textOptions.setFillColor(sf::Color::White);
    textOptions.setStyle(sf::Text::Bold);
    textOptions.setPosition({ 10, window.getView().getSize().y - 50 });

    sf::Clock clockFrames;
    int frames = 0;
    int fps = 0;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (event->is<sf::Event::KeyReleased>()) {
                auto evt = event->getIf<sf::Event::KeyReleased>();
                switch (evt->scancode) {
                    case sf::Keyboard::Scan::Num1:
                        r_type = runtime_type::CPU_1;
                        break;
                    case sf::Keyboard::Scan::Num2:
                        r_type = runtime_type::CPU_2;
                        break;
                        case sf::Keyboard::Scan::Num3:
                        r_type = runtime_type::SIMD;
                        break;
                    case sf::Keyboard::Scan::Up:
                        max_iteraciones += 10;
                        break;
                    case sf::Keyboard::Scan::Down:
                        max_iteraciones -= 10;
                        if (max_iteraciones < 1) max_iteraciones = 1;
                        break;
                    default:
                        break;
                }
            }
        }

        std::string mode = "";

        if (r_type == runtime_type::CPU_1) {
            julia_serial(x_min, y_min, x_max, y_max, pixel_buffer);
            mode = "CPU_1";
        }
        else if (r_type == runtime_type::CPU_2) {
            julia_serial_2(x_min, y_min, x_max, y_max, pixel_buffer);
            mode = "CPU_2";
        }
        else if (r_type == runtime_type::SIMD) {
            julia_simd(x_min, y_min, x_max, y_max, pixel_buffer);
            mode = "SIMD";
        }


        texture.update((const std::uint8_t*)pixel_buffer);

        auto msg = fmt::format("Julia Set: Iterations = {}, FPS: {}, Mode: {}", max_iteraciones, fps, mode);
        text.setString(msg);

        frames++;
        if (clockFrames.getElapsedTime().asSeconds() >= 1.0) {
            fps = frames;
            frames = 0;
            clockFrames.restart();
        }

        window.clear();
        window.draw(sprite);
        window.draw(text);
        window.draw(textOptions);
        window.display();
    }
}
