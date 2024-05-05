#include "color.hpp"

Color::Color(int r, int g, int b): red_(r), green_(g), blue_(b) {
  if (r < 0 || g < 0 || b < 0 || r > kColorValueMax || g > kColorValueMax || b > kColorValueMax) {
    throw std::invalid_argument("color value out of bounds");
  }
}

// do not modify
bool operator==(const Color& rhs, const Color& lhs) {
  return (rhs.Red() == lhs.Red() && rhs.Green() == lhs.Green() &&
          rhs.Blue() == lhs.Blue());
}