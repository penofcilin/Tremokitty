import { Box, Flex, Heading, Separator } from "@radix-ui/themes";
import "./ModSection.css";

export default function ModSection({ style, bypassed, toggleBypass }) {
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
