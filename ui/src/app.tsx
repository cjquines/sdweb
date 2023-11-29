import { useState } from "preact/hooks";
import { Loader } from "./components/Loader";
import { UserInput } from "./components/UserInput";
import { useSD } from "./useSd";

export function App() {
  const [output, setOutput] = useState<string[]>([]);

  useSD(({ output }) => {
    if (output.length > 0) {
      setOutput(output);
    }
  });

  return (
    <Loader>
      <pre>{output.join("\n")}</pre>
      <UserInput />
    </Loader>
  );
}
