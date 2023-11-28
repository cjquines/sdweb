// this code is intended to be used through a web worker!
import * as Comlink from "comlink";
import createSD from "./sdweb";

export type WasmSD = Awaited<ReturnType<typeof createSD>>;

export enum SuspendReason {
  DB_PROGRESS,
  STARTUP_COMMAND,
  CALL_COMMAND,
  RESOLVE_COMMAND,
  SELECTOR_POPUP,
  DIRECTION_POPUP,
  CIRCCER_POPUP,
  TAGGER_POPUP,
  GET_ONE_NUMBER,
}

export enum ResumeReason {
  PROGRESS_ACK,
  TYPE_CHAR,
  SUBMIT,
}

// callbacks should not call init or resumeFn
export type Callback = (
  sd: {
    choices: string[];
    output: string[];
    progress: number;
  },
  reason: SuspendReason
) => void;

const getMethods = async () => {
  let callbackId = 0;
  const callbacks = new Map<number, Callback>();
  let progress = 0;
  const sd = await createSD();

  const addCallback = (cb: Callback) => {
    console.log("callback added");
    const id = callbackId++;
    callbacks.set(id, cb);
    return () => callbacks.delete(id);
  };

  const suspend = (reason: SuspendReason) => {
    const { choices, output } = sd;

    if (reason === SuspendReason.DB_PROGRESS) {
      progress += 1;
      setTimeout(() => resume(ResumeReason.PROGRESS_ACK), 0);
    }

    const cbObj = { choices, output, progress };
    callbacks.forEach((cb) => cb(cbObj, reason));
  };

  const resume = (reason: ResumeReason) => {
    const resumeFn = sd.resumeFn;
    if (!resumeFn) {
      console.warn("failed to resume:", reason);
      return;
    }
    sd.resumeFn = null;
    resumeFn(reason);
    setTimeout(() => suspend(sd.suspendReason), 0);
  };

  const onInputChange = (input: string) => {
    sd.setInput(input);
    resume(ResumeReason.TYPE_CHAR);
  };

  const onInputSubmit = (input: string) => {
    sd.setInput(input);
    resume(ResumeReason.SUBMIT);
  };

  sd.init();
  resume(ResumeReason.PROGRESS_ACK);

  return {
    addCallback,
    onInputChange,
    onInputSubmit,
  };
};

export type WorkerSD = Awaited<ReturnType<typeof getMethods>>;

let callback: null | ((sd: WorkerSD) => void) = null;
let sd: null | WorkerSD = null;

const setCb = (cb: (sd: WorkerSD) => void) => {
  console.log("here");
  if (sd) {
    cb(sd);
  } else {
    callback = cb;
  }
};

Comlink.expose(setCb);

getMethods().then((methods) => {
  console.log("herex");
  if (callback) {
    callback(methods);
  } else {
    sd = methods;
  }
});
