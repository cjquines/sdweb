#include "sdui-web.h"

// clang-format off
EM_JS(void, Suspend, (), {
  return Asyncify.handleSleep(function (resume) {
    Module._resume_fn = resume;
  });
});

EM_JS(void, SetUserInputPtr, (char *m_user_input), {
  Module.write_to_user_input = function (str) {
    stringToUTF8(str, m_user_input, 400);
  }
});

EM_JS(void, ResetChoices, (), {
  Module.choices = [];
});

EM_JS(void, AddChoice, (char *choice), {
  Module.choices.push(UTF8ToString(choice));
});
// clang-format on

// these names are exported and can't get mangled
extern "C" {
void init() {
  iofull ggg;
  sdmain(0, nullptr, ggg);
}
}

// iofull implementation. mostly fake, but some of it's real

// actual functions
const char *iofull::version_string() { return "sdweb"; }
void iofull::set_utils_ptr(ui_utils *utils_ptr) { m_ui_utils_ptr = utils_ptr; }
ui_utils *iofull::get_utils_ptr() { return m_ui_utils_ptr; }

bool iofull::init_step(init_callback_state s, int n) {
  switch (s) {
  case get_session_info:
    session_index = 0;
    sequence_number = -1;
    break;
  case final_level_query:
    calling_level = l_c4;
    break;
  case init_database1:
  case init_database2:
  case calibrate_tick:
  case do_tick:
  case tick_end:
  case do_accelerator:
    break;
  }
  return false;
}

void iofull::fatal_error_exit(int code, Cstring s1, Cstring s2) {
  emscripten_log(EM_LOG_ERROR, "%s: %s", s1, s2);
  general_final_exit(code);
}

void iofull::serious_error_print(Cstring s1) {
  emscripten_log(EM_LOG_ERROR, s1);
}

void iofull::update_resolve_menu(command_kind goal, int cur, int max,
                                 resolver_display_state state) {
  // TODO: set something global that js can fetch?
}

void iofull::show_match() { AddChoice(gg77->matcher_p->m_full_extension); }

// input functions, all of which suspend
int iofull::do_abort_popup() { return POPUP_ACCEPT; }

const uims_reply_thing FAKE_UIMS_REPLY =
    uims_reply_thing(ui_command_select, command_quit);

uims_reply_thing iofull::get_startup_command() {
  SetUserInputPtr(gg77->matcher_p->m_user_input);
  ResetChoices();
  gg77->matcher_p->erase_matcher_input();
  gg77->matcher_p->match_user_input(matcher_class::e_match_startup_commands,
                                    true, true, false);
  Suspend();
  // js should resume after putting something in m_user_input
  gg77->matcher_p->match_user_input(matcher_class::e_match_startup_commands,
                                    false, false, true);

  modifier_block &matchmatch = (*gg77->matcher_p).m_final_result.match;
  int index = matchmatch.index;
  if (matchmatch.kind == ui_start_select) {
    index = (int)startup_command_values[index];
  }
  return uims_reply_thing(matchmatch.kind, index);
}

uims_reply_thing iofull::get_call_command() {
  // TODO: deposit display to js
  return FAKE_UIMS_REPLY;
}

uims_reply_thing iofull::get_resolve_command() { return FAKE_UIMS_REPLY; }

popup_return iofull::get_popup_string(Cstring prompt1, Cstring prompt2,
                                      Cstring final_inline_prompt, Cstring seed,
                                      char *dest) {
  return POPUP_ACCEPT;
}

selector_kind iofull::do_selector_popup(matcher_class &matcher) {
  return selector_uninitialized;
}

direction_kind iofull::do_direction_popup(matcher_class &matcher) {
  return direction_uninitialized;
}

int iofull::do_circcer_popup() { return 0; }

int iofull::do_tagger_popup(int tagger_class) { return 0; }

int iofull::yesnoconfirm(Cstring title, Cstring line1, Cstring line2, bool excl,
                         bool info) {
  return 0;
}

uint32_t iofull::get_one_number(matcher_class &matcher) { return 0; }

// these should never get called:
bool iofull::choose_font() { return false; }
bool iofull::print_this() { return false; }
bool iofull::print_any() { return false; }
bool iofull::help_manual() { return false; }
bool iofull::help_faq() { return false; }
void iofull::dispose_of_abbreviation(const char *linebuff) {}
void iofull::display_help() {}
void iofull::terminate(int code) {}
void iofull::process_command_line(int *argcp, char ***argvp) {}
void iofull::bad_argument(Cstring s1, Cstring s2, Cstring s3) {}

// these should do nothing (modulo debugging):
void iofull::set_pick_string(Cstring string) {}
void iofull::add_new_line(const char the_line[], uint32_t drawing_picture) {
  printf("%s\n", the_line);
}
void iofull::no_erase_before_n(int n) {}
void iofull::reduce_line_count(int n) {}
void iofull::set_window_title(char s[]) {}
void iofull::prepare_for_listing() {}
void iofull::create_menu(call_list_kind cl) {}
void iofull::final_initialize() {}
