import { Box, IconButton } from "@radix-ui/themes";
import { GearIcon } from "@radix-ui/react-icons";

export default function HeaderSection() {
return (
  <Box
    style={{
      position: "relative",
      backgroundColor: "var(--bg-dark)",
      width: "100%",
      height: "65px",
      color: "white",
      fontFamily: "var(--font-sans)",
      fontWeight: "bold",
      alignItems: "center",
      justifyContent: "center",
    }}
  >
    {/* Left icon */}
    <IconButton
    onClick={() => {console.log("Penis")}}
      size="1"
      variant="ghost"
      style={{
        width: "35px",
        height: "35px",
        position: "absolute",
        left: 4,
        top: "50%",
        transform: "translateY(-50%)",
        color: "white",
        display: "flex",
    alignItems: "center",
    justifyContent: "center",
    marginTop: "0.25px",
      }}
    >
      <GearIcon style={{width: "25px", height: "25px"}}/>
    </IconButton>

    {}
    <Box
      style={{
        position: "absolute",
        inset: 0,
        display: "flex",
        alignItems: "center",
        justifyContent: "left",
        pointerEvents: "none", // clicks go to icon
      }}
    >
      Tremokitty!
    </Box>
  </Box>
);
}