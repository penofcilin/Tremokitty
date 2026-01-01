import { Box, Flex, Heading, Separator } from "@radix-ui/themes";
import "./FilterSection.css";

export default function FilterSection({ style, bypassed, toggleBypass }) {
  return (
    <Flex
      width="525px"
      height="185px"
      style={{ style }}
      data-bypassed={bypassed ? "" : undefined}
      className="filterSection"
    ></Flex>
  );
}
