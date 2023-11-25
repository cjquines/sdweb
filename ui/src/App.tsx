import { useEffect, useRef, useState } from "react";

import init from "./sdweb";

function App() {
  const [choices, setChoices] = useState<string[]>(["loading"]);
  const [input, setInput] = useState("");
  const flush = useRef<(resp: string) => void>();

  useEffect(() => {
    init().then((Module) => {
      Module.init();
      setChoices(Module.choices);

      flush.current = (resp) => {
        Module.write_to_user_input(resp);
        Module._resume_fn();
        setChoices(Module.choices);
      };
    });
  }, []);

  return (
    <>
      <div>{choices.join(",")}</div>
      <form
        onSubmit={(e) => {
          e.preventDefault();
          flush.current?.(input);
          setInput("");
        }}
      >
        <input value={input} onChange={(e) => setInput(e.target.value)} />
      </form>
    </>
  );
}

export default App;
