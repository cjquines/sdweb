import { useEffect, useState } from "react";

import init from "./sdweb";

export type SD = Awaited<ReturnType<typeof init>>;

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

export const useSD = () => {
  const [inited, setInited] = useState(false);
  const [sd, setSD] = useState<SD | null>(null);

  init().then((m) => {
    if (!sd) {
      setSD(m);
    }
  });

  useEffect(() => {
    if (inited || !sd) return;
    setInited(true);
    sd.init();
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    (window as any).sd = sd;
  }, [inited, sd]);

  return { sd: inited ? sd : null };
};

export const useSDMethods = (sd: SD) => {
  const [choices, setChoices] = useState(sd.choices);
  const [firstAck, setFirstAck] = useState(false);
  const [output, setOutput] = useState(sd.output);
  const [progress, setProgress] = useState(0);
  const [suspendReason, setSuspendReason] = useState(sd.suspendReason);

  const onSuspend = (reason: SuspendReason) => {
    setChoices(sd.choices);
    if (sd.output?.length > 0) {
      setOutput(sd.output);
      sd.output = [];
    }
    setSuspendReason(reason);

    if (reason === SuspendReason.DB_PROGRESS) {
      setProgress((progress) => progress + 1);
      setTimeout(() => resume(ResumeReason.PROGRESS_ACK), 0);
    }
  };

  const resume = (reason: ResumeReason) => {
    const resumeFn = sd.resumeFn;
    if (!resumeFn) {
      console.warn("failed to resume:", reason);
      return;
    }
    sd.resumeFn = null;
    resumeFn(reason);
    setTimeout(() => onSuspend(sd.suspendReason), 0);
  };

  if (!firstAck) {
    setFirstAck(true);
    resume(ResumeReason.PROGRESS_ACK);
  }

  const onInputChange = (input: string) => {
    sd.setInput(input);
    resume(ResumeReason.TYPE_CHAR);
  };

  const onInputSubmit = (input: string) => {
    sd.setInput(input);
    resume(ResumeReason.SUBMIT);
  };

  return {
    choices,
    onInputChange,
    onInputSubmit,
    output,
    progress,
    resume,
    suspendReason,
  };
};

export type SDMethods = ReturnType<typeof useSDMethods>;
