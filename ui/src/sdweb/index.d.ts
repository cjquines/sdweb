type Module = {
  init: () => void;
  _resume_fn: () => void;
  write_to_user_input: (str: string) => void;
  choices: string[];
  output: string[];
};

function init(): Promise<Module>;

export default init;
