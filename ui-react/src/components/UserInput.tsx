import { useState } from "react";
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
            onChange={(e) => {
              setInputValue(e.target.value);
              onInputChange(e.target.value);
            }}
          />
        </div>
        <button type="submit">submit</button>
      </div>
      <ul>
        {choices.map((item, index) => (
          <li key={index}>
            <span>{item}</span>
          </li>
        ))}
      </ul>
    </form>
  );
}
