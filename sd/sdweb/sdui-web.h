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

extern "C" {
extern void Suspend();
extern void AddChoice(char *choice);

void init();
}

namespace emscripten {
EMSCRIPTEN_BINDINGS(sdweb) { function("init", &init); }
} // namespace emscripten
