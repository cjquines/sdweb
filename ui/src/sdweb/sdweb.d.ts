// the enums are defined in index.ts

type WasmSD = {
  // or, WaSD
  init: () => void;
  suspendReason: SuspendReason;
  resumeFn: ((reason: ResumeReason) => void) | null;
  setInput: (str: string) => void;
  choices: string[];
  output: string[];
};

function createSD(): Promise<WasmSD>;

export default createSD;
