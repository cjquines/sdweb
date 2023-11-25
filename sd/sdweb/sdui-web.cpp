#include "sdui-web.h"

// these names are exported and can't get mangled
extern "C" {

void init() {
  iofull ggg;

  enable_file_writing = false;
  interactivity = interactivity_database_init;
  testing_fidelity = false;
  header_comment[0] = 0;
  abridge_filename[0] = 0;
  verify_options.who.who[0] = selector_uninitialized;
  verify_options.who.who[1] = selector_uninitialized;
  verify_options.who.who[2] = selector_uninitialized;
  verify_options.number_fields = 0;
  verify_options.howmanynumbers = 0;
  history_allocation = 15;
  configuration::history = new configuration[history_allocation];
  global_cache_failed_flag = false;

  matcher_class global_matcher;
  ui_utils thingy(&global_matcher, ggg);
  gg77 = &thingy;
  ggg.set_utils_ptr(gg77);

  open_session(0, nullptr);

  no_erase_before_this = 0;
  global_error_flag = (error_flag_type)0;
  interactivity = interactivity_normal;

  new_sequence();
}

void new_sequence() {
  if (clipboard_size == 0)
    release_parse_blocks_to_mark((parse_block *)0);
  // wait for startup command
  // (or for select command)
}

void on_startup_command(uint16_t minorpart) {
  global_reply = uims_reply_thing(ui_start_select, minorpart);

  configuration::initialize_history(minorpart);
  configuration::history[1].init_warnings_specific();
  configuration::history[1].init_resolve();
  configuration::history[1].state =
      *configuration::history[1].get_startinfo_specific()->the_setup_p;
  two_couple_calling = (attr::klimit(configuration::history[1].state.kind) < 4);
  configuration::history[1].state_is_valid = true;

  written_history_items = -1;
  no_erase_before_this = 1;

  global_error_flag = (error_flag_type)0;
  start_cycle();
}

void redisplay() {
  error_flag_type old_error_flag;
  bool refresh_override = false;
  int concepts_deposited = 0;

  // We are operating in interactive mode.  Update the display and query the
  // user. First, update the output area to the current state, with error
  // messages, etc. We draw a picture for the last two calls.

  // Some things can only be done if we have a "nice" execution error.
  // Otherwise, the history array won't have the "last setup" that we want.
  // But if we are doing a "refresh display", do it anyway.

  if (refresh_override || global_error_flag < error_flag_wrong_command ||
      global_error_flag == error_flag_selector_changed ||
      global_error_flag == error_flag_formation_changed) {
    gg77->display_initial_history(config_history_ptr,
                                  (ui_options.diagnostic_mode ? 1 : 2));

    if (configuration::sequence_is_resolved()) {
      gg77->newline();
      gg77->writestuff("     resolve is:");
      gg77->newline();
      gg77->write_resolve_text(false);
      gg77->newline();
    }
  }

  if (global_error_flag && global_error_flag < error_flag_wrong_command) {
    gg77->writestuff("Can't do this call:");
    gg77->newline();
    gg77->write_history_line(0, false, false, file_write_no);
  }

  if (global_error_flag) {
    switch (global_error_flag) {
    case error_flag_wrong_resolve_command:
    case error_flag_1_line:
    case error_flag_no_retry:
      // Commonplace error message.
      gg77->writestuff(error_message1);
      break;
    case error_flag_2_line:
      gg77->writestuff(error_message1);
      gg77->newline();
      gg77->writestuff("   ");
      gg77->writestuff(error_message2);
      break;
    case error_flag_collision:
      // Very special message -- no text here, two people collided
      // and they are stored in collision_person1 and collision_person2.
      gg77->writestuff("Some people (");
      gg77->print_error_person(collision_person1, false);
      gg77->writestuff(" and ");
      gg77->print_error_person(collision_person2, true);
      gg77->writestuff(") on same spot.");
      break;
    case error_flag_cant_execute:
      // Very special message -- given text is to be prefixed with description
      // of the perpetrator, who is stored in collision_person1.
      gg77->writestuff("Some person (");
      gg77->print_error_person(collision_person1, true);
      gg77->writestuff(") ");
      gg77->writestuff(error_message1);
      break;
    case error_flag_selector_changed:
      gg77->writestuff("Warning -- person identifiers were changed.");
      break;
    case error_flag_formation_changed:
      gg77->writestuff("Warning -- the formation has changed.");
      break;
    case error_flag_wrong_command:
      gg77->writestuff("You can't select that here.");
      break;
    }

    gg77->newline();
    gg77->newline();
  }

  old_error_flag = global_error_flag; /* save for refresh command */
  global_error_flag = error_flag_none;

  if (clipboard_size != 0) {
    int j;

    gg77->writestuff("............................");
    gg77->newline();

    /* Display at most 3 lines. */
    for (j = clipboard_size - 1; j >= 0 && j >= clipboard_size - 3; j--) {
      gg77->writestuff("      ");
      gg77->print_recurse(clipboard[j].command_root, 0);
      gg77->newline();
    }

    gg77->writestuff("............................");
    gg77->newline();
  }

  // Display the call index number, and the partially entered call and/or
  // prompt, as appropriate.

  // See if there are partially entered concepts.  If so, print the index
  // number and those concepts on a separate line.

  if (parse_state.concept_write_ptr !=
      &configuration::next_config().command_root) {

    // This prints the concepts entered so far, with a "header"
    // consisting of the index number.  This partial concept tree
    // is incomplete, so write_history_line has to be (and is) very careful.
    // By giving a third argument ("leave_missing_calls_blank") of true,
    // subcalls that haven't yet been filled in will be left blank
    // rather than showing as "***".  So, after typing in
    // "tally ho but <anything>", it will echo as just "tally ho but"
    // while it is prompting us for the subcall.
    //
    // If we get through the whole call without ever filling in the
    // subcall, as in "no one do your part, tally ho but <anything>",
    // the final result will show the missing subcall as
    // "no one do your part, tally ho but [???]"

    gg77->write_history_line(config_history_ptr + 1, false, true,
                             file_write_no);
  } else {
    // No partially entered concepts.  Just do the sequence number.

    if (!ui_options.diagnostic_mode) {
      char indexbuf[200];
      sprintf(indexbuf, "%2d:",
              config_history_ptr - configuration::whole_sequence_low_lim + 2);
      gg77->writestuff(indexbuf);
      gg77->newline();
    }
  }

  if (parse_state.specialprompt[0] != '\0') {
    gg77->writestuff(parse_state.specialprompt);
    gg77->newline();
  }
}

void start_cycle() {
  allowing_modifications = 0;

  if (history_allocation < config_history_ptr + MAX_RESOLVE_SIZE + 2) {
    int new_history_allocation = history_allocation * 2 + 5;
    configuration *new_history = new configuration[new_history_allocation];
    memcpy(new_history, configuration::history,
           history_allocation * sizeof(configuration));
    delete[] configuration::history;
    history_allocation = new_history_allocation;
    configuration::history = new_history;
  }

  initialize_parse();

  if ((config_history_ptr == 1) &&
      configuration::history[1].get_startinfo_specific()->into_the_middle)
    deposit_concept(&concept_centers_concept);

  // query_for_call would print things here
  // FUCK do we need to edit query_for_call? because it's called elsewhere?
  //   probably, we need to split it before on_call_command
  // maybe we should have a redisplay() here
  redisplay();
  // wait for call command (check_menu())
  // (or for select command)
}

void on_call_command(uint16_t minorpart) {
  // TODO: query_for_call() returning false should go here
  global_reply = uims_reply_thing(ui_call_select, minorpart);
  // actually it discards global_reply on line 915? so this shouldn't matter
  // in fact minorpart isn't used at all ?
  // so get_call_command does all the lifting
  //
  // take the user input, put it into matcher,
  // call matcher.match_user_input, hope that it's a legit final match, deposit
  // matcher.m_final_result.match into call tree, hope it is false, hope
  // there_is_a_call is true? (or use matcher.m_final_result.kind?)
  //
  // this is rough, as deposit_call() can call things like do_selector_popup
  //   --> so that this doesn't happen, we should only ever call things that
  //   won't trigger popups. this means our choices should be what results
  //   after "typing" '!', right?
  //   --> but this *can* trigger a do_subcall_query
  //   which i think is fine? we still do a toplevelmove, and finish it

  try {
    toplevelmove();
    finish_toplevelmove();
    config_history_ptr++;
    return start_cycle();
  } catch (error_flag_type sss) {
    global_error_flag = sss;
    got_an_exception();
  }
}

void got_an_exception() {}
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

// these shouldn't be called, but we have to give silly default values:
int iofull::do_abort_popup() { return POPUP_ACCEPT; }

const uims_reply_thing FAKE_UIMS_REPLY =
    uims_reply_thing(ui_command_select, command_quit);
uims_reply_thing iofull::get_startup_command() { return FAKE_UIMS_REPLY; }
uims_reply_thing iofull::get_resolve_command() { return FAKE_UIMS_REPLY; }
uims_reply_thing iofull::get_call_command() { return FAKE_UIMS_REPLY; }

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

// these should do nothing (modulo debugging):
void iofull::set_pick_string(Cstring string) {}
void iofull::add_new_line(const char the_line[], uint32_t drawing_picture) {
  printf("%s", the_line);
}
void iofull::no_erase_before_n(int n) {}
void iofull::reduce_line_count(int n) {}
void iofull::set_window_title(char s[]) {}
void iofull::prepare_for_listing() {}
void iofull::show_match() {}
void iofull::create_menu(call_list_kind cl) {}
void iofull::final_initialize() {}
