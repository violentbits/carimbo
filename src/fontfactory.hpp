#pragma once

#include "common.hpp"

namespace graphics {
class fontfactory {
public:
  fontfactory() noexcept = delete;
  explicit fontfactory(const std::shared_ptr<graphics::renderer> renderer) noexcept;
  ~fontfactory() noexcept = default;

  std::shared_ptr<font> get(const std::string &family);

  void flush() noexcept;

  void update(float_t delta) noexcept;

private:
  std::list<std::string> _filenames{};
  std::unordered_map<std::string, std::shared_ptr<font>> _pool{};
  std::shared_ptr<graphics::renderer> _renderer{};
};
}
