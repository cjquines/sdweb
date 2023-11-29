import { ComponentChildren } from "preact";
import { useState } from "preact/hooks";
import { SuspendReason } from "../sdweb/types";
import { useSD } from "../useSd";

export function Loader({ children }: { children: ComponentChildren }) {
  const [progress, setProgress] = useState(-1);
  const [dbLoaded, setDbLoaded] = useState(false);

  useSD(({ progress, reason }) => {
    const dbLoaded = reason !== SuspendReason.DB_PROGRESS;
    setDbLoaded(dbLoaded);
    setProgress(progress);
    return dbLoaded;
  });

  if (dbLoaded) {
    return <>{children}</>;
  }
  if (progress >= 0) {
    return <p>initializing database... ({progress} of 20)</p>;
  }
  return <p>downloading sd...</p>;
}
