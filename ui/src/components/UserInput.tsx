import { useState } from "preact/hooks";
import { useSD } from "../useSd";

const debounce = (callback: (arg: string) => void, waitMS: number) => {
  let timeout: number;
  return (arg: string) => {
    clearTimeout(timeout);
    timeout = window.setTimeout(() => {
      callback(arg);
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

  const updateChoices = debounce((inputValue) => {
    sd.onInputChange(inputValue);
  }, 500);

  return (
    <form
      onSubmit={(e) => {
        e.preventDefault();
        sd.onInputSubmit(inputValue);
        setInputValue("");
      }}
    >
      <div>
        <label>command:</label>
        <div>
          <input
            value={inputValue}
            onInput={(e) => {
              const inputValue = (e.target as HTMLInputElement).value;
              setInputValue(inputValue);
              setLoadingChoices(true);
              updateChoices(inputValue);
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
