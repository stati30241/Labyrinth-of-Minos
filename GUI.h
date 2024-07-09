#pragma once

#include "SFML/Graphics.hpp"

#include <numbers>
#include <vector>
#include <iostream>


// Helpful operations
template <typename T, typename U>
inline auto operator+ (const sf::Vector2<T>& left, const sf::Vector2<U>& right) -> sf::Vector2<decltype(left.x + right.x)> {
    return sf::Vector2<decltype(left.x + right.x)>{ left.x + right.x, left.y + right.y };
}
template <typename T, typename U>
inline auto operator- (const sf::Vector2<T>& left, const sf::Vector2<U>& right) -> sf::Vector2<decltype(left.x - right.x)> {
    return sf::Vector2<decltype(left.x - right.x)>{ left.x - right.x, left.y - right.y };
}
template <typename T, typename U>
inline auto operator* (const sf::Vector2<T>& left, const sf::Vector2<U>& right) -> sf::Vector2<decltype(left.x * right.x)> {
    return sf::Vector2<decltype(left.x * right.x)>{ left.x * right.x, left.y * right.y };
}
template <typename T, typename U>
inline auto operator/ (const sf::Vector2<T>& left, const sf::Vector2<U>& right) -> sf::Vector2<decltype(left.x / right.x)> {
    return sf::Vector2<decltype(left.x / right.x)>{ left.x / right.x, left.y / right.y };
}
template <typename T>
inline float length(const sf::Vector2<T>& vec) {
    return std::sqrtf(vec.x * vec.x + vec.y * vec.y);
}
template <typename T>
inline sf::Vector2f normalize(const sf::Vector2<T>& vec) {
    return sf::Vector2f{ vec } / length(vec);
}


class TextureManager {
public:
	sf::Texture texture;

private:
	TextureManager(std::string_view filename) {
		texture.loadFromFile(filename.data());
	}

	TextureManager(const TextureManager& copy);
	TextureManager operator= (const TextureManager& copy);

public:
	static TextureManager& get(std::string_view filename = "") {
		static TextureManager instance{ filename };
		return instance;
	}
};

