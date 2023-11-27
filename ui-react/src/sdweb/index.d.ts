// the enums are defined in useSd.ts

type SD = {
  init: () => void;
  suspendReason: SuspendReason;
  resumeFn: ((reason: ResumeReason) => void) | null;
  setInput: (str: string) => void;
  choices: string[];
  output: string[];
};

function init(): Promise<SD>;

export default init;
