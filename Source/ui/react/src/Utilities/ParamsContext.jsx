import {
  createContext,
  useContext,
  useReducer,
  useMemo,
  useEffect,
} from "react";
import { PARAM_DEFAULTS } from "./juceBridge";

const ParamsContext = createContext(null);

function paramsReducer(state, action) {
  switch (action.type) {
    case "LOCAL_SET":
      return { ...state, [action.id]: action.value };

    case "SET_ALL_PARAMS":
      return { ...action.payload };

    case "BATCH_SET":
      return { ...state, ...action.payload };

    default:
      return state;
  }
}

export function ParamsProvider({ initialParams, children }) {
  const [state, dispatch] = useReducer(paramsReducer, initialParams);

  // Memoize the context value to prevent unnecessary re-renders
  const value = useMemo(
    () => ({
      state,
      dispatch,
      setAllParams: (params) =>
        dispatch({ type: "SET_ALL_PARAMS", payload: params }),
    }),
    [state],
  );

  useEffect(() => {
    const handler = (payload) => {
      if (payload && typeof payload === "object") {
        dispatch({ type: "BATCH_SET", payload });
      }
    };

    window.__JUCE__.backend.addEventListener("ParamsUpdate", handler);
    return () =>
      window.__JUCE__.backend.removeEventListener("ParamsUpdate", handler);
  }, []);

  return (
    <ParamsContext.Provider value={value}>{children}</ParamsContext.Provider>
  );
}

export function useParam(id) {
  const { state } = useContext(ParamsContext);

  return [state[id]];
}

export function parseParams(initialData) {
  // Add validation
  if (!Array.isArray(initialData) || !initialData[0]) {
    console.warn("Invalid initialData provided to parseParams");
    return [];
  }

  const xmlString = initialData[0];

  try {
    const parser = new DOMParser();
    const xml = parser.parseFromString(xmlString, "text/xml");

    // Check for parsing errors
    const parserError = xml.querySelector("parsererror");
    if (parserError) {
      console.error("XML parsing error:", parserError.textContent);
      return [];
    }

    const params = xml.querySelectorAll("PARAM");

    return [...params].map((param) => {
      const id = param.getAttribute("id");
      const valueAttr = param.getAttribute("value");

      return {
        id,
        value: valueAttr !== null ? parseFloat(valueAttr) : undefined,
      };
    });
  } catch (error) {
    console.error("Error parsing XML:", error);
    return [];
  }
}
