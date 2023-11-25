addToLibrary({
  Suspend: function () {
    return Asyncify.handleSleep((resume) => {
      globalThis._resume_fn = resume;
    });
  },
  // Resume: function () {
  //   const resume = Module._resume_fn;
  //   Module._resume_fn = null;
  //   resume();
  // },
});
