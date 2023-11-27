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
