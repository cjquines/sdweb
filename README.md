# sdweb

the sd folder should be a near verbatim copy of sd source, except for:
- .clang-format
- sdweb/

to build, cd `build` and run `emmake make`, then run `node sdweb.js`

## design

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
sdui-web uses, through sdui-web-library.js, these functions:

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
