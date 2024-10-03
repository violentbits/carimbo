#pragma once

#include "common.hpp"

#include "anchor.hpp"
#include "entityprops.hpp"
#include <string_view>

namespace framework {
class entity : public std::enable_shared_from_this<entity> {
public:
  virtual ~entity();

  static std::shared_ptr<entity> create(const entityprops &&props);

  uint64_t id() const;

  virtual void update();

  virtual void draw() const;

  // void set_x(int32_t x);

  // int32_t x() const;

  // void set_y(int32_t y);

  // int32_t y() const;

  // void move(int32_t x, int32_t y);

  // int32_t width() const;

  // int32_t height() const;

  // void scale(double factor);

  // void set_angle(const double_t angle);

  // double_t angle() const;

  // void set_flip(graphics::flip flip);

  // graphics::flip get_flip();

  // void set_alpha(const uint8_t alpha);

  // uint8_t alpha() const;

  const entityprops props() const noexcept;

  void set_props(entityprops props) noexcept;

  void set_placement(int32_t x, int32_t y, anchor anchor = anchor::none) noexcept;

  void set_entitymanager(std::shared_ptr<entitymanager> entitymanager);

  void set_resourcemanager(std::shared_ptr<resourcemanager> resourcemanager);

  void set_onupdate(const std::function<void(std::shared_ptr<entity>)> &fn);

  void set_pixmap(const std::string_view filename);

  void play_sound(const std::string_view filename);

  void set_action(const std::string_view action) noexcept;

  std::string action() const noexcept;

private:
  entity(const entityprops &&props);

  std::string _id;
  entityprops _props;
  std::shared_ptr<entitymanager> _entitymanager;
  std::shared_ptr<resourcemanager> _resourcemanager;
  std::function<void(std::shared_ptr<entity>)> _fn;
};
}
