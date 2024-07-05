#pragma once

#include "SFML/Graphics.hpp"
#include "vector"


// Helpful definition of elemtwise multiplication and division
template <typename T, typename U>
inline auto operator* (const sf::Vector2<T>& left, const sf::Vector2<U>& right) -> sf::Vector2<decltype(left.x * right.x)> {
    return sf::Vector2<decltype(left.x * right.x)>{ left.x * right.x, left.y * right.y };
}
template <typename T, typename U>
inline auto operator/ (const sf::Vector2<T>& left, const sf::Vector2<U>& right) -> sf::Vector2<decltype(left.x / right.x)> {
    return sf::Vector2<decltype(left.x / right.x)>{ left.x / right.x, left.y / right.y };
}
