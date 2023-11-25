# sdweb

tested with emcc version 3.1.48

the sd folder should be a near verbatim copy of sd source, except for:
- .clang-format
- sdweb/

to build, cd `build` and run `emmake make`, then run `node test.js`

## design

Unlike sdui-tty or sdui-win, sdui-web does not use sdmain or iofull, because
it's incompatible with our design. The main thing is that the C++ code is fully
synchronous, so when JS calls it, it should always run to completion. That means
we can't use any of the iofull input commands, like "get_call_command". Hence,
we must pick up execution after 

Instead, sdui-web uses its own handlers to work with the global state.
Because sdui-web doesn't use sdmain, it has to:

- initialize the globals that sdmain() does
- create its own global matcher
- call open_session()
    - this relies on the global gg77. It doesn't rely on any input from that
    global, so we be as uninformative as possible (except for setting call
    levels, in which we pick c4, then filter it JS-side)
    - make sure we don't use any cache file stuff (in 2023, square dance calls
    are not big data)
    - fill in program options too i guess
- have its own run_program() loop. the run_program call should be split:
    - the trick to simplifying run_program is to ignore most of the commands...
    - there's the program start, printing the banner and (lack of) output file.
    - js will call handle_command() to handle a given command
    - js can call startup_command() to give a startup command and initialize
      configuration and such
    - js then calls initialize_call() to run the label under start_cycle to simple_restart
        - in this loop, js will call methods to deal with get_call_command()
        - js *cannot* partly get a call. for example, what happens when
          "catch <ANYTHING> <N>" is clicked?
        - a good first-level might be getting **query_for_call** matching to work
    - after getting a call, js will call toplevelmove(), possibly getting an
      exception to handle
    - full_resolve() needs to be handled
- call close_session()
- call general_final_exit()

## old design

(abandoned because we can't cooperative multithread)

like sdui-win and sdui-tty, sdui-web defines the iofull methods

(calling c++ in javascript)
sdui-web exposes, through embind, these handlers:

- bool on_input_submit()
    - ENTER_INDEX, ACCEPT_BUTTON_INDEX?
    - the doing_escape branch of check_text_change?
    - returns true if we want to resume (that is, if we set WaitingForCommand = false)
- void on_input_update()
    - the lstrcmp branch of check_text_change
- void on_command(int id)
    - MainWindow_OnCommand, switched on ids
- DisplayType[] fetch_lines()
    - dumps the DisplayType array

(calling javascript in c++)
sdui-web uses, through library_sdweb.js, these functions:

for updating the listbox:
- void ClearChoices()
    - LB_RESETCONTENT
- void AddChoice(Cstring name, uint32_t itemdata)
    - LB_ADDSTRING, LB_SETITEMDATA
- void FinalizeChoices()
    - WM_SETREDRAW, kinda

for handling the textbox:
- char[MAX_TEXT_LINE_LENGTH] GetInputText()
    - GetWindowText
- void SetInputText(char[MAX_TEXT_LINE_LENGTH] text)
    - WM_SETTEXT, then move cursor to end or something...?
    - idk how cursors work with controlled inputs

etc:
- void UpdateStatusBar(Cstring status)
    - UpdateStatusBar... or something
- void RequestPaint()
    - no counterpart: this is to request the js to call fetch_lines
- void Suspend()
    - EnterMessageLoop, kinda; blocks until a command is given
    - cf https://github.com/emscripten-core/emscripten/issues/10515#issuecomment-598960180
    - cf https://emscripten.org/docs/porting/asyncify.html#starting-to-rewind-with-compiled-code-on-the-stack
    - when js calls on_input_submit(), if it returns true, js calls doResume(), which will return from Suspend()
