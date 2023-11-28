import { useEffect, useState } from "preact/hooks";
import { Loader } from "./components/Loader";
import { UserInput } from "./components/UserInput";
import { SD } from "./useSd";

function AppInner({ sd }: { sd: SD }) {
  const [output, setOutput] = useState<string[]>([]);

  useEffect(() => {
    return sd.addCallback(({ output }) => {
      if (output.length > 0) {
        setOutput(output);
      }
    });
  }, []);

  return (
    <>
      <pre>{output.join("\n")}</pre>
      <UserInput sd={sd} />
    </>
  );
}

export function App() {
  return <Loader>{(sd) => <AppInner sd={sd} />}</Loader>;
}
