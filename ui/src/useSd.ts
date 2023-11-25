import { useState } from "react";

import init from "./sdweb";
type Module = Awaited<ReturnType<typeof init>>;

const useSd = () => {
  const [module, setModule] = useState<Module | null>(null);

  init().then((m) => {
    !module && setModule(m);
  });

  return module;
};

export default useSd;
