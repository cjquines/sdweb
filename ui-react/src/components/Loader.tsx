import { ReactNode } from "react";
import { SD, SDMethods, SuspendReason, useSD, useSDMethods } from "../useSd";

function DatabaseLoader({
  children,
  sd,
}: {
  children: (methods: SDMethods) => ReactNode;
  sd: SD;
}) {
  const methods = useSDMethods(sd);
  const { progress, suspendReason } = methods;
  const loaded = suspendReason !== SuspendReason.DB_PROGRESS;

  return loaded ? (
    children(methods)
  ) : (
    <p>initializing database... ({progress} of 10)</p>
  );
}

export function Loader({
  children,
}: {
  children: (methods: SDMethods) => ReactNode;
}) {
  const { sd } = useSD();

  return sd ? (
    <DatabaseLoader children={children} sd={sd} />
  ) : (
    <p>downloading sd...</p>
  );
}
