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

export type WorkerReq =
  | { type: "input"; input: string }
  | { type: "submit"; input: string };

export type WorkerRes = {
  type: "suspend";
  choices: string[];
  output: string[];
  progress: number;
  reason: SuspendReason;
};
