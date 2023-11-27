import { ReactNode, useState } from "react";
import { ResumeReason, SD, SuspendReason, useSD } from "../useSd";

function DatabaseLoader({
  children,
  sd,
}: {
  children: (sd: SD) => ReactNode;
  sd: SD;
}) {
  const [progress, setProgress] = useState(-1);
  const loaded = sd.suspendReason !== SuspendReason.DB_PROGRESS;

  if (!loaded) {
    setProgress(progress + 1);
    sd.resumeFn(ResumeReason.PROGRESS_ACK);
  }

  return loaded ? (
    children(sd)
  ) : (
    <p>initializing database... ({progress} of 10)</p>
  );
}

export function Loader({ children }: { children: (sd: SD) => ReactNode }) {
  const { sd } = useSD();

  return sd ? (
    <DatabaseLoader children={children} sd={sd} />
  ) : (
    <p>downloading sd...</p>
  );
}
