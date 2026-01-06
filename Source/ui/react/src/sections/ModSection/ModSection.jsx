import { useState, useEffect } from "react";
import { Box, Flex, Heading, Separator } from "@radix-ui/themes";
import "./ModSection.css";

export default function ModSection({ style, bypassed, toggleBypass }) {
  const [lfoPosition, setLfoPosition] = useState(0);

  //LFO Updates
  useEffect(() => {
    const handler = (v) => {
      setLfoPosition(v);
    };

    window.__JUCE__.backend.addEventListener("ModLFOUpdate", handler);

    return () => {
      window.__JUCE__.backend.removeEventListener("ModLFOUpdate", handler);
    };
  }, []);

  return (
    <Flex
      width="161px"
      height="502px"
      style={{ style }}
      data-bypassed={bypassed ? "" : undefined}
      className="modSection"
    ></Flex>
  );
}
