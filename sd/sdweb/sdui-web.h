// include these if they're not defined, because emcc includes them by default
// (yes, this is sketchy, whatever)
#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>
#include <emscripten/emscripten.h>
#else
#include "emscripten/bind.h"
#include "emscripten/emscripten.h"
#endif

#include "../sd.h"

// roughly in the order these happen
extern "C" {
void init();
void new_sequence();
void on_startup_command(uint16_t minorpart);
void start_cycle();
void on_call_command(uint16_t minorpart);
void got_an_exception();
}

namespace emscripten {
EMSCRIPTEN_BINDINGS(sdweb) {
  function("init", &init);
  function("on_startup_command", &on_startup_command);
  function("on_call_command", &on_call_command);
}
} // namespace emscripten
