// this code is intended to be used through a web worker!
import createSD from "./sdweb";
import { ResumeReason, SuspendReason, WorkerReq, WorkerRes } from "./types";

const getOnRequest = async () => {
  let progress = 0;

  let res: WorkerRes = {
    type: "suspend",
    choices: [],
    output: [],
    progress: 0,
    reason: 0,
  };

  const sd = await createSD();

  const pong = () => {
    postMessage(res);
  };

  const suspend = (reason: SuspendReason) => {
    const { choices = [], output = [] } = sd;
    sd.output = [];

    if (reason === SuspendReason.DB_PROGRESS) {
      progress += 1;
      setTimeout(() => resume(ResumeReason.PROGRESS_ACK), 0);
    }

    res = {
      type: "suspend",
      choices,
      output,
      progress,
      reason,
    };

    pong();
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

  const onRequest = (req: WorkerReq) => {
    switch (req.type) {
      case "ping": {
        pong();
        break;
      }
      case "input": {
        sd.setInput(req.input);
        resume(ResumeReason.TYPE_CHAR);
        break;
      }
      case "submit": {
        sd.setInput(req.input);
        resume(ResumeReason.SUBMIT);
        break;
      }
      default: {
        const unknown: never = req;
        console.error("unknown req:", unknown);
      }
    }
  };

  sd.init();
  resume(ResumeReason.PROGRESS_ACK);

  return onRequest;
};

let onRequest: null | ((req: WorkerReq) => void) = null;
const queue: WorkerReq[] = [];

getOnRequest().then((result) => {
  onRequest = result;
  queue.forEach((req) => result(req));
});

onmessage = (e: MessageEvent<WorkerReq>) => {
  if (onRequest) {
    onRequest(e.data);
  } else {
    queue.push(e.data);
  }
};
