import { useEffect } from "preact/hooks";
import { WorkerReq, WorkerRes } from "./sdweb/types";

const worker = new Worker(new URL("./sdweb/worker.ts", import.meta.url));
const sendReq = (req: WorkerReq) => worker.postMessage(req);

// callback returns true to be removed
// the callback should only close over stable values!
export const useSD = (callback?: (res: WorkerRes) => boolean | void) => {
  useEffect(() => {
    const removeCb = () => worker.removeEventListener("message", cb);

    const cb = (e: MessageEvent<WorkerRes>) => {
      if (callback?.(e.data)) {
        removeCb();
      }
    };

    worker.addEventListener("message", cb);

    return removeCb;
  }, []);

  const onInputChange = (input: string) => sendReq({ type: "input", input });

  const onInputSubmit = (input: string) => sendReq({ type: "submit", input });

  return { onInputChange, onInputSubmit };
};
