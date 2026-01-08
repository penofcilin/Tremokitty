// ParamsContext.js
import { createContext, useContext, useReducer } from "react";

const ParamsContext = createContext(null);

function paramsReducer(state, action) {
  switch (action.type) {
    case "SET_ALL_PARAMS":
      return { ...state, ...action.payload };

    case "SET_PARAM":
      return { ...state, [action.id]: action.value };

    default:
      return state;
  }
}

export function ParamsProvider({ initialParams, children }) {
  const [state, dispatch] = useReducer(paramsReducer, initialParams);

  return (
    <ParamsContext.Provider value={{ state, dispatch }}>
      {children}
    </ParamsContext.Provider>
  );
}

export function useParam(id) {
  const { state, dispatch } = useContext(ParamsContext);

  return [state[id], (value) => dispatch({ type: "SET_PARAM", id, value })];
}
