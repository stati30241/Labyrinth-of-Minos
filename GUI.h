#pragma once

#include "SFML/Graphics.hpp"

#include <numbers>
#include <vector>


// Helpful definition of elemtwise multiplication and division of sf::Vector's
template <typename T, typename U>
inline auto operator* (const sf::Vector2<T>& left, const sf::Vector2<U>& right) -> sf::Vector2<decltype(left.x * right.x)> {
    return sf::Vector2<decltype(left.x * right.x)>{ left.x * right.x, left.y * right.y };
}
template <typename T, typename U>
inline auto operator/ (const sf::Vector2<T>& left, const sf::Vector2<U>& right) -> sf::Vector2<decltype(left.x / right.x)> {
    return sf::Vector2<decltype(left.x / right.x)>{ left.x / right.x, left.y / right.y };
}

// Other vector functions
template <typename T>
inline float length(const sf::Vector2<T>& vec) {
    return std::sqrtf(vec.x * vec.x + vec.y * vec.y);
}
template <typename T>
inline sf::Vector2f normalize(const sf::Vector2<T>& vec) {
    return sf::Vector2f{ vec } / length(vec);
}
