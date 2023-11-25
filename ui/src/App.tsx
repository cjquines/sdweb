import { useEffect, useRef, useState } from "react";

import useSd from "./useSd";

function App() {
  const [inited, setInited] = useState(false);
  const [output, setOutput] = useState<string[]>(["loading"]);
  const [choices, setChoices] = useState<string[]>([]);
  const flush = useRef<(resp: string) => void>();
  const Module = useSd();

  useEffect(() => {
    if (inited || !Module) return;
    setInited(true);

    Module.init();
    setOutput(Module.output);
    setChoices(Module.choices);
    Module.output = [];

    flush.current = (resp) => {
      Module.write_to_user_input(resp);
      Module._resume_fn();
      setOutput(Module.output);
      setChoices(Module.choices);
      Module.output = [];
    };
  }, [inited, Module]);

  return (
    <>
      <pre>{output.join("\n")}</pre>
      <form
        onSubmit={(e) => {
          e.preventDefault();
          flush.current?.(
            String(new FormData(e.target as HTMLFormElement).get("input"))
          );
        }}
      >
        <select name="input">
          {choices.map((choice) => (
            <option key={choice} value={choice}>
              {choice}
            </option>
          ))}
        </select>
        <button type="submit">submit</button>
      </form>
    </>
  );
}

export default App;
