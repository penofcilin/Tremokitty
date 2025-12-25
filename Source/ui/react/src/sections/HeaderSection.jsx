import { Box, Text } from "@radix-ui/themes";
import "../styles/fonts.css";
import "../styles/theme.css";

export default function HeaderSection() {
  return (
    <Box>
      <Box
        px="0"
        style={{
          backgroundColor: "var(--bg-black)",
          width: "100%",
          height: "25px",
          margin: 0,
          padding: 0,

          display: "flex",
          alignItems: "center",
          justifyContent: "center",

          color: "white",
          fontFamily: "var(--font-sans)",
          fontWeight: "bold",
        }}
      >
        Tremokitty!
      </Box>
    </Box>
  );
}
