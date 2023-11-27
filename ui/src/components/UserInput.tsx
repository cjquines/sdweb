import { useState } from "preact/hooks";
import { SDMethods } from "../useSd";

export function UserInput({
  choices,
  onInputChange,
  onInputSubmit,
}: Pick<SDMethods, "choices" | "onInputChange" | "onInputSubmit">) {
  const [inputValue, setInputValue] = useState<string>("");

  return (
    <form
      onSubmit={(e) => {
        e.preventDefault();
        onInputSubmit(inputValue);
      }}
    >
      <div>
        <label>command:</label>
        <div>
          <input
            value={inputValue}
            onInput={(e) => {
              setInputValue((e.target as HTMLInputElement).value);
              onInputChange((e.target as HTMLInputElement).value);
            }}
          />
        </div>
        <button type="submit">submit</button>
      </div>
      <ul>
        {choices.slice(0, 5).map((item, index) => (
          <li key={index}>
            <span>{item}</span>
          </li>
        ))}
      </ul>
    </form>
  );
}
