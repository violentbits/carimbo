#include "scriptengine.hpp"

#include "common.hpp"
#include "delay.hpp"
#include "engine.hpp"
#include "enginefactory.hpp"
#include "entity.hpp"
#include "entityprops.hpp"
#include "event.hpp"
#include "eventreceiver.hpp"
#include "io.hpp"
#include "loopable.hpp"
#include "noncopyable.hpp"
#include "pixmap.hpp"
#include "point.hpp"
#include "resourcemanager.hpp"
#include "soundmanager.hpp"
#include "statemanager.hpp"
#include "ticks.hpp"
#include <sol/property.hpp>
#include <sol/types.hpp>

using namespace framework;

class loopable_proxy : public loopable {
public:
  loopable_proxy(sol::function lua_func) : function(lua_func) {}

  void loop(uint32_t delta) override {
    if (function.valid()) {
      function(delta);
    }
  }

private:
  sol::function function;
};

void scriptengine::run() {
  sol::state lua;

  lua.open_libraries();
  //
  // lua.open_libraries(sol::lib::base, sol::lib::coroutine, sol::lib::package, sol::lib::string);

  lua.new_enum(
      "KeyEvent",
      "w", input::keyevent::w,
      "a", input::keyevent::a,
      "s", input::keyevent::s,
      "d", input::keyevent::d,
      "space", input::keyevent::space);

  lua.new_enum(
      "Anchor",
      "top", anchor::top,
      "bottom", anchor::bottom,
      "left", anchor::left,
      "right", anchor::right,
      "none", anchor::none);

  lua.new_usertype<geometry::point>(
      "Point",
      sol::constructors<geometry::point(int32_t, int32_t)>(),
      "x", sol::property(&geometry::point::x, &geometry::point::set_x),
      "y", sol::property(&geometry::point::y, &geometry::point::set_y));

  lua.new_usertype<enginefactory>(
      "EngineFactory",
      sol::constructors<enginefactory()>(),
      "set_title", &enginefactory::set_title,
      "set_width", &enginefactory::set_width,
      "set_height", &enginefactory::set_height,
      "set_fullscreen", &enginefactory::set_fullscreen,
      "create", &enginefactory::create);

  lua.new_usertype<engine>(
      "Engine",
      "run", &engine::run,
      "spawn", &engine::spawn,
      "destroy", &engine::destroy,
      "ticks", &ticks,
      "is_keydown", &engine::is_keydown,
      "width", sol::property(&engine::width),
      "height", sol::property(&engine::height),
      "soundmanager", &engine::soundmanager,
      "add_loopable", &engine::add_loopable,
      "prefetch", [](engine &engine, sol::table table) {
        std::vector<std::string> filenames{table.size()};
        for (auto &item : table) {
          filenames.push_back(item.second.as<std::string>());
        }
        engine.prefetch(filenames);
      });

  lua.new_usertype<audio::soundmanager>(
      "SoundManager",
      "play", &audio::soundmanager::play,
      "stop", &audio::soundmanager::stop);

  lua.new_usertype<entity>(
      "Entity",
      "id", sol::property(&entity::id),
      // "x", sol::property(&entity::x, &entity::set_x),
      // "y", sol::property(&entity::y, &entity::set_y),
      // "width", sol::property(&entity::width),
      // "height", sol::property(&entity::height),
      // "move", &entity::move,
      // "scale", &entity::scale,
      // "angle", sol::property(&entity::angle, &entity::set_angle),
      // "alpha", sol::property(&entity::alpha, &entity::set_alpha),
      // "pixmap", sol::property(&entity::set_pixmap),
      // "play", &entity::play_sound,
      "on_update", &entity::set_onupdate,
      "set_action", &entity::set_action,
      "set_placement", &entity::set_placement);

  // lua.new_usertype<loopable_proxy>("loopable_proxy",
  //                                  sol::constructors<loopable_proxy(sol::function)>(),
  //                                  "loop", &loopable_proxy::loop);

  lua.new_usertype<loopable_proxy>("loopable_proxy",
                                   sol::constructors<loopable_proxy(sol::function)>(),
                                   "loop", &loopable_proxy::loop);

  lua.set_function("sleep", &sleep);

  const auto script = storage::io::read("scripts/main.lua");

  sol::object loopable_proxy_obj = lua["loopable_proxy"];
  if (loopable_proxy_obj.valid() && loopable_proxy_obj.get_type() == sol::type::userdata) {
    std::cout << "loopable_proxy registrado corretamente como userdata." << std::endl;
  } else {
    std::cerr << "Erro: loopable_proxy não está registrado corretamente no Lua." << std::endl;
  }

  lua.script(std::string_view(reinterpret_cast<const char *>(script.data()), script.size()));
}
