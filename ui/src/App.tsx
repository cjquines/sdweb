import { Loader } from "./components/Loader";
import { UserInput } from "./components/UserInput";

export function App() {
  return (
    <Loader>
      {(sd) => {
        return (
          <>
            <pre>{sd.output.join("\n")}</pre>
            <UserInput sd={sd} />
          </>
        );
      }}
    </Loader>
  );
}
