import { Loader } from "./components/Loader";
import { UserInput } from "./components/UserInput";

export function App() {
  return (
    <Loader>
      {(methods) => {
        const { choices, onInputChange, onInputSubmit, output } = methods;
        return (
          <>
            <pre>{output.join("\n")}</pre>
            <UserInput
              choices={choices}
              onInputChange={onInputChange}
              onInputSubmit={onInputSubmit}
            />
          </>
        );
      }}
    </Loader>
  );
}
