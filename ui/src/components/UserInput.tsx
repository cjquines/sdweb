import { useState } from "preact/hooks";
import { useSD } from "../useSd";

const debounce = (callback: () => void, waitMS: number) => {
  let timeout: number;
  return () => {
    clearTimeout(timeout);
    timeout = window.setTimeout(() => {
      callback();
    }, waitMS);
  };
};

export function UserInput() {
  const [choices, setChoices] = useState<string[]>([]);
  const [loadingChoices, setLoadingChoices] = useState(false);
  const [inputValue, setInputValue] = useState("");

  const sd = useSD(({ choices }) => {
    setChoices(choices);
    setLoadingChoices(false);
  });

  const updateChoices = debounce(() => {
    sd.onInputChange(inputValue);
  }, 500);

  return (
    <form
      onSubmit={(e) => {
        e.preventDefault();
        sd.onInputSubmit(inputValue);
      }}
    >
      <div>
        <label>command:</label>
        <div>
          <input
            value={inputValue}
            onInput={(e) => {
              setInputValue((e.target as HTMLInputElement).value);
              setLoadingChoices(true);
              updateChoices();
            }}
          />
        </div>
        <button type="submit">submit</button>
      </div>
      {loadingChoices ? "loading new choices..." : ""}
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
