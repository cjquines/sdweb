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
#include <string>

std::string the_text = "default";

// these names can't get mangled
extern "C" {
void init() { printf("have: %s\n", the_text.c_str()); }

void resume() { the_text = "asdf"; }
}

namespace emscripten {
EMSCRIPTEN_BINDINGS(sdweb) {
  function("init", &init);
  function("resume", &resume);
}
} // namespace emscripten

int iofull::do_abort_popup() {
  return yesnoconfirm("Confirmation", (char *)0,
                      "Do you really want to abort this sequence?", true,
                      false);
}

void iofull::prepare_for_listing() {}

uims_reply_thing iofull::get_startup_command() {
  // TODO
}

void iofull::set_window_title(char s[]) {}

void iofull::add_new_line(const char the_line[], uint32_t drawing_picture) {
  emscripten_log(EM_LOG_CONSOLE, the_line);
}

void iofull::no_erase_before_n(int n) {}

void iofull::reduce_line_count(int n) {}

void iofull::update_resolve_menu(command_kind goal, int cur, int max,
                                 resolver_display_state state) {}

void iofull::show_match() {}

const char *iofull::version_string() { return "sdweb"; }

uims_reply_thing iofull::get_resolve_command() {
  // TODO
}

bool iofull::choose_font() { return false; }
bool iofull::print_this() { return false; }
bool iofull::print_any() { return false; }
bool iofull::help_manual() { return false; }
bool iofull::help_faq() { return false; }

popup_return iofull::get_popup_string(Cstring prompt1, Cstring prompt2,
                                      Cstring final_inline_prompt, Cstring seed,
                                      char *dest) {
  // TODO
}

void iofull::fatal_error_exit(int code, Cstring s1, Cstring s2) {
  emscripten_log(EM_LOG_ERROR, "%s: %s", s1, s2);
  general_final_exit(code);
}

void iofull::serious_error_print(Cstring s1) {
  emscripten_log(EM_LOG_WARN, s1);
}

void iofull::create_menu(call_list_kind cl) {}

selector_kind iofull::do_selector_popup(matcher_class &matcher) {
  // TODO
}

direction_kind iofull::do_direction_popup(matcher_class &matcher) {
  // TODO
}

int iofull::do_circcer_popup() {
  // TODO
}

int iofull::do_tagger_popup(int tagger_class) {
  // TODO
}

int iofull::yesnoconfirm(Cstring title, Cstring line1, Cstring line2, bool excl,
                         bool info) {
  // TODO
}

void iofull::set_pick_string(Cstring string) {}

uint32_t iofull::get_one_number(matcher_class &matcher) {
  // TODO
}

uims_reply_thing iofull::get_call_command() {
  // TODO
}

void iofull::dispose_of_abbreviation(const char *linebuff) {}

void iofull::display_help() {}

void iofull::terminate(int code) {}

void iofull::process_command_line(int *argcp, char ***argvp) {}

void iofull::bad_argument(Cstring s1, Cstring s2, Cstring s3) {
  fatal_error_exit(1, s1, s2);
}

void iofull::final_initialize() {}

bool iofull::init_step(init_callback_state s, int n) {
  switch (s) {
  case get_session_info:
    session_index = 0;
    sequence_number = -1;
    break;

  case final_level_query:
    calling_level = l_mainstream;
    strncat(outfile_string, filename_strings[calling_level],
            MAX_FILENAME_LENGTH);
    // TODO
    break;

  case init_database1:
  case init_database2:
  case calibrate_tick:
  case do_tick:
  case tick_end:
  case do_accelerator:
    printf("%d %d\n", s, n);
    // most computers should do these instantly now
    break;
  }
  return false;
}

void iofull::set_utils_ptr(ui_utils *utils_ptr) { m_ui_utils_ptr = utils_ptr; }
ui_utils *iofull::get_utils_ptr() { return m_ui_utils_ptr; }
