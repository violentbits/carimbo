#include "font.hpp"

using namespace graphics;

font::font(const glyphmap &glyphs, std::shared_ptr<pixmap> pixmap)
    : _glyphs(glyphs), _pixmap(std::move(pixmap)) {}

void font::draw(const std::string &text, const geometry::point &position) const noexcept {
  geometry::point cursor = position;

  for (const auto &character : text) {
    const auto &glyph = _glyphs.at(character);
    const auto &size = glyph.size();

    _pixmap->draw(glyph, {cursor, size});

    cursor += std::make_pair('x', size.width());
  }
}
