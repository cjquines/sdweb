// can't be bothered setting up proper imports...
#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>
#include <emscripten/emscripten.h>
#else
#include "emscripten/bind.h"
#include "emscripten/emscripten.h"
#endif

#include "../sd.h"

// keep enums and functions in sync with ui
enum suspend_reason {
  DB_PROGRESS,
  STARTUP_COMMAND,
  CALL_COMMAND,
  RESOLVE_COMMAND,
  SELECTOR_POPUP,
  DIRECTION_POPUP,
  CIRCCER_POPUP,
  TAGGER_POPUP,
  GET_ONE_NUMBER,
};

enum resume_reason {
  PROGRESS_ACK,
  TYPE_CHAR,
  SUBMIT,
};

suspend_reason last_suspend;
resume_reason last_resume;

EM_JS(int, Suspend_, (int reason), {
  Module.suspendReason = reason;
  Module.resumeFn = function(value) {
    setTimeout(
        function() { Module.resumeFn_(value); }, 0);
  };
  return Asyncify.handleSleep(
      function(resumeFn) { Module.resumeFn_ = resumeFn; });
});

resume_reason Suspend(suspend_reason reason) {
  last_suspend = reason;
  last_resume = (resume_reason)Suspend_(last_suspend);
  return last_resume;
}

EM_JS(void, SetUserInputPtr, (char *m_user_input, int *length), {
  Module.setInput = function(str) {
    stringToUTF8(str, m_user_input, 400);
    setValue(length, str.length, "i32");
}
});

EM_JS(void, ResetChoices, (), { Module.choices = []; });

EM_JS(void, AddChoice, (char *choice),
      { Module.choices.push(UTF8ToString(choice)); });

EM_JS(void, QueueOutput, (const char *line), {
  if (!Module.output) {
    Module.output = [];
  }
  Module.output.push(UTF8ToString(line));
});

// these names are exported and can't get mangled
extern "C" {
void init() {
  iofull ggg;
  sdmain(0, nullptr, ggg);
}
}

namespace emscripten {
EMSCRIPTEN_BINDINGS(sdweb) { function("init", &init); }
} // namespace emscripten

// iofull implementation. mostly fake, but some of it's real

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
    Suspend(DB_PROGRESS);
    break;
  }
  return false;
}

// output functions
void iofull::fatal_error_exit(int code, Cstring s1, Cstring s2) {
  emscripten_log(EM_LOG_ERROR, "%s: %s", s1, s2);
  general_final_exit(code);
}

void iofull::serious_error_print(Cstring s1) {
  emscripten_log(EM_LOG_ERROR, s1);
}

void iofull::add_new_line(const char the_line[], uint32_t drawing_picture) {
  // TODO: interface with js
  QueueOutput(the_line);
}

void iofull::update_resolve_menu(command_kind goal, int cur, int max,
                                 resolver_display_state state) {
  // TODO: interface with js
}

void iofull::show_match() { AddChoice(gg77->matcher_p->m_full_extension); }

int get_matcher_which(suspend_reason reason) {
  switch (reason) {
  case DB_PROGRESS:
    // shouldn't happen, just return something...
    return matcher_class::e_match_startup_commands;
  case STARTUP_COMMAND:
    return matcher_class::e_match_startup_commands;
  case CALL_COMMAND:
    return parse_state.call_list_to_use;
  case RESOLVE_COMMAND:
    return matcher_class::e_match_resolve_commands;
  case SELECTOR_POPUP:
    return matcher_class::e_match_selectors;
  case DIRECTION_POPUP:
    return matcher_class::e_match_directions;
  case CIRCCER_POPUP:
    return matcher_class::e_match_circcer;
  case TAGGER_POPUP:
    return matcher_class::e_match_taggers;
  case GET_ONE_NUMBER:
    return matcher_class::e_match_number;
  }
}

// input functions
uims_reply_thing get_user_input(suspend_reason reason, int tagger_class = 0) {
  matcher_class &matcher = *gg77->matcher_p;

  int which = get_matcher_which(reason) + tagger_class;

  matcher.erase_matcher_input();
  SetUserInputPtr(matcher.m_user_input, &(matcher.m_user_input_size));

  do {
    ResetChoices();
    matcher.match_user_input(which, true, true, false);
  } while (Suspend(reason) == TYPE_CHAR);

  matcher.match_user_input(which, false, true, true);

  // hope there's a match!
  modifier_block &matchmatch = matcher.m_final_result.match;
  uims_reply_kind kind = matchmatch.kind;
  int index = matchmatch.index;

  if (reason == SELECTOR_POPUP || reason == DIRECTION_POPUP ||
      reason == TAGGER_POPUP || reason == CIRCCER_POPUP ||
      reason == GET_ONE_NUMBER) {
    // don't process index ourselves
  } else if (kind == ui_command_select) {
    index = (int)command_command_values[index];
  } else if (kind == ui_resolve_select) {
    index = (int)resolve_command_values[index];
  } else if (kind == ui_start_select) {
    index = (int)startup_command_values[index];
  } else if (kind == ui_concept_select || kind == ui_call_select) {
    call_conc_option_state save_stuff = matchmatch.call_conc_options;
    there_is_a_call = false;
    bool retval =
        deposit_call_tree(&matchmatch, (parse_block *)0,
                          DFM1_CALL_MOD_MAND_ANYCALL / DFM1_CALL_MOD_BIT);
    matchmatch.call_conc_options = save_stuff;
    if (there_is_a_call) {
      parse_state.topcallflags1 = the_topcallflags;
      kind = ui_call_select;
    }
    if (retval) {
      kind = ui_user_cancel;
    }
  }

  return uims_reply_thing(kind, index);
}

uims_reply_thing iofull::get_startup_command() {
  return get_user_input(STARTUP_COMMAND);
}

uims_reply_thing iofull::get_call_command() {
  return get_user_input(CALL_COMMAND);
}

uims_reply_thing iofull::get_resolve_command() {
  return get_user_input(RESOLVE_COMMAND);
}

int iofull::do_abort_popup() { return POPUP_ACCEPT; }

popup_return iofull::get_popup_string(Cstring prompt1, Cstring prompt2,
                                      Cstring final_inline_prompt, Cstring seed,
                                      char *dest) {
  return POPUP_ACCEPT;
}

selector_kind iofull::do_selector_popup(matcher_class &matcher) {
  match_result saved_match = matcher.m_final_result;

  get_user_input(SELECTOR_POPUP);
  selector_kind retval =
      (selector_kind)(matcher.m_final_result.match.index + 1);

  matcher.m_final_result = saved_match;
  return retval;
}

direction_kind iofull::do_direction_popup(matcher_class &matcher) {
  match_result saved_match = matcher.m_final_result;

  get_user_input(SELECTOR_POPUP);
  direction_kind retval =
      (direction_kind)(matcher.m_final_result.match.index + 1);

  matcher.m_final_result = saved_match;
  return retval;
}

int iofull::do_circcer_popup() {
  uint32_t retval;
  matcher_class &matcher = *gg77->matcher_p;

  if (interactivity == interactivity_verify) {
    retval = verify_options.circcer;
    if (retval == 0)
      retval = 1;
  } else if (!matcher.m_final_result.valid ||
             (matcher.m_final_result.match.call_conc_options.circcer == 0)) {
    match_result saved_match = matcher.m_final_result;

    get_user_input(CIRCCER_POPUP);
    retval = matcher.m_final_result.match.call_conc_options.circcer;

    matcher.m_final_result = saved_match;
  } else {
    retval = matcher.m_final_result.match.call_conc_options.circcer;
    matcher.m_final_result.match.call_conc_options.circcer = 0;
  }

  return 0;
}

int iofull::do_tagger_popup(int tagger_class) {
  matcher_class &matcher = *gg77->matcher_p;
  match_result saved_match = matcher.m_final_result;

  get_user_input(TAGGER_POPUP, tagger_class);
  int retval = matcher.m_final_result.match.call_conc_options.tagger;

  saved_match.match.call_conc_options.tagger =
      matcher.m_final_result.match.call_conc_options.tagger;
  matcher.m_final_result = saved_match;
  matcher.m_final_result.match.call_conc_options.tagger = 0;
  return retval;
}

int iofull::yesnoconfirm(Cstring title, Cstring line1, Cstring line2, bool excl,
                         bool info) {
  return 0;
}

uint32_t iofull::get_one_number(matcher_class &matcher) {
  match_result saved_match = matcher.m_final_result;

  get_user_input(GET_ONE_NUMBER);
  uint32_t retval = matcher.m_final_result.match.index;

  matcher.m_final_result = saved_match;
  return retval;
}

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
void iofull::no_erase_before_n(int n) {}
void iofull::reduce_line_count(int n) {}
void iofull::set_window_title(char s[]) {}
void iofull::prepare_for_listing() {}
void iofull::create_menu(call_list_kind cl) {}
void iofull::final_initialize() {}
