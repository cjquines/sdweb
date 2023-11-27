import { useState } from "react";
import { ResumeReason, SD } from "../useSd";

export function UserInput({ sd }: { sd: SD }) {
  const [inputValue, setInputValue] = useState<string>("");

  return (
    <form
      onSubmit={(e) => {
        e.preventDefault();
        sd.setInput(inputValue);
        sd.resumeFn(ResumeReason.SUBMIT);
      }}
    >
      <div>
        <label>command:</label>
        <div>
          <input
            value={inputValue}
            onChange={(e) => {
              setInputValue(e.target.value);
              sd.setInput(inputValue);
              sd.resumeFn(ResumeReason.TYPE_CHAR);
            }}
          />
        </div>
        <button type="submit">submit</button>
      </div>
      <ul>
        {sd.choices.map((item, index) => (
          <li key={index}>
            <span>{item}</span>
          </li>
        ))}
      </ul>
    </form>
  );
}
