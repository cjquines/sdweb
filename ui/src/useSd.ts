import * as Comlink from "comlink";
import { useEffect, useState } from "preact/hooks";
import { Callback, WorkerSD } from "./sdweb";

export const useSD = () => {
  const [inited, setInited] = useState(false);
  const [methods, setMethods] = useState<WorkerSD | null>(null);

  useEffect(() => {
    if (inited) return;
    setInited(true);

    Comlink.wrap<(cb: (sd: Comlink.Remote<WorkerSD>) => void) => void>(
      new Worker(new URL("./sdweb/index.ts", import.meta.url))
    )(
      Comlink.proxy((worker) => {
        const addCallback = (cb: Callback) => {
          let removeCallback = () => true;
          worker.addCallback(Comlink.proxy(cb)).then((destruct) => {
            removeCallback = destruct;
          });
          return removeCallback;
        };
        setMethods({ ...worker, addCallback });
      })
    );
  }, [inited]);

  return methods;
};

export type SD = NonNullable<ReturnType<typeof useSD>>;
