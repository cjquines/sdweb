import { ComponentChildren } from "preact";
import { useEffect, useState } from "preact/hooks";
import { SuspendReason } from "../sdweb";
import { SD, useSD } from "../useSd";

export function Loader({
  children,
}: {
  children: (sd: SD) => ComponentChildren;
}) {
  const sd = useSD();
  const [progress, setProgress] = useState(0);
  const [dbLoaded, setDbLoaded] = useState(false);

  useEffect(() => {
    if (!sd) return;

    const removeCallback = sd.addCallback(({ progress }, reason) => {
      const dbLoaded = reason !== SuspendReason.DB_PROGRESS;
      setDbLoaded(dbLoaded);
      setProgress(progress);
      if (dbLoaded) {
        removeCallback();
      }
    });

    return removeCallback;
  }, [sd]);

  if (sd && dbLoaded) {
    return <>{children(sd)}</>;
  }
  if (sd) {
    return <p>initializing database... ({progress} of 10)</p>;
  }
  return <p>downloading sd...</p>;
}
